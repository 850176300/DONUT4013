//
//  MoveItem.h
//  HuaRongStreet
//
//  Created by liuwei on 14-11-12.
//
//

#ifndef __HuaRongStreet__MoveItem__
#define __HuaRongStreet__MoveItem__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class MoveItem;
class ItemBeginTouchDelegate{
public:
    virtual bool itemTouchBegan(MoveItem* pItem) = 0;
};
class MoveItemDelegate{
public:
   
    virtual void touchItembegan(MoveItem* pItem){};
    virtual void touchItemMove(MoveItem* pItem, Touch* pTouch){};
    virtual void touchItemEnd(MoveItem* pItem){};
};


class MoveItem : public Sprite{
public:
    ~MoveItem();
    static MoveItem* create(string file, string usertag);
    bool initWithStringValue(string file, string usertag);
    virtual bool onTouchBegan(Touch *touch, Event *event) ;
    virtual void onTouchMoved(Touch *touch, Event *event) ;
    virtual void onTouchEnded(Touch *touch, Event *event) ;
    virtual void onTouchCancelled(Touch *touch, Event *event) ;
    void setTouchEnable(bool enable);
    void setTouchDelegate(MoveItemDelegate* delegate);
    void setTouchBeginDelegate(ItemBeginTouchDelegate* delegate){_beginDelegate = delegate;};
    void setStartLocation(Vec2 location);
    Vec2 getStartLocation();
private:
    EventListener* _listener;
    Vec2 startLocation;
    bool _touchenable;
    MoveItemDelegate* _pDelegate;
    ItemBeginTouchDelegate* _beginDelegate;
};

#endif /* defined(__HuaRongStreet__MoveItem__) */
