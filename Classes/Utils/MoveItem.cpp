//
//  MoveItem.cpp
//  HuaRongStreet
//
//  Created by liuwei on 14-11-12.
//
//

#include "MoveItem.h"
MoveItem::~MoveItem(){
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_listener);
}

MoveItem* MoveItem::create(string file, string userTag){
    MoveItem* pRet = new MoveItem();
    if (pRet && pRet->initWithStringValue(file, userTag)) {
        pRet->autorelease();
        return pRet;
    }else {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

bool MoveItem::initWithStringValue(string file, string usertag) {
    if (Sprite::initWithFile(file.c_str())) {
        setName(usertag);
        _touchenable = true;
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan = CC_CALLBACK_2(MoveItem::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(MoveItem::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(MoveItem::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(MoveItem::onTouchCancelled, this);
        dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        touchListener->setSwallowTouches(true);
        startLocation = Vec2(0, 0);
        _listener = touchListener;
        return true;
    }
    return false;
}

void MoveItem::setStartLocation(cocos2d::Vec2 location) {
    startLocation = location;
}

Vec2 MoveItem::getStartLocation(){
    return startLocation;
}

void MoveItem::setTouchEnable(bool enable) {
    _touchenable = enable;
}

void MoveItem::setTouchDelegate(MoveItemDelegate *delegate) {
    _pDelegate = delegate;
}


bool MoveItem::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_touchenable == false) {
        return false;
    }
    Vec2 location = getParent()->convertTouchToNodeSpace(touch);
    if (!boundingBox().containsPoint(location)) {
        return false;
    }
    
    if (_beginDelegate) {
        bool res = _beginDelegate->itemTouchBegan(this);
        if (res == true) {
            if (_pDelegate ){
                _pDelegate->touchItembegan(this);
            }
        }
        return res;
    }else {
        if (_pDelegate ){
            _pDelegate->touchItembegan(this);
        }
        return true;
    }
}

void MoveItem::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {
//    log("item touch moved!!!");
    if (_touchenable == true) {
        if (_pDelegate ){
            _pDelegate->touchItemMove(this, touch);
        }
    }
}

void MoveItem::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
//    if (_touchenable == true) {
        if (_pDelegate ){
            _pDelegate->touchItemEnd(this);
        }
//    }
}

void MoveItem::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
//    if (_touchenable == true) {
        if (_pDelegate ){
            _pDelegate->touchItemEnd(this);
        }
//    }
}

