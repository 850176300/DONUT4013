//
//  ShareScene.cpp
//  DONUT4013
//
//  Created by liuwei on 5/5/15.
//
//

#include "SceneHead.h"
#include "STFileUtility.h"
#include "FavManager.h"
#include "ShareFrameItem.h"
#include "AlertViewLayer.h"

Scene* ShareScene::scene(){
    Scene* pScene = Scene::create();
    
    ShareScene* pLayer = ShareScene::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}

bool ShareScene::init(){
    DataContainer::DecorateItems allItems = DataContainer::getInstance()->getAllDecorateItems();
    if (GameLayerBase::initWithBgFileName(allItems.bgName.c_str())) {
        if (allItems.bgName == "make/bg_make_table_cover.png") {
            DataContainer::FlavorInfor info = DataContainer::getInstance()->getTheFlavorByName(DataContainer::getInstance()->getChooseFlavor());
            Sprite* bg2 = Sprite::create("make/bg/tablebg_"+info.name+".png");
            bg2->setPosition(STVisibleRect::getCenterOfScene() + Vec2(0, 120));
            addChild(bg2, -1);
        }
        
        Sprite* cannotEat = Sprite::create(STFileUtility::getStoragePath()+"temp1.png");
        cannotEat->setAnchorPoint(Vec2::ZERO);
        addChild(cannotEat, 1);
        
        Sprite* table = Sprite::create("make/bg_make_table_table.png");
        table->setAnchorPoint(Vec2(0.5, 0));
        table->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y+GameLayerBase::getBannerSize()));
        
        float tableMaxy = table->getBoundingBox().getMaxY();
        Sprite* bowl1 = Sprite::create("make/bowl_1.png");
        Sprite* bowl2 = Sprite::create("make/bowl.png");
        bowl1->setPosition(STVisibleRect::getCenterOfScene().x, tableMaxy - 180);
        bowl2->setPosition(bowl1->getPosition());
        this->addChild(bowl1, 6);
        this->addChild(bowl2, 3);

        _scribble = new Scribble(CCSprite::create("ui/eat_mask.png"));
        _scribble->setBrushShader();
        _scribble->setTargetSolid(true);
        _scribble->setTargetAlphaTestValue(0.0f);
        _scribble->setBrushType(Scribble::eEaser);
        
        
        string flavor = DataContainer::getInstance()->getChooseFlavor();
        flavorinBowl = DaubSprite::create(_scribble, "make/cereals_stir/"+flavor+"_1.png");
        flavorinBowl->setPosition(bowl1->getPosition());
        flavorinBowl->show();
        addChild(flavorinBowl, 5);
        
        breakFast = DaubSprite::create(_scribble, allItems.breakfastName);
        breakFast->setPosition(Vec2(STVisibleRect::getPointOfSceneRightBottom().x - 130, tableMaxy-20));
        breakFast->show();
        addChild(breakFast, 2);
        breakFast->setScale(0.8f);

        eatLayer = DaubSprite::create(_scribble, STFileUtility::getStoragePath()+"temp.png");
        eatLayer->setPosition(STVisibleRect::getCenterOfScene());
        eatLayer->show();
        addChild(eatLayer, 8);
        
        DataContainer::ItemThings spoonItem = allItems.alleatThings.at(0);
        Sprite* spoon = Sprite::create(spoonItem.name);
        spoon->setPosition(spoonItem.pos);
        spoon->setScale(spoonItem.scale);
        spoon->setRotation(spoonItem.rotate);
        addChild(spoon, spoonItem.localZorder);
        
        showPreviousBtn(1.0f);
        showHomeButton(1.0f);
        showShareBtn(1.2f);
        showFavoriteBtn(1.5f);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        
        listener->onTouchBegan = CC_CALLBACK_2(Layer::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(Layer::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(Layer::onTouchEnded, this);
        listener->onTouchCancelled = CC_CALLBACK_2(Layer::onTouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        _touchListener = listener;
        
        taptoEat = Sprite::create("ui/eat/tips_eat.png");
        
        taptoEat->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getPointOfSceneLeftUp().y - 100) + Vec2(-STVisibleRect::getGlvisibleSize().width, 0));
        
        addChild(taptoEat, 20);
        
        pBtn = MoregameBtn::create();
        pBtn->addtoParentLayer(this);
        return true;
    }
    return false;
}

void ShareScene::onEnterTransitionDidFinish(){
    GameLayerBase::onEnterTransitionDidFinish();
    pBtn->startLoading();
    totalColorCount = eatLayer->getTotalCount() + flavorinBowl->getTotalCount() + breakFast->getTotalCount();
    if (taptoEat != nullptr) {
        taptoEat->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(1.0, Vec2(STVisibleRect::getGlvisibleSize().width, 0)), 0.8), DelayTime::create(1.0f), EaseElasticInOut::create(MoveBy::create(0.8, Vec2(STVisibleRect::getGlvisibleSize().width, 0)), 0.3), RemoveSelf::create(),NULL));
    }
}

void ShareScene::preClickEvent(){
    GameLayerBase::preClickEvent();
    Director::getInstance()->popScene();
}

void ShareScene::homeClickEvent(){
    GameLayerBase::homeClickEvent();
    replaceTheScene<HomeScene>();
}

void ShareScene::shareClickEvent(){
    GameLayerBase::shareClickEvent();
    shareShotScreen();
}

void ShareScene::favoriteClickEvent(){
    GameLayerBase::favoriteClickEvent();
    favoriteshotScreen();
    AlertViewLayer* alertView = AlertViewLayer::createWithTitle("Your image has been added to your favorites.", AlertViewLayer::SINGLE);
    this->addChild(alertView, kDialog);
}

void ShareScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    eatLayer->paint(touch->getLocation(), touch->getStartLocation());
    flavorinBowl->paint(touch->getLocation(), touch->getStartLocation());
    breakFast->paint(touch->getLocation(), touch->getStartLocation());
    if (arc4random() % 3 == 1) {
        long tempCount = eatLayer->getTotalCount() + flavorinBowl->getTotalCount() + breakFast->getTotalCount();
        if (tempCount < totalColorCount * 0.15) {
            //show eat again
            eatAgain = CocosHelper::getButton("ui/eat/tips_eat_again.png", "ui/eat/tips_eat_again.png");
            eatAgain->setPosition(STVisibleRect::getCenterOfScene() + Vec2(0, 150));
            eatAgain->addTargetWithActionForControlEvents(this, cccontrol_selector(ShareScene::onEatAgainClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
            addChild(eatAgain, kPrompt);
            
            _eventDispatcher->removeEventListenersForTarget(this);
            eatAgain->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(ScaleTo::create(0.8, 0.8)), EaseSineInOut::create(ScaleTo::create(0.8, 1.0)), NULL)));
        }
    }
}

void ShareScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    onTouchEnded(touch, unused_event);
}

void ShareScene::onEatAgainClicked(cocos2d::Ref *pRef, Control::EventType type) {
    DataContainer::DecorateItems allItems = DataContainer::getInstance()->getAllDecorateItems();
    eatLayer->changeTarget(STFileUtility::getStoragePath()+"temp.png");
    flavorinBowl->changeTarget("make/cereals_stir/"+DataContainer::getInstance()->getChooseFlavor()+"_1.png");
    breakFast->changeTarget(allItems.breakfastName);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

void ShareScene::favoriteshotScreen(){
    RenderTexture* pRender = RenderTexture::create(STVisibleRect::getGlvisibleSize().width, STVisibleRect::getGlvisibleSize().height, Texture2D::PixelFormat::RGBA8888);
    
    pRender->setPosition(Vec2(-STVisibleRect::getDesignOffset().x, -STVisibleRect::getDesignOffset().y));
    
    GameLayerBase::setallButton(false);
    if (eatAgain != nullptr) {
        eatAgain->setVisible(false);
    }
    pBtn->setVisible(false);
    pRender->begin();
    this->visit();
    pRender->end();
    
    pRender->getSprite()->setFlippedX(false);
    pRender->getSprite()->getTexture()->setAntiAliasTexParameters();
    

    Director::getInstance()->getRenderer()->render();
    
//    
//    Sprite* pSprite = pRender->getSprite();
//    pSprite->setPosition(Vec2(m_pBg->getContentSize().width*0.6 / 2.0, m_pBg->getContentSize().height*0.6/2.0));
//    
//    RenderTexture* pRender1 = RenderTexture::create(m_pBg->getContentSize().width*0.6, m_pBg->getContentSize().height*0.6, Texture2D::PixelFormat::RGBA8888);
//    if (STVisibleRect::getGlvisibleSize().width / STVisibleRect::getGlvisibleSize().height > 768.0/1136.0) {
//        pSprite->setScale((1136*0.6) / STVisibleRect::getGlvisibleSize().height);
//    }else {
//        pSprite->setScale((768*0.6) / STVisibleRect::getGlvisibleSize().width);
//        
//    }
//    pRender1->begin();
//    pSprite->visit();
//    pRender1->end();
//    
//    pSprite->setScale(1.0);
//    
//    pRender1->getSprite()->setFlippedX(false);
//    pRender1->getSprite()->getTexture()->setAliasTexParameters();
//    Director::getInstance()->getRenderer()->render();
    
    Image* ppImage = pRender->newImage();
    string favName = FavManager::getInstance()->getfavItemName();
    ppImage->saveToFile(favName, false);
    FavManager::getInstance()->addFavItem(favName);
    ppImage->release();
    
    Sprite* favSprite = pRender->getSprite();
    Sprite* recipe = Sprite::create("ui/share/favorite_recipe.png");
    
    favSprite->setPosition(Vec2(124.4 , 189));
    recipe->setAnchorPoint(Vec2(0, 0));
    RenderTexture* pTexture3 = RenderTexture::create(276, 437, Texture2D::PixelFormat::RGBA8888);
    if (STVisibleRect::getGlvisibleSize().width / STVisibleRect::getGlvisibleSize().height > 768.0/1136.0) {
        favSprite->setScale((1136*0.28) / STVisibleRect::getGlvisibleSize().height);
    }else {
        favSprite->setScale((768*0.28) / STVisibleRect::getGlvisibleSize().width);

    }
    pTexture3->begin();
    recipe->visit();
    favSprite->visit();
    pTexture3->end();
    
    pTexture3->getSprite()->setFlippedX(false);
    pTexture3->getSprite()->getTexture()->setAliasTexParameters();
    
    Director::getInstance()->getRenderer()->render();
    Image* tempIamge = pTexture3->newImage();
    tempIamge->saveToFile(favName.replace(favName.size() - 4, favName.size()-1, "_icon.png"), false);
    tempIamge->release();
    
    GameLayerBase::setallButton(true);
    if (eatAgain != nullptr) {
        eatAgain->setVisible(true);
    }
    pBtn->setVisible(true);
}

void ShareScene::shareShotScreen(){
    RenderTexture* pRender = RenderTexture::create(STVisibleRect::getGlvisibleSize().width, STVisibleRect::getGlvisibleSize().height, Texture2D::PixelFormat::RGBA8888);
    
    pRender->setPosition(Vec2(-STVisibleRect::getDesignOffset().x, -STVisibleRect::getDesignOffset().y));
    
    GameLayerBase::setallButton(false);
    if (eatAgain != nullptr) {
        eatAgain->setVisible(false);
    }
    pBtn->setVisible(false);
    pRender->begin();
    this->visit();
    pRender->end();
    
    pRender->getSprite()->setFlippedX(false);
    pRender->getSprite()->getTexture()->setAntiAliasTexParameters();
    
    
    Director::getInstance()->getRenderer()->render();
    Image* pImage = pRender->newImage();
    pImage->saveToFile(STFileUtility::getStoragePath()+"shotscreen.png", false);
    pImage->release();
    
    Sprite* pSprite = pRender->getSprite();
    pSprite->setPosition(Vec2(m_pBg->getContentSize().width*0.6 / 2.0, m_pBg->getContentSize().height*0.6/2.0));
    
    RenderTexture* pRender1 = RenderTexture::create(m_pBg->getContentSize().width*0.6, m_pBg->getContentSize().height*0.6, Texture2D::PixelFormat::RGBA8888);
    if (STVisibleRect::getGlvisibleSize().width / STVisibleRect::getGlvisibleSize().height > 768.0/1136.0) {
        pSprite->setScale((1136*0.6) / STVisibleRect::getGlvisibleSize().height);
    }else {
        pSprite->setScale((768*0.6) / STVisibleRect::getGlvisibleSize().width);
        
    }
    pRender1->begin();
    pSprite->visit();
    pRender1->end();
    
    pSprite->setScale(1.0);
    
    pRender1->getSprite()->setFlippedX(false);
    pRender1->getSprite()->getTexture()->setAliasTexParameters();
    Director::getInstance()->getRenderer()->render();
    
    Image* ppImage = pRender1->newImage();
    ShareFrameItem* pItem = ShareFrameItem::create(ppImage);
    addChild(pItem, kPrompt + 10);
    ppImage->release();
    
    GameLayerBase::setallButton(true);
    if (eatAgain != nullptr) {
        eatAgain->setVisible(true);
    }
    pBtn->setVisible(true);
}





