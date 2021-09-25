#include "include/Api.hpp"
#include "include/Utils.hpp"

int main() {

	// Call Start API
	try {
		callStartApi();
		callLocationApi();
		callTruckApi();
		callSimulateApi();
		callScoreApi();
	}
	catch (const exception& e) {
		printf("Error exception: %s\n", e.what());
	}

	return 0;
}