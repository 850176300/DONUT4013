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
    iconName = name.replace(name.size() - 4, name.size() - 1, "_icon.png");
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

    string rootPath = STFileUtility::getStoragePath() + projDir + "/" + FAVORITE + "/";

    if (!STFileUtility::isFileExist(rootPath+ name)) {
        log("favorite图片没有找到");
        return NULL;
    }
    Image* cImage = new Image();
    cImage->initWithImageFile((rootPath+ name).c_str());
    cImage->autorelease();
    return cImage;
}


string FavItem::getIconSprite(){
    string rootPath = STFileUtility::getStoragePath() + projDir + "/" + FAVORITE + "/";
    
    if (!STFileUtility::isFileExist(rootPath + iconName)) {
        log("favorite icon图片没有找到");
        return NULL;
    }
    Image* cImage = new Image();
    cImage->initWithImageFile((rootPath+ iconName).c_str());
    Director::getInstance()->getTextureCache()->addImage(cImage, iconName);
    cImage->release();
    return iconName;
}
