//
//  FavItem.h
//  BBQParty
//
//  Created by liuwei on 14-8-25.
//
//

#ifndef __BBQParty__FavItem__
#define __BBQParty__FavItem__

#include <iostream>
#include "cocos2d.h"
#define projDir "donut4013"
#define FAVORITE "favorite"
USING_NS_CC;
using namespace std;
class FavItem : public Ref{
private:
    string name;
    int tag;
public:
    FavItem(string name, int tag);
    void setName(string name);
    void setTag(int tag);
    string getName();
    int getTag();
    CC_SYNTHESIZE(string, iconName, IconName);
    
    Image* getImage();
    string getIconSprite();
    
};
#endif /* defined(__BBQParty__FavItem__) */
