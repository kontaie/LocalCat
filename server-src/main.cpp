#include "../LocalCat/include/Server-client.hpp"

int main() {
	try {
		Server server;
		server.run();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	std::getc;
}