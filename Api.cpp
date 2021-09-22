#include "include/Api.hpp"
#include "include/Utils.hpp"

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
void callLocationApi(vector<vector<int>>& locationInfo, int& mapSize) {
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

			  pair<int, int> pos = getLocationIndex(id, mapSize);
			  locationInfo[pos.first][pos.second] = count;
		  }
		}).wait();
}

// Truck API
void callTruckApi(unordered_map<int, pair<int, int>>& truckInfo) {
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
			  truckInfo[id] = make_pair(locId, count);
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
		  failCnt = stoi(jsonObject[U("failed_requests_count")].as_string());
		  dist = jsonObject[U("distance")].as_string();
		}).wait();
}

// Score API
void callScoreApi() {
	http_request request(methods::GET);
	request.headers().add("Authorization", AUTH_KEY);
	request.headers().add("Content-Type", "application/json");

	http_client(U(BASE_URL + "/score")).request(request)
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("Location API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([](json::value jsonObject) {
		  return jsonObject[U("score")];
		})
		.then([&](json::value jsonObject) {
		  cout << ">> Score: " << jsonObject.as_integer() << endl;
		}).wait();
}