//
//  PageItem.cpp
//  DONUT4013
//
//  Created by liuwei on 4/17/15.
//
//

#include "PageItem.h"

PageItem* PageItem::create(const string &filename) {
    PageItem* pRet = new PageItem();
    if (pRet && pRet->init(filename)) {
        pRet->autorelease();
        return pRet;
    }else {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}
