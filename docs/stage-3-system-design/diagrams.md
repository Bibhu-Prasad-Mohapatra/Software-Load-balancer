# System UML Diagrams

## 1. Architecture Diagram
```mermaid
graph TD
    Client1[Client] --> LB[Load Balancer Core]
    Client2[Client] --> LB
    Client3[Client] --> LB
    
    LB -->|Selected by Algorithm| BS1[Backend Server 1: 8001]
    LB -->|Selected by Algorithm| BS2[Backend Server 2: 8002]
    LB -.-x|Failed| BS3[Backend Server 3: 8003]
    
    subgraph Load Balancer Process
        LB
        HM[Health Check Manager]
        CFG[Config Manager]
        HM -.->|Polls status| BS1
        HM -.->|Polls status| BS2
        HM -.->|Polls status| BS3
        CFG --> LB
    end
```

## 2. Class Diagram
```mermaid
classDiagram
    class LoadBalancer {
        -int listen_fd
        -int epoll_fd
        -Config config
        -vector<BackendServer> backends
        -HealthManager health_mgr
        +start()
        +stop()
        -accept_connection()
        -handle_client_data()
    }
    
    class BackendServer {
        +string id
        +string host
        +uint16_t port
        +bool is_healthy
        +int active_connections
        +increment_connections()
        +decrement_connections()
    }

    class HealthManager {
        -thread check_thread
        -bool running
        +start_monitoring()
        +stop_monitoring()
        -perform_checks()
    }

    class Router {
        +BackendServer get_next_backend(vector<BackendServer>& backends, Algorithm algo)
        -BackendServer round_robin()
        -BackendServer least_connections()
    }

    LoadBalancer --> HealthManager
    LoadBalancer --> Router
    LoadBalancer "1" *-- "many" BackendServer
```

## 3. Sequence Diagrams

### 3.1 Normal Request Flow
```mermaid
sequenceDiagram
    participant Client
    participant LB as Load Balancer
    participant Router
    participant Backend

    Client->>LB: TCP Connect (SYN)
    LB->>Client: TCP Accept (SYN-ACK)
    Client->>LB: Send Request Data
    LB->>Router: get_next_backend()
    Router-->>LB: Returns Healthy Backend
    LB->>Backend: TCP Connect & Forward Data
    Backend-->>LB: Send Response
    LB-->>Client: Forward Response
```

### 3.2 Server Failure & Recovery Flow
```mermaid
sequenceDiagram
    participant HealthMgr
    participant LB as Load Balancer
    participant Backend2

    loop Every 2 seconds
        HealthMgr->>Backend2: Send Health Probe
        alt Probe Times Out
            HealthMgr->>LB: Increment fail_count
            Note over LB: fail_count >= 3
            LB->>Backend2: Mark UNHEALTHY
        else Backend Recovers
            HealthMgr->>Backend2: Send Health Probe
            Backend2-->>HealthMgr: Status OK
            HealthMgr->>LB: Increment success_count
            Note over LB: success_count >= 2
            LB->>Backend2: Mark HEALTHY
        end
    end
```

## 4. State Machine Diagram (Backend Server)
```mermaid
stateDiagram-v2
    [*] --> UNKNOWN
    UNKNOWN --> HEALTHY : Initial probe success
    UNKNOWN --> UNHEALTHY : Initial probe fail
    
    HEALTHY --> SUSPECTED : Probe Timeout/Error
    SUSPECTED --> HEALTHY : Next Probe Success
    SUSPECTED --> UNHEALTHY : fail_count >= threshold
    
    UNHEALTHY --> RECOVERING : Probe Success
    RECOVERING --> UNHEALTHY : Probe Timeout/Error
    RECOVERING --> HEALTHY : success_count >= threshold
```
