//
//  TransformTool.cpp
//  DONUT4013
//
//  Created by liuwei on 4/29/15.
//
//

#include "TransformTool.h"
#include "CocosHelper.h"
#include "STVisibleRect.h"
USING_NS_ST;


TransformTool::TransformTool(){
    movableRect = STVisibleRect::getMovableRect();
}

TransformTool::~TransformTool(){
    
}

TransformTool* TransformTool::createWithFile(const string &file){
    TransformTool* pRef = new TransformTool();
    if (pRef && pRef->initWithFile(file)) {
        pRef->autorelease();
        return pRef;
    }else {
        CC_SAFE_DELETE(pRef);
        return nullptr;
    }
}


bool TransformTool::initWithFile(const string &filename) {
    if (Layer::init()) {
        
        decorateItem = Sprite::create(filename);
        setContentSize(decorateItem->getContentSize());
        decorateItem->setPosition(getContentSize().width/2.0, getContentSize().height/2.0);
        addChild(decorateItem, 0);
        
        frameSprite = Scale9Sprite::create("ui/decorate/image_wireframe.png");
        frameSprite->setScale(getContentSize().width/frameWidth, getContentSize().height/frameHeight);
        frameSprite->setPosition(getContentSize().width/2.0, getContentSize().height/2.0);
        addChild(frameSprite, 1);
        
        closeBtn = CocosHelper::getButton("ui/decorate/image_close.png", "ui/decorate/image_close.png");
        closeBtn->setPosition(frameSprite->getBoundingBox().getMinX(), frameSprite->getBoundingBox().getMaxY());
        closeBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(TransformTool::onCloseBtnClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
        addChild(closeBtn, 2);
        
        oparateBtn = Button::create("ui/decorate/image_zoom.png");
        oparateBtn->setPosition(Vec2(frameSprite->getBoundingBox().getMaxX(), frameSprite->getBoundingBox().getMinY()));
        oparateBtn->addTouchEventListener(CC_CALLBACK_2(TransformTool::onOparateBtnTouched, this));
        addChild(oparateBtn, 2);
        
        frameSprite->setOpacity(0);
        closeBtn->setOpacity(0);
        oparateBtn->setOpacity(0);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        
        listener->onTouchBegan = CC_CALLBACK_2(TransformTool::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(TransformTool::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(TransformTool::onTouchEnded, this);
        listener->onTouchCancelled = CC_CALLBACK_2(TransformTool::onTouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, decorateItem);
        ignoreAnchorPointForPosition(false);
        setAnchorPoint(Vec2(0.5, 0.5f));
        
        distance = oparateBtn->getPosition().distance(frameSprite->getPosition());
        return true;
    }
    return false;
}

void TransformTool::onCloseBtnClicked(cocos2d::Ref *pRef, Control::EventType type) {
    stopAllActions();
    removeFromParent();
}

void TransformTool::onOparateBtnTouched(cocos2d::Ref *pRef, Widget::TouchEventType type) {
    Vec2 beginPos;
    Widget* pWidget = dynamic_cast<Widget*>(pRef);
    
    switch (type) {
        case Widget::TouchEventType::BEGAN:
        {
            beginPos = pWidget->getTouchBeganPosition() - frameSprite->getPosition();
        }
            break;
        case Widget::TouchEventType::MOVED:
        {
            Vec2 movePos = pWidget->getTouchMovePosition() - frameSprite->getPosition();
            float angleDelta = beginPos.getAngle(movePos);
            beginPos = movePos;
            log("the angle Delta is %.2f", angleDelta);
        }
            break;
        case Widget::TouchEventType::ENDED:
        case Widget::TouchEventType::CANCELED:
        {
            
        }
            break;
        default:
            break;
    }
}

bool TransformTool::onTouchBegan(Touch *touch, Event *unused_event){
    Vec2 touchP = convertTouchToNodeSpace(touch);
    bool result = CocosHelper::isPointInNode(this, touch->getLocation());
    if (result == true) {
        needShowTool = !needShowTool;
    }
    return result;
}

void TransformTool::onTouchMoved(Touch *touch, Event *unused_event){
    needShowTool = true;
    showTools();
    Vec2 afterPP = getPosition() + touch->getDelta();
    if (movableRect.containsPoint(afterPP)) {
        setPosition(afterPP);
        moveToolsWithDelta(touch->getDelta());
    }
}

void TransformTool::onTouchEnded(Touch *touch, Event *unused_event){
    if (needShowTool == false) {
        hiddenTools(0.1);
    }else {
        showTools();
    }
}

void TransformTool::onTouchCancelled(Touch *touch, Event *unused_event){
    onTouchEnded(touch, unused_event);
}


void TransformTool::hiddenTools(float dt){
    closeBtn->runAction(FadeOut::create(dt));
    oparateBtn->runAction(FadeOut::create(dt));
    frameSprite->runAction(FadeOut::create(dt));
}

void TransformTool::showTools(){
    if (closeBtn->getOpacity() > 100) {
        return;
    }
    closeBtn->runAction(FadeIn::create(0.0f));
    oparateBtn->runAction(FadeIn::create(0.0f));
    frameSprite->runAction(FadeIn::create(0.0f));
}

void TransformTool::moveToolsWithDelta(cocos2d::Vec2 delta) {
//    closeBtn->setPosition(closeBtn->getPosition() + delta);
//    oparateBtn->setPosition(oparateBtn->getPosition() + delta);
//    frameSprite->setPosition(frameSprite->getPosition() + delta);
}