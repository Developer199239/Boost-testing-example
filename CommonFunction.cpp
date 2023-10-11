#include "CommonFunction.hpp"

namespace bp = boost::process;
bp::child cloudConnectorProcess;  // Global variable to store the server process


int add(int a, int b) {
    return a + b;
}

// Helper function to clear/reset the log file
void ClearLogFile(std::string logFilePath) {
    // Open the file in truncation mode to clear/reset its contents
    std::ofstream logFile(logFilePath, std::ios::trunc);

    if (!logFile.is_open()) {
        BOOST_FAIL("Failed to open the log file for truncation: " << logFilePath);
    } else {
        logFile.close();
        BOOST_TEST_MESSAGE("Log file " << logFilePath << " has been cleared/reset.");
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



void StartCloudConnector(const std::vector<std::string>& args, std::string cloudConnectorPath) {
    std::string cmd = "./RTCloudConnector"; // Assuming RTCloudConnector is in the current working directory
    for (const std::string& arg : args) {
        cmd += " " + arg;
    }

    BOOST_TEST_MESSAGE("Running command: " << cmd);

    std::string buildDir = cloudConnectorPath;
    if (chdir(buildDir.c_str()) != 0) {
        BOOST_FAIL("Failed to change working directory to: " << buildDir);
        return;
    }

    // Run RTCloudConnector with the specified arguments in a separate thread
    std::thread([cmd]() {
        cloudConnectorProcess = bp::child(cmd, bp::std_out > bp::null, bp::std_err > bp::null);
        cloudConnectorProcess.wait();
    }).detach();
}

void StopCloudConnector() {
    if (cloudConnectorProcess.running()) {
        BOOST_TEST_MESSAGE("Stopping RTCloudConnector...");
        cloudConnectorProcess.terminate(); // Terminate the process
        cloudConnectorProcess.wait();      // Wait for the process to finish
    } else {
        BOOST_FAIL("RTCloudConnector not running");
    }
}
