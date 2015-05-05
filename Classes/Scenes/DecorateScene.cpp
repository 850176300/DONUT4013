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
        table->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y+GameLayerBase::getBannerSize()));
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
       
        showNextButton(2.0f);
        showPreviousBtn(2.0f);
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
    typeScrollView = ui::ScrollView::create();
    
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
    typeScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    typeScrollView->setPosition(Vec2(STVisibleRect::getOriginalPoint().x+STVisibleRect::getGlvisibleSize().width, STVisibleRect::getOriginalPoint().y+GameLayerBase::getBannerSize()));
    addChild(typeScrollView, 10);
    
    
    
}

void DecorateScene::onEnterTransitionDidFinish(){
    GameLayerBase::onEnterTransitionDidFinish();
    NotificationCenter::getInstance()->postNotification(kShotScreenEvent, __String::create("Yes"));
    if (typeScrollView->getPosition().x > STVisibleRect::getOriginalPoint().x) {
        typeScrollView->runAction(EaseSineInOut::create(MoveBy::create(1.0, Vec2(-STVisibleRect::getGlvisibleSize().width, 0))));
        this->runAction(Sequence::create(DelayTime::create(0.8), CallFunc::create([=]{
            typeScrollView->scrollToPercentHorizontal(100, 0.8, true);
        }), DelayTime::create(1.0), CallFunc::create([=]{
            typeScrollView->scrollToPercentHorizontal(0, 0.5, true);
        }),NULL));
    }
}

void DecorateScene::ontypeItemClicked(cocos2d::Ref *pRef, Widget::TouchEventType toucht){
    if (toucht == Widget::TouchEventType::ENDED) {
        Button* pNode = dynamic_cast<Button*>(pRef);
        if (currentSelType == pNode->getName()) {
            return;
        }
        if (currentSelType != "") {
            Button* preBtn = (Button*)typeScrollView->getChildByName(currentSelType);
            preBtn->loadTextureNormal("ui/decorate/main_d.png");
        }
        pNode->loadTextureNormal("ui/decorate/main_s.png");
        currentSelType = pNode->getName();
        if (itemScrollView == nullptr) {
            frameBanner = Sprite::create("ui/decorate/sub_box.png");
            itemScrollView = ui::ScrollView::create();
            itemScrollView->setContentSize(Size(STVisibleRect::getGlvisibleSize().width, 159));
            itemScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
            itemScrollView->setPosition(Vec2::ZERO);
            frameBanner->addChild(itemScrollView);
            
            frameBanner->setAnchorPoint(Vec2(0, 0));
            frameBanner->setPosition(typeScrollView->getPosition()+Vec2(STVisibleRect::getGlvisibleSize().width, typeScrollView->getContentSize().height));
            addChild(frameBanner, 10);
            
            refreshData();
            
            frameBanner->runAction(Sequence::create(EaseSineInOut::create(MoveBy::create(0.8, Vec2(-STVisibleRect::getGlvisibleSize().width, 0))), DelayTime::create(0.0f),CallFunc::create([=]{
                itemScrollView->scrollToPercentHorizontal(60, 0.7, true);
            }), DelayTime::create(0.8), CallFunc::create([=]{
                itemScrollView->scrollToPercentHorizontal(0, 0.8, true);
            }),NULL));
        }else {
            refreshData();
        }
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
            lock->setPosition(item->getPosition());
            pBtn->addChild(lock, 2, kLockTags);
            lock->setScale(0.5f);
        }
        itemScrollView->addChild(pBtn);
    }
    itemScrollView->setInnerContainerSize(Size(startx, 159));
}

void DecorateScene::onItemsThingClicked(cocos2d::Ref *pRef, Widget::TouchEventType ttouch) {
    if (ttouch == Widget::TouchEventType::ENDED) {
        Node* pNode = dynamic_cast<Node*>(pRef);
        if (pNode->getChildByTag(kLockTags) != nullptr) {
            //goto shop layer 
            return;
        }
        string decorateType = pNode->getName();
        string filePath = "decorate/"+decorateType+"/"+decorateType+convertIntToString(pNode->getTag()) + ".png";
        if (decorateType == "bg") {
            filePath.replace(filePath.size() - 4, filePath.size() - 1, ".jpg");
            m_pBg->setTexture(filePath);
            allItems.bgName = filePath;
        }else if (decorateType == "breakfast food"){
            breakFast->setTexture(filePath);
            allItems.breakfastName = filePath;
        }else if (decorateType == "mascot sticker"){
            mascot->changeItemTexture(filePath);
            allItems.mascotName = filePath;
        }else if (decorateType == "spoon") {
            spoon->changeItemTexture(filePath);
            allItems.spoonName = filePath;
        }else if (decorateType == "cereal box"){
            cerealBox->setTexture(filePath);
            allItems.cerealName = filePath;
        }else {
            FillMaterialModel* ptool = FillMaterialModel::create(filePath);
            ptool->setPosition(STVisibleRect::getCenterOfScene().x, tableMaxy-100);
            canEatLayer->addChild(ptool, 12);
        }
    }
}

void DecorateScene::nextClickEvent(){
    GameLayerBase::nextClickEvent();
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
    
    saveImageTexture->begin();
    cannotEatLayer->visit();
    saveImageTexture->end();
    
    saveImageTexture->getSprite()->setFlippedY(false);
    saveImageTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
    Director::getInstance()->getRenderer()->render();
    Image* pImage2 = saveImageTexture->newImage();
    pImage2->saveToFile(STFileUtility::getStoragePath()+"temp1.png", false);
    pImage2->release();
    
}