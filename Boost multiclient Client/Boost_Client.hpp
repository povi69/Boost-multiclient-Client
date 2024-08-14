#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <memory>

class Client {
public:
    /**
     * @brief Constructs the Client object and initiates a connection to the server.
     * @param io_context The io_context used to handle asynchronous operations.
     * @param host The server's host address (e.g., "localhost").
     * @param port The server's port number (e.g., "12345").
     */
    Client(boost::asio::io_context& io_context, const std::string& host, const std::string& port);

    ~Client();

    /**
     * @brief Closes the client's connection to the server.
     */
    void close();

    /**
     * @brief Sends a message to the server asynchronously.
     * @param message The message to be sent.
     * @throws std::runtime_error if the message size exceeds the buffer size.
     */
    void write(const std::string& message);

private:
    /**
     * @brief Initiates the asynchronous connection to the server.
     * @param endpoints The resolved endpoints for the server's address and port.
     */
    void connect(const boost::asio::ip::tcp::resolver::results_type& endpoints);

    /**
     * @brief Handles the result of the connection attempt.
     * @param error The error code returned by the connection attempt.
     */
    void handleConnect(const boost::system::error_code& error);

    /**
     * @brief Starts an asynchronous read operation from the server.
     */
    void startRead();

    /**
     * @brief Handles the result of the read operation from the server.
     * @param error The error code returned by the read operation.
     * @param bytes_transferred The number of bytes that were successfully read.
     */
    void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred);

    /**
     * @brief Handles the result of the write operation to the server.
     * @param error The error code returned by the write operation.
     * @param bytes_transferred The number of bytes that were successfully written.
     */
    void handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred);

    static constexpr std::size_t BUFFER_SIZE = 1024;
    boost::asio::ip::tcp::socket socket_; 
    std::array<char, BUFFER_SIZE> buffer_;
