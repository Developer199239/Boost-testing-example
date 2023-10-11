#define BOOST_TEST_MODULE MyTestModule
#include <boost/test/unit_test.hpp>

int my_test_main(int argc, char* argv[]) {
    return boost::unit_test::unit_test_main(&init_unit_test_suite, argc, argv);
}
