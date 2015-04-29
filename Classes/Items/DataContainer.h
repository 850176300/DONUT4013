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
    
    typedef struct DecorateInfor{
        string type;
        int freeCount;
        int totalCount;
        int startIndex;
        bool canScale;
        bool canRotate;
    }DecorateInfor;
    
    typedef struct MilkInfor{
        string name;
        Vec3 hsv;
        bool isHSVuse;
    }MilkInfor;
    
    typedef struct EatThings{
        string name;
        int tag;
        Vec2 pos;
        float scale;
        float rotate;
        int localZorder;
    }EatThings;
    
    typedef struct DecorateItems{
        string bgName;
        string breakfastName;
        string spoonName;
        string cerealName;
        string mascotName;
        vector<EatThings> alleatThings;
    }DecorateItems;
    
    typedef std::map<string, FlavorInfor> flavorMap;
    typedef std::map<string, MilkInfor> milkMap;
    typedef std::map<string, DecorateInfor> decorateMap;
public:
    DataContainer();
    
    CC_SYNTHESIZE(string, flavor, ChooseFlavor);
    CC_SYNTHESIZE(string, milk, ChooseMilk);
    int getAnimalAnimationCount(string animal);
    flavorMap getFlavorData();
    FlavorInfor getTheFlavorByName(string name);
    
    milkMap getallMilkData();
    MilkInfor getTheMilkByName(string name);
    
    decorateMap getallDecorateData();
    DecorateInfor getDecorateAtName(string name);
    
    vector<string> getAllDecorateType();
    void clearDecorateItems();
protected:
    void initanimalAnimation();
    void initFlavorData();
    void initmilkData();
    void initDecorate();
private:
    animation_type theType;
    flavorMap flavorData;
    milkMap milkData;
    decorateMap decorateData;
    vector<string> decorateTypes;
    DecorateItems shareDecorateItems;
    
};


#endif /* defined(__DONUT4013__DataContainer__) */
