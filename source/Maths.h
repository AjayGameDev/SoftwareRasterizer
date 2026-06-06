#pragma once

constexpr float DegreeToRadians = .0174533f;

struct Vector3
{
    float x,y,z;


    Vector3() 
    { 
        x = y = z = 0; 
    }

    Vector3(float value)
    {
        x = y = z = value;
    }

    Vector3(float x,float y,float z) 
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

}; 



struct Vector2
{
    float x,y;

    Vector2() 
    { 
        x = y = 0; 
    }

    Vector2(float value)
    {
        x = y = value;
    }

    Vector2(float x,float y) 
    {
        this->x = x;
        this->y = y;
    }

}; 


struct Camera
{
    Vector3 position;
    Vector3 rotation;
    float fov; 
}; 


Vector3 RotateAroundZ(Vector3 inputVector,float angle);
Vector3 RotateAroundX(Vector3 inputVector,float angle);
Vector3 RotateAroundY(Vector3 inputVector,float angle);
