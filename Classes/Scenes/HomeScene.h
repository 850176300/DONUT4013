//
//  HomeScene.h
//  DONUT4013
//
//  Created by liuwei on 4/16/15.
//
//

#ifndef __DONUT4013__HomeScene__
#define __DONUT4013__HomeScene__

#include <iostream>
#include "cocos2d.h"
#include "GameLayerBase.h"
using namespace std;
USING_NS_CC;

class HomeScene: public GameLayerBase{
    
    
public:
    static Scene* scene();
    
    HomeScene(){};
    
    virtual bool init();
    
    CREATE_FUNC(HomeScene);
    
    
protected:
    void addBowlThings();//添加首页的三个元素
    void createCereal();
    void addAllButtons();
private:
    int currentIndex = 0;
    Vec2 tableOriginal;
    
    Sprite* cereal = nullptr;
};

#endif /* defined(__DONUT4013__HomeScene__) */
