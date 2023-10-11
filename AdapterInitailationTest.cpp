#include <boost/test/unit_test.hpp>
#include <boost/process.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "CommonFunction.hpp"
#include "Constants.hpp"

namespace bp = boost::process;

bool CheckAdapterInitialization(const std::vector<std::string>& args, std::string expectedMsg) {
    bp::ipstream out;
    bp::ipstream err;

    std::string cmd = "./RTLite";
    for (const std::string& arg : args) {
        cmd += " " + arg;
    }

    BOOST_TEST_MESSAGE("Running command: " << cmd);
    std::string buildDir = RT_LITE_EXEC_PATH;
    if (chdir(buildDir.c_str()) != 0) {
        BOOST_FAIL("Failed to change working directory to: " << buildDir);
        return false;
    }

    bp::child c(cmd, bp::std_out > out, bp::std_err > err);
    bool isFoundExpectedMsg = false;
    for(int i = 0; i<6; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        isFoundExpectedMsg =  IsStringInFile(LOG_FILE_PATH, expectedMsg);
        if(isFoundExpectedMsg) {
            BOOST_TEST_MESSAGE("isFoundExpectedMsg: " << isFoundExpectedMsg);
            break;
        }
    }

    c.terminate();
    c.wait();
    return isFoundExpectedMsg;
}


BOOST_AUTO_TEST_CASE(FidelioPMS_Initialization_Test) {
    ClearLogFile(LOG_FILE_PATH);
    //start cloudConnector
    std::vector<std::string> cloudConnectorArgs = {"2033", "trace"};
    StartCloudConnector(cloudConnectorArgs, CLOUD_CONNECTOR_EXEC_PATH);

    // Sleep to ensure the server has time to start
    std::this_thread::sleep_for(std::chrono::seconds(5));
    //start RTLite
    std::vector<std::string> args = {"127.0.0.1", "2033", "trace"};
    std::string expectedMsg = "Instantiating Adapter: FidelioPMS";
    bool isFoundExpectedMsg = CheckAdapterInitialization(args, expectedMsg);
    BOOST_CHECK(isFoundExpectedMsg);
    StopCloudConnector();
}

BOOST_AUTO_TEST_CASE(DuVoiceVMS_Initialization_Test) {
    ClearLogFile(LOG_FILE_PATH);
    //start cloudConnector
    std::vector<std::string> cloudConnectorArgs = {"2033", "trace"};
    StartCloudConnector(cloudConnectorArgs, CLOUD_CONNECTOR_EXEC_PATH);

    // Sleep to ensure the server has time to start
    std::this_thread::sleep_for(std::chrono::seconds(5));
    //start RTLite
    std::vector<std::string> args = {"127.0.0.1", "2033", "trace"};
    std::string expectedMsg = "Instantiating Adapter: DuVoiceVMS";
    bool isFoundExpectedMsg = CheckAdapterInitialization(args, expectedMsg);
    BOOST_CHECK(isFoundExpectedMsg);
    StopCloudConnector();
}

BOOST_AUTO_TEST_CASE(OnQ_Initialization_Test) {
    ClearLogFile(LOG_FILE_PATH);
    //start cloudConnector
    std::vector<std::string> cloudConnectorArgs = {"2033", "trace"};
    StartCloudConnector(cloudConnectorArgs, CLOUD_CONNECTOR_EXEC_PATH);

    // Sleep to ensure the server has time to start
    std::this_thread::sleep_for(std::chrono::seconds(5));
    //start RTLite
    std::vector<std::string> args = {"127.0.0.1", "2033", "trace"};
    std::string expectedMsg = "Instantiating Adapter: OnQ";
    bool isFoundExpectedMsg = CheckAdapterInitialization(args, expectedMsg);
    BOOST_CHECK(isFoundExpectedMsg);
    StopCloudConnector();
}

BOOST_AUTO_TEST_CASE(NEC_BCT_ConsolePBX_Initialization_Test) {
    ClearLogFile(LOG_FILE_PATH);
    //start cloudConnector
    std::vector<std::string> cloudConnectorArgs = {"2033", "trace"};
    StartCloudConnector(cloudConnectorArgs, CLOUD_CONNECTOR_EXEC_PATH);

    // Sleep to ensure the server has time to start
    std::this_thread::sleep_for(std::chrono::seconds(5));
    //start RTLite
    std::vector<std::string> args = {"127.0.0.1", "2033", "trace"};
    std::string expectedMsg = "Instantiating Adapter: NEC-BCT-ConsolePBX";
    bool isFoundExpectedMsg = CheckAdapterInitialization(args, expectedMsg);
    BOOST_CHECK(isFoundExpectedMsg);
    StopCloudConnector();
}

