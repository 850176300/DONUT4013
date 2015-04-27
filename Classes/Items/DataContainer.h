//
//  DataContainer.h
//  DONUT4013
//
//  Created by liuwei on 4/21/15.
//
//

#ifndef __DONUT4013__DataContainer__
#define __DONUT4013__DataContainer__

#include <iostream>
#include "cocos2d.h"
#include "SingleTon.h"
using namespace std;
using namespace cocos2d;



class DataContainer : public SingleTon<DataContainer>{
    friend class SingleTon<DataContainer>;
public:
    typedef std::map<std::string, int> animation_type;
    
    typedef struct FlavorInfor{
        string name;
        string type;
        Vec3 hsv;
        bool isFree;
        int pcount;
    }FlavorInfor;
    
    typedef struct MilkInfor{
        string name;
    }MilkInfor;
    
    typedef std::map<string, FlavorInfor> flavorMap;
    typedef std::map<string, MilkInfor> milkMap;
public:
    DataContainer();
    
    CC_SYNTHESIZE(string, flavor, ChooseFlavor);
    CC_SYNTHESIZE(string, milk, ChooseMilk);
    int getAnimalAnimationCount(string animal);
    flavorMap getFlavorData();
    FlavorInfor getTheFlavorByName(string name);
    milkMap getallMilkData();
    MilkInfor getTheMilkByName(string name);
protected:
    void initanimalAnimation();
    void initFlavorData();
    void initmilkData();
    
private:
    animation_type theType;
    flavorMap flavorData;
    milkMap milkData;
};


#endif /* defined(__DONUT4013__DataContainer__) */
