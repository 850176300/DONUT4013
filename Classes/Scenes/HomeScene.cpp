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
    if (GameLayerBase::initWithBgFileName("home/bg.png", false)) {
        Sprite* windows = Sprite::create("home/window.png");
        windows->setPosition(STVisibleRect::getCenterOfScene() + Vec2(72, 160.5));
        addChild(windows, -2);
        
        table = Sprite::create("home/table3.png");
        table->setAnchorPoint(Vec2(0.5, 0));
        table->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y + 60));
        addChild(table, 1);
        
        sunShader = Sprite::create("home/start_bg0.png");
        sunShader->setPosition(STVisibleRect::getCenterOfScene());
        addChild(sunShader, kPrompt);

        tableOriginal = table->getBoundingBox().origin;

        table->setPosition(Vec2(table->getPosition().x - STVisibleRect::getGlvisibleSize().width, table->getPosition().y));
        
        Sprite* sunshine = Sprite::create("home/sun_1.png");
        Sprite* sun = Sprite::create("home/sun.png");
        
        theSun = LayerColor::create();
        theSun->setContentSize(sunshine->getContentSize());
        sunshine->setAnchorPoint(Vec2(372/753.0, 360/739.0));
        sunshine->setPosition(Vec2(sunshine->getContentSize().width/2.0, sunshine->getContentSize().height / 2.0));
        theSun->addChild(sunshine, 1);
        sun->setPosition(sunshine->getContentSize().width/2.0-4.5, sunshine->getContentSize().height / 2.0-8.5);
        theSun->addChild(sun, 2);
        
        theSun->ignoreAnchorPointForPosition(false);
        theSun->setAnchorPoint(Vec2(0.5,0.5));
        theSun->setPosition(windows->getBoundingBox().getMaxX()+80, windows->getBoundingBox().getMinY() - 50);
        addChild(theSun, -1);
        
        
        types = {"bees","bird","squirrel","rabbit"};
        currentIndex = arc4random() % 4 + 1;
        
        this->scheduleOnce(schedule_selector(HomeScene::cachetexutre), 1.5f);
        this->schedule(schedule_selector(HomeScene::performSunShine), 0.18);
        
//        sun->runAction(RepeatForever::create(RotateBy::create(4.0, 360)));
        sunshine->runAction(RepeatForever::create(RotateBy::create(3.6, 360)));
        
        ccBezierConfig config;
        config.controlPoint_1 = theSun->getPosition();
        config.endPosition = theSun->getPosition() + Vec2(-360, 280);
        config.controlPoint_2 = theSun->getPosition() + Vec2(-150, 200);
        theSun->runAction(EaseSineInOut::create(BezierTo::create(5.5, config)));
        
        this->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=]{
            SoundPlayer::getInstance()->playSunRiseUpEffect();
        }), DelayTime::create(2.5), CallFunc::create([=]{
            SoundPlayer::getInstance()->playSunRiseUpEffect();
        }),NULL));
        return true;
    }
    return false;
}

void HomeScene::performSunShine(float) {
    int opacity =  sunShader->getOpacity();
    opacity -= 8;
    if (opacity <= 0) {
        opacity = 0;
    }
    sunShader->setOpacity(opacity);
    if (opacity == 0) {
        unschedule(schedule_selector(HomeScene::performSunShine));
        table->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1.0, Vec2(STVisibleRect::getGlvisibleSize().width, 0)), 0.9), CallFunc::create(std::bind(&HomeScene::addBowlThings, this)),NULL));
    }
}

void HomeScene::cachetexutre(float) {
    int max = DataContainer::getInstance()->getAnimalAnimationCount(types.at(currentIndex - 1));
    AsyncCacheAnimation("animals/animation/"+types.at(currentIndex-1)+"/"+types.at(currentIndex-1), 0, max, ".png");
}

void HomeScene::onEnterTransitionDidFinish(){
    GameLayerBase::onEnterTransitionDidFinish();

}

void HomeScene::addBowlThings(){

    
    bowl = Sprite::create("home/bowl.png");
    bowl->setPosition(tableOriginal + Vec2(242-1100, 456.5));
    addChild(bowl, 2);
    
    
    Sprite* cupcake = Sprite::create("home/cupcake.png");
    cupcake->setPosition(tableOriginal + Vec2(488.5, 438.5+1000));
    addChild(cupcake, 2);
    
    Sprite* logo = Sprite::create("ui/home/start_logo.png");
    logo->setPosition(STVisibleRect::getCenterOfScene().x, STVisibleRect::getPointOfSceneLeftUp().y + 900);
    addChild(logo, 2);
    

    
    cereal = Sprite::create("home/cereal"+convertIntToString(currentIndex)+".png");
    cereal->setPosition(tableOriginal + Vec2(385.5+1000, 565));
    cereal->setTag(currentIndex);
    cereal->setAnchorPoint(Vec2(0.5, 0));
    cereal->setPosition(cereal->getPosition() + Vec2(0, -cereal->getContentSize().height/2.0));
    addChild(cereal, 1);
   
    
    cupcake->runAction(Sequence::create(Spawn::create(EaseCircleActionInOut::create(MoveBy::create(0.8, Vec2(0, -1000))),Sequence::create(DelayTime::create(0.6),CallFunc::create([=]{
        SoundPlayer::getInstance()->playEnterEffect();
    }), NULL), NULL), CallFunc::create([=]{
        bowl->runAction(Spawn::create(EaseExponentialInOut::create(MoveBy::create(0.8, Vec2(1100, 0))),Sequence::create(DelayTime::create(0.6),CallFunc::create([=]{
            SoundPlayer::getInstance()->playEnterEffect();
        }), NULL),nullptr));
        this->schedule(schedule_selector(HomeScene::actionjump), 6.5);
    }),CallFunc::create([=]{
        cereal->runAction(Spawn::create(EaseElasticInOut::create(MoveBy::create(1.0, Vec2(-1000, 0)), 0.8),Sequence::create(DelayTime::create(0.6),CallFunc::create([=]{
            SoundPlayer::getInstance()->playEnterEffect();
        }), NULL),nullptr));
        
        cereal->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(RotateBy::create(0.5, 8)), EaseSineInOut::create(RotateBy::create(0.8, -16)), RotateBy::create(0.5, 8), nullptr)));
    }), DelayTime::create(1.0f),CallFunc::create([=]{
        logo->runAction(EaseElasticInOut::create(MoveBy::create(1.2, Vec2(0, -1100)), 0.3));
        logo->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(MoveBy::create(1.0, Vec2(0, 30))), EaseSineInOut::create(MoveBy::create(1.0, Vec2(0, -30))), NULL)));
    }), CallFunc::create([=]{
        this->createCereal(types);
    }),DelayTime::create(2.0f),CallFunc::create(std::bind(&HomeScene::addAllButtons, this)),NULL));
}

void HomeScene::createCereal(vector<string> types){

    int max = DataContainer::getInstance()->getAnimalAnimationCount(types.at(currentIndex - 1));
    
    Animation* panimation = createAnimation("animals/animation/"+types.at(currentIndex-1)+"/"+types.at(currentIndex-1), 0, max);
    panimation->setLoops(-1);
    panimation->setDelayPerUnit(0.1f);
    
    Sprite* animal = Sprite::create("animals/animation/"+types.at(currentIndex-1)+"/"+types.at(currentIndex-1)+convertIntToString(0)+".png");
    animal->setScale(0.6f);
    animal->setAnchorPoint(Vec2(0.5, 0));
    animal->setPosition(STVisibleRect::getPointOfSceneRightBottom() + Vec2(300, 20));
    addChild(animal, 3);
    
    animal->runAction(Sequence::create(EaseSineInOut::create(JumpBy::create(2.5, Vec2(-500, 0), 500, 3)), Animate::create(panimation),NULL));
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
    
    startbtn->setPosition(STVisibleRect::getCenterOfScene()+Vec2(-STVisibleRect::getGlvisibleSize().width, -150));
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
        }),DelayTime::create(1.0),CallFunc::create([=]{
            startbtn->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(ScaleTo::create(2.0, 1.1f)), EaseSineInOut::create(ScaleTo::create(1.5, 1.0)), NULL)));
        }),NULL));
        shopbtn->runAction(Sequence::create(DelayTime::create(0.6f),  EaseElasticIn::create(MoveBy::create(2.5, Vec2(-800, 0)), 1.8), NULL));
    }else {
        favoritebtn->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(2.5, Vec2(-800, 0)), 2.0), CallFunc::create([=]{
            startbtn->runAction(EaseElasticIn::create(MoveBy::create(0.5f, Vec2(STVisibleRect::getGlvisibleSize().width, 0)), 0.3));
            pBtn->startLoading();
        }), DelayTime::create(1.0),CallFunc::create([=]{
            startbtn->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(ScaleTo::create(1.2, 1.2f)), EaseSineInOut::create(ScaleTo::create(0.8, 1.0)), NULL)));
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
                if (PurchaseManager::getInstance()->getRemoveAd() == false) {
                    STAds ads;
                    ads.requestInterstitialAds(true);
                }
                SoundPlayer::getInstance()->playStartClickedEffect();
                this->scheduleOnce(schedule_selector(HomeScene::changeScene), 0.5f);
                canClick = true;
                break;
            case kShopBtnTag:
                SoundPlayer::getInstance()->playclickeffect();
                pushTheScene<ShopScene>();
                canClick = true;
                break;
            case kFavBtnTag:
                SoundPlayer::getInstance()->playclickeffect();
                pushTheScene<FavoriteScene>();
                canClick = true;
                break;
            default:
                break;
        }
    }), NULL));
}

void HomeScene::actionjump(float) {
    bowl->runAction(JumpBy::create(0.3, Vec2(0, 0), 60, 1));
}

void HomeScene::changeScene(float) {
//    replaceTheScene<ChooseFlavor>();
    Scene* pScene = ChooseFlavor::scene();
    Director::getInstance()->replaceScene(pScene);
}
