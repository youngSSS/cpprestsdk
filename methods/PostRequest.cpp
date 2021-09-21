#include <iostream>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>

using namespace std;
using namespace utility;                // Common utilities like string conversions
using namespace web;                    // Common features like URIs
using namespace web::http;                // Common HTTP functionality
using namespace web::http::client;      // HTTP client features
using namespace concurrency::streams;   // Asynchronous streams

string BASE_URL = "https://kox947ka1a.execute-api.ap-northeast-2.amazonaws.com/prod/users";
string X_AUTH_TOKEN = "1a816001af873ef8b8efec6b36378fc0";

// POST
auto postRequest() {
	auto postJson = pplx::create_task([]() {
	  json::value jsonObject;
	  jsonObject[U("problem")] = json::value::number(1);

	  http_request request(methods::POST);
	  request.headers().add("X-Auth-Token", X_AUTH_TOKEN);
	  request.headers().add("Content-Type", "application/json");
	  request.set_body(jsonObject.serialize());

	  return http_client(U(BASE_URL + "/start")).request(request);
	})
		.then([](http_response response) {
		  if (response.status_code() != 200)
			  throw runtime_error("Returned - " + to_string(response.status_code()));
		  return response.extract_json();
		})
		.then([](json::value jsonObject) {
		  cout << "\n== POST request 확인 ==\n"
			   << jsonObject[U("auth_key")].as_string() << "\n"
			   << jsonObject[U("problem")] << "\n"
			   << jsonObject[U("time")] << "\n";
		});

	return postJson;
}