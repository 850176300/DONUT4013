//
//  SoundPlayer.cpp
//  Doctor
//
//  Created by tanshoumei on 13-8-23.
//
//

#include "SoundPlayer.h"
//#include "MathUtil.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

#define RoleCount 4

SoundPlayer::SoundPlayer():
bgMusic("nomusic"),
pourmilkID(0),
pourcerealID(0)
{

    canPlaySound = UserDefault::getInstance()->getBoolForKey("isSoundOpen", true);

    canPlayMusic = UserDefault::getInstance()->getBoolForKey("isMusicOpen", true);
    
    if (canPlayMusic) {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8f);
    }else {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    }
    if (canPlaySound) {
        SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    }else {
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
    }
    
}

void SoundPlayer::switchMusic(){
    canPlayMusic = !canPlayMusic;
    
    UserDefault::getInstance()->setBoolForKey("isMusicOpen", canPlayMusic);
    UserDefault::getInstance()->flush();
    if (canPlayMusic) {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8f);
    }else {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    }
    if (canPlaySound) {
        SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    }else {
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
    }
}
void SoundPlayer::switchSound(){
    canPlaySound = !canPlaySound;
    UserDefault::getInstance()->setBoolForKey("isSoundOpen", canPlaySound);
    UserDefault::getInstance()->flush();
    if (canPlayMusic) {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8f);
    }else {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    }
    if (canPlaySound) {
        SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    }else {
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
    }
}

void SoundPlayer::switchVolume(){
    switchMusic();
    switchSound();
}

bool SoundPlayer::isMusicOpen(){
    return canPlayMusic;
}
bool SoundPlayer::isSoundOpen(){
    return canPlaySound;
}

#pragma mark 背景音效播放
void SoundPlayer::playBackGroundMusic(const char *bgMusic) {
    stringstream s;
    s<<bgMusic;
    if (this->bgMusic.compare(s.str()) == 0) {
        return;
    }
    this->bgMusic = s.str();
    if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
    SimpleAudioEngine::getInstance()->playBackgroundMusic(bgMusic, true);
    
}

void SoundPlayer::stopBackGroundMusic(){
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void SoundPlayer::resumeBackGroundMusic(){
    if (bgMusic.compare("nomusic") != 0) {
        
        SimpleAudioEngine::getInstance()->playBackgroundMusic(bgMusic.c_str(), true);
    }
}

void SoundPlayer::stopAlleffect(){
    pourcerealID = 0;
    pourmilkID = 0;
    SimpleAudioEngine::getInstance()->stopAllEffects();
}

void SoundPlayer::pauseEffect(){
    if (pourcerealID != 0) {
        SimpleAudioEngine::getInstance()->pauseEffect(pourcerealID);
    }
    if (pourmilkID != 0) {
        SimpleAudioEngine::getInstance()->pauseEffect(pourmilkID);
    }
}

void SoundPlayer::resumeEffect(){
    if (pourcerealID != 0) {
        SimpleAudioEngine::getInstance()->resumeEffect(pourcerealID);
    }
    if (pourmilkID != 0) {
        SimpleAudioEngine::getInstance()->resumeEffect(pourmilkID);
    }
}
#pragma mark 基本音效播放
void SoundPlayer::playclickeffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/all button_sound.mp3");
}


void SoundPlayer::playHomeClickedEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/studio basic/home.mp3");
}

void SoundPlayer::playNextClickedEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/studio basic/next.mp3");
}

void SoundPlayer::playPreviousClickedEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/studio basic/previous.mp3");
}

void SoundPlayer::playResetClickedEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/studio basic/reset.mp3");
}

void SoundPlayer::playStartClickedEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/play.mp3");
}

void SoundPlayer::playCommonEffect(const string &effect) {
    SimpleAudioEngine::getInstance()->playEffect(effect.c_str());

}

void SoundPlayer::playCommonLoopEffect(const string &effect) {
    SimpleAudioEngine::getInstance()->playEffect(effect.c_str(), true);
}

#pragma mark 项目需要音效
void SoundPlayer::playAnimationEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/animation.mp3");
}

void SoundPlayer::playChooseEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/choose.mp3");
}

void SoundPlayer::playDecorateEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/decoration1 .mp3");
}

void SoundPlayer::playRotateEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/decoration.mp3");
}

void SoundPlayer::playShowTipEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/general_swish_2.mp3");
}

void SoundPlayer::playOpenMilkEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/open milk.mp3");
}

void SoundPlayer::playPourCerealEffect(){
    pourcerealID = SimpleAudioEngine::getInstance()->playEffect("sound/pour cereal .mp3", true);
}

void SoundPlayer::playSunRiseUpEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/sun rise.mp3");
}

void SoundPlayer::playEnterEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/swish.mp3");
}

void SoundPlayer::playPourMilkEffect(){
    pourmilkID = SimpleAudioEngine::getInstance()->playEffect("sound/milk.mp3", true);
}

void SoundPlayer::playEatEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/eat.mp3");
}

void SoundPlayer::playStriEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/stir.mp3", true);
}


void SoundPlayer::playSplashEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/splash.mp3");
}

void SoundPlayer::playHurahEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/hurrah.mp3");
}

