#ifndef CPPRESTSDK_INCLUDE_API_HPP_
#define CPPRESTSDK_INCLUDE_API_HPP_

#include <iostream>
#include <string>
#include <unordered_map>

// Reference: Microsoft cpprestsdk (https://github.com/microsoft/cpprestsdk)
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>

using namespace std;
using namespace utility;                // Common utilities like string conversions
using namespace web;                    // Common features like URIs
using namespace web::http;              // Common HTTP functionality
using namespace web::http::client;      // HTTP client features
using namespace concurrency::streams;   // Asynchronous streams

// Start API
void callStartApi(json::value& body, int& time, int& problemNum);

// Location API
void callLocationApi(vector<vector<int>>& locationInfo, int& mapSize);

// Truck API
void callTruckApi(unordered_map<int, pair<int, int>>& truckInfo);

// Simulate API
void callSimulateApi(json::value& body, string& serverStatus, string& dist, int& time, int& failCnt);

// Score API
void callScoreApi();

#endif //CPPRESTSDK_INCLUDE_API_HPP_
