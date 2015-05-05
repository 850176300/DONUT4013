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

class ShareFrameItem : public SwallowTouchLayout{
public:
    static ShareFrameItem* create(Image* pImage);
    virtual bool init(Image* pImage);
private:
    Sprite* frame = nullptr;
};


#endif /* defined(__DONUT4013__ShareFrameItem__) */
