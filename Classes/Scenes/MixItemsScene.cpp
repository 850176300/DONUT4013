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
        
        Image* bg2Image = new Image();
        bg2Image->initWithImageFile("make/bg_make_table.png");
        
        Texture2D* ptexture = new Texture2D();
        ptexture->initWithImage(CCImageColorSpace::imageWithHSB(bg2Image, info.hsv.x, info.hsv.y, info.hsv.z));
        Sprite* bg2 = Sprite::createWithTexture(ptexture);
        bg2->setPosition(STVisibleRect::getCenterOfScene() + Vec2(0, 120));
        addChild(bg2, -1);
        
        bg2Image->autorelease();
        ptexture->autorelease();
        
        Sprite* table = Sprite::create("make/bg_make_table_table.png");
        table->setAnchorPoint(Vec2(0.5, 0));
        table->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y));
        addChild(table, 1);
        
        tableMaxy = table->getBoundingBox().getMaxY();
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
    milkBox = Sprite::create("make/milk/"+milk+".png");
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
    bowl1->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(0.8, Vec2(800, 0)), 0.3), NULL));
    bowl2->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(0.8, Vec2(800, 0)), 0.3), CallFunc::create([=]{
        flavorBox->runAction(Sequence::create(DelayTime::create(0.3),EaseElasticInOut::create(MoveTo::create(0.5, Vec2(STVisibleRect::getCenterOfScene().x - 175, tableMaxy-80)), 0.3), NULL));
        milkBox->runAction(Sequence::create(DelayTime::create(1.0), EaseElasticInOut::create(MoveTo::create(0.9, Vec2(STVisibleRect::getCenterOfScene().x+225, tableMaxy - 80)), 0.4), NULL));
        tiltSprite->runAction(Sequence::create(DelayTime::create(2.2), EaseElasticInOut::create(MoveBy::create(1.2, Vec2(0, -900)), 0.7), NULL));
        this->schedule(schedule_selector(MixItemScene::performTip), 5.0, -1, 4.0);
        flavorBox->setEnabled(true);
        
        Director::getInstance()->getTextureCache()->addImage("make/cereals_stir/"+flavor+".png");
        flavorTexture = Director::getInstance()->getTextureCache()->getTextureForKey("make/cereals_stir/"+flavor+".png");
        flavorTexture->retain();
       
        flavorinBowl = Sprite::createWithTexture(flavorTexture);
        flavorinBowl->setPosition(bowl1->getPosition());
        addChild(flavorinBowl, 5);
        flavorinBowl->setVisible(false);
        
        milkinBowl = Sprite::create("make/milk_stir/"+milk+".png");
        milkinBowl->setOpacity(0);
        milkinBowl->setPosition(bowl1->getPosition());
        addChild(milkinBowl, 5);
    }), NULL));
    
    currentItem = flavorBox;
    currentItem->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(RotateBy::create(0.5, 8)), EaseSineInOut::create(RotateBy::create(0.8, -16)), RotateBy::create(0.5, 8), nullptr)));
}


void MixItemScene::onselectTheItem(cocos2d::Ref *pRef, Control::EventType type) {
    if (type == Control::EventType::TOUCH_UP_INSIDE) {
        ControlButton* pNode = dynamic_cast<ControlButton*>(pRef);
        
        if (pNode->getTag() == kFlavorTag) {
            unschedule(schedule_selector(MixItemScene::performTip));
            
            pNode->stopAllActions();
            pNode->setRotation(0);
            milkBox->runAction(EaseElasticInOut::create(MoveBy::create(0.8, Vec2(1000, 0)), 0.3));
            pNode->setEnabled(false);
            pNode->runAction(Sequence::create(JumpBy::create(0.5, Vec2(STVisibleRect::getCenterOfScene().x - pNode->getPositionX(), -30), 150, 1),CallFunc::create(std::bind(&ControlButton::setLocalZOrder, pNode, 7)), DelayTime::create(0.5), CallFunc::create(std::bind(&MixItemScene::pourFlavor, this)), NULL));
        }else if (pNode->getTag() == kMilkTag) {
            
        }
    }
}

void MixItemScene::performTip(float) {
    currentItem->runAction(Sequence::create(EaseElasticInOut::create(JumpBy::create(0.9, Vec2::ZERO, 180, 1), 0.5), DelayTime::create(0.2), NULL));
}

void MixItemScene::pourFlavor(){
    maskRender = RenderTexture::create(465, 335, Texture2D::PixelFormat::RGBA8888);
    maskRender->retain();
    maskRender->setPosition(0, 0);
    const float rotate = 45;
    flavorBox->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(-460, 100)), RotateBy::create(0.5, rotate),NULL), CallFunc::create([=]{
        DataContainer::FlavorInfor infor = DataContainer::getInstance()->getTheFlavorByName(flavorBox->getName());
        if (infor.pcount == 1) {
            ParticleSystemQuad* _particle = ParticleSystemQuad::create("make/flavor/pourflavor.plist");
            Director::getInstance()->getTextureCache()->addImage("make/pour_cereal/pour_"+flavorBox->getName()+".png");
            _particle->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey("make/pour_cereal/pour_"+flavorBox->getName()+".png"));
            _particle->setPosition(flavorBox->getPosition() + Vec2(346,147.6));
            addChild(_particle, 10);
            _particle->setAutoRemoveOnFinish(true);
        }
        for (int i = 0; i < infor.pcount; ++i) {
            ParticleSystemQuad* _particle = ParticleSystemQuad::create("make/flavor/pourflavor.plist");
            Director::getInstance()->getTextureCache()->addImage("make/pour_cereal/pour_"+flavorBox->getName()+convertIntToString(i+1)+".png");
            _particle->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey("make/pour_cereal/pour_"+flavorBox->getName()+convertIntToString(i+1)+".png"));
            _particle->setTotalParticles(60/infor.pcount);
            _particle->setPosition(flavorBox->getPosition() + Vec2(346,147.6));
            addChild(_particle, 10);
            _particle->setAutoRemoveOnFinish(true);
        }
        this->schedule(schedule_selector(MixItemScene::showflovar), 0.5);
    }),nullptr));


}

void MixItemScene::pourMilk(){
    
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
        unschedule(schedule_selector(MixItemScene::showflovar));
        flavorBox->runAction(Sequence::create(RotateBy::create(0.3, -45), MoveBy::create(0.5, Vec2(-500, 0)), CallFunc::create([=]{
            milkBox->setLocalZOrder(10);
            milkBox->runAction(Sequence::create(EaseElasticIn::create(MoveTo::create(0.8, Vec2(STVisibleRect::getCenterOfScene().x, milkBox->getPositionY())), 0.5), CallFunc::create(std::bind(&MixItemScene::cutMilk, this)), NULL));
        }),RemoveSelf::create(),NULL));
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
    
    lineTip->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 0.8), ScaleTo::create(0.3, 1.0), NULL)));
    

}

void MixItemScene::ontouchLine(cocos2d::Ref *pRef, Widget::TouchEventType rtype) {
    Vec2 begin;
    Vec2 move;
    Button* tip = dynamic_cast<Button*>(pRef);
    if (rtype == Widget::TouchEventType::BEGAN) {
        begin = tip->getTouchBeganPosition();
        moveDistance = 0;
        lineTip->stopAllActions();
        lineTip->setScale(1.0);
    }else if (rtype == Widget::TouchEventType::MOVED) {
        move = tip->getTouchMovePosition();
        float distance = move.distance(begin);
        if (distance > moveDistance) {
            moveDistance = distance;
        }
    }else if (rtype == Widget::TouchEventType::ENDED){
        milkBox->runAction(GameLayerBase::getJellyAction());
        if (moveDistance > 100) {
            cutCount -= 1;
            if (cutCount > 0) {
                lineTip->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 0.8), ScaleTo::create(0.3, 1.0), NULL)));
            }else {
                lineTip->runAction(EaseElasticInOut::create(FadeOut::create(0.5), 0.2));
                milkBox->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=]{
                    milkBox->setTexture("make/milk/"+milkBox->getName()+"_1.png");
                }), DelayTime::create(0.5f), Spawn::create(MoveBy::create(0.3, Vec2(280, 100)), RotateBy::create(0.3, -45), NULL),DelayTime::create(0.2), CallFunc::create([=]{
                    _pourMilk = ParticleSystemQuad::create("make/milk/pour.plist");
                    _pourMilk->setPosition(milkBox->getPosition()+Vec2(-151.5, 24.5));
                    Director::getInstance()->getTextureCache()->addImage("make/pour milk/pour milk_"+milkBox->getName()+".png");
                    _pourMilk->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey("make/pour milk/pour milk_"+milkBox->getName()+".png"));
                    this->addChild(_pourMilk, 10);
                    this->schedule(schedule_selector(MixItemScene::showmilk), 0.5);
                }), NULL));
            }
        }else{
           lineTip->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 0.8), ScaleTo::create(0.3, 1.0), NULL)));
        }
        
        
    }else if (rtype == Widget::TouchEventType::CANCELED){
        milkBox->runAction(GameLayerBase::getJellyAction());
        if (moveDistance > 100) {
            cutCount -= 1;
            if (cutCount > 0) {
                lineTip->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 0.8), ScaleTo::create(0.3, 1.0), NULL)));
            }else {
                lineTip->runAction(Sequence::create(EaseSineInOut::create(FadeOut::create(0.5)), RemoveSelf::create(), NULL));
                milkBox->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=]{
                    milkBox->setTexture("make/milk/"+milkBox->getName()+"_1.png");
                }), DelayTime::create(0.5f), Spawn::create(MoveBy::create(0.3, Vec2(280, 100)), RotateBy::create(0.3, -45), NULL),DelayTime::create(0.2), CallFunc::create([=]{
                    _pourMilk = ParticleSystemQuad::create("make/milk/pour.plist");
                    _pourMilk->setPosition(milkBox->getPosition()+Vec2(-151.5, 24.5));
                    Director::getInstance()->getTextureCache()->addImage("make/pour milk/pour milk_"+milkBox->getName()+".png");
                    _pourMilk->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey("make/pour milk/pour milk_"+milkBox->getName()+".png"));
                    this->addChild(_pourMilk, 10);
                    this->schedule(schedule_selector(MixItemScene::showmilk), 0.5);
                }), NULL));
            }
        }else{
            lineTip->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 0.8), ScaleTo::create(0.3, 1.0), NULL)));
        }
    }
    
}

void MixItemScene::showmilk(float) {
    int opacity = milkinBowl->getOpacity();
    opacity += 10;
    if (opacity > 255) {
        opacity = 255;
    }
    milkinBowl->setOpacity(opacity);
    if (opacity >= 255) {
        milkBox->runAction(Sequence::create(RotateBy::create(0.2, 45), EaseElasticOut::create(MoveBy::create(0.3, Vec2(1000, 0)), 0.2), RemoveSelf::create(),NULL));
        unschedule(schedule_selector(MixItemScene::showmilk));
        _pourMilk->setDuration(0.1);
        _pourMilk->setAutoRemoveOnFinish(true);
        spoon = Sprite::create("make/spoon.png");
        spoon->setAnchorPoint(Vec2(0.5, 0));
        spoon->setRotation(-45);
        this->addChild(spoon, 11);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        
        listener->onTouchBegan = [=](Touch *touch, Event *event){
            return CocosHelper::isPointInNode(spoon, touch->getLocation());
        };
        listener->onTouchMoved = [=](Touch *touch, Event *event){
            
        };
        listener->onTouchEnded = [=](Touch *touch, Event *event){
            
        };
        listener->onTouchCancelled = [=](Touch *touch, Event *event){
            
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spoon);

    }
    
}


