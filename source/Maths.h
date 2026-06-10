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

    Vector3 operator+(Vector3 other)
    {
        return Vector3(this->x+other.x,this->y+other.y,this->z+other.z);
    } 

    Vector3 operator-(Vector3 other)
    {
        return Vector3(this->x-other.x,this->y-other.y,this->z-other.z);
    }

    Vector3 operator*(float value)
    {
        return Vector3(this->x * value,this->y * value,this->z * value);
    }

    Vector3& operator*=(float value)
    {
        this->x *= value;
        this->y *= value;
        this->z *= value;
        return *this;
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

    Vector2 operator+(Vector3 other)
    {
        return Vector2(this->x+other.x,this->y+other.y);
    } 

    Vector2 operator-(Vector2 other)
    {
        return Vector2(this->x-other.x,this->y-other.y);
    }

    Vector2 operator*(float value)
    {
        return Vector2(this->x * value,this->y * value);
    }

    Vector2& operator*=(float value)
    {
        this->x *= value;
        this->y *= value;
        return *this;
    }

}; 


struct Camera
{
    Vector3 position;
    Vector3 rotation;
    float fov; 
    float focalLength;
}; 


Vector3 RotateAroundZ(Vector3& inputVector,float angle);
Vector3 RotateAroundX(Vector3& inputVector,float angle);
Vector3 RotateAroundY(Vector3& inputVector,float angle); 

void RotateAroundY(Vector3& inputVector1,Vector3& inputVector2,Vector3& inputVector3,float sinAngle,float cosAngle);


inline float DotProduct (Vector3& a, Vector3& b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}


inline Vector3 CrossProduct (Vector3& a, Vector3& b)
{
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline float CrossProduct (Vector2& a, Vector2& b)
{
    return (a.x * b.y - a.y * b.x);
}


inline int min(int a,int b)
{
    if(a<b)
        return a;
    return b;
}

inline int min(int a,int b, int c)
{
    int d = min(a,b);
    return min(d,c);
} 

inline int max(int a,int b)
{
    if(a>b)
        return a;
    return b;
}

inline int max(int a,int b, int c)
{
    int d = max(a,b);
    return max(d,c);
}
