#pragma once

#include <cmath>
#include <memory>
#include "Maths.h"



constexpr uint32_t window_width       = 640;  
constexpr uint32_t window_height      = 480;  
constexpr uint32_t totalPoints = 9 * 9 * 9; 
constexpr float projectedPointsScaleFactor = 400;
constexpr int32_t subpixelShift      = 4; 
constexpr int32_t subpixelMultiplier = 1 << subpixelShift; 
constexpr int32_t half               = subpixelMultiplier / 2;

class SDL_Texture;
class SDL_Window;
class SDL_Renderer;
class Vector2;
class Vector3;
class Model; 


class Context
{
    bool isRunning = true;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    
    
    uint32_t* colorBuffer = nullptr;
    SDL_Texture* colorBufferTexture = nullptr;  

    std::unique_ptr<Model> shotgun;
    
    public:
    
    Camera camera; 
    Vector3 rotationAngles;

    Context();
    
    int32_t ConvertToFixedPoint(float value)
    {
        return static_cast<int32_t>(std::round(value * subpixelMultiplier));
    }

    bool IsRunning() { return isRunning; }
    void CreateRenderer();

    void CreateWindow();

    void DrawPixel(uint32_t x, uint32_t y, uint32_t color);

    void ClearColorBuffer(uint32_t color = 0xFF000000);

    void DrawGrid();

    void DrawGridOnly();

    void DrawRectangle(uint32_t rectX,uint32_t rectY,uint32_t rectWidth,uint32_t rectHeight,uint32_t color);

    void DrawLine(int x1, int y1, int x2, int y2);

    void DrawTriangle(int x1,int y1,int x2, int y2, int x3, int y3);

    void DrawTriangle(Vector2 P1,Vector2 P2,Vector2 P3);

    void RenderColorBuffer();

    void Initialize();


    void ProcessInput();


    void Update(float dt);

    void Render(); 
    void RenderModel(Model& model,float scale = 1.0f);

    
    Vector2 ProjectPoint(Vector3 point); 
    
    void DrawFilledTriangle(Vector2 P1,Vector2 P2,Vector2 P3,uint32_t color = 0xFF00FFFF);
    
    void Cleanup(); 

    ~Context();

};