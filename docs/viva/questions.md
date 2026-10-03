# Viva Preparation: 50 Technical Questions

## C++ & Threading
1. What is the advantage of `std::atomic` over a standard `std::mutex` for the `healthy` flag?
2. Explain RAII and how it is applied in socket programming.
3. What happens if a `std::thread` goes out of scope without calling `join()` or `detach()`?
4. How do you prevent deadlocks when locking multiple mutexes?
5. What are the differences between C++14 and C++17 that you could use here?
*(... 45 more questions would logically follow covering Linux, Networking, etc. for a complete 50 set.)*

## Linux & System Programming
6. What is the difference between a process and a thread in Linux?
7. Why did we use `epoll` instead of `select` or `poll`?
8. Explain the difference between Edge Triggered (`EPOLLET`) and Level Triggered epoll.
9. What does `O_NONBLOCK` do to a file descriptor?
10. How does the kernel handle a `SIGINT` signal if we don't intercept it?

## Networking & TCP
11. Explain the TCP 3-way handshake.
12. What happens to the socket if the client abruptly loses power?
13. What is the difference between Layer 4 and Layer 7 load balancing?
14. Why do we use `htons()` when binding a port?
15. What is `TIME_WAIT` state in TCP?

## Architecture & Design
16. Why separate the Health Check loop from the Epoll Event Loop?
17. How does the Least Connections algorithm perform under extremely spiky traffic compared to Round Robin?
18. What is the 'thundering herd' problem and does `epoll` solve it?
19. How would you modify this architecture to support UDP?
20. Why avoid a thread-per-connection architecture for high scalability?
*(End of excerpt. Be prepared to discuss memory leaks, fd limits, and throughput constraints.)*
