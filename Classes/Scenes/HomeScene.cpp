//
//  HomeScene.cpp
//  DONUT4013
//
//  Created by liuwei on 4/16/15.
//
//

#include "SceneHead.h"
#include "MoregameBtn.h"


Scene* HomeScene::scene(){
    Scene* pScene = Scene::create();
    
    HomeScene* pLayer = HomeScene::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}


bool HomeScene::init(){
    if (GameLayerBase::initWithBgFileName("home/start_bg0.jpg", false)) {
        Sprite* table = Sprite::create("home/table0.png");
        table->setAnchorPoint(Vec2(0.5, 0));
        table->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y + 60));
        addChild(table, 1);
        Animation* tableAnimtion = createAnimation("home/table", 0, 3);
        tableAnimtion->setLoops(1);
        tableAnimtion->setDelayPerUnit(0.8f);
        
        
        tableOriginal = table->getBoundingBox().origin;
        
        Animation* pAnimation = createAnimation("home/start_bg", 0, 7, ".jpg");
        pAnimation->setLoops(1);
        pAnimation->setDelayPerUnit(0.8f);
        m_pBg->runAction(Sequence::create(DelayTime::create(1.0), EaseSineInOut::create(Animate::create(pAnimation)), CallFunc::create([=]{
            this->addBowlThings();
        }),NULL));
        table->runAction(Sequence::create(DelayTime::create(1.0f), EaseSineInOut::create(Animate::create(tableAnimtion)), NULL));

        return true;
    }
    return false;
}

void HomeScene::addBowlThings(){
    vector<string> types = {"bees","bird","squirrel","rabbit"};
    currentIndex = arc4random() % 4 + 1;
    
    Sprite* bowl = Sprite::create("home/bowl.png");
    bowl->setPosition(tableOriginal + Vec2(242-1100, 456.5));
    addChild(bowl, 2);
    
    
    Sprite* cupcake = Sprite::create("home/cupcake.png");
    cupcake->setPosition(tableOriginal + Vec2(488.5, 438.5+1000));
    addChild(cupcake, 2);
    
    Sprite* logo = Sprite::create("home/logo.png");
    logo->setPosition(STVisibleRect::getCenterOfScene().x, STVisibleRect::getPointOfSceneLeftUp().y + 900);
    addChild(logo, 2);
    

    
    cereal = Sprite::create("home/cereal"+convertIntToString(currentIndex)+".png");
    cereal->setPosition(tableOriginal + Vec2(385.5+1000, 565));
    cereal->setTag(currentIndex);
    addChild(cereal, 1);
   
    
    cupcake->runAction(Sequence::create(EaseCircleActionInOut::create(MoveBy::create(0.8, Vec2(0, -1000))), DelayTime::create(0.8), CallFunc::create([=]{
        bowl->runAction(EaseExponentialInOut::create(MoveBy::create(0.8, Vec2(1100, 0))));
    }),DelayTime::create(0.8),CallFunc::create([=]{
        cereal->runAction(EaseElasticInOut::create(MoveBy::create(1.0, Vec2(-1000, 0)), 0.8));
    }), DelayTime::create(1.2f),CallFunc::create([=]{
        logo->runAction(EaseElasticInOut::create(MoveBy::create(1.2, Vec2(0, -1100)), 0.3));
    }), DelayTime::create(1.0f), CallFunc::create([=]{
        this->createCereal(types);
    }),DelayTime::create(2.0f),CallFunc::create(std::bind(&HomeScene::addAllButtons, this)),NULL));
}

void HomeScene::createCereal(vector<string> types){

    int max = DataContainer::getInstance()->getAnimalAnimationCount(types.at(currentIndex - 1));
    Animation* panimation = createAnimation("animals/animation/"+types.at(currentIndex-1)+"/"+types.at(currentIndex-1), 0, max);
    panimation->setLoops(-1);
    panimation->setDelayPerUnit(0.2);
    Sprite* animal = Sprite::create("animals/animation/"+types.at(currentIndex-1)+"/"+types.at(currentIndex-1)+convertIntToString(0)+".png");
    animal->setAnchorPoint(Vec2(0.5, 0));
    animal->setPosition(STVisibleRect::getPointOfSceneRightBottom() + Vec2(300, 20));
    addChild(animal, 3);
    
    animal->runAction(Sequence::create(DelayTime::create(1.5f),EaseSineInOut::create(JumpBy::create(0.8, Vec2(-500, 0), 500, 1)), Animate::create(panimation),NULL));
}

void HomeScene::addAllButtons(){
    MoregameBtn* pBtn = MoregameBtn::create();
    pBtn->addtoParentLayer(this);
    
    ControlButton* favoritebtn = CocosHelper::getButton("ui/home/home_btn_fav.png", "ui/home/home_btn_fav.png");
    ControlButton* shopbtn = CocosHelper::getButton("ui/home/home_btn_shop.png", "ui/home/home_btn_shop.png");
    ControlButton* startbtn = CocosHelper::getButton("ui/home/home_btn_play.png", "ui/home/home_btn_play.png");
    float startx = 0;
    shopbtn->setPosition(STVisibleRect::getPointOfSceneRightBottom() + Vec2(-20-shopbtn->getContentSize().width/2.0, 15 + shopbtn->getContentSize().height/2.0));
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    startx = STVisibleRect::getPointOfSceneRightBottom()
    shopbtn->setVisible(false);
#else
    startx = shopbtn->getPositionX() - shopbtn->getContentSize().width/2.0;
#endif
    favoritebtn->setPosition(startx - 15 - favoritebtn->getContentSize().width/2.0, STVisibleRect::getOriginalPoint().y+15+favoritebtn->getContentSize().height/2.0);
    favoritebtn->setPositionX(favoritebtn->getPositionX() + 800);
    shopbtn->setPositionX(shopbtn->getPositionX() + 800);
    
    addChild(favoritebtn, 10);
    addChild(shopbtn, 10);
    
    startbtn->setPosition(STVisibleRect::getCenterOfScene()+Vec2(-STVisibleRect::getGlvisibleSize().width, 0));
    addChild(startbtn, 10);
    
    shopbtn->setTag(kShopBtnTag);
    startbtn->setTag(kStartBtnTag);
    favoritebtn->setTag(kFavBtnTag);
    shopbtn->setZoomOnTouchDown(false);
    favoritebtn->setZoomOnTouchDown(false);
    startbtn->setZoomOnTouchDown(false);
    shopbtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HomeScene::onBtnClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    favoritebtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HomeScene::onBtnClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    startbtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HomeScene::onBtnClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    
    if (shopbtn->isVisible() == true) {
        favoritebtn->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(2.5, Vec2(-800, 0)), 2.0), CallFunc::create([=]{
            startbtn->runAction(EaseElasticIn::create(MoveBy::create(0.5f, Vec2(STVisibleRect::getGlvisibleSize().width, 0)), 0.3));
            pBtn->startLoading();
        }),NULL));
        shopbtn->runAction(Sequence::create(DelayTime::create(0.6f),  EaseElasticIn::create(MoveBy::create(2.5, Vec2(-800, 0)), 1.8), NULL));
    }else {
        favoritebtn->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(2.5, Vec2(-800, 0)), 2.0), CallFunc::create([=]{
            startbtn->runAction(EaseElasticIn::create(MoveBy::create(0.5f, Vec2(STVisibleRect::getGlvisibleSize().width, 0)), 0.3));
            pBtn->startLoading();
        }),NULL));
    }
    
}

void HomeScene::onBtnClicked(cocos2d::Ref *pRef, Control::EventType type) {
    static bool canClick = true;
    if (canClick == true) {
        canClick = false;
    }else {
        return;
    }

    Node* pNode = dynamic_cast<Node*>(pRef);
    pNode->runAction(Sequence::create((Sequence*)GameLayerBase::getJellyAction(), CallFunc::create([=]{
        switch (pNode->getTag()) {
            case kStartBtnTag:
                this->scheduleOnce(schedule_selector(HomeScene::changeScene), 0.5f);
                
                break;
            case kShopBtnTag:
                canClick = true;
                break;
            case kFavBtnTag:
                canClick = true;
                break;
            default:
                break;
        }
    }), NULL));
}

void HomeScene::changeScene(float) {
//    replaceTheScene<ChooseFlavor>();
    Scene* pScene = ChooseFlavor::scene();
    Director::getInstance()->replaceScene(pScene);
}
