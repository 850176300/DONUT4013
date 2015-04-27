//
//  SwallowTouchLayout.cpp
//  DONUT4013
//
//  Created by liuwei on 4/21/15.
//
//

#include "SwallowTouchLayout.h"

SwallowTouchLayout* SwallowTouchLayout::create(){
    SwallowTouchLayout* layout = new SwallowTouchLayout();
    if (layout && layout->init())
    {
        layout->autorelease();
        return layout;
    }
    CC_SAFE_DELETE(layout);
    return nullptr;
}

bool SwallowTouchLayout::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    return true;
}



