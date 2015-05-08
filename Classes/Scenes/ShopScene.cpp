//
//  ShopScene.cpp
//  DONUT4013
//
//  Created by liuwei on 15/5/5.
//
//

#include "SceneHead.h"
#include "PageItem.h"
#include "PurchaseManager.h"

Scene* ShopScene::scene(){
    Scene* pScene = Scene::create();
    
    ShopScene* pLayer = ShopScene::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}

bool ShopScene::init(){
    if (GameLayerBase::initWithBgFileName("ui/shop/bg_shop1136.jpg", false)) {
        inAppIap = new STInAppBilling();
        inAppIap->setIABDelegate(this);

        Sprite* title = Sprite::create("ui/shop/shop_title.png");
        title->setPosition(STVisibleRect::getCenterOfScene().x - STVisibleRect::getGlvisibleSize().width, STVisibleRect::getPointOfSceneLeftUp().y - 100);
        addChild(title, 1);
        
        
        title->runAction(Sequence::create(DelayTime::create(1.0f), EaseElasticInOut::create(MoveBy::create(1.2, Vec2(STVisibleRect::getGlvisibleSize().width, 0)), 1.0f), NULL));
        showPreviousBtn(1.5f);
        showRestoreBtn(1.5f);
        
        showShopScrollView();
        return true;
    }
    return false;
}

void ShopScene::showShopScrollView(){
    
    vector<string> shopitems = {
        "ui/shop/shop_li_best deal.png",
        "ui/shop/shop_li_unlock all.png",
        "ui/shop/shop_li_cereals.png",
        "ui/shop/shop_li_milk.png",
        "ui/shop/shop_li_decorate.png",
        "ui/shop/shop_li_remove ad.png"
    };
    
    shopScroll = ScrollPage::create();
    shopScroll->addEventListener([this](Ref* pRef, ScrollPage::EventType type){
        if (type == ScrollPage::EventType::SCROLLING) {
            if (buyButton) {
                buyButton->setVisible(false);
            }
            Vec2 pos = shopScroll->getInnerContainer()->getPosition();
            int index = ((int)fabs(pos.x)) / ItemSpace;
            float pos1 = -1;
            float pos2 = -1;
            float selfpos = shopScroll->getChildByTag(index)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
            shopScroll->getChildByTag(index)->setScale(1.0-0.2*(fabs(selfpos)/ItemSpace));
            shopScroll->getChildByTag(index)->setPositionY(STVisibleRect::getGlvisibleSize().height/2.0+ 120*(fabs(selfpos))/ItemSpace);
            if (index - 1 >= 0) {
                pos1 = shopScroll->getChildByTag(index - 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (fabs(pos1) < ItemSpace){
                    shopScroll->getChildByTag(index - 1)->setScale(ItemScale+0.2*(ItemSpace-fabs(pos1))/ItemSpace);
                    shopScroll->getChildByTag(index - 1)->setPositionY(STVisibleRect::getGlvisibleSize().height/2.0 + + 120*(fabs(pos1))/ItemSpace);
                }
            }
            if (index + 1 < count) {
                pos2 = shopScroll->getChildByTag(index + 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (fabs(pos2) < ItemSpace){
                    shopScroll->getChildByTag(index + 1)->setScale(ItemScale+0.2*(ItemSpace-fabs(pos2))/ItemSpace);
                    shopScroll->getChildByTag(index + 1)->setPositionY(STVisibleRect::getGlvisibleSize().height/2.0 + 120*(fabs(pos2))/ItemSpace);
                }
            }
        }else if (type == ScrollPage::EventType::SCROLLEDEND){
            
            int temp = 0;
            float deltax = 0;
            Vec2 pos = shopScroll->getInnerContainer()->getPosition();
            int index = ((int)fabs(pos.x)) / ItemSpace;
            temp = index;
            deltax = 0 - (shopScroll->getChildByTag(index)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0);
            
            float scale0 = shopScroll->getChildByTag(index)->getScale();
            if (index - 1 >= 0) {
                float scale1 = shopScroll->getChildByTag(index - 1)->getScale();
                float pos1 = shopScroll->getChildByTag(index - 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (scale1 > scale0) {
                    temp = index - 1;
                    deltax = 0 - pos1;
                }
            }
            if (index + 1 < count) {
                float scale2 = shopScroll->getChildByTag(index + 1)->getScale();
                float pos2 = shopScroll->getChildByTag(index + 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (scale2 > scale0) {
                    temp = index + 1;
                    deltax = 0 - pos2;
                }
            }

            if (temp != currentIndex) {
                currentIndex = temp;
            }
            if (buyButton) {
                if ((currentIndex == 0 || currentIndex == count - 1)) {
                    buyButton->setVisible(true);
                }
                
            }
            if (fabs(deltax) > 1) {
                shopScroll->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(std::bind(&ScrollPage::scrolltoDestinationWithDelta,shopScroll, Vec2(deltax, 0), 0.5, true)),DelayTime::create(0.5f),CallFunc::create([=]{
                    if (buyButton != nullptr) {
                        buyButton->setVisible(true);
                    }else {
                        buyButton = Button::create("ui/shop/shop_btn_buy.png");
                        buyButton->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y + 150));
                        buyButton->addTouchEventListener(CC_CALLBACK_2(ShopScene::onScrollItemClick, this));
                        this->addChild(buyButton, 10);
                    }
                }), NULL));
            }
        }
        
    });
    
    
    shopScroll->setContentSize(Size(STVisibleRect::getGlvisibleSize().width, STVisibleRect::getGlvisibleSize().height));
    vector<string>::iterator it;
    float startX = STVisibleRect::getGlvisibleSize().width / 2.0;
    count = 0;
    for(it = shopitems.begin();it != shopitems.end();++it){
        Button* pButton = Button::create(*it);
        pButton->setName(IAPKEYS[count]);
        pButton->addTouchEventListener(CC_CALLBACK_2(ShopScene::onScrollItemClick, this));
        pButton->setPosition(Vec2(startX, STVisibleRect::getGlvisibleSize().height/2.0+120));
        if (count != 0) {
            pButton->setScale(ItemScale);
        }
        pButton->setEnabled(false);
        startX += pButton->getContentSize().width - 10;
        shopScroll->addChild(pButton);
        pButton->setTag(count++);
        
    }
    startX += STVisibleRect::getGlvisibleSize().width / 2.0 - ItemSpace;
    shopScroll->setDirection(ScrollPage::Direction::HORIZONTAL);
    shopScroll->setInnerContainerSize(Size(startX, STVisibleRect::getGlvisibleSize().height));
    shopScroll->setPosition(Vec2(STVisibleRect::getOriginalPoint().x, STVisibleRect::getOriginalPoint().y ));
    addChild(shopScroll, 1);
}

void ShopScene::showRestoreBtn(float dt) {
    restoreBtn = CocosHelper::getButton("ui/publish/btn_restore.png", "ui/publish/btn_restore.png");
    //    homeBtn->setAnchorPoint(Vec2(1.0, 1.0));
    restoreBtn->setPosition(STVisibleRect::getPointOfSceneRightUp() + Vec2(-20-restoreBtn->getContentSize().width/2.0, -restoreBtn->getContentSize().height/2.0 - 15) + Vec2(800, 0));
    restoreBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(ShopScene::onResetButtonClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    restoreBtn->setZoomOnTouchDown(false);
    addChild(restoreBtn, kHomeBtn);
    
    restoreBtn->runAction(Sequence::create(DelayTime::create(dt), EaseElasticInOut::create(MoveBy::create(2.0, Vec2(-800, 0)), 1.7), NULL));
}

void ShopScene::onRestoreButtonClicked(cocos2d::Ref *pRef, Control::EventType type) {
    inAppIap->restore();
}

void ShopScene::onEnterTransitionDidFinish(){
    GameLayerBase::onEnterTransitionDidFinish();
    shopScroll->scrollToPercentHorizontal(100, 1.5, true);
    
    shopScroll->stopAllActions();
    shopScroll->runAction(Sequence::create(DelayTime::create(0.8), CallFunc::create(std::bind(&ScrollPage::scrollToLeft, shopScroll, 1.5, true)),DelayTime::create(1.0f),  CallFunc::create([=]{
        buyButton = Button::create("ui/shop/shop_btn_buy.png");
        buyButton->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getOriginalPoint().y + 150));
        buyButton->addTouchEventListener(CC_CALLBACK_2(ShopScene::onScrollItemClick, this));
        this->addChild(buyButton, 10);
    }),NULL));
}

void ShopScene::onScrollItemClick(cocos2d::Ref *pRef, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        if (PurchaseManager::getInstance()->checkIsUnlock(IAPKEYS[currentIndex].c_str())) {
            showDialogView( "You have already purchased this package!","");
        }else {
            inAppIap->purchase(IAPKEYS[currentIndex].c_str());
        }

    }
}

void ShopScene::preClickEvent(){
    GameLayerBase::preClickEvent();
    Director::getInstance()->popScene();
}

void ShopScene::purchaseSuccessful(const char* pid)
{
    for (int i = 0; i < IAPCOUTNS; ++i) {
        if (IAPKEYS[i].compare(pid) == 0) {
            PurchaseManager::getInstance()->unlockByKeyString(IAPKEYS[i].c_str());
            return;
        }
    }
}

void ShopScene::purchaseFailed(const char *pid, int errorCode)
{
    log("purchase the %s failed!!!!", pid);
}

void ShopScene::restoreSuccessful(const char* pid)
{
    for (int i = 0; i < IAPCOUTNS; ++i) {
        if (IAPKEYS[i].compare(pid) == 0) {
            PurchaseManager::getInstance()->unlockByKeyString(IAPKEYS[i].c_str());
            return;
        }
    }
}

void ShopScene::restoreSuccessfulNotify()
{
    log("restoreSuccessfulNotify!!!");
}

void ShopScene::restoreFailed(const char* pid, int errorCode)
{
    log("restore the %s failed!!!!", pid);
}

void ShopScene::showDialogView(const char *content, const char *title) {;
    AlertViewLayer* _layer = AlertViewLayer::createWithTitle(content, AlertViewLayer::SINGLE);
    this->addChild(_layer, kDialog);
}


