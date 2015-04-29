//
//  FillMaterialModel.h
//  COOK016_TacoMaker
//
//  Created by jianghai on 15-4-23.
//
//

#ifndef __COOK016_TacoMaker__FillMaterialModel__
#define __COOK016_TacoMaker__FillMaterialModel__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

typedef enum {
    NONE,
    MOVE,
    SCALE,
    ROTATE,
} EventType;

class FillMaterialModel : public Sprite
{
public:
    FillMaterialModel();
    ~FillMaterialModel();
    static FillMaterialModel* create(const char* ImageName);
    virtual bool init(const char* ImageName);
    inline const char* getResourceName()
    {
        return _resourceName.c_str();
    }
    inline void setEnable(bool isEnable)
    {
        _enable = isEnable;
    }
    inline void hiddenTipsFrame()
    {
        _close->setVisible(false);
        _magnify->setVisible(false);
        this->setSpriteFrame(Sprite::create("controlTip_bg_Clear.png")->getSpriteFrame());
        _isTipsFrameShow = false;
    }
private:
    void deleteSelf(Ref *sender);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    virtual void onEnter();
    virtual void onExit();
    
    virtual void updateTipsFrameStatus(float);
private:
    ControlButton *_close;
    Sprite *_magnify;
    Sprite *_item;
    EventListenerTouchOneByOne *_listener;
    EventType _eventType;
    float _orignalRotate;
    bool _isTipsFrameShow;
    std::string _resourceName;
    bool _enable;
};

#endif /* defined(__COOK016_TacoMaker__FillMaterialModel__) */
