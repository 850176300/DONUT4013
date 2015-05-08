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
#include "ScrollPage.h"
#include "STInAppBilling.h"

using namespace std;
class ShopScene : public GameLayerBase, public STIABDelegate{
public:
    static Scene* scene();
    virtual bool init();
    CREATE_FUNC(ShopScene);
    
protected:
    void showRestoreBtn(float dt);
    void onRestoreButtonClicked(cocos2d::Ref *pRef, Control::EventType type);
    void showShopScrollView();
    void onScrollItemClick(Ref* pRef, Widget::TouchEventType type);
    virtual void onEnterTransitionDidFinish();
    virtual void preClickEvent();
    
    virtual void purchaseSuccessful(const char* pid);
    virtual void purchaseFailed(const char *pid, int errorCode);
    virtual void restoreSuccessful(const char* pid);
    virtual void restoreSuccessfulNotify();
    virtual void restoreFailed(const char* pid, int errorCode);
    virtual void showDialogView(const char* content, const char* title);
private:
    Button* buyButton = nullptr;
    ScrollPage* shopScroll = nullptr;
    int count = 0;//统计flavor个数
    int ItemSpace = 408;
    float ItemScale = 0.8;
    int currentIndex = 0;
    const int IAPCOUTNS = 6;
    const string IAPKEYS[6] = {"com.donut_digital.cereal.bestdeal",
        "com.donut_digital.cereal.unlockall",
        "com.donut_digital.cereal.cereals",
        "com.donut_digital.cereal.milk",
        "com.donut_digital.cereal.decorations",
        "com.donut_digital.cereal.noads"
    };
    STInAppBilling* inAppIap = nullptr;
    ControlButton* restoreBtn = nullptr;

};


#endif /* defined(__DONUT4013__ShopScene__) */
