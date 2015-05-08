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
    enum ControlBtnTag{
        kStartBtnTag = 11,
        kFavBtnTag,
        kShopBtnTag,
    };
    
public:
    static Scene* scene();
    
    HomeScene(){};
    
    virtual bool init();
    
    CREATE_FUNC(HomeScene);
    
    
protected:
    void addBowlThings();//添加首页的三个元素
    void createCereal(vector<string> types);
    void addAllButtons();
    void onBtnClicked(Ref* pRef, Control::EventType type);
    void changeScene(float);
    void actionjump(float);
    virtual void onEnterTransitionDidFinish();
    void cachetexutre(float);
    void performSunShine(float);
    
private:
    int currentIndex = 0;
    Vec2 tableOriginal;
    Sprite* bowl = nullptr;
    Sprite* cereal = nullptr;
    Sprite* sunShader = nullptr;
    vector<string> types;
    LayerColor* theSun = nullptr;
    Sprite* table = nullptr;

};

#endif /* defined(__DONUT4013__HomeScene__) */
