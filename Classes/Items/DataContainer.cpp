//
//  DataContainer.cpp
//  DONUT4013
//
//  Created by liuwei on 4/21/15.
//
//

#include "DataContainer.h"
#include "SuperGlobal.h"

DataContainer::DataContainer(){
    initFlavorData();
    initanimalAnimation();
    initmilkData();
    initDecorate();
    flavor = "apple";
    milk = "apple";
}

void DataContainer::initanimalAnimation(){
    theType["bees"] = 12;
    theType["bird"] = 11;
    theType["rabbit"] = 10;
    theType["squirrel"] = 14;
}

void DataContainer::initFlavorData(){
    __Dictionary *dict =  __Dictionary::createWithContentsOfFile("make/flavor/flavordata.plist");
    CCAssert(dict != NULL, "Shape-file not found"); // not triggered - cocos2dx delivers empty dict if non was found
    CCAssert(dict->count() != 0, "plist file empty or not existing");
    __Array* thekeys = dict->allKeys();
    
    Ref *_obj;
    CCARRAY_FOREACH(thekeys, _obj){
        FlavorInfor _theInfor;
        __String* type = dynamic_cast<__String*>(_obj);
        __Dictionary* item = (__Dictionary*)dict->objectForKey(type->getCString());
        __String* H = (__String*)item->objectForKey("S");
        __String* S = (__String*)item->objectForKey("H");
        __String* V = (__String*)item->objectForKey("V");
        __String* _typeName = (__String*)item->objectForKey("type");
        __String* pcount = (__String*)item->objectForKey("pcount");
        
        _theInfor.name = string(type->getCString());
        _theInfor.hsv = Vec3(atoi(H->getCString()), atoi(S->getCString()), atoi(V->getCString()));
        _theInfor.type = string(_typeName->getCString());
        _theInfor.isFree = item->valueForKey("IsFree")->boolValue();
        _theInfor.pcount = atoi(pcount->getCString());
        flavorData[type->getCString()] = _theInfor;
    };
}

void DataContainer::initmilkData(){
    __Dictionary *dict =  __Dictionary::createWithContentsOfFile("make/milk/milkinfo.plist");
    CCAssert(dict != NULL, "Shape-file not found"); // not triggered - cocos2dx delivers empty dict if non was found
    CCAssert(dict->count() != 0, "plist file empty or not existing");
    __Array* thekeys = dict->allKeys();
    
    Ref *_obj;
    CCARRAY_FOREACH(thekeys, _obj){
        MilkInfor _theInfor;
        __String* type = dynamic_cast<__String*>(_obj);
        __Dictionary* item = (__Dictionary*)dict->objectForKey(type->getCString());
        
        _theInfor.name = string(type->getCString());
        __String* H = (__String*)item->objectForKey("S");
        __String* S = (__String*)item->objectForKey("H");
        __String* V = (__String*)item->objectForKey("V");
        _theInfor.hsv = Vec3(atoi(H->getCString()), atoi(S->getCString()), atoi(V->getCString()));
        _theInfor.isHSVuse = item->valueForKey("usethis")->boolValue();
        _theInfor.isFree = item->valueForKey("IsFree")->boolValue();
        milkData[type->getCString()] = _theInfor;
    };
}

void DataContainer::initDecorate(){
    __Dictionary *dict =  __Dictionary::createWithContentsOfFile("decorate/decorate.plist");
    CCAssert(dict != NULL, "Shape-file not found"); // not triggered - cocos2dx delivers empty dict if non was found
    CCAssert(dict->count() != 0, "plist file empty or not existing");
    __Array* thekeys = dict->allKeys();
    
    thekeys->setObject(__String::create("candy"), 0);
    thekeys->setObject(__String::create("chocolate"), 1);
    thekeys->setObject(__String::create("breakfast food"), 2);
    thekeys->setObject(__String::create("cereal box"), 3);
    thekeys->setObject(__String::create("spoon"), 4);
    thekeys->setObject(__String::create("mascot sticker"), 5);
    thekeys->setObject(__String::create("bg"), 6);
    
    Ref *_obj;
    CCARRAY_FOREACH(thekeys, _obj){
        DecorateInfor _theInfor;
        __String* type = dynamic_cast<__String*>(_obj);
        __Dictionary* item = (__Dictionary*)dict->objectForKey(type->getCString());
        
        _theInfor.type = string(type->getCString());
        _theInfor.startIndex = atoi(((__String*)item->objectForKey("IndexC"))->getCString());
        _theInfor.totalCount = atoi(((__String*)item->objectForKey("TotalC"))->getCString());
        _theInfor.freeCount = atoi(((__String*)item->objectForKey("FreeC"))->getCString());
        _theInfor.canRotate = item->valueForKey("CanRotate")->boolValue();
        _theInfor.canScale = item->valueForKey("CanScale")->boolValue();
        decorateData[type->getCString()] = _theInfor;
        decorateTypes.push_back(type->getCString());
    }
}


int DataContainer::getAnimalAnimationCount(string animal){
    return theType[animal];
}

DataContainer::flavorMap DataContainer::getFlavorData(){
    return flavorData;
}

DataContainer::FlavorInfor DataContainer::getTheFlavorByName(string name){
    return flavorData[name];
}

DataContainer::milkMap DataContainer::getallMilkData(){
    return milkData;
}

DataContainer::MilkInfor DataContainer::getTheMilkByName(string name) {
    return milkData[name];
}

DataContainer::decorateMap DataContainer::getallDecorateData(){
    return decorateData;
}

DataContainer::DecorateInfor DataContainer::getDecorateAtName(string name) {
    return decorateData[name];
}

vector<string> DataContainer::getAllDecorateType(){
    return decorateTypes;
}

void DataContainer::unlockTheFlavorData(){
    DataContainer::flavorMap::iterator it = flavorData.begin();
    for (; it != flavorData.end(); ++it) {
        it->second.isFree = true;
    }
}

void DataContainer::unlockTheMilkData(){
    DataContainer::milkMap::iterator it = milkData.begin();
    for (; it != milkData.end(); ++it) {
        it->second.isFree = true;
    }
}

void DataContainer::unlockTheDecorateData(){
    DataContainer::decorateMap::iterator it = decorateData.begin();
    for (; it != decorateData.end(); ++it) {
        it->second.freeCount = it->second.totalCount + 1;
    }
}