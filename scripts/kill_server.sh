#!/bin/bash
if [ -z "$1" ]; then
    echo "Usage: ./kill_server.sh <server_id>"
    exit 1
fi

if [ -f "logs/$1.pid" ]; then
    PID=$(cat logs/$1.pid)
    kill $PID
    rm logs/$1.pid
    echo "Killed $1 (PID: $PID)"
else
    echo "Server $1 not running or PID file missing."
fi
