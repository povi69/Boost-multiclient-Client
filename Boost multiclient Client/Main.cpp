#include "Boost_Client.hpp"
#include <iostream>

int main() {
    try {
        boost::asio::io_context io_context;

        std::string host = "localhost";
        std::string port = "12345";
        auto client = std::make_shared<Client>(io_context, host, port);

        std::thread mainThread([&io_context]() { io_context.run(); });

        std::string message;
        while (std::getline(std::cin, message)) {
            if (message == "exit") {
                client->close();
                break;
            }
            client->write(message);
        }

        mainThread.join();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
