#include "pch.h"
#include "math.hpp"

Vector3 Math::CalcAngle(Vector3 src, Vector3 dst)
{
    Vector3 angles;

    // Delta
    float deltaX = dst.x - src.x;
    float deltaY = src.y - dst.y;
    float deltaZ = dst.z - src.z;

    // Calcul de la distance au sol
    float dist2D = sqrtf(deltaX * deltaX + deltaY * deltaY);


    // Si on est exactement sur la même position X/Y que target tout à zéro
    if (dist2D == 0 && deltaX == 0) {
        angles.x = 0.0f;
        angles.y = 0.0f;
        return angles;
    }

    // Calcul du YAW 

    angles.x = atan2f(deltaX, deltaY) * (180.0f / PI);


    // clamp de 0 à 360 
    if (angles.x < 0) {
        angles.x += 360.0f;
    }

    // Calcul du PITCH
    // On utilise la hauteur (deltaZ) et la distance au sol (dist2D)
    angles.y = atan2f(deltaZ, dist2D) * (180.0f / PI);

    //Roll (Z) à 0 inutile pour la visée
    angles.z = 0.0f;

    return angles;
}


Vector3 Math::CalculateForwardVector(float yaw, float pitch) {
    float radYaw = yaw * (3.14159265f / 180.0f);
    float radPitch = pitch * (3.14159265f / 180.0f);

    Vector3 forward;
    // On aligne sur CalcAngle : X = sin(yaw) et Y = cos(yaw) 
    forward.x = sin(radYaw) * cos(radPitch);  // PLUS DE SIGNE MOINS ICI !!!
    forward.y = cos(radYaw) * cos(radPitch);  // Aligné sur deltaY (src - dst)
    forward.z = sin(radPitch);

    return forward;
}


float Math::magnitude(Vector3* vecteur)
{
    return sqrt(vecteur->x * vecteur->x + vecteur->y * vecteur->y + vecteur->z * vecteur->z);
}

Vector3 Math::normalisation(Vector3* delta, float magnitude)
{
    Vector3 normalised{ 0,0,0 };

    if (magnitude == 0)
    {
        return normalised;
    }

    normalised.x = delta->x / magnitude;
    normalised.y = delta->y / magnitude;
    normalised.z = delta->z / magnitude;

    return normalised;
};

float Math::dotProduct(Vector3* forwardAngle, Vector3* targetNormalized)
{
    float fovDst{ (forwardAngle->x * targetNormalized->x) + (forwardAngle->y * targetNormalized->y) + (forwardAngle->z * targetNormalized->z) };

    return fovDst;
}

Vector3 Math::deltaPos(Vector3 src, Vector3 dst)
{
    Vector3 delta{ 0,0,0 };


    delta.x = dst.x - src.x;
    delta.y = src.y - dst.y;
    delta.z = dst.z - src.z;

    return delta;
}