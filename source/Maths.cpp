#include "Maths.h"
#include <cmath>


Vector3 RotateAroundZ(Vector3& inputVector,float angle)
{
    Vector3 rotatedVector;

    rotatedVector.x = inputVector.x * std::cos(angle) - inputVector.y * std::sin(angle);
    rotatedVector.y = inputVector.y * std::cos(angle) + inputVector.x * std::sin(angle);
    rotatedVector.z = inputVector.z;

    return rotatedVector;
}

Vector3 RotateAroundX(Vector3& inputVector,float angle)
{
    Vector3 rotatedVector;

    rotatedVector.x = inputVector.x;
    rotatedVector.y = inputVector.y * std::cos(angle) - inputVector.z * std::sin(angle);
    rotatedVector.z = inputVector.z * std::cos(angle) + inputVector.y * std::sin(angle);

    return rotatedVector;
}

Vector3 RotateAroundY(Vector3& inputVector,float angle)
{
    Vector3 rotatedVector;

    rotatedVector.x = inputVector.x  * std::cos(angle) + inputVector.z * std::sin(angle);
    rotatedVector.y = inputVector.y;
    rotatedVector.z = inputVector.z * std::cos(angle) - inputVector.x * std::sin(angle);

    return rotatedVector;
} 

void RotateAroundY(Vector3& inputVector1,Vector3& inputVector2,Vector3& inputVector3,float sinAngle,float cosAngle)
{
    Vector3 rotatedVector1,rotatedVector2,rotatedVector3; 

    rotatedVector1.x = inputVector1.x  * cosAngle + inputVector1.z * sinAngle;
    rotatedVector1.y = inputVector1.y;
    rotatedVector1.z = inputVector1.z  * cosAngle - inputVector1.x * sinAngle;

    rotatedVector2.x = inputVector2.x  * cosAngle + inputVector2.z * sinAngle;
    rotatedVector2.y = inputVector2.y;
    rotatedVector2.z = inputVector2.z  * cosAngle - inputVector2.x * sinAngle;

    rotatedVector3.x = inputVector3.x  * cosAngle + inputVector3.z * sinAngle;
    rotatedVector3.y = inputVector3.y;
    rotatedVector3.z = inputVector3.z  * cosAngle - inputVector3.x * sinAngle; 

    inputVector1 = rotatedVector1;
    inputVector2 = rotatedVector2;
    inputVector3 = rotatedVector3;
}

void RotateAroundZ(Vector3& inputVector1,Vector3& inputVector2,Vector3& inputVector3,float sinAngle,float cosAngle)
{
    Vector3 rotatedVector1,rotatedVector2,rotatedVector3; 

    rotatedVector1.x = inputVector1.x * cosAngle - inputVector1.y * sinAngle;
    rotatedVector1.y = inputVector1.y * cosAngle + inputVector1.x * sinAngle;
    rotatedVector1.z = inputVector1.z;

    rotatedVector2.x = inputVector2.x * cosAngle - inputVector2.y * sinAngle;
    rotatedVector2.y = inputVector2.y * cosAngle + inputVector2.x * sinAngle;
    rotatedVector2.z = inputVector2.z;

    rotatedVector3.x = inputVector3.x * cosAngle - inputVector3.y * sinAngle;
    rotatedVector3.y = inputVector3.y * cosAngle + inputVector3.x * sinAngle;
    rotatedVector3.z = inputVector3.z;

    inputVector1 = rotatedVector1;
    inputVector2 = rotatedVector2;
    inputVector3 = rotatedVector3;
}


// for left handed system, we calculate triangle normal with
// Vector3 edge1 = B - A; // A->B
// Vector3 edge2 = C - A; // A->C
// Vector3 normal = CrossProduct(edge1,edge2); // this normal points towards camera so it is front facing(clock wise)
// for right handed system you will use edge2,edge1 and front faces will be counter clockwise



