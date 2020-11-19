#!/bin/bash

exec socat TCP-LISTEN:33333,fork,reuseaddr TCP:hassio:33333 < /dev/null

