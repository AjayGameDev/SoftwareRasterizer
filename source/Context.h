#pragma once

#include <cmath>
#include <memory>
#include "Maths.h"

/*

 x must be non negative integer and smaller than bit width(for 32 bit int x = [0,31])

 y << x = y * 2^x basically multiplyin y by 2 power x
 y >> x = y / 2^x basically dividing y by 2 power x and rounding to nearest int(floor)

*/

constexpr  uint32_t  window_width                =   640 * 2;  
constexpr  uint32_t  window_height               =   480 * 2;  
constexpr  uint32_t  logical_width               =   64 * 4;
constexpr  uint32_t  logical_height              =   48 * 4;
constexpr  float     window_halfWidth            =   logical_width / 2.0f; 
constexpr  float     window_halfHeight           =   logical_height / 2.0f; 
constexpr  uint32_t  totalPoints                 =   9 * 9 * 9; 
constexpr  float     projectedPointsScaleFactor  =   400;
constexpr  int32_t   subpixelShift               =   4; 
constexpr  int32_t   subpixelMultiplier          =   1 << subpixelShift; // = 1 * 2^4 = 16
constexpr  int32_t   half                        =   subpixelMultiplier / 2; // = 8

class SDL_Texture;
class SDL_Window;
class SDL_Renderer;
class Vector2;
class Vector3;
class Model; 

struct Input
{
    float deltaX,deltaY;
    float targetDistance;
};

class Context
{
    bool isRunning = true;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    Input input;
    
    
    uint32_t* colorBuffer           = nullptr; 
    float* depthBuffer              = nullptr;
    SDL_Texture* colorBufferTexture = nullptr;  

    std::unique_ptr<Model> shotgun;
    std::unique_ptr<Model> hands;
    
    public:
    
    Camera camera; 
    Vector3 rotationAngles;

    Context();
    
     

    void Initialize();

    bool IsRunning() { return isRunning; }
    
    void CreateWindow();
    void CreateRenderer();

    
    void ClearColorBuffer(uint32_t color = 0xFF000000);
    void ClearDepthBuffer(float depthValue = 0.0f);
     


    void DrawPixel(uint32_t x, uint32_t y, uint32_t color);
    void DrawGrid();
    void DrawGridOnly();
    void DrawRectangle(uint32_t rectX,uint32_t rectY,uint32_t rectWidth,uint32_t rectHeight,uint32_t color);
    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawTriangle(int x1,int y1,int x2, int y2, int x3, int y3);
    void DrawTriangle(Vector2 P1,Vector2 P2,Vector2 P3);
    void DrawFilledTriangle(Vector2& P1,Vector2& P2,Vector2& P3,float z1,float z2,float z3,Vector3 normal1,Vector3 normal2,Vector3 normal3,uint32_t color = 0xFF00FFFF);
    void DrawFilledProjectedTriangle(Vector2 P1,Vector2 P2,Vector2 P3,uint32_t color = 0xFF00FFFF);


    void Render(); 
    void RenderColorBuffer();
    void RenderModel(Model& model,float scale = 1.0f);
    

    
    

    
    void ProcessInput();
    void Update(float dt);
    void Cleanup(); 


    ~Context();



    Vector2 ProjectPoint(Vector3& point)
    {
        Vector2 projectedPoint;
    
        // perspective divide
        projectedPoint.x = (point.x  * camera.focalLength) / point.z + window_halfWidth;

        // Negate it to match +y up from 3d space to +y in screen space otherwise image will be upside down
        projectedPoint.y = -(point.y  * camera.focalLength) / point.z + window_halfHeight;
        
        return projectedPoint;
    }


    int32_t ConvertToFixedPoint(float value)
    {
        return static_cast<int32_t>(std::lrintf(value * subpixelMultiplier)); // lrintf is much faster than using std::round
                                                                                // lrintf converts float to long 
                                                                                // lrint  converts double to long (since value is float so it will unnecessarly convert to double and then multiply which is slower as double math is slower by architecture)
    }


};