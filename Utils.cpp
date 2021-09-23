#include "include/Utils.hpp"

// Translate Location Id to Coordinate
pair<int, int> getLocationIndex(int locationId, int mapSize) {
	int x = locationId % mapSize;
	int y = locationId / mapSize;

	return make_pair(x, y);
}

// Translate Coordinate to Location Id
int getLocationId(int x, int y, int mapSize) {
	return x + mapSize * y;
}

void printData(vector<vector<int>> locationInfo, unordered_map<int, pair<int, int>> truckInfo,
	int mapSize, int problemNum, string serverStatus, int time, string dist, int failCnt)
{
	cout << "----------------------- DATA -----------------------\n";
	cout << "Location Information" << endl;
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++)
			cout << "ID: " << getLocationId(i, j, mapSize) << " - (" << i << ", " << j << ") - Bike Count: " << locationInfo[i][j] << "\n";
	}
	cout << endl;

	cout << "Truck Information" << endl;
	for (auto item : truckInfo)
		cout << item.first << ", (" << item.second.first << ", " << item.second.second << ")\n";
	cout << endl;

	cout << "Problem: " << problemNum << endl;
	cout << "Server Status: " << serverStatus << endl;
	cout << "Time: " << time << endl;
	cout << "Distance: " << dist << endl;
	cout << "Fail Count: " << failCnt << endl;
	cout << "----------------------------------------------------\n" << endl;
}
