#ifndef CPPRESTSDK_INCLUDE_API_HPP_
#define CPPRESTSDK_INCLUDE_API_HPP_

#include <iostream>
#include <string>
#include <unordered_map>

// Reference: Microsoft cpprestsdk (https://github.com/microsoft/cpprestsdk), 2021.09.25
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
void callStartApi(json::value& body, int& problemNum, int& time);

// WaitingLine API
void callWaitingLineApi(unordered_map<int, int>& waitingInfo);

// GameResult API
void callGameResultApi(vector<vector<int>>& gameResult);

// UserInfo API
void callUserInfoApi(unordered_map<int, int>& userInfo);

// Match API
void callMatchApi(json::value& body, string& serverStatus, int& time);

// ChangeGrade API
void callChangeGradeApi(json::value& body, string& serverStatus);

// Score API
void callScoreApi();

#endif //CPPRESTSDK_INCLUDE_API_HPP_
