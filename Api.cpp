#include "include/Api.hpp"
#include "include/Utils.hpp"

string BASE_URL = "https://huqeyhi95c.execute-api.ap-northeast-2.amazonaws.com/prod";
string X_AUTH_TOKEN = "6b2fe2e24d31396e0a6a4ed1ddbd5ded";
string AUTH_KEY = "";

// Start API
void callStartApi(json::value& body, int& problemNum, int& time) {
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

// WaitingLine API
void callWaitingLineApi(unordered_map<int, int>& waitingInfo) {
	http_request request(methods::GET);
	request.headers().add("Authorization", AUTH_KEY);
	request.headers().add("Content-Type", "application/json");

	http_client(U(BASE_URL + "/waiting_line")).request(request)
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("WaitingLine API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([](json::value jsonObject) {
		  return jsonObject[U("waiting_line")];
		})
		.then([&](json::value jsonObject) {
		  auto array = jsonObject.as_array();
		  for (auto& item : array) {
			  int id = item[U("id")].as_integer();
			  int from = item[U("from")].as_integer();

			  waitingInfo[id] = from;
		  }
		}).wait();
}

// GameResult API
void callGameResultApi(vector<vector<int>>& gameResult) {
	http_request request(methods::GET);
	request.headers().add("Authorization", AUTH_KEY);
	request.headers().add("Content-Type", "application/json");

	http_client(U(BASE_URL + "/game_result")).request(request)
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("GameResult API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([](json::value jsonObject) {
		  return jsonObject[U("game_result")];
		})
		.then([&](json::value jsonObject) {
		  auto array = jsonObject.as_array();
		  for (auto& item : array) {
			  int win = item[U("win")].as_integer();
			  int lose = item[U("lose")].as_integer();
			  int taken = item[U("taken")].as_integer();

			  gameResult.push_back({ win, lose, taken });
		  }
		}).wait();
}

// UserInfo API
void callUserInfoApi(unordered_map<int, int>& userInfo) {
	http_request request(methods::GET);
	request.headers().add("Authorization", AUTH_KEY);
	request.headers().add("Content-Type", "application/json");

	http_client(U(BASE_URL + "/user_info")).request(request)
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("UserInfo API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([](json::value jsonObject) {
		  return jsonObject[U("user_info")];
		})
		.then([&](json::value jsonObject) {
		  auto array = jsonObject.as_array();
		  for (auto& item : array) {
			  int id = item[U("id")].as_integer();
			  int grade = item[U("grade")].as_integer();

			  userInfo[id] = grade;
		  }
		}).wait();
}

// Match API
void callMatchApi(json::value& body, string& serverStatus, int& time) {
	http_request request(methods::PUT);
	request.headers().add("Authorization", AUTH_KEY);
	request.headers().add("Content-Type", "application/json");
	request.set_body(body);

	http_client(U(BASE_URL + "/match")).request(request)
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("Match API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([&](json::value jsonObject) {
		  serverStatus = jsonObject[U("status")].as_string();
		  time = jsonObject[U("time")].as_integer();
		}).wait();
}

// ChangeGrade API
void callChangeGradeApi(json::value& body, string& serverStatus) {
	http_request request(methods::PUT);
	request.headers().add("Authorization", AUTH_KEY);
	request.headers().add("Content-Type", "application/json");
	request.set_body(body);

	http_client(U(BASE_URL + "/change_grade")).request(request)
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("ChangeGrade API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([&](json::value jsonObject) {
		  serverStatus = jsonObject[U("status")].as_string();
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
			  throw runtime_error("Score API - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([&](json::value jsonObject) {
		  cout << jsonObject[U("status")] << endl;
		  cout << jsonObject[U("efficiency_score")] << endl;
		  cout << jsonObject[U("accuracy_score1")] << endl;
		  cout << jsonObject[U("accuracy_score2")] << endl;
		  cout << jsonObject[U("score")] << endl;
		}).wait();
}