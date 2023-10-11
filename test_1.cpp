#include <boost/test/unit_test.hpp>

#include <thread>
#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

int add(int a, int b) {
    return a + b;
}

BOOST_AUTO_TEST_CASE(test_subtraction2) {
    int result = add(5, -3);
    BOOST_CHECK_EQUAL(result, 5);
}

BOOST_AUTO_TEST_CASE(test_subtraction3) {
    int result = add(5, -3);
    BOOST_CHECK_EQUAL(result, 2);
}