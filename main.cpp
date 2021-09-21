#include <iostream>
#include <string>

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

string BASE_URL = "https://kox947ka1a.execute-api.ap-northeast-2.amazonaws.com/prod/users";
string X_AUTH_TOKEN = "1a816001af873ef8b8efec6b36378fc0";
string AUTH_KEY = "";

// Start API
void callStartApi(json::value& body, int& time, int& problemNum) {
	http_request request(methods::POST);
	request.headers().add("X-Auth-Token", X_AUTH_TOKEN);
	request.headers().add("Content-Type", "application/json");
	request.set_body(body);

	http_client(U(BASE_URL + "/start")).request(request)
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("Start API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([&](json::value jsonObject) {
		  AUTH_KEY = jsonObject[U("auth_key")].as_string();
		  problemNum = jsonObject[U("problem")].as_integer();
		  time = jsonObject[U("time")].as_integer();
		}).wait();
}

// Location API
void callLocationApi(unordered_map<int, int>& location_info) {
	http_request request(methods::GET);
	request.headers().add("Authorization", AUTH_KEY);
	request.headers().add("Content-Type", "application/json");

	http_client(U(BASE_URL + "/locations")).request(request)
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("Location API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([](json::value jsonObject) {
		  return jsonObject[U("locations")];
		})
		.then([&](json::value jsonObject) {
		  auto array = jsonObject.as_array();
		  for (auto& item : array) {
			  int id = item[U("id")].as_integer();
			  int count = item[U("located_bikes_count")].as_integer();
			  location_info[id] = count;
		  }
		}).wait();
}

// Truck API
void callTruckApi(unordered_map<int, pair<int, int>>& truck_info) {
	http_request request(methods::GET);
	request.headers().add("Authorization", AUTH_KEY);
	request.headers().add("Content-Type", "application/json");

	http_client(U(BASE_URL + "/trucks")).request(request)
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("Truck API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([](json::value jsonObject) {
		  return jsonObject[U("trucks")];
		})
		.then([&](json::value jsonObject) {
		  auto array = jsonObject.as_array();
		  for (auto& item : array) {
			  int id = item[U("id")].as_integer();
			  int locId = item[U("location_id")].as_integer();
			  int count = item[U("loaded_bikes_count")].as_integer();
			  truck_info[id] = make_pair(locId, count);
		  }
		}).wait();
}

// Simulate API
void callSimulateApi(json::value& body, string& serverStatus, string& dist, int& time, int& failCnt) {
	http_request request(methods::PUT);
	request.headers().add("Authorization", AUTH_KEY);
	request.headers().add("Content-Type", "application/json");
	request.set_body(body);

	http_client(U(BASE_URL + "/simulate")).request(request)
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("Simulate API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([&](json::value jsonObject) {
		  serverStatus = jsonObject[U("status")].as_string();
		  time = jsonObject[U("time")].as_integer();
		  // failCnt = jsonObject[U("failed_requests_count")].as_integer();
		  dist = jsonObject[U("distance")].as_string();
		}).wait();
}

int main() {
	int time = -1, problemNum = -1, failCnt = 0;
	string serverStatus = "Init", dist = "Init";

	unordered_map<int, int> location_info;
	unordered_map<int, pair<int, int>> truck_info;

	json::value startBody;
	startBody[U("problem")] = json::value::number(1);

	json::value commands;
	commands[U("commands")][0][U("truck_id")] = json::value::number(0);
	commands[U("commands")][0][U("command")] = json::value::array({ 2, 5, 4, 1, 6 });

	try {
		callStartApi(startBody, time, problemNum);
		callLocationApi(location_info);
		callTruckApi(truck_info);
		callSimulateApi(commands, serverStatus, dist, time, failCnt);
	}
	catch (const exception& e) {
		printf("Error exception: %s\n", e.what());
	}

	cout << "Location Information" << endl;
	for (auto item : location_info)
		cout << item.first << ", " << item.second << "\n";
	cout << endl;

	cout << "Truck Information" << endl;
	for (auto item : truck_info)
		cout << item.first << ", (" << item.second.first << ", " << item.second.second << ")\n";
	cout << endl;

	cout << "AUTH_KEY: " << AUTH_KEY << endl;
	cout << "Problem: " << problemNum << endl;
	cout << "Server Status: " << serverStatus << endl;
	cout << "Time: " << time << endl;
	cout << "Distance: " << dist << endl;
	cout << "Fail Count: " << failCnt << endl;

	return 0;
}