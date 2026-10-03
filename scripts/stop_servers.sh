#!/bin/bash
echo "Stopping all backend servers..."
kill $(cat logs/server1.pid) 2>/dev/null
kill $(cat logs/server2.pid) 2>/dev/null
kill $(cat logs/server3.pid) 2>/dev/null
rm -f logs/*.pid
echo "Servers stopped."
