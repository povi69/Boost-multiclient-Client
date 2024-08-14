#include "Boost_Client.hpp"
#include <sstream>
#include <iostream>

Client::Client(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
    : socket_(io_context) {
    boost::asio::ip::tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(host, port);
    connect(endpoints);
}

Client::~Client() {
    close();
    std::cout << "Client destructor called. Connection closed." << std::endl;
}
void Client::connect(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
    boost::asio::async_connect(socket_, endpoints, [this](const boost::system::error_code& error, const boost::asio::ip::tcp::endpoint&) {
        handleConnect(error);
        });
}

void Client::handleConnect(const boost::system::error_code& error) {
    if (!error) {
        startRead();
    }
    else {
        close();
    }
}

void Client::startRead() {
    socket_.async_read_some(boost::asio::buffer(buffer_), [this](boost::system::error_code error, std::size_t length) {
        handleRead(error, length);
        });
}

void Client::handleRead(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        std::string data(buffer_.data(), bytes_transferred);
        std::cout << "Received: " << data << std::endl;
        startRead();
    }
    else {
        close();
    }
}


void Client::write(const std::string& message) {
    if (message.size() > BUFFER_SIZE) {
        throw std::runtime_error("Message is too large to fit in the buffer");
    }
    std::copy(message.begin(), message.end(), buffer_.begin());

    boost::asio::async_write(socket_, boost::asio::buffer(buffer_, message.size()), [this](boost::system::error_code error, std::size_t bytes_transferred) {
        handleWrite(error, bytes_transferred);
        });
}


void Client::handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (error) {
        close();
    }
}

void Client::close() {
    socket_.close();
    std::cout << "Connection closed." << std::endl;
}
