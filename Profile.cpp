#include "Profile.h"
#include <iostream>
#include <fstream>

void Profile::saveToFile() {
	std::ofstream file(std::to_string(std::hash<std::string>() (profileName)));
	file << playerName << std::endl;
	file << std::to_string(gender) << std::endl;
	file << std::to_string(age) << std::endl;
	file << std::to_string(height) << std::endl;
	file << std::to_string(weight) << std::endl;
	file.close();
}

Profile Profile::readFromFile(const std::string& path) {
	std::ifstream file(path);
	char nameBuf[1024] = {0};
	file.getline(nameBuf, 32);
	char genderBuf[2] = {0};
	file.getline(genderBuf, 2);
	char ageBuf[3] = {0};
	file.getline(ageBuf, 3);
	char heightBuf[3] = {0};
	file.getline(heightBuf, 3);
	char weightBuf[3] = {0};
	file.getline(weightBuf, 3);
	return {"curr", nameBuf, genderBuf[0] == 0 ? MALE : FEMALE, atoi(ageBuf), atoi(heightBuf), atoi(weightBuf)};
}

Profile::Profile(const std::string& basicString, const std::string& basicString1, Gender gender1, int i, int i1, int i2) {
	profileName = basicString;
	playerName = basicString1;
	gender = gender1;
	age = i;
	height = i1;
	weight = i2;
}
