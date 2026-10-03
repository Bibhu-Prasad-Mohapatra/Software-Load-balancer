#!/bin/bash
echo "Starting Backend Servers..."
./build/backend_server --id server1 --port 8001 --delay 10 &
P1=$!
./build/backend_server --id server2 --port 8002 --delay 20 &
P2=$!
./build/backend_server --id server3 --port 8003 --delay 30 &
P3=$!

echo $P1 > logs/server1.pid
echo $P2 > logs/server2.pid
echo $P3 > logs/server3.pid

echo "Servers started on ports 8001, 8002, 8003"
