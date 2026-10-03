# Architecture, Thread Model & Network Flow

## 1. Thread Model

To maximize performance while demonstrating advanced Linux system programming, we will use a **hybrid epoll + worker thread pool** model, keeping the health manager entirely separate.

1. **Main Thread (Event Loop):**
   - Sets up the `epoll` instance.
   - Blocks on `epoll_wait()`.
   - Accepts incoming client connections.
   - Pushes active client/backend socket events to a synchronized task queue.
2. **Worker Threads (Thread Pool):**
   - Wait on the task queue.
   - Read data from client socket -> write to backend socket.
   - Read data from backend socket -> write to client socket.
   - Handles socket closures and updates the `activeConnections` atomic counters.
3. **Health-Check Thread:**
   - A dedicated detached thread that sleeps for `health_check_interval`.
   - Iterates over the backend pool.
   - Attempts non-blocking `connect()` to test reachability.
   - Updates `BackendServer::healthy` status atomically.

*Note: Why epoll over Thread-per-Client?* 
Creating a thread for every single TCP connection consumes excessive memory and OS context-switch overhead. `epoll` is an O(1) event-notification facility in Linux that allows a single thread to monitor thousands of sockets simultaneously.

## 2. Network Flow (TCP Proxying)

1. Client connects via `connect()` to Load Balancer IP:9000.
2. Load Balancer `accept()`s connection -> creates `client_fd`.
3. Load balancer selects a backend based on algorithm.
4. Load Balancer calls `socket()` and `connect()` to the selected Backend IP:Port -> creates `backend_fd`.
5. LB sets both sockets to non-blocking and adds them to `epoll_fd` with `EPOLLIN`.
6. Data arriving on `client_fd` is `recv()`'d and `send()`'d to `backend_fd`.
7. Data arriving on `backend_fd` is `recv()`'d and `send()`'d to `client_fd`.
8. If `recv()` returns 0 (EOF), the connection is cleanly closed on both ends, and `activeConnections` is decremented.
