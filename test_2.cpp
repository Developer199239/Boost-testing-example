#include <boost/test/unit_test.hpp>

#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include "test_functions.hpp"
using namespace boost::asio;

BOOST_AUTO_TEST_CASE(test_addition) {
    int result = add(2, 3);
    BOOST_CHECK_EQUAL(result, 5);
}

BOOST_AUTO_TEST_CASE(test_subtraction) {
    int result = add(5, -3);
    BOOST_CHECK_EQUAL(result, 2);
}