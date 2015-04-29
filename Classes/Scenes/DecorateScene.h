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
#include "DataContainer.h"
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
    void onItemsThingClicked(Ref* pRef, Widget::TouchEventType ttouch);
    void refreshData();
    void addFixedThings();
private:
    ui::ScrollView* typeScrollView = nullptr;
    ui::ScrollView* itemScrollView = nullptr;
    Sprite* cerealBox = nullptr;
    Sprite* plate = nullptr;
    Sprite* breakFast = nullptr;
    Sprite* spoon = nullptr;
    Sprite* frameBanner = nullptr;
    Sprite* mascot = nullptr;
    string currentSelType="";
    DataContainer::DecorateItems allItems;
    float tableMaxy = 0;
};

#endif /* defined(__DONUT4013__DecorateScene__) */
