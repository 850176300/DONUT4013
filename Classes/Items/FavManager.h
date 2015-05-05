//
//  FavManager.h
//  BBQParty
//
//  Created by liuwei on 14-8-25.
//
//

#ifndef __BBQParty__FavManager__
#define __BBQParty__FavManager__

#include <iostream>
#include <vector>
#include <sstream>
#include "SingleTon.h"
#include "cocos2d.h"
#include "FavItem.h"

USING_NS_CC;
using namespace std;
//typedef struct FavData{
//    string name;
//    int tag;
//}FavData;
class FavManager: public SingleTon<FavManager>
{
    friend class SingleTon<FavManager>;
public:
    FavManager();
    ~FavManager();
    void loadFavdata();
    void saveData();
    string getfavItemName();
    __Array* getAllFavs();
    void addFavItem(string name);
    bool removeFavByIndex(int index);
private:
    int maxTag;
    __Array* allFavs;
    
};
#endif /* defined(__BBQParty__FavManager__) */
