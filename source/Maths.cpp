#include "Maths.h"
#include "math.h"


Vector3 RotateAroundZ(Vector3 inputVector,float angle)
{
    Vector3 rotatedVector;

    rotatedVector.x = inputVector.x * std::cos(angle) - inputVector.y * std::sin(angle);
    rotatedVector.y = inputVector.y * std::cos(angle) + inputVector.x * std::sin(angle);
    rotatedVector.z = inputVector.z;

    return rotatedVector;
}

Vector3 RotateAroundX(Vector3 inputVector,float angle)
{
    Vector3 rotatedVector;

    rotatedVector.x = inputVector.x;
    rotatedVector.y = inputVector.y * std::cos(angle) - inputVector.z * std::sin(angle);
    rotatedVector.z = inputVector.z * std::cos(angle) + inputVector.y * std::sin(angle);

    return rotatedVector;
}

Vector3 RotateAroundY(Vector3 inputVector,float angle)
{
    Vector3 rotatedVector;

    rotatedVector.x = inputVector.x  * std::cos(angle) + inputVector.z * std::sin(angle);
    rotatedVector.y = inputVector.y;
    rotatedVector.z = inputVector.z * std::cos(angle) - inputVector.x * std::sin(angle);

    return rotatedVector;
}