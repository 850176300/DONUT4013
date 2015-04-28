//
//  HomeScene.cpp
//  DONUT4013
//
//  Created by liuwei on 4/16/15.
//
//

#include "SceneHead.h"


Scene* HomeScene::scene(){
    Scene* pScene = Scene::create();
    
    HomeScene* pLayer = HomeScene::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}


bool HomeScene::init(){
    if (GameLayerBase::initWithBgFileName("home/start_bg0.jpg")) {
        Sprite* table = Sprite::create("home/table0.png");
        table->setAnchorPoint(Vec2(0.5, 0));
        table->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y + 60));
        addChild(table, 1);
        Animation* tableAnimtion = createAnimation("home/table", 0, 3);
        tableAnimtion->setLoops(1);
        tableAnimtion->setDelayPerUnit(0.8f);
        table->runAction(Sequence::create(DelayTime::create(1.0f), Animate::create(tableAnimtion), NULL));
        
        tableOriginal = table->getBoundingBox().origin;
        
        Animation* pAnimation = createAnimation("home/start_bg", 0, 7, ".jpg");
        pAnimation->setLoops(1);
        pAnimation->setDelayPerUnit(0.8f);
        m_pBg->runAction(Sequence::create(DelayTime::create(1.0), EaseSineInOut::create(Animate::create(pAnimation)), CallFunc::create([=]{
            this->addBowlThings();
        }),NULL));
        

        return true;
    }
    return false;
}

void HomeScene::addBowlThings(){
    
    log("the orignal x = %.2f, the y = %.2f", STVisibleRect::getOriginalPoint().x, STVisibleRect::getOriginalPoint().y);
    
    log("the orignal width = %.2f, the height = %.2f", STVisibleRect::getGlvisibleSize().width, STVisibleRect::getGlvisibleSize().height);
    
    log("the orignal windows width = %.2f, the height = %.2f", Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
    
    log("the orignal offset width = %.2f, the height = %.2f", STVisibleRect::getDesignOffset().x, STVisibleRect::getDesignOffset().y);
    
    log("the direction scale is %.2f, y = %.2f", Director::getInstance()->getOpenGLView()->getScaleX(), Director::getInstance()->getOpenGLView()->getScaleY());
    
    
    
    Sprite* bowl = Sprite::create("home/bowl.png");
    bowl->setPosition(tableOriginal + Vec2(242-1100, 456.5));
    addChild(bowl, 2);
    
    
    Sprite* cupcake = Sprite::create("home/cupcake.png");
    cupcake->setPosition(tableOriginal + Vec2(488.5, 438.5+1000));
    addChild(cupcake, 2);
    
    Sprite* logo = Sprite::create("home/logo.png");
    logo->setPosition(STVisibleRect::getCenterOfScene().x, STVisibleRect::getPointOfSceneLeftUp().y + 900);
    addChild(logo, 2);
    
    
    
    cupcake->runAction(Sequence::create(EaseCircleActionInOut::create(MoveBy::create(0.8, Vec2(0, -1000))), DelayTime::create(0.8), CallFunc::create([=]{
        bowl->runAction(EaseExponentialInOut::create(MoveBy::create(0.8, Vec2(1100, 0))));
    }),DelayTime::create(0.8),CallFunc::create([=]{
        logo->runAction(EaseElasticInOut::create(MoveBy::create(1.2, Vec2(0, -1100)), 0.3));
    }), CallFunc::create([=]{
        this->createCereal();
    }),NULL));
}

void HomeScene::createCereal(){
    vector<string> types = {"bees","bird","squirrel","rabbit"};
    currentIndex = arc4random() % 4 + 1;
    
    if (cereal == nullptr) {
        cereal = Sprite::create("home/cereal"+convertIntToString(currentIndex)+".png");
        cereal->setPosition(tableOriginal + Vec2(385.5+1000, 565));
        cereal->setTag(currentIndex);
        addChild(cereal, 1);
        cereal->runAction(EaseElasticInOut::create(MoveBy::create(1.0, Vec2(-1000, 0)), 0.8));
    }else {
        cereal->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(0.8, Vec2(-1000, 0)), 0.3),CallFunc::create([=]{
            cereal->setTexture("home/cereal"+convertIntToString(currentIndex)+".png");
            cereal->setPosition(tableOriginal + Vec2(385.5+1000, 565));
            cereal->runAction(EaseElasticInOut::create(MoveBy::create(1.0, Vec2(-1000, 0)), 0.8));
        }),NULL));
    }
    int max = DataContainer::getInstance()->getAnimalAnimationCount(types.at(currentIndex - 1));
    Animation* panimation = createAnimation("animals/animation/"+types.at(currentIndex-1)+"/"+types.at(currentIndex-1), 0, max);
    panimation->setLoops(-1);
    panimation->setDelayPerUnit(0.2);
    Sprite* animal = Sprite::create("animals/animation/"+types.at(currentIndex-1)+"/"+types.at(currentIndex-1)+convertIntToString(0)+".png");
    animal->setAnchorPoint(Vec2(0.5, 0));
    animal->setPosition(STVisibleRect::getPointOfSceneRightBottom() + Vec2(300, 20));
    addChild(animal, 3);
    
    animal->runAction(Sequence::create(DelayTime::create(1.0f),EaseSineInOut::create(JumpBy::create(0.8, Vec2(-500, 0), 500, 1)), Animate::create(panimation), DelayTime::create(8.0f), CallFunc::create(std::bind(&HomeScene::createCereal, this)),EaseElasticInOut::create(JumpBy::create(0.8, Vec2(-500, 0), 500, 1), 0.3), RemoveSelf::create(),NULL));
}

