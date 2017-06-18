#include "../Headers/Geometry.h"

#include "../Headers/Utilities.h"

#pragma region Triangle
Triangle::Triangle() {
	// Empty
}

Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, bool worldPosFlag) {
	if (!worldPosFlag) { // If the users want the triangle to be put in world coordinates or not
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
	} else {
		this->p0 = v0;
		this->p1 = v1;
		this->p2 = v2;
	}
}

Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::mat4 objMatrix) {
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
	calculateWorldPos(objMatrix);
}

Triangle::~Triangle() {
	// Empty
}

void Triangle::calculateWorldPos(glm::mat4 objMatrix) {
	glm::vec4 tempV0(this->v0, 1);
	glm::vec4 tempV1(this->v1, 1);
	glm::vec4 tempV2(this->v2, 1);
		
	tempV0 = objMatrix * tempV0;
	tempV1 = objMatrix * tempV1;
	tempV2 = objMatrix * tempV2;
				
	this->p0 = glm::vec3(tempV0.x, tempV0.y, tempV0.z);
	this->p1 = glm::vec3(tempV1.x, tempV1.y, tempV1.z);
	this->p2 = glm::vec3(tempV2.x, tempV2.y, tempV2.z);
}

#pragma endregion

#pragma region Plane

Plane::Plane() {
	// Empty
}

Plane::Plane(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, bool worldPosFlag) {
	if (!worldPosFlag) { // If the users want the triangle to be put in world coordinates or not
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
	} else {
		this->p0 = v0;
		this->p1 = v1;
		this->p2 = v2;
		this->p3 = v3;
	}
}

Plane::Plane(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::mat4 objMatrix) {
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	calculateWorldPos(objMatrix);
}

Plane::~Plane() {
	// Empty
}

void Plane::calculateWorldPos(glm::mat4 objMatrix) {
	glm::vec4 tempV0(this->v0, 0);
	glm::vec4 tempV1(this->v1, 0);
	glm::vec4 tempV2(this->v2, 0);
	glm::vec4 tempV3(this->v3, 0);
	
	tempV0 = tempV0 * objMatrix;
	tempV1 = tempV1 * objMatrix;
	tempV2 = tempV2 * objMatrix;
	tempV3 = tempV3 * objMatrix;
			
	this->p0 = glm::vec3(tempV0.x, tempV0.y, tempV0.z);
	this->p1 = glm::vec3(tempV1.x, tempV1.y, tempV1.z);
	this->p2 = glm::vec3(tempV2.x, tempV2.y, tempV2.z);
	this->p3 = glm::vec3(tempV3.x, tempV3.y, tempV3.z);
}

#pragma endregion

#pragma region Ray
Ray::Ray() {
	// Empty
}

Ray::Ray(glm::vec3 o, glm::vec3 d, bool inv) {
	this->origin	= o;
	this->direction = d;
	
	// If inv is true then we calculate the inverse direction
	if (inv) {
		this->inverseDirection = 1.0f / this->direction;
		this->sign[0] = (inverseDirection.x < 0); 
		this->sign[1] = (inverseDirection.y < 0); 
		this->sign[2] = (inverseDirection.z < 0); 
	}
}

Ray::~Ray() {
	// Empty
}

bool Ray::rayTriangleIntersect(Triangle t, float &l) {
	// Temporary Values
	glm::vec3 e1, e2; // Triangle edges
	glm::vec3 P, Q, T;
	float det, invDev, u, v;
	float distance; // This goes out
	
	// Calculate the two edges near vertex 0
	e1 = (t.p1 - t.p0);
	e2 = (t.p2 - t.p0);
	
	// Calculate the determinant - det is near zero, the ray is either inside or parallel to the triangle/plane
	P = glm::cross(this->direction, e2);
	det = glm::dot(e1, P);
	if (det > -EPSILON && det < EPSILON) { return false; }
	invDev = 1.0f / det;
	
	// Calculate distance between vertex 0 and the ray's origin
	T = (this->origin - t.p0);
	
	// Calculate the u parameters and test if it is outside u triangle
	u = glm::dot(T, P) * invDev;
	if (u < 0.0f || u > 1.0f) { return false; }
		
	// Calculate the v parameters and if it is outside of the v triangle
	Q = glm::cross(T, e1);
	v = glm::dot(this->direction, Q) * invDev;
	if (v < 0.0f || u + v > 1.0f) { return false; }	
	
	// Calculate the distance
	distance = glm::dot(e2, Q) * invDev;
	
	if (distance > EPSILON) {
		l = distance;
		return true; // There is a collision with the triangle
	}
	
	return false;
}

bool Ray::rayPlaneIntersect(Plane p, float &t) {
	return false;
}

bool Ray::rayBoxIntersect(Box b, float &t) {
	float tmin, tmax, tymin, tymax, tzmin, tzmax; 
 
	tmin	 =(b.bounds[this->sign[0]].x		- this->origin.x) * this->inverseDirection.x; 
	tmax	= (b.bounds[1 - this->sign[0]].x	- this->origin.x) * this->inverseDirection.x; 
	tymin	= (b.bounds[this->sign[1]].y		- this->origin.y) * this->inverseDirection.y; 
	tymax	= (b.bounds[1 - this->sign[1]].y	- this->origin.y) * this->inverseDirection.y; 
 
	if ((tmin > tymax) || (tymin > tmax)) 
		return false; 
	if (tymin > tmin) 
		tmin = tymin; 
	if (tymax < tmax) 
		tmax = tymax; 
 
	tzmin = (b.bounds[this->sign[2]].z		- this->origin.z) * this->inverseDirection.z; 
	tzmax = (b.bounds[1 - this->sign[2]].z	- this->origin.z) * this->inverseDirection.z; 
 
	if ((tmin > tzmax) || (tzmin > tmax)) 
		return false; 
	if (tzmin > tmin) 
		tmin = tzmin; 
	if (tzmax < tmax) 
		tmax = tzmax; 
	t	 = tmax;
	
	return true;	
}

#pragma endregion

#pragma region Sphere
Sphere::Sphere() {
	// Empty
}

Sphere::Sphere(glm::vec3 pos, float r) {
	this->pos		= pos;
	this->radius	= r;
}

Sphere::~Sphere() {
	// Empty
}

void Sphere::updateSpherePos(glm::vec3 pos) {
	this->pos = pos;
}

#pragma endregion 

#pragma region Box

Box::Box() {
	// Empty
}

Box::Box(glm::vec3 pos, float w, float h, float d) {
	this->pos		= pos;
	this->height	= h;
	this->width		= w;
	this->depth		= d;
	
	this->bounds[0] = glm::vec3(-width + this->pos.x, -height + this->pos.y, -depth + this->pos.z);
	this->bounds[1] = glm::vec3( width + this->pos.x,  height + this->pos.y,  depth + this->pos.z);
}

Box::~Box() {
	// Empty
}

void Box::updateBoxPos(glm::vec3 pos) {
	this->pos = pos;
	this->bounds[0] = glm::vec3( (-width) + pos.x, (-height) + pos.y, (-depth) + pos.z);
	this->bounds[1] = glm::vec3( ( width) + pos.x, ( height) + pos.y, ( depth) + pos.z);
}

bool Box::AABBAABBIntersect(Box b) {
		return (Utilities::rangeIntersect(this->pos.x - this->width	, this->pos.x + this->width	, b.pos.x - b.width	, b.pos.x + b.width )	&&
		        Utilities::rangeIntersect(this->pos.y - this->height, this->pos.y + this->height, b.pos.y - b.height, b.pos.y + b.height)	&&
		        Utilities::rangeIntersect(this->pos.z - this->depth	, this->pos.z + this->depth	, b.pos.z - b.depth	, b.pos.z + b.depth )	  );
}

#pragma endregion