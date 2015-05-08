//
//  DecorateScene.cpp
//  DONUT4013
//
//  Created by liuwei on 4/28/15.
//
//

#include "SceneHead.h"
#include "STFileUtility.h"

Scene* DecorateScene::scene(){
    Scene* pScene = Scene::create();
    
    DecorateScene* pLayer = DecorateScene::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}

bool DecorateScene::init(){
    if (GameLayerBase::initWithBgFileName("make/bg_make_table_cover.png")) {
        allItems.bgName = "make/bg_make_table_cover.png";
        
        DataContainer::FlavorInfor info = DataContainer::getInstance()->getTheFlavorByName(DataContainer::getInstance()->getChooseFlavor());
        Sprite* bg2 = Sprite::create("make/bg/tablebg_"+info.name+".png");
        bg2->setPosition(STVisibleRect::getCenterOfScene() + Vec2(0, 120));
        addChild(bg2, -1);
        
        cannotEatLayer = Layer::create();
        cannotEatLayer->setContentSize(STVisibleRect::getGlvisibleSize());
        cannotEatLayer->setPosition(STVisibleRect::getOriginalPoint());
        addChild(cannotEatLayer, 1);
        
        canEatLayer = Layer::create();
        canEatLayer->setContentSize(STVisibleRect::getGlvisibleSize());
        canEatLayer->setPosition(STVisibleRect::getOriginalPoint());
        addChild(canEatLayer, 8);
        
        Sprite* table = Sprite::create("make/bg_make_table_table.png");
        table->setAnchorPoint(Vec2(0.5, 0));
        table->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y));
        cannotEatLayer->addChild(table, 1);
        
        tableMaxy = table->getBoundingBox().getMaxY();
        Sprite* bowl1 = Sprite::create("make/bowl_1.png");
        Sprite* bowl2 = Sprite::create("make/bowl.png");
        bowl1->setPosition(STVisibleRect::getCenterOfScene().x, tableMaxy - 180);
        bowl2->setPosition(bowl1->getPosition());
        this->addChild(bowl1, 6);
        this->addChild(bowl2, 3);
        
        string flavor = DataContainer::getInstance()->getChooseFlavor();
        flavorinBowl = Sprite::create("make/cereals_stir/"+flavor+"_1.png");
        flavorinBowl->setPosition(bowl1->getPosition());
        flavorinBowl->setName(flavor);
        addChild(flavorinBowl, 5);
        
        addFixedThings();
        addScrollView();
       
        showNextButton(2.0f, false);
        showPreviousBtn(2.0f);
        showResetBtn(2.5f);
        return true;
    }
    
    return false;
}

void DecorateScene::addFixedThings(){
    allItems.cerealName = "decorate/cereal box/cereal box1.png";
    allItems.breakfastName = "decorate/breakfast food/breakfast food1.png";
    allItems.spoonName = "decorate/spoon/spoon1.png";
    allItems.mascotName = "decorate/mascot sticker/mascot sticker1.png";
    
    cerealBox = Sprite::create("decorate/cereal box/cereal box1.png");
    plate = Sprite::create("decorate/breakfast food/plate.png");
    breakFast = Sprite::create("decorate/breakfast food/breakfast food1.png");
    spoon = FillMaterialModel::create("decorate/spoon/spoon1.png");
    mascot = FillMaterialModel::create("decorate/mascot sticker/mascot sticker1.png");
    spoon->removeCloseBtn();
    mascot->removeCloseBtn();
    spoon->setIsHidden(true);
    mascot->setIsHidden(true);
    mascot->setPosition(Vec2(195, 169));
    plate->setScale(0.8);
    breakFast->setScale(0.8f);
    
    cerealBox->setAnchorPoint(Vec2(0.5, 0));
    cerealBox->setPosition(Vec2(STVisibleRect::getOriginalPoint().x + 140 - 500, tableMaxy - 100));
    cannotEatLayer->addChild(cerealBox, 2);
    
    cerealBox->addChild(mascot);
    cerealBox->setScale(0.7);
    mascot->setScale(1/0.7f);
    
    plate->setPosition(Vec2(STVisibleRect::getPointOfSceneRightBottom().x - 130, tableMaxy-20));
    cannotEatLayer->addChild(plate, 2);
    
    breakFast->setPosition(plate->getPosition());
    addChild(breakFast, 2);
    
    breakFast->setScale(0);
    plate->setPositionX(plate->getPositionX() + 600);
    spoon->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, tableMaxy - 100));
    addChild(spoon, 10);
    
    cerealBox->runAction(EaseElasticInOut::create(MoveBy::create(0.8, Vec2(500, 0)), 0.5));
    cerealBox->runAction(Sequence::create(DelayTime::create(0.5f), RotateBy::create(0.1, 15), DelayTime::create(0.2f), RotateBy::create(0.2, -15),CallFunc::create([=]{
        FillMaterialModel* temp = FillMaterialModel::create("decorate/mascot sticker/mascot sticker1.png");
        temp->setPosition(cerealBox->convertToWorldSpace(mascot->getPosition()));
        temp->removeCloseBtn();
        temp->setIsHidden(true);
        temp->setScale(1/0.7f);
        cannotEatLayer->addChild(temp, 2);
        mascot->removeFromParent();
        mascot = nullptr;
        mascot = temp;
        CC_SAFE_RETAIN(mascot);
        
    }),NULL));
    plate->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(0.8, Vec2(-600, 0))), DelayTime::create(0.1f), CallFunc::create([=]{
        breakFast->runAction(Sequence::create(ScaleTo::create(0.2, 0.8*1.1), ScaleTo::create(0.1, 0.8f), NULL));
    }), NULL));
}

void DecorateScene::addScrollView(){
    typeScrollView = ScrollPage::create();
    
    typeScrollView->setContentSize(Size(STVisibleRect::getGlvisibleSize().width, 124));
    vector<string> types = DataContainer::getInstance()->getAllDecorateType();
    vector<string>::iterator it;
    float startx = 20;
    for (it = types.begin(); it != types.end(); ++it) {
        Button* pBtn = Button::create("ui/decorate/main_d.png", "ui/decorate/main_s.png");
        Sprite* type = Sprite::create("ui/decorate/"+*it+".png");
        type->setPosition(Vec2(pBtn->getContentSize().width/2.0, pBtn->getContentSize().height/2.0));
        pBtn->setPosition(Vec2(startx + pBtn->getContentSize().width/2.0, 62));
        pBtn->addChild(type);
        pBtn->addTouchEventListener(CC_CALLBACK_2(DecorateScene::ontypeItemClicked, this));
        startx += pBtn->getContentSize().width + 15;
        pBtn->setName(*it);
        typeScrollView->addChild(pBtn);
    }
    typeScrollView->setInnerContainerSize(Size(startx, 124));
    typeScrollView->setDirection(ScrollPage::Direction::HORIZONTAL);
    typeScrollView->setPosition(Vec2(STVisibleRect::getOriginalPoint().x+STVisibleRect::getGlvisibleSize().width, STVisibleRect::getOriginalPoint().y+GameLayerBase::getBannerSize()));
    addChild(typeScrollView, 7);
    
    
    
}

void DecorateScene::onEnterTransitionDidFinish(){
    GameLayerBase::onEnterTransitionDidFinish();
    NotificationCenter::getInstance()->postNotification(kShotScreenEvent, __String::create("Yes"));

    scrollPosition1 = Vec2(STVisibleRect::getOriginalPoint().x, STVisibleRect::getOriginalPoint().y+GameLayerBase::getBannerSize());
    scrollPosition2 = Vec2(STVisibleRect::getOriginalPoint().x , STVisibleRect::getOriginalPoint().y+GameLayerBase::getBannerSize()) + Vec2(0, -300);
    if (isFirstOnenter == false) {
        return;
    }
    isFirstOnenter = false;
    if (typeScrollView->getPosition().x > STVisibleRect::getOriginalPoint().x) {
        typeScrollView->runAction(EaseSineInOut::create(MoveTo::create(1.0, scrollPosition1)));
        this->runAction(Sequence::create(DelayTime::create(0.8), CallFunc::create([=]{
            typeScrollView->scrollToPercentHorizontal(100, 0.8, true);
        }), DelayTime::create(1.0), CallFunc::create([=]{
            typeScrollView->scrollToPercentHorizontal(0, 0.5, true);
        }),NULL));
    }
}

void DecorateScene::ontypeItemClicked(cocos2d::Ref *pRef, Widget::TouchEventType toucht){
    if (toucht == Widget::TouchEventType::ENDED) {
        if (typeScrollView->getNumberOfRunningActions() != 0) {
            return;
        }
        Button* pNode = dynamic_cast<Button*>(pRef);

        typeScrollView->runAction(EaseElasticInOut::create(MoveTo::create(0.5, scrollPosition2), 0.4));
        currentSelType = pNode->getName();
        if (itemScrollView == nullptr) {
            frameBanner = Sprite::create("ui/decorate/sub_box.png");
            itemScrollView = ScrollPage::create();
            itemScrollView->setContentSize(Size(STVisibleRect::getGlvisibleSize().width, 159));
            itemScrollView->setDirection(ScrollPage::Direction::HORIZONTAL);
            itemScrollView->setPosition(Vec2::ZERO);
            frameBanner->addChild(itemScrollView);
            
            frameBanner->setAnchorPoint(Vec2(0, 0));
            frameBanner->setPosition(scrollPosition1 + Vec2(STVisibleRect::getGlvisibleSize().width, 0));
            addChild(frameBanner, 7);
            
            Button* downButton = Button::create("ui/decorate/sub_pulldown.png");
            downButton->setAnchorPoint(Vec2(0.5, 0));
            downButton->setPosition(Vec2(frameBanner->getContentSize().width/2.0, frameBanner->getContentSize().height-15));
            downButton->addTouchEventListener(CC_CALLBACK_2(DecorateScene::onDownButtonClicked, this));
            frameBanner->addChild(downButton, 1);
//            downButton->setScale9Enabled(true);
            
            refreshData();
            
            frameBanner->runAction(Sequence::create(EaseSineInOut::create(MoveTo::create(0.8, scrollPosition1)), DelayTime::create(0.0f),CallFunc::create([=]{
                itemScrollView->scrollToPercentHorizontal(60, 0.7, true);
            }), DelayTime::create(0.8), CallFunc::create([=]{
                itemScrollView->scrollToPercentHorizontal(0, 0.8, true);
            }),NULL));
        }else {
            refreshData();
            frameBanner->runAction(EaseElasticInOut::create(MoveTo::create(0.5, scrollPosition1), 0.4));
        }
    }
}

void DecorateScene::onDownButtonClicked(cocos2d::Ref *pRef, Widget::TouchEventType touch) {
    if (touch == Widget::TouchEventType::ENDED) {
        frameBanner->runAction(Sequence::create(EaseElasticInOut::create(MoveTo::create(0.5, scrollPosition2), 0.4f), DelayTime::create(0.1), CallFunc::create([=]{
            typeScrollView->runAction(EaseElasticInOut::create(MoveTo::create(0.5, scrollPosition1), 0.4f));
        }),nullptr));
    }
}

void DecorateScene::refreshData(){
    if (itemScrollView == nullptr) {
        return;
    }
    
    float startx = 20;
    
    if (itemScrollView->getChildren().size() != 0) {
        itemScrollView->removeAllChildren();
    }
    
    DataContainer::DecorateInfor _infor = DataContainer::getInstance()->getDecorateAtName(currentSelType);
    for (int i = _infor.startIndex; i < _infor.totalCount; ++i) {
        Button* pBtn = Button::create("ui/decorate/sub_d.png", "ui/decorate/sub_s.png");
        pBtn->setPosition(Vec2(startx + pBtn->getContentSize().width/2.0, 159/2.0));
        startx += pBtn->getContentSize().width;
        pBtn->addTouchEventListener(CC_CALLBACK_2(DecorateScene::onItemsThingClicked, this));
        Sprite* item = Sprite::create("decorate/icon_category/icon_"+currentSelType+"/icon_"+currentSelType+convertIntToString(i)+".png");
        item->setPosition(Vec2(120/2.0, 120/2.0));
        pBtn->addChild(item, 1);
        pBtn->setName(currentSelType);
        pBtn->setTag(i);

        if (i > _infor.freeCount) {
            Sprite* lock = Sprite::create("ui/select/lock.png");
            lock->setAnchorPoint(Vec2(1.0, 0));
            lock->setPosition(item->getContentSize().width-15, 15);
            pBtn->addChild(lock, 2, kLockTags);
            lock->setScale(0.5f);
        }
        itemScrollView->addChild(pBtn);
    }
    itemScrollView->setInnerContainerSize(Size(startx, 159));
}

void DecorateScene::onItemsThingClicked(cocos2d::Ref *pRef, Widget::TouchEventType ttouch) {
    if (ttouch == Widget::TouchEventType::ENDED) {
        if (itemScrollView->getNumberOfRunningActions() != 0) {
            return;
        }
        Node* pNode = dynamic_cast<Node*>(pRef);
        if (pNode->getChildByTag(kLockTags) != nullptr) {
            //goto shop layer
            pushTheScene<ShopScene>();
            return;
        }
        if (decorateItemCantBeClick == true) {
            return;
        }
        
        string decorateType = pNode->getName();
        string filePath = "decorate/"+decorateType+"/"+decorateType+convertIntToString(pNode->getTag()) + ".png";
        Vec2 pos = Vec2::ZERO;
        Vec2 startPos = pNode->convertToWorldSpace(Vec2(0, 0)) + Vec2(pNode->getContentSize().width/2.0, pNode->getContentSize().height/2.0);
        int localZorder = 0;
        Node* decorateItem = nullptr;
        if (decorateType == "bg") {
            filePath.replace(filePath.size() - 4, filePath.size() - 1, ".jpg");
            m_pBg->setTexture(filePath);
            allItems.bgName = filePath;
            SoundPlayer::getInstance()->playDecorateEffect();
            
        }else if (decorateType == "breakfast food"){
            pos = breakFast->getPosition();
            localZorder = 2;
            breakFast->setOpacity(0);
            breakFast->setTexture(filePath);
            allItems.breakfastName = filePath;
            breakFast->setPosition(startPos);
            breakFast->setZOrder(50);
            decorateItem = breakFast;
            
        }else if (decorateType == "mascot sticker"){
            pos = mascot->getPosition();
            mascot->runAction(Sequence::create(FadeOut::create(0.2), CallFunc::create([=]{
                mascot->changeItemTexture(filePath);
            }),FadeIn::create(0.3f),CallFunc::create([=]{
                if (pos != Vec2::ZERO) {
                    ParticleSystemQuad* _partilce = ParticleSystemQuad::create("decorate/starBlink.plist");
                    _partilce->setPosition(pos);
                    addChild(_partilce, 30);
                    _partilce->setAutoRemoveOnFinish(true);
                    SoundPlayer::getInstance()->playDecorateEffect();
                }
            }), NULL));
            
            allItems.mascotName = filePath;
 
        }else if (decorateType == "spoon") {
            localZorder = 10;
            pos = spoon->getPosition();
            spoon->setOpacity(0);
            spoon->changeItemTexture(filePath);
            allItems.spoonName = filePath;
            spoon->setPosition(startPos);
            spoon->setZOrder(50);
            decorateItem = spoon;
        }else if (decorateType == "cereal box"){
            pos = cerealBox->getPosition();
            cerealBox->setTexture(filePath);
            allItems.cerealName = filePath;
            if (pos != Vec2::ZERO) {
                ParticleSystemQuad* _partilce = ParticleSystemQuad::create("decorate/starBlink.plist");
                _partilce->setPosition(pos);
                addChild(_partilce, 30);
                _partilce->setAutoRemoveOnFinish(true);
                SoundPlayer::getInstance()->playDecorateEffect();
            }
        }else {
            FillMaterialModel* ptool = FillMaterialModel::create(filePath, true);
            ptool->setPosition(startPos);
            ptool->setOpacity(0);
            canEatLayer->addChild(ptool, 50);
        
            localZorder = 12;
            decorateItem = ptool;
            pos = Vec2(STVisibleRect::getCenterOfScene().x, tableMaxy-100);
        }
        if (decorateItem != nullptr) {
            log("the position us %.2f, %.2f", pos.x, pos.y);
            log("the localZorder is %d", localZorder);
            decorateItemCantBeClick = true;
            decorateItem->runAction(Sequence::create(FadeIn::create(0.05f),EaseSineInOut::create(MoveTo::create(0.5, pos)), CallFunc::create(std::bind(&Node::setLocalZOrder, decorateItem, localZorder)), CallFunc::create([=]{
                if (pos != Vec2::ZERO) {
                    ParticleSystemQuad* _partilce = ParticleSystemQuad::create("decorate/starBlink.plist");
                    _partilce->setPosition(pos);
                    addChild(_partilce, 30);
                    _partilce->setAutoRemoveOnFinish(true);
                    SoundPlayer::getInstance()->playDecorateEffect();
                    decorateItemCantBeClick = false;
                }
            }),NULL));
        }
        
    }
}

void DecorateScene::purchaseSucceed(){
    if (PurchaseManager::getInstance()->getIsDecorationUnlock() == true) {
        Vector<Node*> childrens = itemScrollView->getChildren();
        for (int i = 0; i < childrens.size(); ++i) {
            childrens.at(i)->removeChildByTag(kLockTags);
        }
    }
    itemScrollView->runAction(EaseSineInOut::create(MoveTo::create(0.3, scrollPosition1)));
}

void DecorateScene::onBannerDidLoaded(cocos2d::Ref *pref) {
    NotificationCenter::getInstance()->removeObserver(this, kDidLoadBanner);
    if (typeScrollView->getNumberOfRunningActions() != 0) {
        this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
            typeScrollView->runAction(EaseSineInOut::create(MoveBy::create(0.5, Vec2(0, GameLayerBase::getBannerSize()))));
        }), NULL));
        return;
    }
    typeScrollView->runAction(EaseSineInOut::create(MoveBy::create(0.5, Vec2(0, GameLayerBase::getBannerSize()))));
}

void DecorateScene::nextClickEvent(){
    GameLayerBase::nextClickEvent();
    allItems.alleatThings.clear();
    allItems.alleatThings.push_back(spoon->getItemThings());
    DataContainer::getInstance()->setAllDecorateItems(allItems);
    saveTheScreenShot();
    pushTheScene<ShareScene>();
    GameLayerBase::setNextButtonEnable(true);
}

void DecorateScene::preClickEvent(){
    GameLayerBase::preClickEvent();
    replaceTheScene<MixItemScene>();
    
}


void DecorateScene::resetClickEvent(){
    GameLayerBase::resetClickEvent();
    canEatLayer->removeAllChildren();
    
    allItems.bgName = "make/bg_make_table_cover.png";
    allItems.cerealName = "decorate/cereal box/cereal box1.png";
    allItems.breakfastName = "decorate/breakfast food/breakfast food1.png";
    allItems.spoonName = "decorate/spoon/spoon1.png";
    allItems.mascotName = "decorate/mascot sticker/mascot sticker1.png";
    
    breakFast->setTexture("decorate/breakfast food/breakfast food1.png");
    mascot->changeItemTexture("decorate/mascot sticker/mascot sticker1.png");
    mascot->setPosition(cerealBox->convertToWorldSpace(Vec2(195, 169)));
    mascot->setRotation(0);
    mascot->setScale(1/0.7f);
    
    cerealBox->setTexture("decorate/cereal box/cereal box1.png");
    m_pBg->setTexture("make/bg_make_table_cover.png");
    spoon->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, tableMaxy - 100));
    spoon->changeItemTexture("decorate/spoon/spoon1.png");
    spoon->setScale(1.0);
    spoon->setRotation(0);
    
}

void DecorateScene::saveTheScreenShot(){
    NotificationCenter::getInstance()->postNotification(kShotScreenEvent, __String::create("No"));
    RenderTexture* saveImageTexture = RenderTexture::create(canEatLayer->getContentSize().width, canEatLayer->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    
    saveImageTexture->begin();
    canEatLayer->visit();
    saveImageTexture->end();
    
    saveImageTexture->getSprite()->setFlippedY(false);
    saveImageTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
    Director::getInstance()->getRenderer()->render();
    
    log("the path is %s", (STFileUtility::getStoragePath()+"temp.png").c_str());
    Image* pImage = saveImageTexture->newImage();
    pImage->saveToFile(STFileUtility::getStoragePath()+"temp.png", false);
    pImage->release();
    
    saveImageTexture->beginWithClear(1.0, 1.0, 1.0, 0);
    cannotEatLayer->visit();
    saveImageTexture->end();
    
    saveImageTexture->getSprite()->setFlippedY(false);
    saveImageTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
    Director::getInstance()->getRenderer()->render();
    Image* pImage2 = saveImageTexture->newImage();
    pImage2->saveToFile(STFileUtility::getStoragePath()+"temp1.png", false);
    pImage2->release();
    
}