//
//  LoadingScene.cpp
//  DONUT4013
//
//  Created by liuwei on 15/5/7.
//
//


#include "SceneHead.h"

Scene* LoadingScene::scene(){
    Scene* pScene = Scene::create();
    
    LoadingScene* pLayer = LoadingScene::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}


bool LoadingScene::init(){
    if (GameLayerBase::initWithBgFileName("ui/loading.png", false)) {
        
        return true;
    }
    return false;
}

void LoadingScene::onEnterTransitionDidFinish(){
    GameLayerBase::onEnterTransitionDidFinish();
    SoundPlayer::getInstance()->playBackGroundMusic("sound/background.mp3");
    this->scheduleOnce(schedule_selector(LoadingScene::switchScene), 2.0f);
}

void LoadingScene::switchScene(float) {
    STSystemFunction st;
    st.showNewsBlast();
    replaceTheScene<HomeScene>();
}