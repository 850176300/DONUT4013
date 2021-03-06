//
// AlertViewLayer.h
// PetPlay
//
// Created by jianghai on 14-9-9.
//
//

#ifndef __PetPlay__AlertViewLayer__
#define __PetPlay__AlertViewLayer__

#include "cocos2d.h"
#include "STVisibleRect.h"
#include "extensions/cocos-ext.h"
#include "SuperGlobal.h"
#include "KeypadDispatchCenter.h"
USING_NS_CC;
USING_NS_ST;
USING_NS_CC_EXT;
//class AlertViewLayer;

//class AlertViewDelegate
//{
//public:
//    virtual void alertViewClickedButtonAtIndex(AlertViewLayer* alert,int index){};
//};

class AlertViewLayer : public LayerColor, public KeypadDelegate
{

    typedef std::function<void(AlertViewLayer*, int)> ccAlertViewCallBack;
public:
    enum AlertButtonCount{
        SINGLE = 1,
        DOUBLE,
    };
public:
    AlertViewLayer();
    ~AlertViewLayer();
    
    static AlertViewLayer* createWithTitle(const char* title, AlertButtonCount pcount = DOUBLE);
    
    virtual bool init();
    virtual bool initWithTitle(const char* title, AlertButtonCount pcount);
    void setAlertViewCallBack(ccAlertViewCallBack _call);
    CREATE_FUNC(AlertViewLayer);
    
//    inline void setAlertViewDelegate(AlertViewDelegate* delegate)
//    {
//        m_pDelegate = delegate;
//    }
    inline void setAlertViewTitle(const char* title)
    {
        _title = title;
    }
    CC_SYNTHESIZE(bool, justOneButton, isOneButton);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    void buttonClicked(Ref* psender, Control::EventType type);
    virtual void onKeyBackClicked();
private:
    ccAlertViewCallBack _callBack;
//    AlertViewDelegate* m_pDelegate;
    std::string _title;
    Sprite *_banner;
    ControlButton *_Yes;
    ControlButton *_No;
    EventListenerTouchOneByOne *__touchListener;
};

#endif /* defined(__PetPlay__AlertViewLayer__) */