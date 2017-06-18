#pragma once

#include "../glm/include/gtc/matrix_transform.hpp"

class Plane; class Ray; class Sphere; class Box; // Prototypes

class Triangle {
public:
	Triangle();
	Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, bool worldPosFlag);
	Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::mat4 objMatrix);
	~Triangle();
	
	void calculateWorldPos(glm::mat4 objMatrix);
	
	glm::vec3 v0; // Vertex0 - "Model" Space
	glm::vec3 v1; // Vertex1 - "Model" Space
	glm::vec3 v2; // Vertex2 - "Model" Space
	
	glm::vec3 p0; // Point0 - World Space
	glm::vec3 p1; // Point1 - World Space
	glm::vec3 p2; // Point2 - Wolrd Space
};

class Plane {
public:
	Plane();
	Plane(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, bool worldPosFlag);
	Plane(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::mat4 objMatrix);
	~Plane();
	
	void calculateWorldPos(glm::mat4 objMatrix);
	
	glm::vec3 v0; // Vertex0 - "Model" Space
	glm::vec3 v1; // Vertex1 - "Model" Space
	glm::vec3 v2; // Vertex2 - "Model" Space
	glm::vec3 v3; // Vertex3 - "Model" Space
	
	glm::vec3 p0; // Point0 - World Space
	glm::vec3 p1; // Point1 - World Space
	glm::vec3 p2; // Point2 - Wolrd Space
	glm::vec3 p3; // Point3 - Wolrd Space
};
	
class Ray {
public:
	Ray();
	Ray(glm::vec3 o, glm::vec3 d, bool inv = false);
	~Ray();
	
	bool rayTriangleIntersect(Triangle t, float &l);
	bool rayPlaneIntersect(Plane p, float &l);
	bool rayBoxIntersect(Box b, float &l);
		
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 inverseDirection;
	int sign[3];
};
	
class Sphere {
public:
	Sphere();
	Sphere(glm::vec3 pos, float radius);
	~Sphere();
	
	bool SphereSphereIntersect(Sphere a, Sphere b);
	void updateSpherePos(glm::vec3 pos);
	
	float radius;
	glm::vec3 pos;
};
	
class Box {
public:
	Box();
	Box(glm::vec3 pos, float width, float height, float depth);
	~Box();
	
	void updateBoxPos(glm::vec3 pos);
	bool AABBAABBIntersect(Box b);
	
	glm::vec3 pos;
	float height;
	float width;
	float depth;
	glm::vec3 bounds[2]; // bMax and bMin
};