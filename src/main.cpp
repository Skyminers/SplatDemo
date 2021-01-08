#include <Shaders.h>
#include <Camera.h>
#include <GameLogic.h>
#include <Renderer.h>
#include <GameWindow.h>

GameWindow window;
GameLogic game;

int main() {
    //Overall Initialization
    ShadersInit();
    game.init(3);

    // render loop
    // -----------
    window.run();
    return 0;
}




