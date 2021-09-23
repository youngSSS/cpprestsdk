#include "include/Api.hpp"
#include "include/Utils.hpp"

#include <queue>

unordered_map<int, queue<int>> nextLocIdList;

vector<queue<int>> getInitCommands(int mapSize, int truckCnt) {
	vector<queue<int>> initCommands(truckCnt);
	int routineSize = mapSize * mapSize / truckCnt;
	if ((mapSize * mapSize) % truckCnt != 0) routineSize += 1;

	int locId = 0;
	int adder = 1;
	for (int i = 0; i < truckCnt; i++) {
		pair<int, int> pos = getLocationIndex(locId, mapSize);
		stack<int> s;

//		cout << locId << endl;
//		cout << pos.first << ", " << pos.second << endl;

		for (int j = 0; j < pos.first; j++) initCommands[i].push(1);
		for (int j = 0; j < pos.second; j++) initCommands[i].push(2);

		for (int j = 0; j < routineSize; j++) {
			if (j != 0 && locId % mapSize == 0 && adder == 1) {
				adder = -1;
				locId += mapSize - 1;
			}
			else if ((locId + 1) % mapSize == 0 && adder == -1) {
				adder = 1;
				locId = (locId + 1) + mapSize;
			}

			cout << locId << endl;

			nextLocIdList[i].push(locId);
			s.push(locId);
			locId += adder;

			if (locId >= mapSize * mapSize) break;
		}

		s.pop();
		while (s.size() > 1) {
			nextLocIdList[i].push(s.top());
			s.pop();
		}
	}

//	cout << "INIT COMMAND\n";
//	for (int i = 0; i < truckCnt; i++) {
//		int size = initCommands[i].size();
//		for (int j = 0; j < size; j++) {
//			int v = initCommands[i].front();
//			cout << v << " ";
//			initCommands[i].pop();
//			initCommands[i].push(v);
//		}
//		cout << endl;
//	}
//
//	cout << "NEXT LOC LIST\n";
//	for (int i = 0; i < truckCnt; i++) {
//		int size = nextLocIdList[i].size();
//		for (int j = 0; j < size; j++) {
//			int v = nextLocIdList[i].front();
//			cout << v << " ";
//			nextLocIdList[i].pop();
//			nextLocIdList[i].push(v);
//		}
//		cout << endl;
//	}
//
//	cout << "END\n";

	return initCommands;
}

json::value moveTrucks(vector<vector<int>> locationInfo, unordered_map<int, pair<int, int>> truckInfo, vector<queue<int>>& initCommands) {
	unordered_map<int, vector<json::value>> commands;
	int mapSize = locationInfo.size();
	int truckCnt = truckInfo.size();

//	cout << endl;
//	for (int i = 0; i < truckCnt; i++) {
//		cout << i << ", " << truckInfo[i].first << ", " << truckInfo[i].second << endl;
//	}
//	cout << endl;
//
//	cout << "\n";
//	for (int i = locationInfo.size() - 1; i >= 0; i--) {
//		for (int j = 0; j < locationInfo[i].size(); j++) {
//			cout << locationInfo[i][j] << " ";
//		}
//		cout << endl;
//	}
//	cout << "\n";

	// Init Trucks
	for (int i = 0; i < truckCnt; i++) {
		int cnt = 0;
		while (!initCommands[i].empty() && cnt < 10) {
			int cmd = initCommands[i].front();
			initCommands[i].pop();

			commands[i].push_back(cmd);
			cnt += 1;

			pair<int, int> pos = getLocationIndex(truckInfo[i].first, mapSize);
			if (cmd == 1) pos.first += 1;
			else if (cmd == 2) pos.second += 1;
			else if (cmd == 3) pos.first -= 1;
			else if (cmd == 4) pos.second -= 1;

			truckInfo[i].first = getLocationId(pos.first, pos.second, mapSize);
		}
	}

	for (int i = 0; i < truckCnt; i++) {
		// Initialization is not completed
		if (!initCommands[i].empty()) continue;

		int truckLocationId = truckInfo[i].first;
		pair<int, int> truckPos = getLocationIndex(truckLocationId, mapSize);

		int x = truckPos.first;
		int y = truckPos.second;
		int bikeCnt = truckInfo[i].second;

		while (commands[i].size() < 10) {
			// Get Bike
			while (locationInfo[x][y] > 2 && bikeCnt < 20 && commands[i].size() < 10) {
				commands[i].push_back(5);
				locationInfo[x][y] -= 1;
				bikeCnt += 1;
			}

			// Move
			if (commands[i].size() < 10) {
				int nextLocId = nextLocIdList[i].front();
				nextLocIdList[i].pop();
				nextLocIdList[i].push(nextLocId);

				pair<int, int> nextCoord = getLocationIndex(nextLocId, mapSize);

				if (nextCoord.first == x - 1) commands[i].push_back(3);
				else if (nextCoord.first == x + 1) commands[i].push_back(1);
				else if (nextCoord.second == y - 1) commands[i].push_back(4);
				else if (nextCoord.second == y + 1) commands[i].push_back(2);

				x = nextCoord.first;
				y = nextCoord.second;
			}

			// Release Bike
			while (locationInfo[x][y] < 3 && bikeCnt > 0 && commands[i].size() < 10) {
				commands[i].push_back(6);
				locationInfo[x][y] += 1;
				bikeCnt -= 1;
			}
		}
	}

	json::value retVal;
	for (int i = 0; i < truckCnt; i++) {
		retVal[U("commands")][i][U("truck_id")] = json::value::number(i);
		retVal[U("commands")][i][U("command")] = json::value::array(commands[i]);
	}

//	for (int i = 0; i < truckCnt; i++) {
//		cout << "Truck ID: " << i << endl;
//		for (auto a : commands[i])
//			cout << a << ", " ;
//		cout << endl;
//	}

	return retVal;
}

int main() {
	int time = -1, problemNum = -1, failCnt = 0;
	string serverStatus = "Init", dist = "Init";
	int mapSize;

	json::value startBody;
	startBody[U("problem")] = json::value::number(2);

	// Call Start API
	try {
		callStartApi(startBody, time, problemNum);
	}
	catch (const exception& e) {
		printf("Error exception: %s\n", e.what());
	}

	mapSize = problemNum == 1 ? 5 : 60;
	vector<vector<int>> locationInfo(mapSize, vector<int>(mapSize, 0));
	unordered_map<int, pair<int, int>> truckInfo;
	vector<queue<int>> initCommands = getInitCommands(mapSize, problemNum == 1 ? 5 : 10);

	json::value commands;

	while (true) {
		if (serverStatus == "finished") break;

		try {
			callLocationApi(locationInfo, mapSize);
			callTruckApi(truckInfo);
		}
		catch (const exception& e) {
			printf("Error exception: %s\n", e.what());
		}

		// Processing
		commands = moveTrucks(locationInfo, truckInfo, initCommands);

		try {
			callSimulateApi(commands, serverStatus, dist, time, failCnt);
		}
		catch (const exception& e) {
			printf("Error exception: %s\n", e.what());
		}

		if (time % 100 == 0) cout << time << ", " << failCnt << "\n";
	}

	cout << failCnt << endl;

	callScoreApi();

	return 0;
}