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
    
    ChooseFlavor(){ hsvBackground = new Image(); };
    ~ChooseFlavor(){ CC_SAFE_DELETE(hsvBackground); };
    
    virtual bool init();
    
    CREATE_FUNC(ChooseFlavor);
protected:
    virtual void onEnterTransitionDidFinish();
    void addScrollView();
    void onScrollingItems(Vec2 offset);
    void onScrollItemClick(Ref* pRef,Widget::TouchEventType type);
    void changeBackGround(float);
    void animalAction(Node* pNode, Layout* pLayout);
private:
    ScrollPage* flavorScrollview = nullptr;
    int count = 0;//统计flavor个数
    int ItemSpace = 384.0;
    float ItemScale = 0.7;
    Image* hsvBackground = nullptr;
    Sprite* bg2 = nullptr;
    int currentIndex = 0;
    DataContainer::flavorMap flavorData;
};

#endif /* defined(__DONUT4013__ChooseFlavor__) */
