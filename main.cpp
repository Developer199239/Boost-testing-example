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


std::string LOG_FILE_PATH = "/Users/user/Downloads/today/oct_5/RTLite+Logger/Client/build/m_RTLite.log";
std::string LICENSE_FOLDER_PATH = "/Users/user/Downloads/today/oct_5/RTLite+Logger/Client/xmls/";
std::string CLOUD_CONNECTOR_EXEC_PATH = "/Users/user/Downloads/today/oct_5/RTCloudConnector/Server/build/";
std::string INVALID_LICENSE_MSG = "Invalid License Key!";
std::string VALIDATED_LICENSE_MSG = "License has been successfully validated.";

namespace bp = boost::process;
// Global variable to store the child process
bp::child cloudConnectorProcess;

// Helper function to clear/reset the log file
void ClearLogFile() {
    // Open the file in truncation mode to clear/reset its contents
    std::ofstream logFile(LOG_FILE_PATH, std::ios::trunc);

    if (!logFile.is_open()) {
        BOOST_FAIL("Failed to open the log file for truncation: " << LOG_FILE_PATH);
    } else {
        logFile.close();
        BOOST_TEST_MESSAGE("Log file " << LOG_FILE_PATH << " has been cleared/reset.");
    }
}

void UpdateLicense(std::string newLicense){
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
                    tree.put("sitekey.license_key", newLicense);

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

// Helper function to check if a string is present in a file
bool IsStringInFile(const std::string& filePath, const std::string& searchString) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;  // Unable to open the file
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find(searchString) != std::string::npos) {
            file.close();
            return true;  // Found the string in the file
        }
    }

    file.close();
    return false;  // String not found in the file
}

std::string RunRTLite(const std::vector<std::string>& args) {
    bp::ipstream out;
    bp::ipstream err;

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

    // Run RTLite with the specified arguments
    bp::child c(cmd, bp::std_out > out, bp::std_err > err);

    // Sleep for the specified duration (e.g., 5 seconds)
    std::this_thread::sleep_for(std::chrono::seconds(50));

    // Send a termination signal to RTLite
    c.terminate();

    // Wait for the child process to finish
    int exitCode = c.exit_code();

    if (exitCode == 0) {
        return "";
    } else {
        BOOST_TEST_MESSAGE("RTLite Exit Code: " << exitCode);
        return "";
    }
}

std::string RunCloudConnector(const std::vector<std::string>& args) {
    bp::ipstream out;
    bp::ipstream err;

    // Construct the command for running RTCloudConnector
    std::string cmd = "./RTCloudConnector"; // Assuming RTCloudConnector is in the current working directory
    for (const std::string& arg : args) {
        cmd += " " + arg;
    }

    BOOST_TEST_MESSAGE("Running command: " << cmd);

    // Change the working directory to the build directory
    std::string buildDir = CLOUD_CONNECTOR_EXEC_PATH;
    if (chdir(buildDir.c_str()) != 0) {
        BOOST_FAIL("Failed to change working directory to: " << buildDir);
        return "";
    }

    // Run RTCloudConnector with the specified arguments
    cloudConnectorProcess = bp::child(cmd, bp::std_out > out, bp::std_err > err);

    // Sleep for the specified duration (e.g., 5 seconds)
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Return any relevant output here

    return "";
}

void StopCloudConnector(){
    if (cloudConnectorProcess.running()) {
        BOOST_TEST_MESSAGE("Stopping RTCloudConnector...");
        cloudConnectorProcess.terminate(); // Terminate the process
        cloudConnectorProcess.wait();      // Wait for the process to finish
    } else {
        BOOST_FAIL("RTCloudConnector not running");
    }
}

BOOST_AUTO_TEST_CASE(Valid_License_Test_Case) {
    ClearLogFile();
    UpdateLicense("demo_license");
    std::vector<std::string> args = {"127.0.0.1", "2033", "trace"};
    RunRTLite(args);
    BOOST_CHECK(IsStringInFile(LOG_FILE_PATH, VALIDATED_LICENSE_MSG));

    // std::vector<std::string> cloudConnectorArgs = {"2033", "trace"};
    // RunCloudConnector(cloudConnectorArgs);
    // StopCloudConnector();
}
