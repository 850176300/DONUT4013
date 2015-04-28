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

using namespace std;
USING_NS_CC_EXT;
class DecorateScene : public GameLayerBase{
public:
    static Scene* scene();
    
    virtual bool init();
    
    CREATE_FUNC(DecorateScene);
protected:
    virtual void onEnterTransitionDidFinish();
    void addScrollView();
    
private:
    cocos2d::extension::ScrollView* typeScrollView = nullptr;
};

#endif /* defined(__DONUT4013__DecorateScene__) */
