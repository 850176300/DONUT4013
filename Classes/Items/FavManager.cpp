//
//  FavManager.cpp
//  BBQParty
//
//  Created by liuwei on 14-8-25.
//
//

#include "FavManager.h"
#include "STFileUtility.h"
#define FavPath "favadata.plist"

FavManager::FavManager(){

    STFileUtility::createDirectory((STFileUtility::getStoragePath() +"/"+ projDir).c_str());
    STFileUtility::createDirectory((STFileUtility::getStoragePath() +"/"+ projDir +"/"+ FAVORITE).c_str());
}

FavManager::~FavManager(){
    if (allFavs) {
        CC_SAFE_RELEASE(allFavs);
        allFavs = NULL;
    }
}


void FavManager::loadFavdata(){
    allFavs = __Array::createWithCapacity(10);
    allFavs->retain();
    string filepath = FileUtils::getInstance()->getWritablePath()+FavPath;
    if (FileUtils::getInstance()->isFileExist(filepath.c_str())) {
        
        log("首次进入the fav data file is  %s", filepath.c_str());
        __Dictionary* dict = __Dictionary::createWithContentsOfFile((FileUtils::getInstance()->getWritablePath()+FavPath).c_str());
        
        if (dict == NULL) {
            return;
        }else {
            __Array* arr = dict->allKeys();
            Ref* obj = NULL;
            maxTag = 1;
            CCARRAY_FOREACH(arr, obj) {
                __String* key = dynamic_cast<__String*>(obj);
                const __String* value = dict->valueForKey(key->getCString());
                FavItem* item = new FavItem(value->getCString(), key->intValue());
                if (item->getTag() >= maxTag) {
                    maxTag = item->getTag();
                }
                allFavs->addObject(item);
            };
        }
    }
}

void FavManager::saveData(){
    __Dictionary* dict = new __Dictionary();
    for (int i = 0; i < allFavs->count(); ++i) {
        FavItem* item = (FavItem*)allFavs->getObjectAtIndex(i);
        char key[10];
        sprintf(key, "%d", item->getTag());
        dict->setObject(__String::create(item->getName()), key);
    }
    string filename = (FileUtils::getInstance()->getWritablePath()+FavPath).c_str();
    log("首次保存the fav data file is  %s", filename.c_str());
    dict->writeToFile(filename.c_str());
    dict->autorelease();
}

bool FavManager::removeFavByIndex(int index) {
    log("the index is %d", index);
    bool flag = false;
    FavItem* item = (FavItem*)allFavs->getObjectAtIndex(index);
    string rootPath("");
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    rootPath = STFileUtility::getStoragePath()+"/"+projDir + "/" + FAVORITE + "/";
#else
    rootPath = STFileUtility::getStoragePath() + projDir + "/" + FAVORITE + "/";
#endif
    if (STFileUtility::removeDir((rootPath+item->getName()).c_str())) {
        log("delete the file succeed!!!");
        allFavs->removeObject(item);
        flag = true;
    }
    Ref* pObj = NULL;
    maxTag = 1;
    CCARRAY_FOREACH(allFavs, pObj){
        FavItem* item = dynamic_cast<FavItem*>(pObj);
        if (item->getTag() >= maxTag) {
            maxTag = item->getTag();
        }
    };
    if (flag == true) {
        saveData();
    }
    return flag;
}

string FavManager::getfavItemName()
{
    char fileName[256];
    if (allFavs->count() <= 0) {
        sprintf(fileName, "fav_%d.png", 0);
    }else {
        sprintf(fileName, "fav_%d.png", maxTag);
    }
    string rootPath("");
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    rootPath = STFileUtility::getStoragePath()+"/"+projDir + "/" + FAVORITE + "/";
#else
    rootPath = STFileUtility::getStoragePath() + projDir + "/" + FAVORITE + "/";
#endif
    
    log("the new fav item is %s", (rootPath+fileName).c_str());
    return rootPath+fileName;
}

__Array* FavManager::getAllFavs(){
    return allFavs;
}

void FavManager::addFavItem(string name) {
    char fileName[256];
    if (allFavs->count() <= 0) {
        sprintf(fileName, "fav_%d.png", 0);
    }else {
        sprintf(fileName, "fav_%d.png", maxTag);
    }
    ++maxTag;
    FavItem* item = new FavItem(fileName, maxTag);
    allFavs->addObject(item);
    log("item has been added to fav");

}

