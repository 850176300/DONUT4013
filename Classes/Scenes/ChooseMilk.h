//
//  ChooseMilk.h
//  DONUT4013
//
//  Created by liuwei on 4/21/15.
//
//

#ifndef __DONUT4013__ChooseMilk__
#define __DONUT4013__ChooseMilk__

#include <iostream>
#include "cocos2d.h"
#include "GameLayerBase.h"
#include "ScrollPage.h"

class ChooseMilk : public GameLayerBase{
public:
    static Scene* scene();
    
    virtual bool init();
    
    CREATE_FUNC(ChooseMilk);
protected:
    virtual void nextClickEvent();
    virtual void preClickEvent();
    void addScrollView();
    void onScrollItemClick(Ref* pref, Widget::TouchEventType type);
    virtual void onEnterTransitionDidFinish();
    void shakeTheTitle(float);
    virtual void purchaseSucceed();
private:
    ScrollPage* milkScrollview = nullptr;
    Sprite* title = nullptr;
    int count = 0;
    int currentIndex = 0;
    int ItemSpace = 359;
    float ItemScale = 0.7;
};

#endif /* defined(__DONUT4013__ChooseMilk__) */
