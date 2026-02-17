# API Explorer

## About

API Explorer is a development and debugging tool for Home Assistant app and integration developers. It lets you see exactly what Supervisor's ingress proxy does to requests and test Core API endpoints without leaving the browser.

## Panels

### Ingress Headers

Displays all HTTP headers arriving at the app after Supervisor's ingress proxy has processed the request. Useful for verifying that user identity headers (`X-Remote-User-Id`, `X-Remote-User-Name`, `X-Remote-User-Display-Name`) and forwarding headers (`X-Forwarded-For`, `X-Forwarded-Host`, `X-Forwarded-Proto`) are set correctly.

### API Request Builder

Send arbitrary HTTP requests to the Home Assistant Core API via the Supervisor proxy. The app authenticates using its `SUPERVISOR_TOKEN` automatically. The full response (status code, headers, and body) is displayed for inspection.

### Quick Actions

One-click buttons for commonly used API endpoints: status, config, states, services, and events.

## Configuration

This app has no configuration options. It uses `homeassistant_api: true` to access the Core API through Supervisor.
