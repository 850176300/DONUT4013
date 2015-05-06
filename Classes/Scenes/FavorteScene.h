//
//  FavorteScene.h
//  DONUT4013
//
//  Created by liuwei on 5/6/15.
//
//

#ifndef __DONUT4013__FavorteScene__
#define __DONUT4013__FavorteScene__

#include <iostream>
#include "GameLayerBase.h"
#include "ui/CocosGUi.h"
using namespace std;
using namespace cocos2d::ui;

class FavoriteScene : public GameLayerBase{
public:
    ~FavoriteScene(){CC_SAFE_RELEASE(favItems);};
    
    static Scene* scene();
    virtual bool init();
    CREATE_FUNC(FavoriteScene);
    
protected:
    void addScrollview();
private:
    ui::ScrollView* pScrollView = nullptr;
    __Array* favItems = nullptr;
    float innerContentHeight = 0;
};

#endif /* defined(__DONUT4013__FavorteScene__) */
