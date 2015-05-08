//
//  LoadingScene.h
//  DONUT4013
//
//  Created by liuwei on 15/5/7.
//
//

#ifndef __DONUT4013__LoadingScene__
#define __DONUT4013__LoadingScene__

#include <iostream>
#include "GameLayerBase.h"

class LoadingScene : public GameLayerBase{
public:
    static Scene* scene();
    virtual bool init();
    CREATE_FUNC(LoadingScene);
protected:
    virtual void onEnterTransitionDidFinish();
    void switchScene(float);
};

#endif /* defined(__DONUT4013__LoadingScene__) */
