//
//  DecorateScene.cpp
//  DONUT4013
//
//  Created by liuwei on 4/28/15.
//
//

#include "SceneHead.h"

Scene* DecorateScene::scene(){
    Scene* pScene = Scene::create();
    
    DecorateScene* pLayer = DecorateScene::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}

bool DecorateScene::init(){
    if (GameLayerBase::initWithBgFileName("make/bg_make_table_cover.png")) {
        DataContainer::FlavorInfor info = DataContainer::getInstance()->getTheFlavorByName(DataContainer::getInstance()->getChooseFlavor());
        
        Image* bg2Image = new Image();
        bg2Image->initWithImageFile("make/bg_make_table.png");
        
        Texture2D* ptexture = new Texture2D();
        ptexture->initWithImage(CCImageColorSpace::imageWithHSB(bg2Image, info.hsv.x, info.hsv.y, info.hsv.z));
        Sprite* bg2 = Sprite::createWithTexture(ptexture);
        bg2->setPosition(STVisibleRect::getCenterOfScene() + Vec2(0, 120));
        addChild(bg2, -1);
        
        bg2Image->autorelease();
        ptexture->autorelease();
        
        Sprite* table = Sprite::create("make/bg_make_table_table.png");
        table->setAnchorPoint(Vec2(0.5, 0));
        table->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y));
        addChild(table, 1);
        
        float tableMaxy = table->getBoundingBox().getMaxY();
        Sprite* bowl1 = Sprite::create("make/bowl_1.png");
        Sprite* bowl2 = Sprite::create("make/bowl.png");
        bowl1->setPosition(STVisibleRect::getCenterOfScene().x, tableMaxy - 150);
        bowl2->setPosition(bowl1->getPosition());
        addChild(bowl1, 6);
        addChild(bowl2, 3);
        
        string flavor = DataContainer::getInstance()->getChooseFlavor();
        Sprite* flavorinBowl = Sprite::create("make/cereals_stir/"+flavor+"_1.png");
        flavorinBowl->setPosition(bowl1->getPosition());
        flavorinBowl->setName(flavor);
        addChild(flavorinBowl, 5);
        
        addScrollView();
        
        return true;
    }
    
    return false;
}

void DecorateScene::addScrollView(){
    typeScrollView = cocos2d::extension::ScrollView::create(Size(STVisibleRect::getGlvisibleSize().width, 124));
    
    vector<string> types = DataContainer::getInstance()->getAllDecorateType();
    vector<string>::iterator it;
    
    for (it = types.begin(); it != types.end(); ++it) {
        Button* 
    }
}

void DecorateScene::onEnterTransitionDidFinish(){
    GameLayerBase::onEnterTransitionDidFinish();
    
}




