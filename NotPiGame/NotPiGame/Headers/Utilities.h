#pragma once

#include "../glm/include/glm.hpp"
#include "../glm/include/gtc/matrix_transform.hpp"

#define TRUE  1
#define FALSE 0
#define PI 3.141592f
#define EPSILON 0.000001
#define DEG2RAD(x) (x*PI)/180
#define RAD2DEG(x) x*(180/PI)

#undef min
#undef max

#include <string>
#include <algorithm>
#include <random>

// Define this if you want to check for any debugging code such as glGetError
/*
#define ACTIVATE_ERROR_CHECKING
//*/

namespace Utilities {
	
#pragma region Debug
	inline void printVec3(glm::vec3 vec) { printf("X: %f| Y: %f| Z: %f\n", vec.x, vec.y, vec.z); }
	inline void printMat4(glm::mat4 mat4) {
		for (int i = 0; i < 4; i++) {
			float one	= mat4[i][0];
			float two	= mat4[i][1];
			float three	= mat4[i][2];
			float four  = mat4[i][3];
			printf("%f | %f | %f | %f |\n", one, two, three, four);
		}
	}
#pragma endregion
	
#pragma region Other Utilities
	inline float Rand(float a_Range) { return ((float)rand() / RAND_MAX) * a_Range; }

	inline int randomIntNumRange(int min, int max) {
		int n = max - min + 1;
		int remainder = RAND_MAX % n;
		int x;
		do {
			x = rand();
		} while (x >= RAND_MAX - remainder);
		return min + x % n;
	}
	inline float randomFloatNumRange(float a, float b) {
		float random = ((float) rand()) / (float) RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}
	
	inline bool rangeIntersect(float min0, float max0, float min1, float max1) {
		return (std::max(min0, max0) >= std::min(min1, max1) && std::min(min0, max0) <= std::max(min1, max1));
	}
	
	inline float calculateSquaredDistance(glm::vec3 a, glm::vec3 b) {
		float distanceSquared;
		float squaredX = (b.x - a.x)*(b.x - a.x);
		float squaredY = (b.y - a.y)*(b.y - a.y);
		float squaredZ = (b.z - a.z)*(b.z - a.z);
		distanceSquared = squaredX + squaredY + squaredZ;
		
		return distanceSquared;
	}
	
	inline float calculateDistance(glm::vec3 a, glm::vec3 b) {
		float distance;
		float squaredX = (b.x - a.x)*(b.x - a.x);
		float squaredY = (b.y - a.y)*(b.y - a.y);
		float squaredZ = (b.z - a.z)*(b.z - a.z);
		distance = squaredX + squaredY + squaredZ;
		
		return sqrt(distance);
	}
	
	inline std::string seperateIntFromString(std::string str) {
		std::string seperatedString = "";
		for (int i = 0; i < str.size(); i++) {
			if (!isdigit(str[i]) && str[i] != '\000') {  // If the character is a number then don't add it to the string
				seperatedString.push_back(str[i]);
		} }
		return seperatedString;
	}
	
	inline void seperateIntFromStringAndStore(std::string originalString, std::string &str, std::string &strNums) {
		str = "";
		strNums = "";
		for (int i = 0; i < originalString.size(); i++) {
			if (!isdigit(originalString[i]) && originalString[i] != '\000') {  // If the character is a number then don't add it to the string
				str.push_back(originalString[i]);
			} else if (isdigit(originalString[i])) {
				strNums.push_back(originalString[i]);
			} else if (originalString[i] == '\000') {
				break;
			}
		} 
	}
	
	inline std::string seperateStringFromInt(std::string str) {
		std::string seperatedString = "";
		for (int i = 0; i < str.size(); i++) {
			if (isdigit(str[i])) {  // If the character is a number then add it to the string
				seperatedString.push_back(str[i]);
			}
		}
		return seperatedString;
	}
	
#pragma endregion
	
} // End of namespace Utilities