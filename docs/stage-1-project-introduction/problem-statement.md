# Problem Statement

In an environment where a service relies on a pool of multiple backend servers, there must be a resilient mechanism to distribute incoming client traffic efficiently.

If a client attempts to connect directly to a backend server, several critical issues arise:
1. **Overload:** A single server may be overwhelmed with connections while others sit idle.
2. **Single Point of Failure:** If the target server crashes or loses network connectivity, the client's request fails completely, degrading the user experience.
3. **Lack of Dynamic Recovery:** There is no automated way for the client to know which servers are currently active or how to shift traffic away from a failed instance and back to a recovered one.

Therefore, the problem is to design and implement an intermediate **Software-Defined Load Balancer** that acts as a transparent proxy. It must intelligently distribute incoming traffic across healthy backends while automatically detecting failures, routing around them, and dynamically reintroducing servers when they recover.
