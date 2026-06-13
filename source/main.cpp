#include "Context.h"
#include "Maths.h"
#include "GameTime.h"
#include <cmath>


int main()
{
    
    
    GameTime time;

    Context context;
    

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


