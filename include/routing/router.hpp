#pragma once
#include "../common/config.hpp"
#include <mutex>

class Router {
public:
    BackendServer* get_next_backend(LoadBalancerConfig& config);
private:
    std::mutex mtx;
    size_t current_index = 0;
};
