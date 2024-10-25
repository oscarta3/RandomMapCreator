#include "Window.h"
#include "MapCreator.h"

using namespace std;

int main(int argc, char* argv[])
{
    Window window("2D Random Map Creator", 800, 800);    
    window.Clear();
    MapManager mapManager(&window);

    while (!window.isClosed())
    {
        mapManager.pollEvents();
    }
    return 0;
}