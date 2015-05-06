//
//  ShareFrameItem.h
//  DONUT4013
//
//  Created by liuwei on 5/5/15.
//
//

#ifndef __DONUT4013__ShareFrameItem__
#define __DONUT4013__ShareFrameItem__

#include <iostream>
#include "cocos2d.h"
#include "SwallowTouchLayout.h"
using namespace std;
USING_NS_CC;

class ShareFrameItem : public Layout{
    enum ButtonsTags{
        kCloseTags = 11,
        kDeleteTags,
        kEmailTags,
        kSaveTag,
    };
public:
    ~ShareFrameItem(){CC_SAFE_RELEASE(shareImage);}
    static ShareFrameItem* create(Image* pImage, bool withDelete = false);
    virtual bool init(Image* pImage, bool withDelete);
protected:
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    void onButtonsClicked(Ref* pRef, Widget::TouchEventType type);
private:
    Sprite* frame = nullptr;
    Image* shareImage = nullptr;
    bool hasDelete = false;
};


#endif /* defined(__DONUT4013__ShareFrameItem__) */
