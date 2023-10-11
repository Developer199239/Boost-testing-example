// Boost C++ TCP Socket Server Example with Boost.Test

#define BOOST_TEST_MODULE MyTestModule
#include <boost/test/unit_test.hpp>

#include <thread>
#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

// Define a function to run the server in a separate thread
void RunServer() {
    try {
        io_service io_service;
        ip::tcp::endpoint endpoint(ip::tcp::v4(), 12345);
        ip::tcp::acceptor acceptor(io_service, endpoint);

        ip::tcp::socket socket(io_service);
        acceptor.accept(socket);

        char data[1024];
        size_t length = socket.read_some(buffer(data));

        // BOOST_CHECK_EQUAL(std::string(data, length), "Hello, Client!");

        BOOST_CHECK_EQUAL(std::string(data, length), "Hello, Server!");

        std::string response = "Hello, Client!";
        socket.write_some(buffer(response));
        socket.close();
    }
    catch (std::exception& e) {
        BOOST_FAIL("Exception: " << e.what());
    }
}

int add(int a, int b) {
    return a + b;
}

BOOST_AUTO_TEST_CASE(test_addition) {
    int result = add(2, 3);
    BOOST_CHECK_EQUAL(result, 5);
}

BOOST_AUTO_TEST_CASE(test_subtraction) {
    int result = add(5, -3);
    BOOST_CHECK_EQUAL(result, 2);
}

BOOST_AUTO_TEST_CASE(test_subtraction2) {
    int result = add(5, -3);
    BOOST_CHECK_EQUAL(result, 5);
}

BOOST_AUTO_TEST_CASE(test_subtraction3) {
    int result = add(5, -3);
    BOOST_CHECK_EQUAL(result, 2);
}

// Define test cases for the socket server and client
BOOST_AUTO_TEST_CASE(SocketServerTest) {
    std::thread serverThread(RunServer);
    serverThread.detach();

    // Sleep to give the server time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    try {
        io_service io_service;
        ip::tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"), 12345);
        ip::tcp::socket socket(io_service);
        socket.connect(endpoint);

        std::string message = "Hello, Server!";
        socket.write_some(buffer(message));

        char data[1024];
        size_t length = socket.read_some(buffer(data));
        BOOST_CHECK_EQUAL(std::string(data, length), "Hello, Client!");

        socket.close();
    }
    catch (std::exception& e) {
        BOOST_FAIL("Exception: " << e.what());
    }
}
