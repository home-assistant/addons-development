# API Explorer

A development app for Home Assistant that provides a web UI (via ingress) to inspect Supervisor ingress behavior and test Home Assistant Core API calls.

## What it does

- **Ingress Header Inspector**: Shows all HTTP headers that Supervisor's ingress proxy sends to apps, including `X-Remote-User-Id`, `X-Remote-User-Display-Name`, `X-Forwarded-For`, etc.
- **API Request Builder**: Make API calls to Home Assistant Core through the Supervisor proxy (`http://supervisor/core/api/...`) with configurable method, path, headers, and JSON body.
- **Quick Actions**: Preset buttons for common Core API endpoints like `/api/states`, `/api/config`, and `/api/services`.

## Usage

1. Copy this folder to `/addons/` on a Home Assistant OS or Supervised system
2. In the HA UI: Settings > Apps > App Store > reload
3. Find "API Explorer" in local apps, install and start
4. Open via the sidebar panel

## Architecture

The app runs a lightweight aiohttp Python server inside an HA base container with s6-overlay. The browser cannot reach `http://supervisor` directly (it's Docker-internal), so the app server acts as a proxy for API calls.
