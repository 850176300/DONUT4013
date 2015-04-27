//
//  PageItem.h
//  DONUT4013
//
//  Created by liuwei on 4/17/15.
//
//

#ifndef __DONUT4013__PageItem__
#define __DONUT4013__PageItem__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace std;
USING_NS_CC;
using namespace cocos2d::ui;

class PageItem : public ui::Button{
public:
    PageItem():isCenter(false){};
    static PageItem* create(const string& filename);
    CC_SYNTHESIZE(bool, isCenter, CenterOrNot);
};

#endif /* defined(__DONUT4013__PageItem__) */
