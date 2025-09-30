#include "Server-client.hpp"

int main() {
    try {
        Client client;
        client.connectTo("127.0.0.1", 8080);

        std::thread receiver([&client]() {
            while (true) {
                std::string msg = client.receiveMessage();
                if (!msg.empty()) {
                    std::cout  << msg << std::endl;
                    std::cout << " >>> ";
                }
            }
            });

        while (true) {
            std::string msg;
            std::cout << " >>> ";
            std::getline(std::cin, msg);
            if (!msg.empty()) {
                client.sendMessage(msg);
            }
        }

        receiver.join(); 
    }
    catch (const std::exception& e) {
        std::cerr << "Client Error: " << e.what() << std::endl;
    }

    return 0;
}
