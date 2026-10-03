#!/bin/bash
echo "Running Load Test with 50 concurrent clients..."
./build/client_simulator --ip 127.0.0.1 --port 9000 --clients 50
echo "Load test complete."
