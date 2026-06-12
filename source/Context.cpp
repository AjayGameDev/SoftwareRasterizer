#include "Context.h"
#include "Maths.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept> 

#include "Model.h" 




Context::Context()
{

}



void Context::CreateWindow()
{
    
    if(SDL_Init(SDL_INIT_VIDEO) == false)
        throw std::runtime_error("Can't initialize SDL!");

    window = SDL_CreateWindow("Software Rasterizer", window_width * 2 , window_height * 2, 0);     
    
    if(!window)
        throw std::runtime_error("Can't create it! ");

       
} 

void Context::CreateRenderer()
{
    renderer = SDL_CreateRenderer(window,nullptr);

    if(!renderer)
    {
        std::cout << "Can't create renderer! " << SDL_GetError() << std::endl;
        throw std::runtime_error("Can't create renderer! ");
    } 

    SDL_SetRenderVSync(renderer, 1); // to match the refresh rate of monitor
    //if(!SDL_SetRenderLogicalPresentation(renderer, 320, 240,SDL_LOGICAL_PRESENTATION_LETTERBOX))
    //{
    //    std::cout << "/n Can't create different resolution of 320 x 240";
    //}
    

} 

void Context::Initialize()
{
    colorBuffer = static_cast<uint32_t*>(std::malloc(sizeof(uint32_t) * window_width * window_height) ); 

    colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    shotgun = std::make_unique<Model>();

    shotgun->LoadModel("shotgun.glb");
     
}



void Context::ProcessInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        
        if(event.type == SDL_EVENT_QUIT)
        {
            isRunning = false;
            std::cout << "SDL_Quit Event happend! ";
        }
        /*
        if(event.type == SDL_EVENT_KEY_DOWN)
        {
            isRunning = false;
            std::cout << "Escape pressed! ";
        }
          */  
    }

} 



void Context::Update(float dt)
{

    std::cout << "Delta time: " << static_cast<int>(1.0f/dt) << "\n";
    rotationAngles.y   += 30.0f    * DegreeToRadians * dt;
}


void Context::Render()
{
    
    //DrawLine(0, 0, 640*2, 480*2);
    //Vector2 A(window_width/2.0, window_height/2.0 - 100),B(window_width/2.0 - 100, window_height/2.0 + 100),C(window_width/2.0 + 100, window_height/2.0 + 100),P4(window_width/2.0 + 200,window_height/2.0 - 100);
    
    Vector3 P1(0,  1,  2);   // top center
    Vector3 P2(1,  -1, 2);   // bottom right  ← go right first (CW)
    Vector3 P3(-1, -1, 2);   // bottom left

    
    float sinY = std::sin(rotationAngles.y);
    float cosY = std::cos(rotationAngles.y); 

    //RotateAroundZ(P1,P2,P3,sinY,cosY);

    Vector2 A = ProjectPoint(P1);
    Vector2 B = ProjectPoint(P2);
    Vector2 C = ProjectPoint(P3);

    //std::cout << "A: " << A.x << ", " << A.y << "\n";
    //std::cout << "B: " << B.x << ", " << B.y << "\n";
    //std::cout << "C: " << C.x << ", " << C.y << "\n";

    //DrawFilledTriangle(A,B,C,0xFF000000);
    //DrawTriangle(A,B,C); 
    //DrawTriangle(A,C,P4); 
    
    if(shotgun)
        RenderModel(*shotgun);

    RenderColorBuffer(); // upload buffer data to final texture or gpu
    SDL_RenderPresent(renderer); // preset data to the screen

    ClearColorBuffer(0xFFFFFFDD); // now clear the buffer 
    

} 



void Context::DrawPixel(uint32_t x, uint32_t y, uint32_t color)
{
    if( x < window_width && y < window_height)
        colorBuffer[y * window_width + x] = color;
}

void Context::ClearColorBuffer(uint32_t color)
{
    for (int y = 0; y < window_height; y++) 
    {
        for (int x = 0; x < window_width; x++) 
        {

            colorBuffer[window_width * y + x] = color;
            //DrawPixel(x,y,color); // unneccessary check for all pixels 640x480 = ~307k checks
        }
    }
}

void Context::DrawGrid()
{
    for (int y = 0; y < window_height; y++) 
    {
        for (int x = 0; x < window_width; x++) 
        {
            //uint32_t index = width * y + x;
            if(x%30 == 0 || y%30 == 0)
                DrawPixel(x,y,0xFF000000);//colorBuffer[index] = 0xFF000000;
            
        }
    }
}

void Context::DrawGridOnly()
{
    for (int y = 0; y < window_height; y++) 
    {
        for (int x = 0; x < window_width; x++) 
        {
            //uint32_t index = width * y + x;
            if(x%30 == 0 || y%30 == 0)
                DrawPixel(x,y,0xFF000000);//colorBuffer[index] = 0x000000;
            else
                DrawPixel(x,y,0xFFFFFFFF);//colorBuffer[index] = 0xFFFFFFFF;
        }
    }
} 

void Context::DrawRectangle(uint32_t rectX,uint32_t rectY,uint32_t rectWidth,uint32_t rectHeight,uint32_t color)
{
    uint32_t startingX,startingY;


    if(rectX >= rectWidth/2)
        startingX = rectX - rectWidth  / 2; 
    else
        startingX = 0; 

    if(rectY >= rectHeight/2)
        startingY = rectY - rectHeight / 2; 
    else
        startingY = 0; 

    if(startingX > window_width)    startingX = window_width;
    if(startingY > window_height)   startingY = window_height;
    

    uint32_t endingX = startingX + rectWidth; 
    uint32_t endingY = startingY + rectHeight; 

    if(endingX > window_width)    endingX = window_width;
    if(endingY > window_height)   endingY = window_height;
    
    for (int y = startingY; y < endingY; y++) 
    {
        for (int x = startingX; x < endingX; x++) 
        {
            DrawPixel(x,y,color);
            //uint32_t index = width * y + x;
            //colorBuffer[index] = color;
           
        }
    }
}

void Context::DrawLine(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    int sideLength = std::abs(dx) >= std::abs(dy) ? std::abs(dx) : std::abs(dy); 

    float increment_x = dx / (float)sideLength; 
    float increment_y = dy / (float)sideLength; 

    float current_x = x1;
    float current_y = y1;
   
    for(int i = 0; i <= sideLength; i++)
    {
        DrawPixel(std::round(current_x), std::round(current_y), 0xFF000000);
        current_x += increment_x;
        current_y += increment_y; 
    }
}


void Context::DrawTriangle(int x1,int y1,int x2, int y2, int x3, int y3)
{
    DrawLine(x1, y1, x2, y2);
    DrawLine(x2, y2, x3, y3);
    DrawLine(x3, y3, x1, y1);
} 

void Context::DrawTriangle(Vector2 A,Vector2 B,Vector2 C)
{
    DrawTriangle(A.x,A.y,B.x,B.y,C.x,C.y);
}


void Context::RenderColorBuffer()
{
    SDL_UpdateTexture(colorBufferTexture,nullptr,colorBuffer,static_cast<int>(window_width * sizeof(uint32_t)));
    SDL_RenderTexture(renderer,colorBufferTexture,nullptr,nullptr);
}



bool IsTopLeftEdge(Vector2_int32& edge)
{
    bool isTopEdge  = (edge.y == 0) && (edge.x > 0);
    bool isLeftEdge = edge.y < 0;
    
    return isTopEdge || isLeftEdge; 
}


bool InsideTriangle(Vector2& P,Vector2& A,Vector2& B,Vector2& C,Vector2& AB, Vector2& BC, Vector2& CA)
{
    
    Vector2 AP = P - A; 
    Vector2 BP = P - B; 
    Vector2 CP = P - C; 

   
    if(CrossProduct(AB,AP) < 0 )
        return false;

    if(CrossProduct(BC,BP) < 0 )
        return false;

    if(CrossProduct(CA,CP) < 0 )
        return false;
    return true;

    
}



void Context::DrawFilledTriangle(Vector2 P1,Vector2 P2,Vector2 P3,uint32_t color) // Pineda's alogrithm
{

    Vector2_int32 A(ConvertToFixedPoint(P1.x),ConvertToFixedPoint(P1.y));
    Vector2_int32 B(ConvertToFixedPoint(P2.x),ConvertToFixedPoint(P2.y));
    Vector2_int32 C(ConvertToFixedPoint(P3.x),ConvertToFixedPoint(P3.y));

    Vector2_int32 AB = B - A; 
    Vector2_int32 BC = C - B; 
    Vector2_int32 CA = A - C; 


    Vector2_int32 AC = C - A;
    // area of triangle can be calculated using cross product of edges from same vertex
    // for ABC triangle clockwise AB cross AC will give area of entire triangle
    int64_t area = CrossProduct_int64(AB,AC);
        if(area <= 0)
            return;

    // calculating bounding box that covers the triangle

    int32_t xmin = min_int32(A.x,B.x,C.x) >> subpixelShift;  // 2.3 = floor  2  ceiling  3
    int32_t ymin = min_int32(A.y,B.y,C.y) >> subpixelShift; // -2.3 = floor -3  ceiling -2

    int32_t xmax = (max_int32(A.x,B.x,C.x) + subpixelMultiplier - 1) >> subpixelShift;
    int32_t ymax = (max_int32(A.y,B.y,C.y) + subpixelMultiplier - 1) >> subpixelShift; 

    // limiting bounding box to the edges of the screen

    xmin = max(0,xmin);
    ymin = max(0,ymin);
    xmax = min(xmax,window_width  - 1);
    ymax = min(ymax,window_height - 1); 

    

     int32_t bias0 = IsTopLeftEdge(AB) ? 0 : -1; 
     int32_t bias1 = IsTopLeftEdge(BC) ? 0 : -1; 
     int32_t bias2 = IsTopLeftEdge(CA) ? 0 : -1; 

    for (int32_t y = ymin; y <= ymax; y++)  // traversing all pixels inside the bounding box
    {
        for (int32_t x = xmin; x <= xmax; x++) 
        {
            Vector2_int32 P(x * subpixelMultiplier + half ,y * subpixelMultiplier + half); 

            Vector2_int32 AP = P - A; 
            Vector2_int32 BP = P - B; 
            Vector2_int32 CP = P - C; 

            int64_t w0 = CrossProduct_int64(AB,AP) + bias0;
            int64_t w1 = CrossProduct_int64(BC,BP) + bias1;
            int64_t w2 = CrossProduct_int64(CA,CP) + bias2;  

            
            

            


            if(w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                
                 // alpha beta and gamma are barycentric co-ordinates that can be used to interpolate values between vertices like uvs
                //  basically alpha * A + beta * B + gamma * C
                float alpha  = static_cast<float>(w0) / static_cast<float>(area);   // area of triangle ABP) / area of entire triangle ABC
                float beta   = static_cast<float>(w1) / static_cast<float>(area);  //  area of triangle BCP) / area of entire triangle ABC
                float gamma  = static_cast<float>(w2) / static_cast<float>(area); //   area of triangle CAP) / area of entire triangle ABC
                
                uint8_t r = (uint8_t)(255 * alpha + 0   * beta + 0   * gamma);
                uint8_t g = (uint8_t)(0   * alpha + 255 * beta + 0   * gamma);
                uint8_t b = (uint8_t)(0   * alpha + 0   * beta + 255 * gamma);
                
                //color = 0xFF000000 | (r << 16) | (g << 8) | b;
                DrawPixel(x,y,color);
            }
        }
    }
}



void Context::RenderModel(Model& model,float scale)
{
    
    //rotationAngles.y   += 0.4f    * DegreeToRadians * dt;
    float sinY = std::sin(rotationAngles.y);
    float cosY = std::cos(rotationAngles.y);
    //rotationAngles.x    = 180.0f  * DegreeToRadians;
 
    /*
    for (int i = 0; i < model.vertices.size(); i++) 
    {
        Vector3 position = model.vertices[i].position; 
        position.x *= scale;
        position.y *= scale;
        position.z *= scale; 

        position = RotateAroundX(position, rotationAngles.x);
        position = RotateAroundY(position, rotationAngles.y);

        position.z -= camera.position.z;
        if(position.z <= 0.0f)
            continue;

        Vector2 projectedPoint = ProjectPoint(position); 
        
        if(projectedPoint.x<0 || projectedPoint.y<0 || projectedPoint.x >= window_width || projectedPoint.y >= window_height)
            continue;

        //DrawRectangle(projectedPoint.x, projectedPoint.y, 10, 10, 0xFF00FFFF);
        DrawPixel(projectedPoint.x, projectedPoint.y, 0xFF00FFFF);

    }

     */

    
    for (int i = 0; i+2 < model.indices.size(); i += 3) 
    {
        uint32_t index1 = model.indices[i];
        uint32_t index2 = model.indices[i+1];
        uint32_t index3 = model.indices[i+2]; 

        Vector3 vertex1 = model.vertices[index1].position;
        Vector3 vertex2 = model.vertices[index2].position;
        Vector3 vertex3 = model.vertices[index3].position; 

        vertex1 *=  scale;
        vertex2 *=  scale;
        vertex3 *=  scale;

        //vertex1 = RotateAroundX(vertex1, rotationAngles.x);
        //vertex1 = RotateAroundY(vertex1, rotationAngles.y);

        //vertex2 = RotateAroundX(vertex2, rotationAngles.x);
        //vertex2 = RotateAroundY(vertex2, rotationAngles.y);

        //vertex3 = RotateAroundX(vertex3, rotationAngles.x);
        //vertex3 = RotateAroundY(vertex3, rotationAngles.y);

        RotateAroundY(vertex1,vertex2,vertex3,sinY,cosY);

        vertex1.z -= camera.position.z;
        vertex2.z -= camera.position.z;
        vertex3.z -= camera.position.z;
        
        if(vertex1.z <= 0.0f || vertex2.z <= 0.0f || vertex3.z <= 0.0f)
            continue;


        
        
        Vector2 A = ProjectPoint(vertex1);
        Vector2 B = ProjectPoint(vertex2);
        Vector2 C = ProjectPoint(vertex3);  
        
        // use back face culling here for better performance
   
        
        
        
        DrawFilledTriangle(A, B, C);

        // this is way too aggressive and culls even if some of it is on screen

        //if(A.x<0 || A.y<0 || A.x >= window_width || A.y >= window_height)
        //    continue;
        //if(B.x<0 || B.y<0 || B.x >= window_width || B.y >= window_height)
        //    continue;
        //if(C.x<0 || C.y<0 || C.x >= window_width || C.y >= window_height)
        //    continue;


       //if(A.x < 0 && B.x < 0 && C.x < 0) continue;  
       //if(A.y < 0 && B.y < 0 && C.y < 0) continue; 
       //if(A.x > window_width && B.x > window_width && C.x > window_width) continue;
       //if(A.y > window_height && B.y > window_height && C.y > window_height) continue; 
        
        //DrawTriangle(A,B,C);
    }

   
}


Vector2 Context::ProjectPoint(Vector3 point)
{
    Vector2 projectedPoint;
    
    // perspective divide
    projectedPoint.x = (point.x  * camera.focalLength) / point.z + (window_width   / 2.0f) ;

    // Negate it to match +y up from 3d space to +y in screen space otherwise image will be upside down
    projectedPoint.y = -(point.y  * camera.focalLength) / point.z + (window_height / 2.0f) ;
    
    return projectedPoint;
}




void Context::Cleanup()
{

    // clear everything in reverse order
    std::free(colorBuffer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); 

}



Context::~Context() = default;