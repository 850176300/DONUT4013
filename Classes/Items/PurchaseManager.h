//
//  PurchaseManager.h
//  DONUT4013
//
//  Created by liuwei on 15/5/6.
//
//

#ifndef __DONUT4013__PurchaseManager__
#define __DONUT4013__PurchaseManager__

#include <iostream>
#include "cocos2d.h"
#include "SingleTon.h"
using namespace std;
USING_NS_CC;

class PurchaseManager : public SingleTon<PurchaseManager>{
    friend class SingleTon<PurchaseManager>;
public:
    PurchaseManager();
    void preloadAll();
    CC_SYNTHESIZE_READONLY(bool, unlockBestDeal, IsBestDealUnlock);
    CC_SYNTHESIZE_READONLY(bool, unlockAll, IsAllUnlock);
    CC_SYNTHESIZE_READONLY(bool, unlockCereal, IsCerealUnlock);
    CC_SYNTHESIZE_READONLY(bool, unlockMilk, IsFlavorUnlock);
    CC_SYNTHESIZE_READONLY(bool, unlockDecoration, IsDecorationUnlock);
    CC_SYNTHESIZE_READONLY(bool, unlockAd, RemoveAd);
    void unlockByKeyString(const string& iapKey);
    bool checkIsUnlock(const string& iapkey);
    
private:
    const string IAPKEYS[6] = {"com.donut_digital.cereal.bestdeal",
        "com.donut_digital.cereal.unlockall",
        "com.donut_digital.cereal.cereals",
        "com.donut_digital.cereal.milk",
        "com.donut_digital.cereal.decorations",
        "com.donut_digital.cereal.noads"
    };
};


#endif /* defined(__DONUT4013__PurchaseManager__) */
