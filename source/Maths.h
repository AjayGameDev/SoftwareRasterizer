#pragma once

#include <cstdint>

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

    Vector3 operator/(float value)
    {
        return Vector3(this->x / value,this->y / value,this->z / value);
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

    Vector2 operator/(float value)
    {
        return Vector2(this->x / value,this->y / value);
    }

    Vector2& operator*=(float value)
    {
        this->x *= value;
        this->y *= value;
        return *this;
    }

}; 

struct Vector2_int32
{
    int32_t x,y;

    Vector2_int32() 
    { 
        x = y = 0; 
    }

    Vector2_int32(int32_t value)
    {
        x = y = value;
    }

    Vector2_int32(int32_t x,int32_t y) 
    {
        this->x = x;
        this->y = y;
    }

    Vector2_int32 operator+(Vector2_int32 other)
    {
        return Vector2_int32(this->x+other.x,this->y+other.y);
    } 

    Vector2_int32 operator-(Vector2_int32 other)
    {
        return Vector2_int32(this->x-other.x,this->y-other.y);
    }

    Vector2_int32 operator*(int32_t value)
    {
        return Vector2_int32(this->x * value,this->y * value);
    }

    Vector2_int32& operator*=(int32_t value)
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
    float nearPlane;
    float fov; 
    float focalLength;
}; 


Vector3 RotateAroundZ(Vector3& inputVector,float angle);
Vector3 RotateAroundX(Vector3& inputVector,float angle);
Vector3 RotateAroundY(Vector3& inputVector,float angle); 

void RotateAroundX(Vector3& inputVector1,Vector3& inputVector2,Vector3& inputVector3,float sinAngle,float cosAngle);
void RotateAroundY(Vector3& inputVector1,Vector3& inputVector2,Vector3& inputVector3,float sinAngle,float cosAngle);
void RotateAroundZ(Vector3& inputVector1,Vector3& inputVector2,Vector3& inputVector3,float sinAngle,float cosAngle);


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


inline int64_t CrossProduct_int64 (Vector2_int32& a, Vector2_int32& b)
{
    // cast one operator instead of casting after multiplication
    int64_t area = static_cast<int64_t>(a.x) * b.y - static_cast<int64_t>(a.y) * b.x;
    return area; 
}


inline float min(float a,float b)
{
    if(a<b)
        return a;
    return b;
}

inline float min(float a,float b, float c)
{
    float d = min(a,b);
    return min(d,c);
} 

inline float max(float a,float b)
{
    if(a>b)
        return a;
    return b;
}

inline float max(float a,float b, float c)
{
    float d = max(a,b);
    return max(d,c);
}

inline int32_t min_int32(int32_t a,int32_t b)
{
    if(a<b)
        return a;
    return b;
}

inline int32_t min_int32(int32_t a,int32_t b, int32_t c)
{
    int32_t d = min_int32(a,b);
    return min_int32(d,c);
} 

inline int32_t max_int32(int32_t a,int32_t b)
{
    if(a>b)
        return a;
    return b;
}

inline int32_t max_int32(int32_t a,int32_t b, int32_t c)
{
    int32_t d = max_int32(a,b);
    return max_int32(d,c);
}
