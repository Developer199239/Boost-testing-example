#ifndef CommonFunction_HPP
#define CommonFunction_HPP

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

int add(int a, int b);
void ClearLogFile(std::string logFilePath);
bool IsStringInFile(const std::string& filePath, const std::string& searchString);
void StartCloudConnector(const std::vector<std::string>& args, std::string cloudConnectorPath);
void StopCloudConnector();

#endif
