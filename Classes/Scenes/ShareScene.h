//
//  ShareScene.h
//  DONUT4013
//
//  Created by liuwei on 5/5/15.
//
//

#ifndef __DONUT4013__ShareScene__
#define __DONUT4013__ShareScene__

#include <iostream>
#include "GameLayerBase.h"
#include "DaubSprite.h"
#include "MoregameBtn.h"

class ShareScene: public GameLayerBase{
public:
    ~ShareScene(){CC_SAFE_DELETE(_scribble);};
    static Scene* scene();
    virtual bool init();
    CREATE_FUNC(ShareScene);
    
protected:
    virtual void favoriteClickEvent();
    virtual void preClickEvent();
    virtual void homeClickEvent();
    virtual void shareClickEvent();
    virtual bool onTouchBegan(Touch *touch, Event *unused_event){return true;}
    virtual void onTouchMoved(Touch *touch, Event *unused_event){};
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    virtual void onEnterTransitionDidFinish();
    void onEatAgainClicked(Ref* pRef, Control::EventType type);
    void favoriteshotScreen();
    void shareShotScreen();
private:
    long totalColorCount = 0;
    Scribble* _scribble = nullptr;
    DaubSprite* flavorinBowl = nullptr;
    DaubSprite* eatLayer = nullptr;
    DaubSprite* breakFast = nullptr;
    Sprite* taptoEat = nullptr;
    ControlButton* eatAgain = nullptr;
    MoregameBtn* pBtn = nullptr;
};
#endif /* defined(__DONUT4013__ShareScene__) */
