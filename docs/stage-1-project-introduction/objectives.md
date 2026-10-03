# Objectives

The primary objectives of this project are to build, test, and document a robust software-based load balancer.

## Measurable Objectives

1. **Client Connection Handling:**
   - Successfully accept and manage concurrent connections from multiple clients (e.g., 50-100 simultaneous connections).

2. **Traffic Distribution:**
   - Implement at least two distinct load-balancing algorithms:
     - **Round Robin:** Equal sequential distribution.
     - **Least Connections:** Distribution based on lowest active socket count.

3. **Health Monitoring:**
   - Continuously poll the configured backend pool at configurable intervals (e.g., every 2 seconds).
   - Accurately transition a server's state from `HEALTHY` to `UNHEALTHY` after a specified threshold of consecutive failures (e.g., 3 failed checks).

4. **Fault Tolerance & Failover:**
   - Demonstrate a 100% success rate in routing *new* client traffic exclusively to healthy servers within 5 seconds of a backend failure detection.

5. **Dynamic Recovery:**
   - Automatically detect when a previously down backend becomes responsive again.
   - Transition its state to `HEALTHY` after a recovery threshold and seamlessly reintroduce it to the routing pool.

6. **System Stability:**
   - Operate without crashing, deadlocking, or leaking memory during stress testing and graceful shutdown (SIGINT/SIGTERM handling).

7. **Observability:**
   - Maintain comprehensive statistical counters (active connections, total requests, failure counts) and output structured logs indicating routing decisions and state changes.
