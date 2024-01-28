#!/bin/sh

case "$1" in
  start)
    start-stop-daemon -S -n sensor_server -a /usr/bin/sensor_server -- -d
    ;;
  stop)
    start-stop-daemon -K -n sensor_server
    ;;
  *)
    echo "Usage: $0 { start | stop }"
    exit 1
esac

exit 0
