#include "Context.h"
#include "Maths.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>



Context::Context()
{

}


void Context::CreateWindow()
{
    
    if(SDL_Init(SDL_INIT_VIDEO) == false)
        throw std::runtime_error("Can't initialize SDL!");

    window = SDL_CreateWindow("Software Rasterizer", window_width, window_height, 0);     
    
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



void Context::RenderColorBuffer()
{
    SDL_UpdateTexture(colorBufferTexture,nullptr,colorBuffer,static_cast<int>(window_width * sizeof(uint32_t)));
    SDL_RenderTexture(renderer,colorBufferTexture,nullptr,nullptr);
}

void Context::Initialize()
{
    colorBuffer = static_cast<uint32_t*>(std::malloc(sizeof(uint32_t) * window_width * window_height) ); 

    colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    int p = 0;
    for(float x = -1; x<= 1; x += 0.25)
    {
        for(float y = -1; y<= 1; y += 0.25)
        {
            for(float z = -1; z<= 1; z += 0.25)
            {
                Vector3 point(x,y,z); 
                points[p++] = point;
            } 
        } 
    }

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
    pointsRotationAngle.x += 60.0f * DegreeToRadians * dt;
    pointsRotationAngle.y += 60.0f * DegreeToRadians * dt;
    pointsRotationAngle.z += 60.0f * DegreeToRadians * dt;

    for (int i = 0; i < totalPoints; i++) 
    {
        Vector3 point = points[i];
        
        // Scale -> Rotate -> Translate
        point    = RotateAroundX(point, pointsRotationAngle.x);        
        point    = RotateAroundY(point, pointsRotationAngle.y);        
        point    = RotateAroundZ(point, pointsRotationAngle.z);        
        point.z += camera.position.z;  

        projectedPoints[i] = ProjectPoint(point);
    }
}


void Context::Render()
{
    //SDL_SetRenderDrawColor(renderer,0,0,0,255);
    //SDL_RenderClear(renderer);

    //DrawRectangle(width/2,height/2,width/2  ,height/2 ,0xFF00AAFF);
    //DrawGrid(); 
    //DrawPixel(30,30,0xFF44FFCC); 

    for(int i=0;i<totalPoints;i++)
    {
        DrawRectangle(projectedPoints[i].x * projectedPointsScaleFactor + (window_width/2.0),
                      projectedPoints[i].y * projectedPointsScaleFactor + (window_height/2.0),
                      4,4,0xFF000000);
    }


    RenderColorBuffer();
    ClearColorBuffer(0xFFFFFFDD);

    SDL_RenderPresent(renderer);
}


Vector2 Context::ProjectPoint(Vector3 point)
{
    Vector2 projectedPoint;
    
    // perspective divide
    projectedPoint.x = point.x / point.z;
    projectedPoint.y = point.y / point.z;
    
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



