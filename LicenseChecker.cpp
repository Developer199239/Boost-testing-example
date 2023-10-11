#define BOOST_TEST_MODULE MyTestSuite
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

void UpdateLicense(std::string propertyID, std::string license_key){
    if (!std::filesystem::exists(LICENSE_FOLDER_PATH) || !std::filesystem::is_directory(LICENSE_FOLDER_PATH))
    {
       BOOST_FAIL("Failed to open License folder: " << LICENSE_FOLDER_PATH); 
    } else {
        std::string licenseXmlPath = "";
        for (const auto &xmlEntry : std::filesystem::directory_iterator(LICENSE_FOLDER_PATH))
        {
            if (xmlEntry.path().filename() == "license.config.xml")
            {
                licenseXmlPath = xmlEntry.path().string();
                break;
            }
        }

        if (!licenseXmlPath.empty() && licenseXmlPath.size() > 0)
        {
            try
                {
                    boost::property_tree::ptree tree;
                    read_xml(licenseXmlPath, tree);

                    // Modify the 'propertyID' in the tree
                    tree.put("sitekey.<xmlattr>.propertyID", propertyID);
                    tree.put("sitekey.license_key", license_key);

                    // Save the modified tree back to the XML file
                    write_xml(licenseXmlPath, tree);
                }
                catch (const boost::property_tree::xml_parser_error &e)
                {
                    BOOST_FAIL("Xml edit failed: " << e.what());
                }    
        }
        else
        {
            BOOST_FAIL("License xml not found");  
        }
    }
}


bool RunRTLiteAndCheckLicense(const std::vector<std::string>& args, std::string expectedMsg) {
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

    // Run RTLite with the specified arguments
    // bp::child c(cmd, bp::std_out > out, bp::std_err > err);
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

    // Send a termination signal to RTLite
    c.terminate();
    c.wait();
    return isFoundExpectedMsg;
}


BOOST_AUTO_TEST_CASE(Valid_License_Test_Case) {
    ClearLogFile(LOG_FILE_PATH);
    UpdateLicense("64","demo_license");
    //start cloudConnector
    std::vector<std::string> cloudConnectorArgs = {"2033", "trace"};
    StartCloudConnector(cloudConnectorArgs, CLOUD_CONNECTOR_EXEC_PATH);

    // Sleep to ensure the server has time to start
    std::this_thread::sleep_for(std::chrono::seconds(5));
    //start RTLite
    std::vector<std::string> args = {"127.0.0.1", "2033", "trace"};
    bool isFoundExpectedMsg = RunRTLiteAndCheckLicense(args, VALIDATED_LICENSE_MSG);
    BOOST_CHECK(isFoundExpectedMsg);
    //stop cloudConnector
    StopCloudConnector();
}

BOOST_AUTO_TEST_CASE(InValid_License_Test_Case) {
    ClearLogFile(LOG_FILE_PATH);
    UpdateLicense("64","demo_license_invalid");
    //start cloudConnector
    std::vector<std::string> cloudConnectorArgs = {"2033", "trace"};
    StartCloudConnector(cloudConnectorArgs, CLOUD_CONNECTOR_EXEC_PATH);

    // Sleep to ensure the server has time to start
    std::this_thread::sleep_for(std::chrono::seconds(5));
    //start RTLite
    std::vector<std::string> args = {"127.0.0.1", "2033", "trace"};
    bool isFoundExpectedMsg = RunRTLiteAndCheckLicense(args, INVALID_LICENSE_MSG);
    BOOST_CHECK(isFoundExpectedMsg);
    //stop cloudConnector
    StopCloudConnector();
}

BOOST_AUTO_TEST_CASE(InValid_Property_Id_Test_Case) {
    ClearLogFile(LOG_FILE_PATH);
    UpdateLicense("640","demo_license_invalid");
    //start cloudConnector
    std::vector<std::string> cloudConnectorArgs = {"2033", "trace"};
    StartCloudConnector(cloudConnectorArgs, CLOUD_CONNECTOR_EXEC_PATH);

    // Sleep to ensure the server has time to start
    std::this_thread::sleep_for(std::chrono::seconds(5));
    //start RTLite
    std::vector<std::string> args = {"127.0.0.1", "2033", "trace"};
    bool isFoundExpectedMsg = RunRTLiteAndCheckLicense(args, VALIDATED_LICENSE_MSG);
    BOOST_CHECK(isFoundExpectedMsg);
    //stop cloudConnector
    StopCloudConnector();
}

BOOST_AUTO_TEST_CASE(Large_License_Key_Test_Case) {
    ClearLogFile(LOG_FILE_PATH);
    std::string largeKey = "5rul86Oxox07wqWLvpUI1lgAeSLw7jFCHqRgqiaPmmqnnmKCEaplosZerIPZeV8026K8Rmjbc3uCiFcJow2o7lRXcrvNuVsclBMR";
    UpdateLicense("100",largeKey);
    //start cloudConnector
    std::vector<std::string> cloudConnectorArgs = {"2033", "trace"};
    StartCloudConnector(cloudConnectorArgs, CLOUD_CONNECTOR_EXEC_PATH);

    // Sleep to ensure the server has time to start
    std::this_thread::sleep_for(std::chrono::seconds(5));
    //start RTLite
    std::vector<std::string> args = {"127.0.0.1", "2033", "trace"};
    bool isFoundExpectedMsg = RunRTLiteAndCheckLicense(args, VALIDATED_LICENSE_MSG);
    BOOST_CHECK(isFoundExpectedMsg);
    //stop cloudConnector
    StopCloudConnector();
}
