#include "../../include/routing/router.hpp"

BackendServer* Router::get_next_backend(LoadBalancerConfig& config) {
    std::lock_guard<std::mutex> lock(mtx);
    if (config.backends.empty()) return nullptr;

    size_t start_index = current_index;
    do {
        BackendServer& server = config.backends[current_index];
        current_index = (current_index + 1) % config.backends.size();
        
        if (server.enabled && server.healthy.load()) {
            return &server;
        }
    } while (current_index != start_index);

    return nullptr; // No healthy backend found
}
