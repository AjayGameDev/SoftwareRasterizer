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

void Context::DrawTriangle(Vector2 P1,Vector2 P2,Vector2 P3)
{
    DrawTriangle(P1.x,P1.y,P2.x,P2.y,P3.x,P3.y);
}


void Context::RenderColorBuffer()
{
    SDL_UpdateTexture(colorBufferTexture,nullptr,colorBuffer,static_cast<int>(window_width * sizeof(uint32_t)));
    SDL_RenderTexture(renderer,colorBufferTexture,nullptr,nullptr);
}

void Context::Initialize()
{
    colorBuffer = static_cast<uint32_t*>(std::malloc(sizeof(uint32_t) * window_width * window_height) ); 

    colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);


    
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

// Pineda's alogrithm
void Context::DrawFilledTriangle(Vector2 P1,Vector2 P2,Vector2 P3)
{
    int xmin = min(P1.x,P2.x,P3.x); 
    int ymin = min(P1.y,P2.y,P3.y); 

    int xmax = max(P1.x,P2.x,P3.x);
    int ymax = max(P1.y,P2.y,P3.y); 

    xmin = max(0,xmin);
    ymin = max(0,ymin);
    xmax = min(xmax,window_width - 1);
    ymax = min(ymax,window_height - 1); 

    Vector2 P1P2 = P2 - P1; 
    Vector2 P2P3 = P3 - P2; 
    Vector2 P3P1 = P1 - P3;

    for (int y = ymin; y <= ymax; y++) 
    {
        for (int x = xmin; x <= xmax; x++) 
        {
            Vector2 P(x,y);

            if(InsideTriangle(P,P1,P2,P3,P1P2,P2P3,P3P1))
                DrawPixel(x,y,0xFF00FFFF);
        }
    }
}

void Context::Update(float dt)
{

    
}


void Context::Render()
{
    
    //DrawLine(0, 0, 640*2, 480*2);
    //Vector2 P1(window_width/2.0, window_height/2.0 - 100),P2(window_width/2.0 - 100, window_height/2.0 + 100),P3(window_width/2.0 + 100, window_height/2.0 + 100),P4(window_width/2.0 + 200,window_height/2.0 - 100);
    
    Vector3 A(0,  1,  2);   // top center
    Vector3 B(1,  -1, 2);   // bottom right  ← go right first (CW)
    Vector3 C(-1, -1, 2);   // bottom left


    Vector2 P1 = ProjectPoint(A);
    Vector2 P2 = ProjectPoint(B);
    Vector2 P3 = ProjectPoint(C);

    //std::cout << "P1: " << P1.x << ", " << P1.y << "\n";
    //std::cout << "P2: " << P2.x << ", " << P2.y << "\n";
    //std::cout << "P3: " << P3.x << ", " << P3.y << "\n";

    //DrawFilledTriangle(P1,P2,P3);
    //DrawTriangle(P1,P2,P3); 
    //DrawTriangle(P1,P3,P4); 
    RenderColorBuffer();
    ClearColorBuffer(0xFFFFFFDD); 

    SDL_RenderPresent(renderer);
} 

void Context::RenderModel(Model& model,float scale)
{
    
    rotationAngles.y   += 0.4f    * DegreeToRadians;
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


        
        
        Vector2 P1 = ProjectPoint(vertex1);
        Vector2 P2 = ProjectPoint(vertex2);
        Vector2 P3 = ProjectPoint(vertex3);  
        
        // use back face culling here for better performance
        
        Vector2 P1P2 = P2-P1;
        Vector2 P1P3 = P3-P1; 
        
        float area = CrossProduct(P1P2,P1P3);
        if(area <= 0)
            continue;
        
        DrawFilledTriangle(P1, P2, P3);

        // this is way too aggressive and culls even if some of it is on screen

        //if(P1.x<0 || P1.y<0 || P1.x >= window_width || P1.y >= window_height)
        //    continue;
        //if(P2.x<0 || P2.y<0 || P2.x >= window_width || P2.y >= window_height)
        //    continue;
        //if(P3.x<0 || P3.y<0 || P3.x >= window_width || P3.y >= window_height)
        //    continue;


       //if(P1.x < 0 && P2.x < 0 && P3.x < 0) continue;  
       //if(P1.y < 0 && P2.y < 0 && P3.y < 0) continue; 
       //if(P1.x > window_width && P2.x > window_width && P3.x > window_width) continue;
       //if(P1.y > window_height && P2.y > window_height && P3.y > window_height) continue; 
        
        //DrawTriangle(P1,P2,P3);
    }

   
}


Vector2 Context::ProjectPoint(Vector3 point)
{
    Vector2 projectedPoint;
    
    // perspective divide
    projectedPoint.x = (point.x  * camera.focalLength) / point.z + (window_width   / 2.0f) ;

    // Negate it to 
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



