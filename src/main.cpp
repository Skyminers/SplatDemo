#include <Shaders.h>
#include <Camera.h>
#include <Player.h>
#include <Renderer.h>
#include <GameWindow.h>

GameWindow window;

const unsigned int PlayerNum = 3;

int main() {
    //Overall Initialization
    ShadersInit();
    
    for (int i = 0; i < PlayerNum; ++i) {
        Player::playerQueue.push_back(new Player(glm::vec3(i * 10, 0, 0)));
    }
    Player::renderInit();
    Bullet::renderInit();

    // render loop
    // -----------
    window.run();
    return 0;
}




