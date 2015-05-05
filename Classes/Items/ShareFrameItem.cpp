//
//  ShareFrameItem.cpp
//  DONUT4013
//
//  Created by liuwei on 5/5/15.
//
//

#include "ShareFrameItem.h"

ShareFrameItem* ShareFrameItem::create(cocos2d::Image *pImage) {
    ShareFrameItem* pRet = new ShareFrameItem();
    
    if (pRet && pRet->init(pImage)) {
        pRet->autorelease();
        return pRet;
    }else {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

bool ShareFrameItem::init(cocos2d::Image *pImage) {
    if (SwallowTouchLayout::init()) {
        setBackGroundImage("ui/mask.png");
        setBackGroundImageOpacity(150);
        
        
        return true;
    }
    return false;
}



