//
//  GameLayerBase.cpp
//  LibDoctor
//
//  Created by tanshoumei on 13-10-14.
//
//

#include "GameLayerBase.h"
#include "CocosHelper.h"
//#include "PurchaseManager.h"
#include "STAds.h"
#include "STSystemFunction.h"
#include "PurchaseManager.h"

GameLayerBase::GameLayerBase()
{
    homeBtn = nullptr;
    nextBtn = nullptr;
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayerBase::onPauseGame), kPuaseGame, nullptr);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayerBase::onResumeGame), kResumeGame, nullptr);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayerBase::onRecievePurchaseMesg), PURCHASESUCCEED, nullptr);
    
}

GameLayerBase::~GameLayerBase(){
    log("the class %s consturctor========", __FUNCTION__);
    NotificationCenter::getInstance()->removeObserver(this, kPuaseGame);
    NotificationCenter::getInstance()->removeObserver(this, kResumeGame);
    NotificationCenter::getInstance()->removeObserver(this, PURCHASESUCCEED);
}

void GameLayerBase::onRecievePurchaseMesg(cocos2d::Ref *pRef) {
    purchaseSucceed();
}

void GameLayerBase::purchaseSucceed(){
    log("the purchase has been succeed!");
}

void GameLayerBase::onPauseGame(cocos2d::Ref *ref) {
    pauseCount++;
    if (pauseCount == 1) {
        onPause();
    }
}

void GameLayerBase::onResumeGame(cocos2d::Ref *ref) {
    pauseCount -= 1;
    if (pauseCount == 0) {
        onResume();
    }
}

bool GameLayerBase::init(){
    if (CCLayer::init()) {
        
        return true;
    }
    return false;
}

bool GameLayerBase::initWithBgFileName(const char *apFileName, bool showAds /*= true*/)
{
    if(CCLayer::init())
    {
        //场景背景
        m_pBg = Sprite::create(apFileName);
        m_pBg->setPosition(STVisibleRect::getCenterOfScene());
        this->addChild(m_pBg, 0);
        
        
        isShowAds = showAds;
        if (showAds) {
            log("show the banner %s", __FUNCTION__);
        }else {
            log("donnot need show banner %s", __FUNCTION__);
        }
        
        //        this->setKeypadEnabled(true);
        return true;
    }
    return false;
}

void GameLayerBase::setShowAds(bool isShow) {
    isShowAds = isShow;
}
void GameLayerBase::onEnter(){
    KeypadBaseLayer::onEnter();
    if (isShowAds) {
        if (PurchaseManager::getInstance()->getRemoveAd() == false) {
            STAds ads;
            ads.requestAds();
            ads.setAdsVisibility(true);
        }
    }else {
        if (PurchaseManager::getInstance()->getRemoveAd() == false) {
            STAds ads;
            ads.setAdsVisibility(false);
        }
    }
}


void GameLayerBase::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
    scheduleOnce(schedule_selector(GameLayerBase::removeunuseCache), 0);
}

void GameLayerBase::removeunuseCache(float) {
     Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void GameLayerBase::showHomeButton(float dt){
    if (homeBtn != nullptr) {
        return;
    }
    homeBtn = CocosHelper::getButton("ui/publish/home.png", "ui/publish/home.png");
    homeBtn->setPosition(STVisibleRect::getPointOfSceneRightBottom() + Vec2(-20-homeBtn->getContentSize().width/2.0, homeBtn->getContentSize().height/2.0 + 15 + GameLayerBase::getBannerSize()) + Vec2(800, 0));
    homeBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameLayerBase::onHomeButtonClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    homeBtn->setZoomOnTouchDown(false);
    addChild(homeBtn, kHomeBtn);
    
    homeBtn->runAction(Sequence::create(DelayTime::create(dt), EaseElasticInOut::create(MoveBy::create(2.0, Vec2(-800, 0)), 1.7), NULL));
    
}

void GameLayerBase::showShareBtn(float dt) {
    if (shareBtn != nullptr) {
        return;
    }
    shareBtn = CocosHelper::getButton("ui/publish/btn_share.png", "ui/publish/btn_share.png");
    shareBtn->setPosition(STVisibleRect::getPointOfSceneRightUp() + Vec2(-20-shareBtn->getContentSize().width/2.0, -shareBtn->getContentSize().height/2.0 - 15 - 94 - 10) + Vec2(800, 0));
    shareBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameLayerBase::onShareButtonClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    shareBtn->setZoomOnTouchDown(false);
    addChild(shareBtn, kHomeBtn);
    
    shareBtn->runAction(Sequence::create(DelayTime::create(dt), EaseElasticInOut::create(MoveBy::create(2.0, Vec2(-800, 0)), 1.7), NULL));
}

void GameLayerBase::showFavoriteBtn(float dt) {
    if (favoriteBtn != nullptr) {
        return;
    }
    favoriteBtn = CocosHelper::getButton("ui/publish/btn_fav.png", "ui/publish/btn_fav.png");
    favoriteBtn->setPosition(STVisibleRect::getPointOfSceneRightUp() + Vec2(-20-favoriteBtn->getContentSize().width/2.0, -favoriteBtn->getContentSize().height/2.0 - 15 - 104*2) + Vec2(800, 0));
    favoriteBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameLayerBase::onFavoriteButtonClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    favoriteBtn->setZoomOnTouchDown(false);
    addChild(favoriteBtn, kHomeBtn);
    
    favoriteBtn->runAction(Sequence::create(DelayTime::create(dt), EaseElasticInOut::create(MoveBy::create(2.0, Vec2(-800, 0)), 1.7), NULL));
}

void GameLayerBase::onHomeButtonClicked(cocos2d::Ref *pRef, Control::EventType type) {
    
    if (canbeClicked == false) {
        return;
    }
    canbeClicked = false;
    ControlButton* pBtn = dynamic_cast<ControlButton*>(pRef);
    pBtn->setEnabled(false);
    SoundPlayer::getInstance()->playHomeClickedEffect();
    
    pBtn->runAction(Sequence::create((Sequence*)getJellyAction(),CallFunc::create(std::bind(&GameLayerBase::showConfirmDialog, this)), CallFunc::create([=]{
        pBtn->setEnabled(true);
    }),NULL));
    
}

void GameLayerBase::showConfirmDialog(){
    AlertViewLayer* _layer = AlertViewLayer::createWithTitle("Do you want to start over?");
    _layer->setAlertViewCallBack(CC_CALLBACK_2(GameLayerBase::alertViewClickedButtonAtIndex, this));
    _layer->setTag(kHomeDialog);
    this->addChild(_layer, kDialog);
    this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(std::bind(&GameLayerBase::onPauseGame, this, nullptr)), NULL));
}

void GameLayerBase::alertViewClickedButtonAtIndex(AlertViewLayer *alert, int index) {
    if (alert->getTag() == kHomeDialog) {
        if (index == 1) {
            canbeClicked = true;
            onResumeGame(nullptr);
        }else{
            homeClickEvent();
        }
    }else if (alert->getTag() == kBackDialog){
        if (index == 1) {
            canbeClicked = true;
        }else {
            homeClickEvent();
        }
    }
}

void GameLayerBase::homeClickEvent(){
    SoundPlayer::getInstance()->playclickeffect();
    if (PurchaseManager::getInstance()->getRemoveAd() == false) {
        STAds ads;
        ads.requestInterstitialAds(true);
    }
}

void GameLayerBase::showPreviousBtn(float dt){
    if (preBtn != nullptr) {
        return;
    }
    preBtn = CocosHelper::getButton("ui/publish/btn_back.png", "ui/publish/btn_back.png");
    //    homeBtn->setAnchorPoint(Vec2(1.0, 1.0));
    preBtn->setPosition(STVisibleRect::getPointOfSceneLeftUp() + Vec2(20+preBtn->getContentSize().width/2.0, -preBtn->getContentSize().height/2.0 - 15) + Vec2(-800, 0));
    preBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameLayerBase::onPreButtonClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    preBtn->setZoomOnTouchDown(false);
    addChild(preBtn, kHomeBtn);
    
    preBtn->runAction(Sequence::create(DelayTime::create(dt), EaseElasticInOut::create(MoveBy::create(2.0, Vec2(800, 0)), 1.7), NULL));
}

void GameLayerBase::showNextButton(float dt, bool withJump){
    if (nextBtn != nullptr) {
        return;
    }
    nextBtn = CocosHelper::getButton("ui/publish/next.png", "ui/publish/next.png");
    //    homeBtn->setAnchorPoint(Vec2(1.0, 1.0));
    nextBtn->setPosition(STVisibleRect::getPointOfSceneRightUp() + Vec2(-20-nextBtn->getContentSize().width/2.0, -nextBtn->getContentSize().height/2.0 - 15) + Vec2(800, 0));
    nextBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameLayerBase::onNextButtonClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    nextBtn->setZoomOnTouchDown(false);
    addChild(nextBtn, kHomeBtn);
    
    nextBtn->runAction(Sequence::create(DelayTime::create(dt), EaseElasticInOut::create(MoveBy::create(2.0, Vec2(-800, 0)), 1.7), NULL));
    
    if (withJump == true) {
        nextBtn->runAction(JumpBy::create(0.8, Vec2(0, 0), 20, 2));
        this->schedule(schedule_selector(GameLayerBase::nextButtonAction), 5.0f);
    }
}

void GameLayerBase::nextButtonAction(float) {
    nextBtn->runAction(JumpBy::create(0.8, Vec2(0, 0), 20, 2));
}

void GameLayerBase::showResetBtn(float dt){
    if (resetBtn != nullptr) {
        return;
    }
    resetBtn = CocosHelper::getButton("ui/publish/btn_reset.png", "ui/publish/btn_reset.png");
    //    homeBtn->setAnchorPoint(Vec2(1.0, 1.0));
    resetBtn->setPosition(STVisibleRect::getPointOfSceneLeftUp() + Vec2(20+resetBtn->getContentSize().width/2.0, -resetBtn->getContentSize().height*1.5 - 15) + Vec2(-800, 0));
    resetBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameLayerBase::onResetButtonClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    resetBtn->setZoomOnTouchDown(false);
    addChild(resetBtn, kHomeBtn);
    
    resetBtn->runAction(Sequence::create(DelayTime::create(dt), EaseElasticInOut::create(MoveBy::create(2.0, Vec2(800, 0)), 1.7), NULL));
}

void GameLayerBase::onResetButtonClicked(cocos2d::Ref *pRef, Control::EventType type) {
    if (canbeClicked == false) {
        return;
    }
    canbeClicked = true;
    //    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    ControlButton* pNode = dynamic_cast<ControlButton*>(pRef);
    if (pNode->getNumberOfRunningActions() != 0) {
        return;
    }
    pNode->setEnabled(false);
    pNode->runAction(Sequence::create((Sequence*)getJellyAction(),CallFunc::create(std::bind(&GameLayerBase::resetClickEvent, this)), NULL));
}

void GameLayerBase::resetClickEvent(){
    SoundPlayer::getInstance()->playclickeffect();
    resetBtn->setEnabled(true);
}


void GameLayerBase::onNextButtonClicked(cocos2d::Ref *pRef, Control::EventType type) {
    if (canbeClicked == false) {
        return;
    }
    canbeClicked = true;
//    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    ControlButton* pNode = dynamic_cast<ControlButton*>(pRef);
    if (pNode->getNumberOfRunningActions() != 0) {
        return;
    }
    pNode->setEnabled(false);
    pNode->runAction(Sequence::create((Sequence*)getJellyAction(),CallFunc::create(std::bind(&GameLayerBase::nextClickEvent, this)), NULL));
}

void GameLayerBase::onPreButtonClicked(cocos2d::Ref *pRef, Control::EventType type) {
    if (canbeClicked == false) {
        return;
    }
    canbeClicked = true;
    //    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    ControlButton* pNode = dynamic_cast<ControlButton*>(pRef);
    if (pNode->getNumberOfRunningActions() != 0) {
        return;
    }
    pNode->setEnabled(false);
    pNode->runAction(Sequence::create((Sequence*)getJellyAction(),CallFunc::create(std::bind(&GameLayerBase::preClickEvent, this)), NULL));
}

void GameLayerBase::onShareButtonClicked(cocos2d::Ref *pRef, Control::EventType type) {
    if (canbeClicked == false) {
        return;
    }
    canbeClicked = true;
    //    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    ControlButton* pNode = dynamic_cast<ControlButton*>(pRef);
    if (pNode->getNumberOfRunningActions() != 0) {
        return;
    }
    pNode->setEnabled(false);
    pNode->runAction(Sequence::create((Sequence*)getJellyAction(),CallFunc::create(std::bind(&GameLayerBase::shareClickEvent, this)), NULL));
}

void GameLayerBase::onFavoriteButtonClicked(cocos2d::Ref *pRef, Control::EventType type) {
    if (canbeClicked == false) {
        return;
    }
    canbeClicked = true;
    //    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    ControlButton* pNode = dynamic_cast<ControlButton*>(pRef);
    if (pNode->getNumberOfRunningActions() != 0) {
        return;
    }
    pNode->setEnabled(false);
    pNode->runAction(Sequence::create((Sequence*)getJellyAction(),CallFunc::create(std::bind(&GameLayerBase::favoriteClickEvent, this)), NULL));
}

void GameLayerBase::favoriteClickEvent(){
    SoundPlayer::getInstance()->playclickeffect();
    favoriteBtn->setEnabled(true);
}

void GameLayerBase::shareClickEvent(){
    SoundPlayer::getInstance()->playclickeffect();
    shareBtn->setEnabled(true);
}

void GameLayerBase::preClickEvent(){
    SoundPlayer::getInstance()->playclickeffect();
}

void GameLayerBase::nextClickEvent(){
    SoundPlayer::getInstance()->playclickeffect();
//    SceneManager::getInstance()->gotoChooseMap();
}

void GameLayerBase::setNextButtonEnable(bool isEnable){
    nextBtn->setEnabled(isEnable);
}

void GameLayerBase::setADVisible(bool visible) {

}

void GameLayerBase::onKeyBackClicked(){
    showConfirmDialog();
    log("点击返回键按钮！！");


}

void GameLayerBase::onExit(){
    SoundPlayer::getInstance()->stopAlleffect();
    KeypadBaseLayer::onExit();
}


void GameLayerBase::onPause()
{

    NotificationCenter::getInstance()->postNotification(kPuaseTip);
    this->pause();

}

void GameLayerBase::onResume()
{
    
    NotificationCenter::getInstance()->postNotification(kResumeTip);
    this->resume();
    
}


Action *GameLayerBase::getJellyAction()
{
    Action *ret = NULL;
    
    ScaleTo *lScale1 = ScaleTo::create(0.13, 1.0, 0.90);
    ScaleTo *lScale2 = ScaleTo::create(0.11, 0.92, 1.0);
    ScaleTo *lScale3 = ScaleTo::create(0.10, 1.0, 0.96);
    ScaleTo *lScale4 = ScaleTo::create(0.09, 0.98, 1.0);
    ScaleTo *lScale5 = ScaleTo::create(0.08, 1.0, 0.99);
    ScaleTo *lScale6 = ScaleTo::create(0.07, 0.99, 1.0);
    ret = Sequence::create(lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, NULL);
    return ret;
}

void GameLayerBase::setNextHidden(bool hidden) {
    if (nextBtn != nullptr) {
        nextBtn->setVisible(hidden);
    }
}

void GameLayerBase::setHomeHidden(bool hidden) {
    if (homeBtn != nullptr) {
        homeBtn->setVisible(hidden);
    }
}

void GameLayerBase::setallButton(bool isVisible) {
    if (nextBtn != nullptr) {
        nextBtn->setVisible(isVisible);
    }
    if (preBtn != nullptr) {
        preBtn->setVisible(isVisible);
    }
    if (homeBtn != nullptr) {
        homeBtn->setVisible(isVisible);
    }
    if (favoriteBtn != nullptr) {
        favoriteBtn->setVisible(isVisible);
    }
    if (shareBtn != nullptr) {
        shareBtn->setVisible(isVisible);
    }
}


void GameLayerBase::setNextButtonZorder(int zorder) {
    if (nextBtn != nullptr) {
        nextBtn->setLocalZOrder(zorder);
    }
}

float GameLayerBase::getBannerSize(){
    float bannerSize = 0;
    if (PurchaseManager::getInstance()->getRemoveAd() == true) {
        return 0;
    }
    STSystemFunction st;
    bool istablet = st.isTabletAvailable();
    if (istablet == false) {
        bannerSize = 50;
    }else {
        bannerSize = 90;
        
    }
    return bannerSize;
}
