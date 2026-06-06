#include "Context.h"
#include "Maths.h"
#include "GameTime.h"
#include "iostream"




int main()
{
    
    GameTime time;

    Context context;
    context.camera.position = Vector3(0,0,-5);

    context.CreateWindow();
    context.CreateRenderer();

    context.Initialize();
    
    while(context.IsRunning())
    {
        time.Update();
        context.ProcessInput();
        context.Update(time.deltaTime);
        std::cout << "Delta time: " << time.deltaTime << "\n";
        context.Render();
    } 

    context.Cleanup();
    
    return 0;
}


