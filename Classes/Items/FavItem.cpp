//
//  FavItem.cpp
//  BBQParty
//
//  Created by liuwei on 14-8-25.
//
//

#include "FavItem.h"
#include "STFileUtility.h"
FavItem::FavItem(string name, int tag){
    this->name = name;
    this->tag = tag;
}

void FavItem::setName(string name) {
    this->name = name;
}

void FavItem::setTag(int tag) {
    this->tag = tag;
}

string FavItem::getName(){
    return name;
}

int FavItem::getTag(){
    return tag;
}

Image* FavItem::getImage(){
//    string rootPath("");
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string rootPath = STFileUtility::getStoragePath()+ "/"+projDir + "/" + FAVORITE + "/";
#else
    string rootPath = STFileUtility::getStoragePath() + projDir + "/" + FAVORITE + "/";
#endif
    if (!STFileUtility::isFileExist(rootPath+ name)) {
        log("favorite图片没有找到");
        return NULL;
    }
    Image* cImage = new Image();
    cImage->initWithImageFile((rootPath+ name).c_str());
    cImage->autorelease();
    return cImage;
}