#include <Shaders.h>
#include <Camera.h>
#include <GameLogic.h>
#include <Renderer.h>
#include <GameWindow.h>

GameWindow window;

int main() {
    //Overall Initialization
    ShadersInit();
    GameLogic::init(1, 1);

    // render loop
    // -----------
    window.run();
    return 0;
}




