#include <Shaders.h>
#include <Camera.h>
#include <Music.h>
#include <GameLogic.h>
#include <Renderer.h>
#include <GameWindow.h>

GameWindow window;

int main() {
    //Overall Initialization
    ShadersInit();
    musicInit("../resource/mp3/bgm.wav",
              "../resource/mp3/shoot.wav",
              "../resource/mp3/boom.wav");
    GameLogic::init(2,3);

    playBGM();

    // render loop
    // -----------
    window.run();
    return 0;
}




