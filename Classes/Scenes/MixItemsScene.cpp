//
//  MixItemsScene.cpp
//  DONUT4013
//
//  Created by liuwei on 4/23/15.
//
//

#include "SceneHead.h"


Scene* MixItemScene::scene(){
    Scene* pScene = Scene::create();
    
    MixItemScene* pLayer = MixItemScene::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}

bool MixItemScene::init(){
    if (GameLayerBase::initWithBgFileName("make/bg_make_table_cover.png")) {
        DataContainer::FlavorInfor info = DataContainer::getInstance()->getTheFlavorByName(DataContainer::getInstance()->getChooseFlavor());
        
        Sprite* bg2 = Sprite::create("make/bg/tablebg_"+info.name+".png");
        bg2->setPosition(STVisibleRect::getCenterOfScene() + Vec2(0, 120));
        addChild(bg2, -1);
        
        Sprite* table = Sprite::create("make/bg_make_table_table.png");
        table->setAnchorPoint(Vec2(0.5, 0));
        table->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y));
        addChild(table, 1);
        
        tableMaxy = table->getBoundingBox().getMaxY();

        showPreviousBtn(2.0f);
        return true;
    }
    return false;
}


void MixItemScene::onEnterTransitionDidFinish(){
    GameLayerBase::onEnterTransitionDidFinish();
    
    string flavor = DataContainer::getInstance()->getChooseFlavor();
    flavorBox = CocosHelper::getButton("make/flavor/"+flavor+".png","make/flavor/"+flavor+".png");
    flavorBox->setAnchorPoint(Vec2(0.5, 0));
    flavorBox->setName(flavor);
    flavorBox->setPosition(-500, tableMaxy-80);
    flavorBox->setTag(kFlavorTag);
    addChild(flavorBox, 2);
    
    flavorBox->setScale(0.7f);
    flavorBox->addTargetWithActionForControlEvents(this, cccontrol_selector(MixItemScene::onselectTheItem) , cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    flavorBox->setZoomOnTouchDown(false);
    flavorBox->setEnabled(false);
    
    string milk = DataContainer::getInstance()->getChooseMilk();
    milkBox = Sprite::create("make/milk/"+milk+"_small.png");
    milkBox->setAnchorPoint(Vec2(0.5, 0));
    milkBox->setName(milk);
    milkBox->setPosition(STVisibleRect::getGlvisibleSize().width+500, tableMaxy-80);
    milkBox->setTag(kMilkTag);
    addChild(milkBox, 2);
    
    Sprite* bowl1 = Sprite::create("make/bowl_1.png");
    Sprite* bowl2 = Sprite::create("make/bowl.png");
    bowl1->setPosition(STVisibleRect::getCenterOfScene().x - 800, tableMaxy - 150);
    bowl2->setPosition(bowl1->getPosition());
    addChild(bowl1, 6);
    addChild(bowl2, 3);
    

    
    tiltSprite = Sprite::create("ui/make/tips_tilt.png");
    tiltSprite->setPosition(STVisibleRect::getCenterOfScene().x, STVisibleRect::getGlvisibleSize().height + 800);
    addChild(tiltSprite, 10);
    bowl1->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(1.0, Vec2(800, 0)), 0.8), NULL));
    bowl2->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(1.0, Vec2(800, 0)), 0.8), CallFunc::create([=]{
        flavorBox->runAction(Spawn::create(EaseElasticInOut::create(MoveTo::create(1.2, Vec2(STVisibleRect::getCenterOfScene().x - 175, tableMaxy-80)), 1.0), Sequence::create(DelayTime::create(1.0), CallFunc::create([=]{
            SoundPlayer::getInstance()->playEnterEffect();
        }), NULL), NULL));
        milkBox->runAction(Sequence::create(DelayTime::create(1.2), Spawn::create( EaseElasticInOut::create(MoveTo::create(1.2, Vec2(STVisibleRect::getCenterOfScene().x+225, tableMaxy - 80)), 1.0),Sequence::create(DelayTime::create(1.0), CallFunc::create([=]{
            SoundPlayer::getInstance()->playEnterEffect();
        }), NULL), nullptr), NULL));
        this->schedule(schedule_selector(MixItemScene::performTip), 5.0, -1, 4.0);
        flavorBox->setEnabled(true);
        
        flavorTexture = Director::getInstance()->getTextureCache()->addImage("make/cereals_stir/"+flavor+".png");
        flavorTexture->retain();
       
        flavorinBowl = Sprite::createWithTexture(flavorTexture);
        flavorinBowl->setPosition(bowl1->getPosition());
        flavorinBowl->setName(flavorBox->getName());
        addChild(flavorinBowl, 5);
        flavorinBowl->setVisible(false);
        
        milkinBowl = Sprite::create("make/milk_stir/"+milk+".png");
        milkinBowl->setOpacity(0);
        milkinBowl->setPosition(bowl1->getPosition());
        addChild(milkinBowl, 5);
    }), NULL));
    
    currentItem = flavorBox;
    currentItem->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(RotateBy::create(0.5, 8)), EaseSineInOut::create(RotateBy::create(0.8, -16)), RotateBy::create(0.5, 8), nullptr)));
    
    DataContainer::FlavorInfor _info = DataContainer::getInstance()->getTheFlavorByName(flavor);
    vector<string> types = {"bees","bird","squirrel","rabbit"};
    for (vector<string>::size_type i = 0; i < types.size(); ++i) {
        if (_info.type == types.at(i)) {
            int max = DataContainer::getInstance()->getAnimalAnimationCount(types.at(i));
            AsyncCacheAnimation("animals/animation/"+types.at(i)+"/"+types.at(i), 0, max, ".png");
        }

    }
}


void MixItemScene::onselectTheItem(cocos2d::Ref *pRef, Control::EventType type) {
    if (type == Control::EventType::TOUCH_UP_INSIDE) {
        ControlButton* pNode = dynamic_cast<ControlButton*>(pRef);
        
        if (pNode->getTag() == kFlavorTag) {
            unschedule(schedule_selector(MixItemScene::performTip));
            
            tiltSprite->runAction(Spawn::create(EaseElasticInOut::create(MoveBy::create(1.5, Vec2(0, -900)), 1.2), Sequence::create(DelayTime::create(0.6), CallFunc::create([=]{
                SoundPlayer::getInstance()->playShowTipEffect();
            }), NULL), NULL));
            
            this->schedule(schedule_selector(MixItemScene::shakeTheTitle), 4.0f);
            pNode->stopAllActions();
            pNode->setRotation(0);
            milkBox->runAction(EaseElasticInOut::create(MoveBy::create(0.8, Vec2(1000, 0)), 0.3));
            pNode->setEnabled(false);
            pNode->runAction(Sequence::create(JumpBy::create(0.5, Vec2(STVisibleRect::getCenterOfScene().x - pNode->getPositionX(), -30), 150, 1),CallFunc::create(std::bind(&ControlButton::setLocalZOrder, pNode, 7)),CallFunc::create(std::bind(&MixItemScene::startDeviceTip, this)), NULL));
        }else if (pNode->getTag() == kMilkTag) {
            
        }
    }
}


void MixItemScene::shakeTheTitle(float) {
    tiltSprite->runAction(Sequence::create(EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))), EaseSineInOut::create(MoveBy::create(0.2, Vec2(0, -20))), EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))),EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))), EaseSineInOut::create(MoveBy::create(0.2, Vec2(0, -20))), EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))), NULL));
}

void MixItemScene::startDeviceTip(){
    if (deviceTip == nullptr) {
        deviceTip = Sprite::create("ui/devicerotate.png");
        deviceTip->setPosition(currentItem->getBoundingBox().getMidX(), currentItem->getBoundingBox().getMidY());
        deviceTip->setOpacity(0);
        this->addChild(deviceTip, 20);
    }
    deviceTip->runAction(Sequence::create(EaseSineInOut::create(FadeIn::create(0.5)), DelayTime::create(1.5f), CallFunc::create([=]{
        this->setAccelerometerEnabled(true);
        this->setAccelerometerInterval(0.1f);
    }), NULL));
    deviceTip->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(RotateBy::create(1.0, -10)), EaseSineInOut::create(RotateBy::create(1.0, 20)),EaseSineInOut::create(RotateBy::create(1.0, -10)),NULL)));

}

void MixItemScene::performTip(float) {
    currentItem->runAction(Sequence::create(EaseElasticInOut::create(JumpBy::create(0.9, Vec2::ZERO, 180, 1), 0.5), DelayTime::create(0.2), NULL));
}

void MixItemScene::pourFlavor(){
    maskRender = RenderTexture::create(465, 335, Texture2D::PixelFormat::RGBA8888);
    maskRender->retain();
    maskRender->setPosition(0, 0);
    const float rotate = 45;
    flavorBox->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(-460, 100)),CallFunc::create(std::bind(&Node::setLocalZOrder, flavorBox, 10)), RotateBy::create(0.5, rotate),NULL), CallFunc::create([=]{
        flavorBox->setBackgroundSpriteForState(Scale9Sprite::create("make/flavor/"+flavorBox->getName()+"1.png"), cocos2d::extension::Control::State::NORMAL);
        flavorBox->setBackgroundSpriteForState(Scale9Sprite::create("make/flavor/"+flavorBox->getName()+"1.png"), cocos2d::extension::Control::State::DISABLED);
    }), CallFunc::create([=]{
        DataContainer::FlavorInfor infor = DataContainer::getInstance()->getTheFlavorByName(flavorBox->getName());
        if (infor.pcount == 1) {
            ParticleSystemQuad* _particle = ParticleSystemQuad::create("make/flavor/pourflavor.plist");
            Texture2D* ptexture = Director::getInstance()->getTextureCache()->addImage("make/pour_cereal/pour_"+flavorBox->getName()+".png");
            _particle->setTexture(ptexture);
            _particle->setPosition(flavorBox->getPosition() + Vec2(337,163.6));
            addChild(_particle, 9);
            _particle->setAutoRemoveOnFinish(true);
        }else {
            for (int i = 0; i < infor.pcount; ++i) {
                ParticleSystemQuad* _particle = ParticleSystemQuad::create("make/flavor/pourflavor.plist");
                Texture2D* ptexture = Director::getInstance()->getTextureCache()->addImage("make/pour_cereal/pour_"+flavorBox->getName()+convertIntToString(i+1)+".png");
                _particle->setTexture(ptexture);
                _particle->setTotalParticles(60/infor.pcount);
                _particle->setPosition(flavorBox->getPosition() + Vec2(337,163.6));
                addChild(_particle, 9);
                _particle->setAutoRemoveOnFinish(true);
            }
        }
        SoundPlayer::getInstance()->playPourCerealEffect();
        this->schedule(schedule_selector(MixItemScene::showflovar), 0.5);
    }),nullptr));


}

void MixItemScene::pourMilk(){
    milkBox->runAction(Sequence::create(Spawn::create(MoveBy::create(0.3, Vec2(280, 100)), RotateBy::create(0.3, -45), NULL),DelayTime::create(0.2), CallFunc::create([=]{
        _pourMilk = ParticleSystemQuad::create("make/milk/pour.plist");
        _pourMilk->setPosition(milkBox->getPosition()+Vec2(-151.5, 24.5));
        Texture2D* ptexture = Director::getInstance()->getTextureCache()->addImage("make/pour milk/pour milk_"+milkBox->getName()+".png");
        _pourMilk->setTexture(ptexture);
        this->addChild(_pourMilk, 10);
        SoundPlayer::getInstance()->playPourMilkEffect();
        this->schedule(schedule_selector(MixItemScene::showmilk), 0.5);
    }), nullptr));
}

void MixItemScene::showflovar(float) {
    upH = upH + 20;
    if (upH >= 0) {
        upH = 0;
    }
    log("the up H is %.2f", upH);
    Sprite* mask = Sprite::create("make/mask.png");
    mask->setAnchorPoint(Vec2(0, 0));
    mask->setBlendFunc(BlendFunc{GL_ZERO,GL_ONE_MINUS_SRC_ALPHA});
    
    
    Sprite* flavor = Sprite::createWithTexture(flavorTexture);
    flavor->setAnchorPoint(Vec2::ZERO);
    flavor->setPositionY(upH);
    maskRender->beginWithClear(1.0, 1.0, 1.0, 0);
    flavor->visit();
    mask->visit();
    maskRender->end();
    
    Director::getInstance()->getRenderer()->render();
    maskRender->getSprite()->getTexture()->setAntiAliasTexParameters();
    maskRender->getSprite()->setFlippedY(false);
    
    Texture2D* texture = new Texture2D();
    texture->initWithImage(maskRender->newImage());
    flavorinBowl->setTexture(texture);
    texture->autorelease();
    flavorinBowl->setVisible(true);

    if (upH >= 0) {
        SoundPlayer::getInstance()->stopAlleffect();
        unschedule(schedule_selector(MixItemScene::showflovar));
        flavorBox->runAction(Sequence::create(RotateBy::create(0.3, -45), MoveBy::create(0.5, Vec2(-500, 0)), CallFunc::create([=]{
            milkBox->setLocalZOrder(10);
            milkBox->runAction(Sequence::create(EaseElasticIn::create(MoveTo::create(1.1, Vec2(STVisibleRect::getCenterOfScene().x, milkBox->getPositionY())), 0.9), CallFunc::create(std::bind(&MixItemScene::cutMilk, this)), NULL));
        }),RemoveSelf::create(),NULL));
    }
}

void MixItemScene::setStriResetTexture(){
    Sprite* mask = Sprite::create("make/mask.png");
    mask->setAnchorPoint(Vec2(0, 0));
    mask->setBlendFunc(BlendFunc{GL_ZERO,GL_ONE_MINUS_SRC_ALPHA});
    
    
    Sprite* flavor = Sprite::createWithTexture(flavorTexture);
    flavor->setAnchorPoint(Vec2::ZERO);
    flavor->setPositionY(upH);
    maskRender->beginWithClear(1.0, 1.0, 1.0, 0);
    flavor->visit();
    mask->visit();
    maskRender->end();
    
    Director::getInstance()->getRenderer()->render();
    maskRender->getSprite()->getTexture()->setAntiAliasTexParameters();
    maskRender->getSprite()->setFlippedY(false);
    
    Texture2D* texture = new Texture2D();
    texture->initWithImage(maskRender->newImage());
    flavorinBowl->setTexture(texture);
    texture->autorelease();
    
    if (milkinBowl != nullptr) {
        milkinBowl->setPositionY(tableMaxy - 150);
    }
}

void MixItemScene::setStriDoneTexture(){
    Sprite* mask = Sprite::create("make/mask.png");
    mask->setAnchorPoint(Vec2(0, 0));
    mask->setBlendFunc(BlendFunc{GL_ZERO,GL_ONE_MINUS_SRC_ALPHA});
    
    
    Sprite* flavor = Sprite::create("make/cereals_stir/"+flavorinBowl->getName()+"_1.png");
    flavor->setAnchorPoint(Vec2::ZERO);
    flavor->setPositionY(upH);
    maskRender->beginWithClear(1.0, 1.0, 1.0, 0);
    flavor->visit();
    mask->visit();
    maskRender->end();
    
    Director::getInstance()->getRenderer()->render();
    maskRender->getSprite()->getTexture()->setAntiAliasTexParameters();
    maskRender->getSprite()->setFlippedY(false);
    
    Texture2D* texture = new Texture2D();
    texture->initWithImage(maskRender->newImage());
    flavorinBowl->setTexture(texture);
    texture->autorelease();
}

void MixItemScene::stritheCereal(float) {
    if (striMove == true && striFinished == false) {
        recordCount++;
        if (recordCount / 10 == 1) {
            recordCount = 0;
            
            upH -= 4;
            Sprite* mask = Sprite::create("make/mask.png");
            mask->setAnchorPoint(Vec2(0, 0));
            mask->setBlendFunc(BlendFunc{GL_ZERO,GL_ONE_MINUS_SRC_ALPHA});
            
            
            Sprite* flavor = Sprite::createWithTexture(flavorTexture);
            flavor->setAnchorPoint(Vec2::ZERO);
            flavor->setPositionY(upH);
            maskRender->beginWithClear(1.0, 1.0, 1.0, 0);
            flavor->visit();
            mask->visit();
            maskRender->end();
            
            Director::getInstance()->getRenderer()->render();
            maskRender->getSprite()->getTexture()->setAntiAliasTexParameters();
            maskRender->getSprite()->setFlippedY(false);
            
            Texture2D* texture = new Texture2D();
            texture->initWithImage(maskRender->newImage());
            flavorinBowl->setTexture(texture);
            texture->autorelease();
            flavorinBowl->setVisible(true);
            if (milkinBowl != nullptr) {
                 milkinBowl->setPositionY(milkinBowl->getPositionY() - 4);
            }
           
        }
    }
}

void MixItemScene::cutMilk(){
    milkBox->setAnchorPoint(Vec2(0.5, 0.5));
    milkBox->setPosition(milkBox->getPosition()+Vec2(0, milkBox->getContentSize().height/2.0));
    lineTip = Button::create("ui/make/tips_line.png");
    lineTip->setPosition(Vec2(milkBox->getBoundingBox().getMinX()+50, milkBox->getBoundingBox().getMaxY()-50));
    addChild(lineTip, 10);
    lineTip->setEnabled(true);
    lineTip->addTouchEventListener(CC_CALLBACK_2(MixItemScene::ontouchLine, this));

    gestures = Sprite::create("make/gestures.png");
    gestures->setPosition(Vec2(lineTip->getBoundingBox().getMaxX()-15, lineTip->getBoundingBox().getMaxY()+15) + Vec2(-gestures->getContentSize().width/2.0, -gestures->getContentSize().height/2.0));
    addChild(gestures, 11);
    
    this->schedule(schedule_selector(MixItemScene::replayHandTip), 5.0, -1, 1.0);
    
//    lineTip->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 0.8), ScaleTo::create(0.3, 1.0), NULL)));
    

}

void MixItemScene::replayHandTip(float) {
    gestures->runAction(Sequence::create(EaseSineInOut::create(MoveBy::create(1.0, Vec2(-150, -tan(CC_DEGREES_TO_RADIANS(46.9))*150))), DelayTime::create(0.5), EaseSineInOut::create(MoveBy::create(0.5, Vec2(150, tan(CC_DEGREES_TO_RADIANS(46.9))*150))),NULL));
}

void MixItemScene::startHandTip(){
    gestures->stopAllActions();
    gestures->setPosition(Vec2(lineTip->getBoundingBox().getMaxX()-15, lineTip->getBoundingBox().getMaxY()+15) + Vec2(-gestures->getContentSize().width/2.0, -gestures->getContentSize().height/2.0));
    gestures->setVisible(true);
    gestures->runAction(Sequence::create(EaseSineInOut::create(MoveBy::create(1.0, Vec2(-150, -tan(CC_DEGREES_TO_RADIANS(46.9))*150))), DelayTime::create(0.5), EaseSineInOut::create(MoveBy::create(0.5, Vec2(150, tan(CC_DEGREES_TO_RADIANS(46.9))*150))),NULL));
    schedule(schedule_selector(MixItemScene::replayHandTip), 5.0);
}

void MixItemScene::ontouchLine(cocos2d::Ref *pRef, Widget::TouchEventType rtype) {
    Vec2 begin;
    Vec2 move;
    Button* tip = dynamic_cast<Button*>(pRef);
    if (rtype == Widget::TouchEventType::BEGAN) {
        begin = tip->getTouchBeganPosition();
        moveDistance = 0;
        if (gestures != nullptr) {
            gestures->setVisible(false);
        }
        unschedule(schedule_selector(MixItemScene::replayHandTip));
    }else if (rtype == Widget::TouchEventType::MOVED) {
        move = tip->getTouchMovePosition();
        float distance = move.distance(begin);
        if (distance > moveDistance) {
            moveDistance = distance;
        }
    }else if (rtype == Widget::TouchEventType::ENDED){
        Rect rect;
        rect.origin = tip->convertToWorldSpace(Vec2(0, 0)) + Vec2(-150, -150);
        rect.size = tip->getContentSize().operator+(Size(150, 150));
        if (!rect.containsPoint(tip->getTouchEndPosition())) {
            
            startHandTip();
            return;
        }
        if (moveDistance > 100) {
            milkBox->runAction(GameLayerBase::getJellyAction());
            cutCount -= 1;
            if (cutCount > 0) {
                
                startHandTip();
            }else {
                removeChild(gestures);
                gestures = nullptr;
                lineTip->setEnabled(false);
                lineTip->runAction(Sequence::create(EaseSineInOut::create(FadeOut::create(0.5)), RemoveSelf::create(), NULL));
                milkBox->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=]{
                    milkBox->setTexture("make/milk/"+milkBox->getName()+"_1_small.png");
                    SoundPlayer::getInstance()->playOpenMilkEffect();
                }), DelayTime::create(0.5f),CallFunc::create([=]{
                    tiltSprite->runAction(Spawn::create(EaseElasticInOut::create(MoveBy::create(1.5, Vec2(0, -900)), 1.2), Sequence::create(DelayTime::create(1.2), CallFunc::create([=]{
                        SoundPlayer::getInstance()->playShowTipEffect();
                    }), NULL), NULL));
                    this->schedule(schedule_selector(MixItemScene::shakeTheTitle), 4.0f);
                }), CallFunc::create(std::bind(&MixItemScene::startDeviceTip, this)), NULL));
                currentItem = milkBox;
            }
        }else{
            
            startHandTip();
        }
        
        
    }else if (rtype == Widget::TouchEventType::CANCELED){
        Rect rect;
        rect.origin = tip->convertToWorldSpace(Vec2(0, 0)) + Vec2(-150, -150);
        rect.size = tip->getContentSize().operator+(Size(150, 150));
        if (!rect.containsPoint(tip->getTouchEndPosition())) {
            
            startHandTip();
            return;
        }
        
        if (moveDistance > 100) {
            milkBox->runAction(GameLayerBase::getJellyAction());
            cutCount -= 1;
            if (cutCount > 0) {
                
                startHandTip();
            }else {
                removeChild(gestures);
                gestures = nullptr;
                lineTip->setEnabled(false);
                lineTip->runAction(Sequence::create(EaseSineInOut::create(FadeOut::create(0.5)), RemoveSelf::create(), NULL));
                milkBox->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=]{
                    milkBox->setTexture("make/milk/"+milkBox->getName()+"_1_small.png");
                    SoundPlayer::getInstance()->playOpenMilkEffect();
                }), DelayTime::create(0.5f),CallFunc::create([=]{
                    tiltSprite->runAction(Spawn::create(EaseElasticInOut::create(MoveBy::create(1.2, Vec2(0, -900)), 1.2), Sequence::create(DelayTime::create(0.6), CallFunc::create([=]{
                        SoundPlayer::getInstance()->playShowTipEffect();
                    }), NULL), NULL));
                    this->schedule(schedule_selector(MixItemScene::shakeTheTitle), 4.0f);
                }), CallFunc::create(std::bind(&MixItemScene::startDeviceTip, this)), NULL));
                currentItem = milkBox;
            }
        }else{
            
            startHandTip();
        }
    }
    
}

void MixItemScene::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *unused_event) {
    if (fabs(acc->x) > 0.5) {
        if (currentItem == nullptr) {
            return;
        }
        switch (currentItem->getTag()) {
            case kFlavorTag:
            {
                this->unschedule(schedule_selector(MixItemScene::shakeTheTitle));
                tiltSprite->stopAllActions();
                tiltSprite->runAction(EaseElasticInOut::create(MoveBy::create(0.8, Vec2(0, 900)), 0.6));
                currentItem = nullptr;
                setAccelerometerEnabled(false);
                deviceTip->stopAllActions();
                deviceTip->runAction(Sequence::create(EaseSineInOut::create(FadeOut::create(0.2f)), RotateTo::create(0.0, 0), CallFunc::create(std::bind(&MixItemScene::pourFlavor, this)),NULL));

                
            }
                break;
            case kMilkTag:
            {
                this->unschedule(schedule_selector(MixItemScene::shakeTheTitle));
                tiltSprite->stopAllActions();
                tiltSprite->runAction(EaseElasticInOut::create(MoveBy::create(0.8, Vec2(0, 900)), 0.6));
                currentItem = nullptr;
                setAccelerometerEnabled(false);
                deviceTip->stopAllActions();
                deviceTip->runAction(Sequence::create(EaseSineInOut::create(FadeOut::create(0.2f)), RotateTo::create(0.0, 0), CallFunc::create(std::bind(&MixItemScene::pourMilk, this)),NULL));
                
            }
                break;
            default:
                break;
        }
    }
}

void MixItemScene::showmilk(float) {
    int opacity = milkinBowl->getOpacity();
    opacity += 20;
    if (opacity > 255) {
        opacity = 255;
    }
    milkinBowl->setOpacity(opacity);
    if (opacity >= 255) {
        SoundPlayer::getInstance()->stopAlleffect();
        unschedule(schedule_selector(MixItemScene::showmilk));
        milkBox->runAction(Sequence::create(RotateBy::create(0.2, 45), EaseElasticOut::create(MoveBy::create(0.3, Vec2(1000, 0)), 0.2), RemoveSelf::create(),NULL));
        
        _pourMilk->setDuration(0.1);
        _pourMilk->setAutoRemoveOnFinish(true);
        spoon = Sprite::create("make/spoon.png");
        spoon->setAnchorPoint(Vec2(0.5, 0));
        spoon->setPosition(Vec2(STVisibleRect::getCenterOfScene().x + 1000, milkinBowl->getPositionY()));
        this->addChild(spoon, 11);
        spoon->runAction(Sequence::create(DelayTime::create(0.5f),EaseElasticOut::create(MoveBy::create(1.2, Vec2(-1000, 0)), .7f), CallFunc::create([=]{
            Sprite* tip = Sprite::create("make/tips1.png");
            tip->setPosition(spoon->getBoundingBox().getMidX(), spoon->getBoundingBox().getMidY());
            tip->setTag(kSpoonTip);
            
            Animation* pAnimation = createAnimation("make/tips", 1, 4);
            pAnimation->setLoops(-1);
            pAnimation->setDelayPerUnit(0.3);
            this->addChild(tip, 11);
            tip->runAction(Animate::create(pAnimation));
        }), NULL));
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        
        listener->onTouchBegan = [=](Touch *touch, Event *event){
            if (striFailed == true) {
                return false;
            }
            
            bool isResult = CocosHelper::isPointInNode(spoon, touch->getLocation());
            if (isResult == true) {
                SoundPlayer::getInstance()->playStriEffect();
                striMove = false;
                if (striFinished == false) {
                    this->schedule(schedule_selector(MixItemScene::stritheCereal), 0.1);
                }
                
            }
            return isResult;
        };
        listener->onTouchMoved = [=](Touch *touch, Event *event){
            striMove = true;
            if (striFailed == true) {
                return ;
            }
            if (this->getChildByTag(kSpoonTip)) {
                this->removeChildByTag(kSpoonTip);
            }
            Vec2 afterPP = Vec2(spoon->getPositionX(), spoon->getBoundingBox().getMidY()) + touch->getDelta();
            if (STVisibleRect::getMovableRect().containsPoint(afterPP)) {
                Vec2 angleVec1 = flavorinBowl->getPosition() - spoon->getPosition();
                spoon->setPosition(spoon->getPosition() + touch->getDelta());
                Vec2 anglevec2 = flavorinBowl->getPosition() - spoon->getPosition();
                float angle = angleVec1.getAngle(anglevec2);
                
                
                if (striFinished == false && striFailed == false) {
                    log("the stri angle is %.2f", angle*180.0/M_PI);
                    if (fabs(angle*180.0/M_PI) > 18) {
                        superSpeed += 1;
                        normalSpeed = 0;
                    }else {
                        normalSpeed += 1;
                        if (normalSpeed > 6) {
                            superSpeed = 0;
                        }
                    }
                    if (superSpeed > 5) {
                        log("stri failed!!!!!!");
                        superSpeed = 0;
                        normalSpeed = 0;
                        striFailed = true;
                        milkinBowl->runAction(Sequence::create(FadeIn::create(0.3f), DelayTime::create(4.5f), CallFunc::create([=]{
                            this->setStriResetTexture();
                            striFailed = false;
                        }), NULL));
                        this->schedule(schedule_selector(MixItemScene::addExploreImage), 1.0, 3, 0.5f);
                        return ;
                    }
                }
                if (milkinBowl == nullptr) {
                    return ;
                }
                Rect donutRect = Rect(flavorinBowl->getPosition().x - 460/2.0, flavorinBowl->getPosition().y - 248/2.0, 460, 248);
                if (donutRect.containsPoint(spoon->getPosition())) {
                    int opacity = milkinBowl->getOpacity();
                    opacity -= 1.0;
                    if (opacity <= 0) {
                        opacity = 0;
                    }
                    milkinBowl->setOpacity(opacity);
                    if (opacity == 0) {
                        striFinished = true;
                        this->unschedule(schedule_selector(MixItemScene::stritheCereal));
                        milkinBowl->removeFromParent();
                        milkinBowl = nullptr;
                        
//                        string file = "make/cereals_stir/"+flavorinBowl->getName()+"_1.png";
//                        log("change file texture is %s", file.c_str());
//                        flavorinBowl->setTexture(file);
                        this->setStriDoneTexture();
                        DataContainer::FlavorInfor infor = DataContainer::getInstance()->getTheFlavorByName(flavorinBowl->getName());
                        if (infor.pcount == 1) {
                            SoundPlayer::getInstance()->playHurahEffect();
                            ParticleSystemQuad* _particle = ParticleSystemQuad::create("make/pour_cereal/flowerparticle.plist");
                            Texture2D* ptexture = Director::getInstance()->getTextureCache()->addImage("make/pour_cereal/pour_"+flavorinBowl->getName()+".png");
                            _particle->setTexture(ptexture);
                            _particle->setPosition(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y - 150);
                            addChild(_particle, 10);
                                    _particle->setAutoRemoveOnFinish(true);
                        }else {
                            SoundPlayer::getInstance()->playHurahEffect();
                            for (int i = 0; i < infor.pcount; ++i) {
                                ParticleSystemQuad* _particle = ParticleSystemQuad::create("make/pour_cereal/flowerparticle.plist");
                                Texture2D* ptexture = Director::getInstance()->getTextureCache()->addImage("make/pour_cereal/pour_"+flavorinBowl->getName()+convertIntToString(i+1)+".png");
                                _particle->setTexture(ptexture);
                                _particle->setTotalParticles(120.0/infor.pcount);
                                _particle->setPosition(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y - 150);
                                addChild(_particle, 10);
                                _particle->setAutoRemoveOnFinish(true);
                            }
                        }
                        
                    }
                }
            }
        };
        listener->onTouchEnded = [=](Touch *touch, Event *event){
            this->unschedule(schedule_selector(MixItemScene::stritheCereal));
            SoundPlayer::getInstance()->stopAlleffect();
            if (striFinished == true) {
                _eventDispatcher->removeEventListenersForTarget(spoon);
                spoon->runAction(Sequence::create(EaseSineInOut::create(MoveBy::create(0.8, Vec2(1000, 0))), RemoveSelf::create(), NULL));
                this->striDone();
            }
        };
        listener->onTouchCancelled = [=](Touch *touch, Event *event){
            this->unschedule(schedule_selector(MixItemScene::stritheCereal));
            SoundPlayer::getInstance()->stopAlleffect();
            if (striFinished == true) {
                _eventDispatcher->removeEventListenersForTarget(spoon);
                spoon->runAction(Sequence::create(EaseSineInOut::create(MoveBy::create(0.8, Vec2(1000, 0))), RemoveSelf::create(), NULL));
                this->striDone();
               
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spoon);

    }
    
}

void MixItemScene::striDone(){
    
    

    showNextButton(2.0f);
    
    DataContainer::FlavorInfor _info = DataContainer::getInstance()->getTheFlavorByName(flavorinBowl->getName());
    
    int max = DataContainer::getInstance()->getAnimalAnimationCount(_info.type);
    Animation* panimation = createAnimation("animals/animation/"+_info.type+"/"+_info.type, 0, max);
    panimation->setLoops(-1);
    panimation->setDelayPerUnit(0.1f);
    Sprite* animal = Sprite::create("animals/animation/"+_info.type+"/"+_info.type+convertIntToString(0)+".png");
    animal->setScale(0.6f);
    animal->setAnchorPoint(Vec2(0.5, 0));
    animal->setPosition(STVisibleRect::getPointOfSceneRightBottom() + Vec2(300, 70));
    addChild(animal, 12);
    
    animal->runAction(Sequence::create(DelayTime::create(0.2f),EaseSineInOut::create(JumpBy::create(2.5, Vec2(-500, 0), 500, 3)), Animate::create(panimation),NULL));
    this->runAction(Sequence::create(DelayTime::create(1.0),CallFunc::create([=]{
        SoundPlayer::getInstance()->playAnimationEffect();
    }), NULL));
    
}

void MixItemScene::addExploreImage(float) {
    if (explorTexture == nullptr) {
        Image* pImage = new Image();
        pImage->initWithImageFile("make/splatter.png");
        string milk = DataContainer::getInstance()->getChooseMilk();
        string flavor = DataContainer::getInstance()->getChooseFlavor();
        
        DataContainer::MilkInfor _milkinfor = DataContainer::getInstance()->getTheMilkByName(milk);
        DataContainer::FlavorInfor _flaovrinfo = DataContainer::getInstance()->getTheFlavorByName(flavor);
        
        explorTexture = new Texture2D();
        
        if (_milkinfor.isHSVuse == false) {
            explorTexture->initWithImage(CCImageColorSpace::imageWithHSB(pImage, _flaovrinfo.hsv.x, _flaovrinfo.hsv.y, _flaovrinfo.hsv.z));
        }else {
            explorTexture->initWithImage(CCImageColorSpace::imageWithHSB(pImage, _milkinfor.hsv.x, _milkinfor.hsv.y, _milkinfor.hsv.z));
        }
        pImage->autorelease();
    }
    SoundPlayer::getInstance()->playSplashEffect();
    Sprite* explor = Sprite::createWithTexture(explorTexture);
    float x = arc4random() % ((int)STVisibleRect::getGlvisibleSize().width - 300) + 150;
    float y = arc4random() % ((int)STVisibleRect::getGlvisibleSize().height - 300) + 150;
    explor->setPosition(x, y);
    explor->setScale(0);
    this->addChild(explor, 12);
    explor->runAction(Sequence::create(ScaleTo::create(0.1, 1.1), ScaleTo::create(0.05, 1.0), DelayTime::create(1.5f), EaseSineInOut::create(FadeOut::create(1.0f)), RemoveSelf::create(),NULL));
}

void MixItemScene::nextClickEvent(){
    GameLayerBase::nextClickEvent();
    replaceTheScene<DecorateScene>();
}

void MixItemScene::preClickEvent(){
    GameLayerBase::preClickEvent();
    replaceTheScene<ChooseMilk>();
}


