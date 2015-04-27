//
//  SwallowTouchLayout.h
//  DONUT4013
//
//  Created by liuwei on 4/21/15.
//
//

#ifndef __DONUT4013__SwallowTouchLayout__
#define __DONUT4013__SwallowTouchLayout__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace std;
USING_NS_CC;
using namespace cocos2d::ui;

class SwallowTouchLayout : public Layout{
public:
    static SwallowTouchLayout* create();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

};


#endif /* defined(__DONUT4013__SwallowTouchLayout__) */
