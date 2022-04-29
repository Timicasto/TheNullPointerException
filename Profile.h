#ifndef OPENGLC___PROFILE_H
#define OPENGLC___PROFILE_H

#include "string"

typedef enum gend {
	MALE,
	FEMALE,
	INTERSEX,
	TRANSGENDER,
	NON_BINARY,
	UNKNOWN
} Gender;

class Profile {
public:
	Profile(const std::string& basicString, const std::string& basicString1, Gender gender1, int i, int i1, int i2);

	std::string profileName;
	std::string playerName;
	Gender gender;
	int age{};
	int height{};
	int weight{};

	void saveToFile();
	static Profile readFromFile(const std::string& path);
};


#endif //OPENGLC___PROFILE_H
