//
// Created by 刘一辰 on 2021/1/11.
//

#include "Music.h"
#include <string>
#include <irrKlang/irrKlang.h>

using namespace irrklang;

ISoundEngine *sound;
std::string bgmFilename, shootFilename, bloomFilename;

void musicInit(std::string bgmFilename, std::string shootFilename, std::string bloomFilename){
    sound = createIrrKlangDevice();
    ::bgmFilename = bgmFilename;
    ::shootFilename = shootFilename;
    ::bloomFilename = bloomFilename;
}

void playBGM(){
    sound->setSoundVolume(0.6);
    sound->play2D(bgmFilename.c_str(), true);
}

void playShootSound(){
    sound->setSoundVolume(0.4);
    sound->play2D(shootFilename.c_str(), false);
}

void playBloomSound(){
    sound->setSoundVolume(0.8);
    sound->play2D(bloomFilename.c_str(), false);
}