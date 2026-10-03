#include "../../include/routing/router.hpp"

BackendServer* Router::get_next_backend(LoadBalancerConfig& config) {
    std::lock_guard<std::mutex> lock(mtx);
    if (config.backends.empty()) return nullptr;

    if (config.algorithm == "least_connections") {
        BackendServer* best = nullptr;
        uint32_t min_conn = UINT32_MAX;

        for (auto& server : config.backends) {
            if (server.enabled && server.healthy.load()) {
                uint32_t active = server.activeConnections.load();
                if (active < min_conn) {
                    min_conn = active;
                    best = &server;
                }
            }
        }
        return best;
    } 
    else {
        // Default to Round Robin
        size_t start_index = current_index;
        do {
            BackendServer& server = config.backends[current_index];
            current_index = (current_index + 1) % config.backends.size();
            
            if (server.enabled && server.healthy.load()) {
                return &server;
            }
        } while (current_index != start_index);
        
        return nullptr;
    }
}
