#include "Context.h"
#include "Maths.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept> 

#include "Model.h" 




Context::Context() {}



void Context::CreateWindow()
{
    
    if(SDL_Init(SDL_INIT_VIDEO) == false)
        throw std::runtime_error("Can't initialize SDL!");

    window = SDL_CreateWindow("Software Rasterizer", window_width , window_height, 0);     
    
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
    if(!SDL_SetRenderLogicalPresentation(renderer, logical_width,logical_height,SDL_LOGICAL_PRESENTATION_LETTERBOX))
    {
        std::cout << "/n Can't create different resolution of 320 x 240";
    }
    

} 

void Context::Initialize()
{
    colorBuffer = static_cast<uint32_t*>(std::malloc(sizeof(uint32_t) * logical_width * logical_height)); 
    depthBuffer = static_cast<float*>(std::malloc(sizeof(float) * logical_width * logical_height));

    colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, logical_width, logical_height);

    shotgun = std::make_unique<Model>(); 
    hands   = std::make_unique<Model>();

    shotgun->LoadModel("shotgun.glb");
    hands->LoadModel("hands.glb");

    camera.position = Vector3(0,0,-0.6f);
    camera.fov = 90 * DegreeToRadians; 
    camera.focalLength = (logical_width / 2.0f) / std::tan(camera.fov / 2.0f);  
    camera.nearPlane = 0.1f;
     
    input.targetDistance = camera.position.z; 



}



void Context::ProcessInput()
{
    input.deltaX = input.deltaY = 0;

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        
        if(event.type == SDL_EVENT_QUIT)
        {
            isRunning = false;
            std::cout << "\nSDL_Quit Event happend! ";
        }
        

        if (event.type == SDL_EVENT_MOUSE_MOTION) 
        {
          input.deltaX += event.motion.xrel * 50;
          input.deltaY += event.motion.yrel * 50;
        }

        if (event.type == SDL_EVENT_MOUSE_WHEEL) 
        {
          if (event.wheel.y > 0)
            input.targetDistance += .1f;

          else if (event.wheel.y < 0)
            input.targetDistance -= .1f;
        }
    }

} 



void Context::Update(float dt)
{

    std::cout << "\33[2KDelta time: " << static_cast<int>(1.0f/dt) << "\r"<<std::flush;
    rotationAngles.y   += input.deltaX * DegreeToRadians * dt; 
    rotationAngles.x   += input.deltaY * DegreeToRadians * dt; 
    camera.position.z  = input.targetDistance;
}


void Context::Render()
{
    ClearColorBuffer(0xFFFFFFDD); // now clear the buffer 
    ClearDepthBuffer();
    
    //DrawLine(0, 0, 640*2, 480*2);
    //Vector2 A(window_width/2.0, window_height/2.0 - 100),B(window_width/2.0 - 100, window_height/2.0 + 100),C(window_width/2.0 + 100, window_height/2.0 + 100),P4(window_width/2.0 + 200,window_height/2.0 - 100);
    
    Vector3 P1(0,  1,  2);   // top center
    Vector3 P2(1,  -.5f, 2);   // bottom right  ← go right first (CW)
    Vector3 P3(-1, -.5f, 2);   // bottom left
    
    
    float sinY = std::sin(rotationAngles.y);
    float cosY = std::cos(rotationAngles.y); 
    
    //RotateAroundZ(P1,P2,P3,sinY,cosY);
    
    Vector2 A = ProjectPoint(P1);
    Vector2 B = ProjectPoint(P2);
    Vector2 C = ProjectPoint(P3);
    
    //std::cout << "A: " << A.x << ", " << A.y << "\n";
    //std::cout << "B: " << B.x << ", " << B.y << "\n";
    //std::cout << "C: " << C.x << ", " << C.y << "\n";
    
    //DrawGrid();
    DrawFilledProjectedTriangle(A,B,C,0xFF0FFFFF);
    //DrawTriangle(A,B,C); 
    //DrawTriangle(A,C,P4); 

    //
    //if(shotgun)
    //    RenderModel(*shotgun);
    //if(hands)
    //    RenderModel(*hands);

    RenderColorBuffer(); // upload buffer data to final texture or gpu
    SDL_RenderPresent(renderer); // preset data to the screen
    
    

} 



void Context::DrawPixel(uint32_t x, uint32_t y, uint32_t color)
{
    if( x < logical_width && y < logical_height)
        colorBuffer[y * logical_width + x] = color;
}

void Context::ClearColorBuffer(uint32_t color)
{
    //for (int y = 0; y < window_height; y++) 
    //{
    //    for (int x = 0; x < window_width; x++) 
    //    {
//
    //        colorBuffer[window_width * y + x] = color;
    //        //DrawPixel(x,y,color); // unneccessary check for all pixels 640x480 = ~307k checks
    //    }
    //} 
    std::fill(colorBuffer,colorBuffer + logical_width * logical_height,color);
}

void Context::ClearDepthBuffer(float depthValue)
{
    //for (int y = 0; y < window_height; y++) 
    //{
    //    for (int x = 0; x < window_width; x++) 
    //    {
//
    //        depthBuffer[window_width * y + x] = depthValue;
    //        //DrawPixel(x,y,color); // unneccessary check for all pixels 640x480 = ~307k checks
    //    }
    //}
    std::fill(depthBuffer,depthBuffer + logical_width * logical_height,depthValue);
}

void Context::DrawGrid()
{
    for (int y = 0; y < logical_height; y++) 
    {
        for (int x = 0; x < logical_width; x++) 
        {
            //uint32_t index = width * y + x;
            if(x%30 == 0 || y%30 == 0)
                DrawPixel(x,y,0xFF000000);//colorBuffer[index] = 0xFF000000;
            
        }
    }
}

void Context::DrawGridOnly()
{
    for (int y = 0; y < logical_height; y++) 
    {
        for (int x = 0; x < logical_width; x++) 
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

    if(startingX > logical_width)    startingX = logical_width;
    if(startingY > logical_height)   startingY = logical_height;
    

    uint32_t endingX = startingX + rectWidth; 
    uint32_t endingY = startingY + rectHeight; 

    if(endingX > logical_width)    endingX = logical_width;
    if(endingY > logical_height)   endingY = logical_height;
    
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
    SDL_UpdateTexture(colorBufferTexture,nullptr,colorBuffer,static_cast<int>(logical_width * sizeof(uint32_t)));
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


void Context::DrawFilledTriangle(Vector2& P1,Vector2& P2,Vector2& P3,float z1,float z2,float z3,Vector3 normal1,Vector3 normal2,Vector3 normal3,uint32_t color) // Pineda's alogrithm
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

    float inverse_area = 1.0 / static_cast<float>(area);

    // calculating bounding box that covers the triangle

    int32_t xmin = min_int32(A.x,B.x,C.x) >> subpixelShift;  // 2.3 = floor  2  ceiling  3
    int32_t ymin = min_int32(A.y,B.y,C.y) >> subpixelShift; // -2.3 = floor -3  ceiling -2

    int32_t xmax = (max_int32(A.x,B.x,C.x) + subpixelMultiplier - 1) >> subpixelShift;
    int32_t ymax = (max_int32(A.y,B.y,C.y) + subpixelMultiplier - 1) >> subpixelShift; 

    // limiting bounding box to the edges of the screen

    xmin = max(0,xmin);
    ymin = max(0,ymin);
    xmax = min(xmax,logical_width  - 1);
    ymax = min(ymax,logical_height - 1); 

    

    int32_t bias0 = IsTopLeftEdge(AB) ? 0 : -1; 
    int32_t bias1 = IsTopLeftEdge(BC) ? 0 : -1; 
    int32_t bias2 = IsTopLeftEdge(CA) ? 0 : -1; 

    float inverse_zA = 1.0f / z1;
    float inverse_zB = 1.0f / z2;
    float inverse_zC = 1.0f / z3;

    normal1 = normal1 * inverse_zA;
    normal2 = normal2 * inverse_zB;
    normal3 = normal3 * inverse_zC;

    Vector3 lightDirection(0,0,-1);
    
    Vector2_int32 Pmin(xmin * subpixelMultiplier + half ,ymin * subpixelMultiplier + half );
     
    Vector2_int32 APmin = Pmin - A;
    Vector2_int32 BPmin = Pmin - B;
    Vector2_int32 CPmin = Pmin - C; 
    
    int64_t w0_row_start = CrossProduct_int64(AB, APmin);
    int64_t w1_row_start = CrossProduct_int64(BC, BPmin);
    int64_t w2_row_start = CrossProduct_int64(CA, CPmin); 

    int64_t delta_w0_x = (int64_t)(-AB.y) * subpixelMultiplier; 
    int64_t delta_w1_x = (int64_t)(-BC.y) * subpixelMultiplier;
    int64_t delta_w2_x = (int64_t)(-CA.y) * subpixelMultiplier;
    
    int64_t delta_w0_y = (int64_t)(AB.x) * subpixelMultiplier; 
    int64_t delta_w1_y = (int64_t)(BC.x) * subpixelMultiplier;
    int64_t delta_w2_y = (int64_t)(CA.x) * subpixelMultiplier;

    for (int32_t y = ymin; y <= ymax; y++)  // traversing all pixels inside the bounding box
    {
        int64_t w0 = w0_row_start;
        int64_t w1 = w1_row_start;
        int64_t w2 = w2_row_start;

        for (int32_t x = xmin; x <= xmax; x++) 
        {
            //Vector2_int32 P(x * subpixelMultiplier + half ,y * subpixelMultiplier + half); 

            //Vector2_int32 AP = P - A; 
            //Vector2_int32 BP = P - B; 
            //Vector2_int32 CP = P - C; 

            //int64_t w0 = CrossProduct_int64(AB,AP);  // + bias0;
            //int64_t w1 = CrossProduct_int64(BC,BP);  // + bias1;
            //int64_t w2 = CrossProduct_int64(CA,CP);  // + bias2;  

           
            
            // bias is only used for top edge rule to know the owner of overlapping pixels, don't use it in calculation of alpha beta gamma

            if((w0+bias0) >= 0 && (w1+bias1) >= 0 && (w2+bias2) >= 0)
            {
                
                 // alpha beta and gamma are barycentric co-ordinates that can be used to interpolate values between vertices like uvs
                //  basically alpha * A + beta * B + gamma * C
                float alpha  = static_cast<float>(w1) * inverse_area;   // area of triangle opposite to A(BCP) / area of entire triangle ABC
                float beta   = static_cast<float>(w2) * inverse_area;  //  area of triangle opposite to B(CAP) / area of entire triangle ABC
                float gamma  = static_cast<float>(w0) * inverse_area; //   area of triangle opposite to C(ABP) / area of entire triangle ABC 

                
                // screen space is non linear due to perspective projection ie division by z and not projection matrix so interpolating linear attributes like uv,color,normals will result in incorrect ps1 style result
                // z and attribute are not linear but 1/z and attribute/z are, so basically we want (interpolated 1/z) =  alpha * 1/z1 + beta * 1/z2 + gamma * 1/z3 and interpolated z = 1 / (interpolated 1/z) 
                // interpolated attribute A = interpolated(A/z) / interpolaterd (1/z) = interpolated(A/z) * interpolated(z)

                float interpolated_inverse_z = alpha * inverse_zA + beta * inverse_zB + gamma * inverse_zC;
                float interpolated_z = 1.0f / interpolated_inverse_z;

                Vector3 interpolated_normal = normal1 * alpha + normal2 * beta + normal3 * gamma; 
                interpolated_normal = interpolated_normal * interpolated_z; 

                float len = std::sqrt(interpolated_normal.x * interpolated_normal.x +
                                         interpolated_normal.y * interpolated_normal.y +
                                         interpolated_normal.z * interpolated_normal.z);
                if(len > 0.0001f)
                    interpolated_normal = interpolated_normal / len; 

                
                float diffuse = max(DotProduct(interpolated_normal, lightDirection), 0.0f);
                
                uint8_t shade = (uint8_t)(min(diffuse, 1.0f) * 255.0f);
                color = 0xFF000000 | (shade << 16) | (shade << 8) | shade;
               
                int32_t index = y * logical_width + x;
                
                if(interpolated_inverse_z > depthBuffer[index])
                {
                    //uint8_t r = (uint8_t)(255 * alpha + 0   * beta + 0   * gamma);
                    //uint8_t g = (uint8_t)(0   * alpha + 255 * beta + 0   * gamma);
                    //uint8_t b = (uint8_t)(0   * alpha + 0   * beta + 255 * gamma);
            
                    //color = 0xFF000000 | (r << 16) | (g << 8) | b; 
                    depthBuffer[index] = interpolated_inverse_z;
                    
                                        
                    //color = 0xFF000000
                    //                    | ((uint8_t)((interpolated_normal.x * 0.5f + 0.5f) * 255.0f) << 16)
                    //                    | ((uint8_t)((interpolated_normal.y * 0.5f + 0.5f) * 255.0f) << 8)
                    //                    |  (uint8_t)((interpolated_normal.z * 0.5f + 0.5f) * 255.0f);
                    //DrawPixel(x,y,color); 
                    colorBuffer[index]  =  color; 
                }
                
            }

            w0 += delta_w0_x;
            w1 += delta_w1_x;
            w2 += delta_w2_x;
        }

        w0_row_start += delta_w0_y;
        w1_row_start += delta_w1_y;
        w2_row_start += delta_w2_y;
    }
}

void Context::DrawFilledProjectedTriangle(Vector2 P1,Vector2 P2,Vector2 P3,uint32_t color)
{
    int xmin = min(P1.x,P2.x,P3.x); 
    int ymin = min(P1.y,P2.y,P3.y); 

    int xmax = max(P1.x,P2.x,P3.x);
    int ymax = max(P1.y,P2.y,P3.y); 

    xmin = max(0,xmin);
    ymin = max(0,ymin);
    xmax = min(xmax,logical_width - 1);
    ymax = min(ymax,logical_height - 1); 

    Vector2 P1P2 = P2 - P1; 
    Vector2 P2P3 = P3 - P2; 
    Vector2 P3P1 = P1 - P3;

    for (int y = ymin; y <= ymax; y++) 
    {
        for (int x = xmin; x <= xmax; x++) 
        {
            Vector2 P(x,y);

            if(InsideTriangle(P,P1,P2,P3,P1P2,P2P3,P3P1))
                DrawPixel(x,y,color);
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

        Vector3 normal1 = model.vertices[index1].normal;
        Vector3 normal2 = model.vertices[index2].normal;
        Vector3 normal3 = model.vertices[index3].normal; 


        vertex1 *=  scale;
        vertex2 *=  scale;
        vertex3 *=  scale;



        RotateAroundY(vertex1,vertex2,vertex3,sinY,cosY);
        //RotateAroundX(vertex1,vertex2,vertex3,sinY,cosY);

        RotateAroundY(normal1,normal2,normal3,sinY,cosY);
        //RotateAroundX(normal1,normal2,normal3,sinY,cosY);

        vertex1.z -= camera.position.z;
        vertex2.z -= camera.position.z;
        vertex3.z -= camera.position.z;

        
        if(vertex1.z <= camera.nearPlane || vertex2.z <= camera.nearPlane || vertex3.z <= camera.nearPlane) // fix later with clipping or something else
            continue;

        //Vector3 faceNormal = normal1 / 3.0f + normal2 / 3.0f + normal3 / 3.0f;
        //Vector3 lightDirection(0,0,-1);
        //float diffuse = max(DotProduct(faceNormal,lightDirection),0);

        //uint8_t shade = (uint8_t)(diffuse * 255.0f);
        //uint32_t color = 0xFF000000 | (shade << 16) | (shade << 8) | shade;
        
        Vector2 A = ProjectPoint(vertex1);
        Vector2 B = ProjectPoint(vertex2);
        Vector2 C = ProjectPoint(vertex3);  
        
        // use back face culling here for better performance
   
        Vector2 AB = B - A;
        Vector2 AC = C - A;

        float area = CrossProduct(AB,AC);
        if(area <= 0)
            continue;
        

        DrawFilledTriangle(A, B, C,vertex1.z,vertex2.z,vertex3.z,normal1,normal2,normal3);

    }

   
}

/*
Vector2 Context::ProjectPoint(Vector3& point)
{
    Vector2 projectedPoint;
    
    // perspective divide
    projectedPoint.x = (point.x  * camera.focalLength) / point.z + (window_width   / 2.0f) ;

    // Negate it to match +y up from 3d space to +y in screen space otherwise image will be upside down
    projectedPoint.y = -(point.y  * camera.focalLength) / point.z + (window_height / 2.0f) ;
    
    return projectedPoint;
}
*/



void Context::Cleanup()
{

    // clear everything in reverse order
    std::free(colorBuffer);
    std::free(depthBuffer); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); 

}



Context::~Context() = default;