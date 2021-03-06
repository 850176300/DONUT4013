//
//  ChooseFlavor.h
//  DONUT4013
//
//  Created by liuwei on 4/17/15.
//
//

#ifndef __DONUT4013__ChooseFlavor__
#define __DONUT4013__ChooseFlavor__

#include <iostream>
#include "cocos2d.h"
#include "GameLayerBase.h"
#include "ScrollPage.h"
#include "DataContainer.h"
using namespace std;
USING_NS_CC;

class ChooseFlavor : public GameLayerBase{

public:
    static Scene* scene();
    
    ChooseFlavor();
    ~ChooseFlavor(){  };
    
    virtual bool init();
    
    CREATE_FUNC(ChooseFlavor);
protected:
    virtual void onEnterTransitionDidFinish();
    void addScrollView();
    void onScrollingItems(Vec2 offset);
    void onScrollItemClick(Ref* pRef,Widget::TouchEventType type);
    void changeBackGround(float);
    void animalAction(Node* pNode, Layout* pLayout);
    virtual void nextClickEvent();
    virtual void preClickEvent();
    void shakeTheTitle(float);
    void cachePictures(float);
    virtual void purchaseSucceed();
private:
    ScrollPage* flavorScrollview = nullptr;
    int count = 0;//统计flavor个数
    int ItemSpace = 386.0;
    float ItemScale = 0.7;
//    Image* hsvBackground = nullptr;
    Sprite* bg2 = nullptr;
    int currentIndex = 0;
    DataContainer::flavorMap flavorData;
    Sprite* title= nullptr;
};

#endif /* defined(__DONUT4013__ChooseFlavor__) */
