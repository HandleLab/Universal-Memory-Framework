#pragma once

#include <cmath> 

struct Vector2
{
    float yaw, pitch;
};

struct Vector3
{
    float x, y, z;
};

#define PI 3.14159265358979323846f 

class Math
{
public:

	static Vector3 deltaPos(Vector3 src, Vector3 dst);

	static Vector3 CalculateForwardVector(float yaw, float pitch);

	static Vector3 CalcAngle(Vector3 src, Vector3 dst);

	static Vector3 normalisation(Vector3* vecteurPos, float magnitude);

	static float magnitude(Vector3* vecteur);

	static float dotProduct(Vector3* forwardAngle, Vector3* targetNormalized);

private:

};

