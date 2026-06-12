#include "Context.h"
#include "Maths.h"
#include "GameTime.h"
#include <cmath>


int main()
{
    
    
    GameTime time;

    Context context;
    context.camera.position = Vector3(0,0,-0.6f);
    context.camera.fov = 90 * DegreeToRadians; 
    context.camera.focalLength = (window_width / 2.0f) / std::tan(context.camera.fov / 2.0f);

    context.CreateWindow();
    context.CreateRenderer();

    context.Initialize();
    
    while(context.IsRunning())
    {
        time.Update();
        
        context.ProcessInput();
        context.Update(time.deltaTime);
        context.Render();
    } 

    context.Cleanup();
    
    return 0;
}


