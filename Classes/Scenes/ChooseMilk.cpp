//
//  ChooseMilk.cpp
//  DONUT4013
//
//  Created by liuwei on 4/21/15.
//
//

#include "SceneHead.h"
#include "PageItem.h"

Scene* ChooseMilk::scene(){
    Scene* pScene = Scene::create();
    
    ChooseMilk* pLayer = ChooseMilk::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}


bool ChooseMilk::init(){
    if (GameLayerBase::initWithBgFileName("make/bg_make_table_cover.png")) {
        DataContainer::FlavorInfor info = DataContainer::getInstance()->getTheFlavorByName(DataContainer::getInstance()->getChooseFlavor());
        
        Sprite* bg2 = Sprite::create("make/bg/tablebg_"+info.name+".png");
        bg2->setPosition(STVisibleRect::getCenterOfScene() + Vec2(0, 120));
        addChild(bg2, -1);
        
        addScrollView();
        showPreviousBtn(3.0f);
        return true;
    }
    return false;
}


void ChooseMilk::addScrollView(){
    milkScrollview = ScrollPage::create();
    milkScrollview->addEventListener([this](Ref* pRef, ScrollPage::EventType type){
        if (type == ScrollPage::EventType::SCROLLING) {
            Vec2 pos = milkScrollview->getInnerContainer()->getPosition();
            int index = ((int)fabs(pos.x)) / ItemSpace;
            float pos1 = -1;
            float pos2 = -1;
            float selfpos = milkScrollview->getChildByTag(index)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
            milkScrollview->getChildByTag(index)->setScale(1.0-0.2*(fabs(selfpos)/ItemSpace));
            milkScrollview->getChildByTag(index)->setPositionY(STVisibleRect::getGlvisibleSize().height/2.0+ 120*(fabs(selfpos))/ItemSpace);
            if (index - 1 >= 0) {
                pos1 = milkScrollview->getChildByTag(index - 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (fabs(pos1) < ItemSpace){
                    milkScrollview->getChildByTag(index - 1)->setScale(ItemScale+0.2*(ItemSpace-fabs(pos1))/ItemSpace);
                    milkScrollview->getChildByTag(index - 1)->setPositionY(STVisibleRect::getGlvisibleSize().height/2.0 + + 120*(fabs(pos1))/ItemSpace);
                }
            }
            if (index + 1 < count) {
                pos2 = milkScrollview->getChildByTag(index + 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (fabs(pos2) < ItemSpace){
                    milkScrollview->getChildByTag(index + 1)->setScale(ItemScale+0.2*(ItemSpace-fabs(pos2))/ItemSpace);
                    milkScrollview->getChildByTag(index + 1)->setPositionY(STVisibleRect::getGlvisibleSize().height/2.0 + 120*(fabs(pos2))/ItemSpace);
                }
            }
        }else if (type == ScrollPage::EventType::SCROLLEDEND){
            
            int temp = 0;
            float deltax = 0;
            Vec2 pos = milkScrollview->getInnerContainer()->getPosition();
            int index = ((int)fabs(pos.x)) / ItemSpace;
            temp = index;
            deltax = 0 - (milkScrollview->getChildByTag(index)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0);
            
            float scale0 = milkScrollview->getChildByTag(index)->getScale();
            if (index - 1 >= 0) {
                float scale1 = milkScrollview->getChildByTag(index - 1)->getScale();
                float pos1 = milkScrollview->getChildByTag(index - 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (scale1 > scale0) {
                    temp = index - 1;
                    deltax = 0 - pos1;
                }
            }
            if (index + 1 < count) {
                float scale2 = milkScrollview->getChildByTag(index + 1)->getScale();
                float pos2 = milkScrollview->getChildByTag(index + 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (scale2 > scale0) {
                    temp = index + 1;
                    deltax = 0 - pos2;
                }
            }
            
            if (temp != currentIndex) {
                ((PageItem*)(milkScrollview->getChildByTag(currentIndex)))->setCenterOrNot(false);
                ((PageItem*)(milkScrollview->getChildByTag(currentIndex)))->setPressedActionEnabled(false);
                currentIndex = temp;
                ((PageItem*)(milkScrollview->getChildByTag(currentIndex)))->setCenterOrNot(true);
                ((PageItem*)(milkScrollview->getChildByTag(currentIndex)))->setPressedActionEnabled(true);
            }
            if (fabs(deltax) > 1) {
                milkScrollview->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(std::bind(&ScrollPage::scrolltoDestinationWithDelta,milkScrollview, Vec2(deltax, 0), 0.5, true)),DelayTime::create(0.5f), NULL));
            }
        }
        
    });
    
    milkScrollview->setContentSize(Size(STVisibleRect::getGlvisibleSize().width, STVisibleRect::getGlvisibleSize().height));
    DataContainer::milkMap::iterator it;
    float startX = STVisibleRect::getGlvisibleSize().width / 2.0;
    count = 0;
    DataContainer::milkMap milkData = DataContainer::getInstance()->getallMilkData();
    for(it = milkData.begin();it != milkData.end();++it){
        //        Vec3 vec = it->second;
        //        cout<<it->first<<"====("<<vec.x << ","<<vec.y<<","<<vec.z<<")"<<endl;
        PageItem* pButton = PageItem::create("make/milk/"+it->first+".png");
        pButton->setScale9Enabled(true);
        pButton->setName(it->first);
        pButton->addTouchEventListener(CC_CALLBACK_2(ChooseMilk::onScrollItemClick, this));
        pButton->setPosition(Vec2(startX, STVisibleRect::getGlvisibleSize().height/2.0+120));
        if (count != 0) {
            pButton->setScale(ItemScale);
            pButton->setCenterOrNot(false);
            pButton->setPressedActionEnabled(false);
        }else {
            pButton->setCenterOrNot(true);
            pButton->setPressedActionEnabled(true);
            pButton->setPositionY(STVisibleRect::getGlvisibleSize().height/2.0);
        }
        startX += pButton->getContentSize().width + 40;
        milkScrollview->addChild(pButton);
        pButton->setTag(count++);
        
        DataContainer::MilkInfor _infor = it->second;
        if (_infor.isFree == false) {
            Sprite* lock = Sprite::create("ui/select/lock.png");
            lock->setAnchorPoint(Vec2(1.0, 0));
            lock->setPosition(Vec2(pButton->getContentSize().width-15, 15));
            lock->setTag(kLockTags);
            pButton->addChild(lock);
        }
        
    }
    startX += STVisibleRect::getGlvisibleSize().width / 2.0 - ItemSpace;
    milkScrollview->setDirection(ScrollPage::Direction::HORIZONTAL);
    milkScrollview->setInnerContainerSize(Size(startX, STVisibleRect::getGlvisibleSize().height));
    milkScrollview->setPosition(Vec2(STVisibleRect::getOriginalPoint().x, STVisibleRect::getOriginalPoint().y ));
    addChild(milkScrollview);
}

void ChooseMilk::onEnterTransitionDidFinish(){
    GameLayerBase::onEnterTransitionDidFinish();
    if (title != nullptr) {
        return;
    }
    
    milkScrollview->scrollToPercentHorizontal(100, 2.5f, true);
    
    milkScrollview->stopAllActions();
    milkScrollview->runAction(Sequence::create(DelayTime::create(2.1), CallFunc::create(std::bind(&ScrollPage::scrollToLeft, milkScrollview, 3.0, true)),DelayTime::create(2.5f), CallFunc::create([=]{
        title = Sprite::create("ui/select/select_milk.png");
        title->setPosition(STVisibleRect::getCenterOfScene().x, STVisibleRect::getPointOfSceneRightUp().y + 200);
        this->addChild(title);
        SoundPlayer::getInstance()->playShowTipEffect();
        title->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(0.9, Vec2(0, -300)), 0.5), DelayTime::create(0.5f),Sequence::create(EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))), EaseSineInOut::create(MoveBy::create(0.2, Vec2(0, -20))), EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))),EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))), EaseSineInOut::create(MoveBy::create(0.2, Vec2(0, -20))), EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))), NULL),nullptr));
        
        this->schedule(schedule_selector(ChooseMilk::shakeTheTitle), 5.0f);
    }), NULL));
}

void ChooseMilk::shakeTheTitle(float) {
    title->runAction(Sequence::create(EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))), EaseSineInOut::create(MoveBy::create(0.2, Vec2(0, -20))), EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))),EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))), EaseSineInOut::create(MoveBy::create(0.2, Vec2(0, -20))), EaseSineInOut::create(MoveBy::create(0.1, Vec2(0, 10))), NULL));
}


void ChooseMilk::purchaseSucceed(){
    if (PurchaseManager::getInstance()->getIsFlavorUnlock() == true) {
        Vector<Node*> childrens = milkScrollview->getChildren();
        for (int i = 0; i < childrens.size(); ++i) {
            childrens.at(i)->removeChildByTag(kLockTags);
        }
    }
}

void ChooseMilk::onScrollItemClick(cocos2d::Ref *pref, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED){
        PageItem* pNode = dynamic_cast<PageItem*>(pref);
        if (pNode->getCenterOrNot() == false) {
            return;
        }
        if (pNode->getChildByTag(kLockTags) != nullptr) {
            pushTheScene<ShopScene>();
        }else {
            unschedule(schedule_selector(ChooseMilk::shakeTheTitle));
            DataContainer::getInstance()->setChooseMilk(pNode->getName());
            SoundPlayer::getInstance()->playChooseEffect();
            Sprite* pSprite = Sprite::create("make/milk/"+pNode->getName()+".png");
            pSprite->setPosition(STVisibleRect::getCenterOfScene());
            this->addChild(pSprite);
            pNode->setVisible(false);
            milkScrollview->runAction(Sequence::create(EaseElasticInOut::create(MoveBy::create(1.0, Vec2(-1000, 0)), 0.3), DelayTime::create(1.5f), NULL));
            pSprite->runAction(RepeatForever::create(EaseSineInOut::create(JumpBy::create(0.8, Vec2(0, 0), 150, 1))));
            showNextButton(1.0f);
        }
        

    }
}

void ChooseMilk::nextClickEvent(){
    GameLayerBase::nextClickEvent();
    replaceTheScene<MixItemScene>();
}

void ChooseMilk::preClickEvent(){
    GameLayerBase::preClickEvent();
    replaceTheScene<ChooseFlavor>();
}
