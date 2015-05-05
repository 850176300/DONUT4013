//
//  ShopScene.cpp
//  DONUT4013
//
//  Created by liuwei on 15/5/5.
//
//

#include "ShopScene.h"

Scene* ShopScene::scene(){
    Scene* pScene = Scene::create();
    
    ShopScene* pLayer = ShopScene::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}

bool ShopScene::init(){
    if (GameLayerBase::initWithBgFileName("make/bg_make_table_cover.png")) {
        Sprite* title = Sprite::create("ui/shop/shop_title.png");
        title->setPosition(STVisibleRect::getCenterOfScene().x, STVisibleRect::getPointOfSceneLeftUp().y - 100);
        addChild(title, 1);
        
        return true;
    }
    return false;
}


