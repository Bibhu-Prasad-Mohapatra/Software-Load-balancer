#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void print_usage() {
    std::cout << "Usage: ./backend_server --id <server_id> --port <port> [--delay <ms>]\n";
}

int main(int argc, char* argv[]) {
    std::string id = "unknown";
    int port = -1;
    int delay_ms = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--id" && i + 1 < argc) {
            id = argv[++i];
        } else if (arg == "--port" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else if (arg == "--delay" && i + 1 < argc) {
            delay_ms = std::stoi(argv[++i]);
        }
    }

    if (port == -1 || id == "unknown") {
        print_usage();
        return 1;
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "[ERROR] Socket creation failed\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "[ERROR] Bind failed on port " << port << "\n";
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "[ERROR] Listen failed\n";
        return 1;
    }

    std::cout << "[INFO] Backend '" << id << "' listening on port " << port << " (Delay: " << delay_ms << "ms)\n";

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            std::cerr << "[ERROR] Accept failed\n";
            continue;
        }

        char buffer[1024] = {0};
        int valread = read(client_fd, buffer, 1024);
        if (valread > 0) {
            if (delay_ms > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
            }

            std::string response = "Backend: " + id + "\nStatus: OK\nRequest processed successfully\n";
            send(client_fd, response.c_str(), response.length(), 0);
            std::cout << "[INFO] Handled request on " << id << "\n";
        }
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
