#define BOOST_TEST_MODULE MyTestSuite
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstdio>

// Helper function to run RTLite and capture its output
std::string RunRTLite(const std::vector<std::string>& args) {
    // Construct the command for running RTLite
    std::string cmd = "./RTLite"; // Assuming RTLite is in the current working directory
    for (const std::string& arg : args) {
        cmd += " " + arg;
    }

    BOOST_TEST_MESSAGE("Running command: " << cmd);

    // Change the working directory to the build directory
    std::string buildDir = "/Users/user/Downloads/today/oct_5/RTLite+Logger/Client/build/";
    if (chdir(buildDir.c_str()) != 0) {
        BOOST_FAIL("Failed to change working directory to: " << buildDir);
        return "";
    }
    BOOST_TEST_MESSAGE("RTLite buildDir found\n");

    // Run RTLite with the specified arguments and wait for 5 seconds
    std::string result; // Declare the 'result' variable here
    FILE* pipe = popen(cmd.c_str(), "r");
    if (pipe) {
        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr) {
                result += buffer;
            }
        }

        int exitCode = pclose(pipe);

        if (exitCode == 0) {
            BOOST_TEST_MESSAGE("RTLite exited normally");
        } else if (WIFEXITED(exitCode)) {
            BOOST_TEST_MESSAGE("RTLite exited with status: " << WEXITSTATUS(exitCode));
        } else if (WIFSIGNALED(exitCode)) {
            BOOST_TEST_MESSAGE("RTLite terminated by signal: " << WTERMSIG(exitCode));
        } else {
            BOOST_TEST_MESSAGE("RTLite exit status unknown");
        }
    } else {
        BOOST_FAIL("Failed to run RTLite command: " << cmd);
    }

    // Sleep for the specified duration (e.g., 5 seconds)
    std::this_thread::sleep_for(std::chrono::seconds(5));

    BOOST_TEST_MESSAGE("RTLite Output:\n" << result);

    return result;
}


// Define a test case that runs RTLite with specific arguments
BOOST_AUTO_TEST_CASE(RTLiteTest1) {
    // Define your test arguments as needed
    std::vector<std::string> args = {"127.0.0.1", "2033", "trace"};

    // Run RTLite with the specified arguments and capture the output
    std::string output = RunRTLite(args);

    // Perform your tests based on the captured output
    // For example, you can check if the output contains specific strings
    BOOST_CHECK(output.find("Expected output") != std::string::npos);
}
