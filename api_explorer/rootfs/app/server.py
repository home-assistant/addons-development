"""API Explorer app server."""

import asyncio
import json
import logging
import os

import aiohttp
from aiohttp import web

_LOGGER = logging.getLogger(__name__)

SUPERVISOR_URL = "http://supervisor"
SUPERVISOR_WS_URL = "ws://supervisor/core/websocket"
SUPERVISOR_TOKEN = os.environ.get("SUPERVISOR_TOKEN", "")
INGRESS_PORT = int(os.environ.get("INGRESS_PORT", "8099"))
INDEX_PATH = os.path.join(os.path.dirname(__file__), "index.html")

with open(INDEX_PATH) as _f:
    INDEX_HTML_CONTENT = _f.read()


async def serve_index(request: web.Request) -> web.Response:
    """Serve the main UI page."""
    return web.Response(text=INDEX_HTML_CONTENT, content_type="text/html")


async def ingress_headers(request: web.Request) -> web.Response:
    """Return all incoming request headers as JSON."""
    headers = dict(request.headers)
    return web.json_response(headers)


async def proxy_to_core(request: web.Request) -> web.Response:
    """Proxy a request to Home Assistant Core via Supervisor."""
    path = request.match_info.get("path", "")
    target_url = f"{SUPERVISOR_URL}/core/api/{path}"

    if request.query_string:
        target_url += f"?{request.query_string}"

    # Read body if present
    body = None
    if request.can_read_body:
        body = await request.read()

    # Forward selected headers
    forward_headers = {
        "Authorization": f"Bearer {SUPERVISOR_TOKEN}",
    }
    content_type = request.headers.get("Content-Type")
    if content_type:
        forward_headers["Content-Type"] = content_type

    # Forward custom headers from the browser (skip internal/auth headers)
    skip_headers = {
        "host", "content-type", "content-length", "authorization",
        "connection", "accept", "accept-encoding", "accept-language",
        "origin", "referer", "user-agent",
    }
    for key, value in request.headers.items():
        if key.lower() not in skip_headers:
            forward_headers[key] = value

    async with aiohttp.ClientSession() as session:
        try:
            async with session.request(
                method=request.method,
                url=target_url,
                headers=forward_headers,
                data=body,
            ) as resp:
                resp_body = await resp.text()
                resp_headers = dict(resp.headers)

                # Try to parse as JSON for nicer display
                try:
                    resp_json = json.loads(resp_body)
                except (json.JSONDecodeError, ValueError):
                    resp_json = None

                return web.json_response(
                    {
                        "status": resp.status,
                        "headers": resp_headers,
                        "body": resp_json if resp_json is not None else resp_body,
                    }
                )
        except aiohttp.ClientError as err:
            return web.json_response(
                {"status": 0, "headers": {}, "body": f"Connection error: {err}"},
                status=502,
            )


async def websocket_proxy(request: web.Request) -> web.WebSocketResponse:
    """Proxy WebSocket connection to Home Assistant Core via Supervisor."""
    browser_ws = web.WebSocketResponse()
    await browser_ws.prepare(request)

    session = aiohttp.ClientSession()
    try:
        core_ws = await session.ws_connect(SUPERVISOR_WS_URL)
    except aiohttp.ClientError as err:
        _LOGGER.error("Failed to connect to Core WebSocket: %s", err)
        await browser_ws.close(
            code=aiohttp.WSCloseCode.GOING_AWAY,
            message=b"Failed to connect to Core",
        )
        await session.close()
        return browser_ws

    try:
        # Auth handshake: read auth_required from Core
        msg = await core_ws.receive()
        if msg.type != aiohttp.WSMsgType.TEXT:
            await browser_ws.close(
                code=aiohttp.WSCloseCode.PROTOCOL_ERROR,
                message=b"Unexpected message from Core",
            )
            return browser_ws

        # Send auth token to Core
        await core_ws.send_json(
            {"type": "auth", "access_token": SUPERVISOR_TOKEN}
        )

        # Read auth result from Core
        msg = await core_ws.receive()
        if msg.type != aiohttp.WSMsgType.TEXT:
            await browser_ws.close(
                code=aiohttp.WSCloseCode.PROTOCOL_ERROR,
                message=b"Auth failed",
            )
            return browser_ws

        auth_result = json.loads(msg.data)
        if auth_result.get("type") != "auth_ok":
            await browser_ws.send_json(auth_result)
            await browser_ws.close(
                code=aiohttp.WSCloseCode.PROTOCOL_ERROR,
                message=b"Auth rejected",
            )
            return browser_ws

        # Notify browser that auth succeeded
        await browser_ws.send_json(auth_result)

        # Bidirectional forwarding
        async def forward_browser_to_core():
            async for msg in browser_ws:
                if msg.type == aiohttp.WSMsgType.TEXT:
                    await core_ws.send_str(msg.data)
                elif msg.type in (
                    aiohttp.WSMsgType.CLOSE,
                    aiohttp.WSMsgType.CLOSING,
                    aiohttp.WSMsgType.CLOSED,
                    aiohttp.WSMsgType.ERROR,
                ):
                    break

        async def forward_core_to_browser():
            async for msg in core_ws:
                if msg.type == aiohttp.WSMsgType.TEXT:
                    await browser_ws.send_str(msg.data)
                elif msg.type in (
                    aiohttp.WSMsgType.CLOSE,
                    aiohttp.WSMsgType.CLOSING,
                    aiohttp.WSMsgType.CLOSED,
                    aiohttp.WSMsgType.ERROR,
                ):
                    break

        # Cancel the other direction when one side disconnects
        browser_task = asyncio.create_task(forward_browser_to_core())
        core_task = asyncio.create_task(forward_core_to_browser())

        done, pending = await asyncio.wait(
            {browser_task, core_task},
            return_when=asyncio.FIRST_COMPLETED,
        )

        for task in pending:
            task.cancel()

        await asyncio.gather(*pending, return_exceptions=True)
    finally:
        if not core_ws.closed:
            await core_ws.close()
        await session.close()
        if not browser_ws.closed:
            await browser_ws.close()

    return browser_ws


def main() -> None:
    """Start the server."""
    app = web.Application()
    app.router.add_get("/", serve_index)
    app.router.add_get("/api/ingress-headers", ingress_headers)
    app.router.add_get("/api/ws", websocket_proxy)
    app.router.add_route("*", "/api/proxy/{path:.*}", proxy_to_core)
    web.run_app(app, host="0.0.0.0", port=INGRESS_PORT)


if __name__ == "__main__":
    main()
