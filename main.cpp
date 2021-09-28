#include "include/Api.hpp"
#include "include/Utils.hpp"

#include <set>

json::value changeGrade(unordered_map<int, int>& userInfo, vector<vector<int>> gameResult) {
	json::value retVal;

	if (gameResult.empty()) {
		retVal[U("commands")] = json::value::array({});
		return retVal;
	}

	int idx = 0;
	for (int i = 0; i < gameResult.size(); i++) {
		int winner = gameResult[i][0];
		int loser = gameResult[i][1];
		int taken = gameResult[i][2];

		float gap = (float)((float)(40 - taken) / (float)35) * 99000;
		int gradeGap = ((int)(gap) / 10);

		if (userInfo[loser] + gradeGap < 10000) {
			retVal[U("commands")][idx][U("id")] = winner;
			retVal[U("commands")][idx][U("grade")] = userInfo[loser] + gradeGap;
			idx += 1;
		}
		else if (userInfo[winner] - gradeGap >= 0) {
			retVal[U("commands")][idx][U("id")] = loser;
			retVal[U("commands")][idx][U("grade")] = userInfo[winner] - gradeGap;
			idx += 1;
		}
		else {
			int a = 9999 - userInfo[winner];
			int b = userInfo[loser];

			gradeGap /= 2;

			if (a > gradeGap) {
				retVal[U("commands")][idx][U("id")] = winner;
				retVal[U("commands")][idx][U("grade")] = userInfo[winner] + gradeGap;
				idx += 1;
			}
			else {
				retVal[U("commands")][idx][U("id")] = winner;
				retVal[U("commands")][idx][U("grade")] = 9999;
				idx += 1;
			}

			if (b > gradeGap) {
				retVal[U("commands")][idx][U("id")] = loser;
				retVal[U("commands")][idx][U("grade")] = userInfo[loser] - gradeGap;
				idx += 1;
			}
			else {
				retVal[U("commands")][idx][U("id")] = loser;
				retVal[U("commands")][idx][U("grade")] = 0;
				idx += 1;
			}
		}
	}

	return retVal;
}

json::value matchGame(unordered_map<int, int> waitingInfo, unordered_map<int, int> userInfo, int serverTime) {
	vector<pair<int, pair<int, int>>> userList, rest;
	vector<json::value> pairs;
	json::value retVal;
	set<int> emergency;

	if (waitingInfo.empty() || waitingInfo.size() == 1) {
		retVal[U("pairs")] = json::value::array({});
		return retVal;
	}

	for (auto user : waitingInfo) {
		int id = user.first;
		int waitingTime = user.second;
		int grade = userInfo[id];
		userList.emplace_back(grade, make_pair(waitingTime, id));
	}
	sort(userList.begin(), userList.end());

	int gap = 1000;
	for (int i = 0; i < userList.size() - 1; i++) {
		if (userList[i + 1].first - userList[i].first <= gap) {
			pairs.push_back(json::value::array({ userList[i].second.second, userList[i + 1].second.second }));
			i++;
		}
		else {
			if (serverTime - userList[i].second.first >= 12) {
				if (userList[i + 1].first - userList[i].first <= gap * 2) {
					pairs.push_back(json::value::array({ userList[i].second.second, userList[i + 1].second.second }));
					i++;
				}
			}
		}
	}

	retVal[U("pairs")] = json::value::array(pairs);
	return retVal;
}

int main() {
	int problemNum = 2;
	int serverTime = -1;
	string serverStatus;

	// Call Start API
	try {
		json::value startApiBody;
		startApiBody[U("problem")] = problemNum;
		callStartApi(startApiBody, problemNum, serverTime);
	}
	catch (const exception& e) {
		printf("Error exception: %s\n", e.what());
	}

	while (serverStatus != "finished") {
		unordered_map<int, int> waitingInfo, userInfo;
		vector<vector<int>> gameResult;

		// Step 1, 유저 등급과 게임 결과 얻기
		callUserInfoApi(userInfo);
		callGameResultApi(gameResult);

		// Step 2, 게임 결과를 바탕으로 유저 등급 재계산
		json::value changeGradeBody = changeGrade(userInfo, gameResult);
		callChangeGradeApi(changeGradeBody, serverStatus);

		// Step 3, 대기 목록 얻기
		callWaitingLineApi(waitingInfo);

		// Step 4, 대기 목록에서 매치 성사
		json::value matchBody = matchGame(waitingInfo, userInfo, serverTime);
		callMatchApi(matchBody, serverStatus, serverTime);

		if (serverTime % 100 == 0) cout << serverTime << endl;
	}

	// Get Score
	try {
		callScoreApi();
	}
	catch (const exception& e) {
		printf("Error exception: %s\n", e.what());
	}

	return 0;
}