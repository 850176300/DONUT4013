//
//  DataContainer.cpp
//  DONUT4013
//
//  Created by liuwei on 4/21/15.
//
//

#include "DataContainer.h"


DataContainer::DataContainer(){
    initFlavorData();
    initanimalAnimation();
    initmilkData();
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
        _theInfor.name = string(type->getCString());
        milkData[type->getCString()] = _theInfor;
    };
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