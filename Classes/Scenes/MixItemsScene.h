//
//  MixItemsScene.h
//  DONUT4013
//
//  Created by liuwei on 4/23/15.
//
//

#ifndef __DONUT4013__MixItemsScene__
#define __DONUT4013__MixItemsScene__

#include <iostream>
#include "GameLayerBase.h"

class MixItemScene : public GameLayerBase{
    enum MixItemTags{
        kFlavorTag = 111,
        kMilkTag, 
    };
public:
    MixItemScene(){};
    ~MixItemScene(){CC_SAFE_RELEASE(flavorTexture); CC_SAFE_RELEASE(maskRender);};
    static Scene* scene();
    
    virtual bool init();
    
    CREATE_FUNC(MixItemScene);
    
protected:
    virtual void onEnterTransitionDidFinish();
    void onselectTheItem(Ref* pRef, Control::EventType type);
    void performTip(float);
    void pourFlavor();
    void pourMilk();
    void showflovar(float);
    void showmilk(float);
    void cutMilk();
    void ontouchLine(Ref* pRef,Widget::TouchEventType rtype);
private:
    float upH = -335 + 40;
    ControlButton* flavorBox = nullptr;
    Sprite* milkBox = nullptr;
    Sprite* flavorinBowl = nullptr;
    Sprite* milkinBowl = nullptr;
    Sprite* tiltSprite = nullptr;
    Texture2D* flavorTexture = nullptr;
    RenderTexture* maskRender = nullptr;
    Button* lineTip = nullptr;
    ParticleSystemQuad* _pourMilk = nullptr;
    float tableMaxy = 0;
    float maxmove = 0;
    float moveDistance = 0;
    int cutCount = 3;
    Node* currentItem = nullptr;
    Sprite* spoon = nullptr;
    
};

#endif /* defined(__DONUT4013__MixItemsScene__) */
