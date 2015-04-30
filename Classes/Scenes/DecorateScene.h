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
#include "FillMaterialModel.h"
using namespace std;
using namespace cocos2d::ui;


class DecorateScene : public GameLayerBase{
public:
    ~DecorateScene(){CC_SAFE_RELEASE(mascot);};
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
    Sprite* flavorinBowl = nullptr;
    ui::ScrollView* typeScrollView = nullptr;
    ui::ScrollView* itemScrollView = nullptr;
    Sprite* cerealBox = nullptr;
    Sprite* plate = nullptr;
    Sprite* breakFast = nullptr;
    FillMaterialModel* spoon = nullptr;
    Sprite* frameBanner = nullptr;
    FillMaterialModel* mascot = nullptr;
    string currentSelType="";
    DataContainer::DecorateItems allItems;
    float tableMaxy = 0;
    Layer* cannotEatLayer = nullptr;
    Layer* canEatLayer = nullptr;
};

#endif /* defined(__DONUT4013__DecorateScene__) */
