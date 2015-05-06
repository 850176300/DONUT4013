//
//  FavorteScene.cpp
//  DONUT4013
//
//  Created by liuwei on 5/6/15.
//
//

#include "FavorteScene.h"
#include "FavManager.h"

Scene* FavoriteScene::scene(){
    Scene* pScene = Scene::create();
    
    FavoriteScene* player = FavoriteScene::create();
    
    pScene->addChild(player);
    
    return pScene;
}


bool FavoriteScene::init(){
    if (GameLayerBase::initWithBgFileName("make/bg_make_table_cover.png")) {
        addScrollview();
        return true;
    }
    return false;
}

void FavoriteScene::addScrollview(){
    if (favItems == nullptr) {
        favItems = FavManager::getInstance()->getAllFavs();
        favItems->retain();
    }
    
    pScrollView = ui::ScrollView::create();
    
    int totalColums = favItems->count() / 2;
    for (int i = totalColums - 1; i >= 0; --i) {
        int index1 = i * 2;
        int index2 = i * 2 + 1;
        
        FavItem* pItem = (FavItem*)favItems->getObjectAtIndex(index1);

        Button* itemBtn = Button::create(pItem->getIconSprite());
        
        if (index2 < favItems->count()) {
            
        }
    }
    
}



