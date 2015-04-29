//
//  TransformTool.h
//  DONUT4013
//
//  Created by liuwei on 4/29/15.
//
//

#ifndef __DONUT4013__TransformTool__
#define __DONUT4013__TransformTool__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUi.h"
using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class TransformTool : public Sprite{
public:
    static TransformTool* createWithFile(const string& file);
    
    TransformTool();
    ~TransformTool();
    virtual bool initWithFile(const string& filename);
    
    CC_SYNTHESIZE(Rect, movableRect, MovableRect);
protected:
    void onCloseBtnClicked(Ref* pRef, Control::EventType type);
    void onOparateBtnTouched(Ref* pRef, Widget::TouchEventType type);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
    void showTools();
    void hiddenTools(float dt = 0.2f);
    void moveToolsWithDelta(Vec2 delta);
private:
    bool needShowTool = false;
    ControlButton* closeBtn = nullptr;//关闭按钮
    Button* oparateBtn = nullptr;//操作按钮
    Scale9Sprite* frameSprite = nullptr;//缩放边框图片
    const float frameWidth = 348.0;
    const float frameHeight = 528.0;
};

#endif /* defined(__DONUT4013__TransformTool__) */
