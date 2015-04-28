//
//  DecorateScene.h
//  DONUT4013
//
//  Created by liuwei on 4/28/15.
//
//

#ifndef __DONUT4013__DecorateScene__
#define __DONUT4013__DecorateScene__

#include <iostream>
#include "GameLayerBase.h"
#include "ui/CocosGUI.h"
using namespace std;
using namespace cocos2d::ui;


class DecorateScene : public GameLayerBase{
public:
    static Scene* scene();
    
    virtual bool init();
    
    CREATE_FUNC(DecorateScene);
protected:
    virtual void onEnterTransitionDidFinish();
    void addScrollView();
    void ontypeItemClicked(Ref* pRef,Widget::TouchEventType toucht);
private:
    ui::ScrollView* typeScrollView = nullptr;
    ui::ScrollView* itemScrollView = nullptr;
};

#endif /* defined(__DONUT4013__DecorateScene__) */
