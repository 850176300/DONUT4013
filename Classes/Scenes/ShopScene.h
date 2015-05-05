//
//  ShopScene.h
//  DONUT4013
//
//  Created by liuwei on 15/5/5.
//
//

#ifndef __DONUT4013__ShopScene__
#define __DONUT4013__ShopScene__

#include <iostream>
#include "GameLayerBase.h"
#include "ui/CocosGUI.h"
using namespace std;
using namespace cocos2d::ui;
class ShopScene : public GameLayerBase{
public:
    static Scene* scene();
    virtual bool init();
    CREATE_FUNC(ShopScene);
private:
    ui::ScrollView* shopScroll = nullptr;
    
};


#endif /* defined(__DONUT4013__ShopScene__) */
