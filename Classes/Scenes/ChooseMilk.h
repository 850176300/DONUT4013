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
    void addScrollView();
    void onScrollItemClick(Ref* pref, Widget::TouchEventType type);
    virtual void onEnterTransitionDidFinish();
private:
    ScrollPage* milkScrollview = nullptr;
    int count = 0;
    int currentIndex = 0;
    int ItemSpace = 285.0;
    float ItemScale = 0.7;
};

#endif /* defined(__DONUT4013__ChooseMilk__) */
