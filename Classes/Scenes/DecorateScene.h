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
#include "ScrollPage.h"
#include "FillMaterialModel.h"
using namespace std;


class DecorateScene : public GameLayerBase{
public:
    DecorateScene(){
        if (GameLayerBase::getBannerSize() == 0) {
            NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DecorateScene::onBannerDidLoaded), kDidLoadBanner, nullptr);
        }
    }
    ~DecorateScene(){
        NotificationCenter::getInstance()->removeObserver(this, kDidLoadBanner); CC_SAFE_RELEASE(mascot);};
    static Scene* scene();
    
    virtual bool init();
    
    CREATE_FUNC(DecorateScene);
protected:
    virtual void purchaseSucceed();
    virtual void resetClickEvent();
    virtual void nextClickEvent();
    virtual void preClickEvent();
    virtual void onEnterTransitionDidFinish();
    void addScrollView();
    void ontypeItemClicked(Ref* pRef,Widget::TouchEventType toucht);
    void onItemsThingClicked(Ref* pRef, Widget::TouchEventType ttouch);
    void onDownButtonClicked(Ref* pRef, Widget::TouchEventType touch);
    void refreshData();
    void addFixedThings();
    void saveTheScreenShot();
    void onBannerDidLoaded(Ref* pref);
    
private:
    bool decorateItemCantBeClick = false;
    bool isFirstOnenter = true;
    Sprite* flavorinBowl = nullptr;
    ScrollPage* typeScrollView = nullptr;
    ScrollPage* itemScrollView = nullptr;
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
    Vec2 scrollPosition1;
    Vec2 scrollPosition2;
};

#endif /* defined(__DONUT4013__DecorateScene__) */
