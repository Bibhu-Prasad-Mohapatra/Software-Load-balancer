#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void send_request(const std::string& ip, int port, int client_id) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "[Client " << client_id << "] Socket creation error\n";
        return;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "[Client " << client_id << "] Invalid address / Address not supported\n";
        close(sock);
        return;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "[Client " << client_id << "] Connection failed\n";
        close(sock);
        return;
    }

    std::string message = "GET / HTTP/1.1\r\nHost: " + ip + "\r\n\r\n";
    send(sock, message.c_str(), message.length(), 0);

    char buffer[1024] = {0};
    read(sock, buffer, 1024);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> latency = end_time - start_time;

    std::cout << "[Client " << client_id << "] Response received in " << latency.count() << " ms:\n" << buffer << "\n";

    close(sock);
}

int main(int argc, char* argv[]) {
    std::string target_ip = "127.0.0.1";
    int target_port = 9000;
    int num_clients = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--ip" && i + 1 < argc) {
            target_ip = argv[++i];
        } else if (arg == "--port" && i + 1 < argc) {
            target_port = std::stoi(argv[++i]);
        } else if (arg == "--clients" && i + 1 < argc) {
            num_clients = std::stoi(argv[++i]);
        }
    }

    std::cout << "[INFO] Simulating " << num_clients << " clients targeting " << target_ip << ":" << target_port << "\n";

    std::vector<std::thread> threads;
    for (int i = 0; i < num_clients; ++i) {
        threads.push_back(std::thread(send_request, target_ip, target_port, i + 1));
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // stagger requests slightly
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    return 0;
}
