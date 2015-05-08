//
//  PurchaseManager.cpp
//  DONUT4013
//
//  Created by liuwei on 15/5/6.
//
//

#include "PurchaseManager.h"
#include "DataContainer.h"
#include "STAds.h"
#include "SuperGlobal.h"

PurchaseManager::PurchaseManager(){
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    unlockBestDeal = UserDefault::getInstance()->getBoolForKey("unlockBestDeal", false);
    unlockAll = UserDefault::getInstance()->getBoolForKey("unlockAll", true);
    unlockCereal = UserDefault::getInstance()->getBoolForKey("unlockCereal", true);
    unlockMilk = UserDefault::getInstance()->getBoolForKey("unlockMilk", true);
    unlockDecoration = UserDefault::getInstance()->getBoolForKey("unlockDecoration", true);
    unlockAd = UserDefault::getInstance()->getBoolForKey("unlockAd", false);
#else
    unlockBestDeal = UserDefault::getInstance()->getBoolForKey("unlockBestDeal", false);
    unlockAll = UserDefault::getInstance()->getBoolForKey("unlockAll", false);
    unlockCereal = UserDefault::getInstance()->getBoolForKey("unlockCereal", false);
    unlockMilk = UserDefault::getInstance()->getBoolForKey("unlockMilk", false);
    unlockDecoration = UserDefault::getInstance()->getBoolForKey("unlockDecoration", false);
    unlockAd = UserDefault::getInstance()->getBoolForKey("unlockAd", false);
#endif
}

void PurchaseManager::preloadAll(){
    if (unlockMilk == true) {
        DataContainer::getInstance()->unlockTheMilkData();
    }
    if (unlockCereal == true) {
        DataContainer::getInstance()->unlockTheFlavorData();
    }
    if (unlockDecoration == true) {
        DataContainer::getInstance()->unlockTheDecorateData();
    }
}

void PurchaseManager::unlockByKeyString(const string& iapKey) {
    if (iapKey == IAPKEYS[0]) {
        unlockBestDeal = true;
        UserDefault::getInstance()->setBoolForKey("unlockBestDeal", unlockBestDeal);
        unlockAll = true;
        UserDefault::getInstance()->setBoolForKey("unlockAll", unlockAll);
        unlockCereal = true;
        UserDefault::getInstance()->setBoolForKey("unlockCereal", unlockCereal);
        unlockMilk = true;
        UserDefault::getInstance()->setBoolForKey("unlockMilk", unlockMilk);
        unlockDecoration = true;
        UserDefault::getInstance()->setBoolForKey("unlockDecoration", unlockDecoration);
        unlockAd = true;
        UserDefault::getInstance()->setBoolForKey("unlockBestDeal", unlockAd);
        DataContainer::getInstance()->unlockTheDecorateData();
        DataContainer::getInstance()->unlockTheFlavorData();
        DataContainer::getInstance()->unlockTheMilkData();
        STAds ads;
        ads.removeAds();
    }else if (iapKey == IAPKEYS[1]) {
        unlockAll = true;
        UserDefault::getInstance()->setBoolForKey("unlockAll", unlockAll);
        unlockCereal = true;
        UserDefault::getInstance()->setBoolForKey("unlockCereal", unlockCereal);
        unlockMilk = true;
        UserDefault::getInstance()->setBoolForKey("unlockMilk", unlockMilk);
        unlockDecoration = true;
        UserDefault::getInstance()->setBoolForKey("unlockDecoration", unlockDecoration);
        DataContainer::getInstance()->unlockTheDecorateData();
        DataContainer::getInstance()->unlockTheFlavorData();
        DataContainer::getInstance()->unlockTheMilkData();
    }else if (iapKey == IAPKEYS[2]) {
        unlockCereal = true;
        UserDefault::getInstance()->setBoolForKey("unlockCereal", unlockCereal);
        DataContainer::getInstance()->unlockTheFlavorData();
    }else if (iapKey == IAPKEYS[3]) {
        unlockMilk = true;
        UserDefault::getInstance()->setBoolForKey("unlockMilk", unlockMilk);
        DataContainer::getInstance()->unlockTheMilkData();
    }else if (iapKey == IAPKEYS[4]) {
        unlockDecoration = true;
        UserDefault::getInstance()->setBoolForKey("unlockDecoration", unlockDecoration);
        DataContainer::getInstance()->unlockTheDecorateData();
    }else if (iapKey == IAPKEYS[5]) {
        unlockAd = true;
        UserDefault::getInstance()->setBoolForKey("unlockBestDeal", unlockAd);
        STAds ads;
        ads.removeAds();
    }
    if (unlockAll == false) {
        if (unlockCereal == true && unlockMilk == true && unlockDecoration == true) {
            unlockAll = true;
            UserDefault::getInstance()->setBoolForKey("unlockAll", true);
        }
    }
    
    if (unlockBestDeal == false) {
        if (unlockAll == true && unlockAd == true) {
            unlockBestDeal = true;
            UserDefault::getInstance()->setBoolForKey("unlockBestDeal", true);
        }
    }
    UserDefault::getInstance()->flush();
    NotificationCenter::getInstance()->postNotification(PURCHASESUCCEED);
}

bool PurchaseManager::checkIsUnlock(const string& iapkey) {
    if (iapkey == IAPKEYS[0]) {
        return unlockBestDeal;
    }else if (iapkey == IAPKEYS[1]) {
        return unlockAll;
    }else if (iapkey == IAPKEYS[2]) {
        return unlockCereal;
    }else if (iapkey == IAPKEYS[3]) {
        return unlockMilk;
    }else if (iapkey == IAPKEYS[4]) {
        return unlockDecoration;
    }else if (iapkey == IAPKEYS[5]) {
        return unlockAd;
    }
    return false;
}