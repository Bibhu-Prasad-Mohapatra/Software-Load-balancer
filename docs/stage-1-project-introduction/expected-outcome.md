# Expected Outcome

By the end of this project, the expected outcome is a fully functional, highly documented **Software-Defined Load Balancer & Health-Check Router** ecosystem. 

The delivered software suite will include:

1. **Load Balancer Executable (`load_balancer`)**: The primary C++ daemon running in Linux user-space, successfully multiplexing TCP traffic using `epoll` and threads, executing health-checks, and demonstrating failover.
2. **Backend Server Emulator (`backend_server`)**: A configurable dummy server to accept traffic and simulate varying latency and failure conditions.
3. **Client Simulator (`client_simulator`)**: A load-generating tool designed to stress-test the system and measure responsiveness.
4. **Configuration & Scripts**: Bash scripts to automate testing scenarios, spin up clusters, and trigger failures, alongside a `.conf` based configuration system.
5. **Academic Artifacts**: 
   - A complete 6-stage development documentation structure.
   - Comprehensive UML diagrams (Architecture, Class, Sequence).
   - A final project report detailing Linux C++ system engineering concepts.
   - A compiled list of 50+ technical Viva questions.
   - A slide deck for final presentation and defense.

When demonstrated, the system will visually prove—via terminal output and logs—that it can distribute traffic evenly, instantly isolate a killed backend server, redirect traffic successfully to survivors, and gracefully reintroduce the server once it is restarted.
