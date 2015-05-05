
//
//  SuperGlobal.h
//  DND2052_Baby_Play_Town
//
//  Created by liuwei on 14-12-25.
//
//

#ifndef DND2052_Baby_Play_Town_SuperGlobal_h
#define DND2052_Baby_Play_Town_SuperGlobal_h

#include <iostream>
#include "cocos2d.h"
using namespace std;
USING_NS_CC;

template<typename T> void replaceTheScene() {
    Scene* targetScene = T::scene();
    auto pScene=TransitionFade::create(1, targetScene);
    if (Director::getInstance()->getRunningScene()) {
        Director::getInstance()->replaceScene(pScene);
    }else {
        Director::getInstance()->runWithScene(pScene);
    }
}

template<typename T> void pushTheScene() {
    Scene* targetScene = T::scene();
    auto pScene=TransitionFade::create(1, targetScene);
    if (Director::getInstance()->getRunningScene()) {
        Director::getInstance()->pushScene(pScene);
    }else {
        Director::getInstance()->runWithScene(pScene);
    }
}

enum ALLItemOrder{
    kHomeBtn = 100,
    kPrompt,
    kDialog,
    kHomeDialog,
    kBackDialog,
    kLockTags = 120125,
};


#pragma mark game layer base will use these strings
#define PURCHASESUCCEED "PurchaseSucceed"

#define kPuaseGame "pause"
#define kResumeGame "resume"
#define kPuaseTip "pauseTip"
#define kResumeTip "resumeTip"

inline string convertIntToString(int num);
string convertIntToString(int num){
    __String* tempString = __String::createWithFormat("%d", num);
    stringstream ss;
    ss<<tempString->getCString();
    return ss.str();
}

inline Animation* createAnimation(const string& path, int min, int max, string formate = ".png");
Animation* createAnimation(const string& path, int min, int max, const string formate){
    Animation* pAnimation = Animation::create();
    for (int i = min; i <= max; ++i) {
        pAnimation->addSpriteFrameWithFile(path+convertIntToString(i)+formate);
    }
    return pAnimation;
}




#endif
