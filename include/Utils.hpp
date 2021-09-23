#ifndef CPPRESTSDK__UTILS_HPP_
#define CPPRESTSDK__UTILS_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Translate Location Id to Coordinates
pair<int, int> getLocationIndex(int locationId, int mapSize);

// Translate Coordinate to Location Id
int getLocationId(int x, int y, int mapSize);

// Print Data
void printData(vector<vector<int>> locationInfo, unordered_map<int, pair<int, int>> truckInfo,
	int mapSize, int problemNum, string serverStatus, int time, string dist, int failCnt);

#endif //CPPRESTSDK__UTILS_HPP_
