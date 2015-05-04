//
//  ChooseFlavor.cpp
//  DONUT4013
//
//  Created by liuwei on 4/17/15.
//
//

#include "SceneHead.h"
#include "PageItem.h"
#include "SwallowTouchLayout.h"

Scene* ChooseFlavor::scene(){
    Scene* pScene = Scene::create();
    
    ChooseFlavor* pLayer = ChooseFlavor::create();
    
    pScene->addChild(pLayer);
    
    return pScene;
}

bool ChooseFlavor::init(){
    if (GameLayerBase::initWithBgFileName("make/bg_make_cover.png")) {

        flavorData = DataContainer::getInstance()->getFlavorData();
        DataContainer::flavorMap::iterator it = flavorData.begin();
        bg2 = Sprite::create("make/bg/bg_"+it->first+".png");
        bg2->setPosition(STVisibleRect::getCenterOfScene() + Vec2(0, 120));
        addChild(bg2, -1);

        addScrollView();
        showPreviousBtn(3.0);
        return true;
    }
    return false;
}

void ChooseFlavor::onScrollingItems(cocos2d::Vec2 offset) {
    log("the scroll vec2 is %.2f, %.2f", offset.x, offset.y);
}

void ChooseFlavor::addScrollView(){

    flavorScrollview = ScrollPage::create();
    flavorScrollview->addEventListener([this](Ref* pRef, ScrollPage::EventType type){
        if (type == ScrollPage::EventType::SCROLLING) {
            Vec2 pos = flavorScrollview->getInnerContainer()->getPosition();
            int index = ((int)fabs(pos.x)) / ItemSpace;
            float pos1 = -1;
            float pos2 = -1;
            float selfpos = flavorScrollview->getChildByTag(index)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
            flavorScrollview->getChildByTag(index)->setScale(1.0-0.2*(fabs(selfpos)/ItemSpace));
            flavorScrollview->getChildByTag(index)->setPositionY(STVisibleRect::getGlvisibleSize().height/2.0+ 120*(fabs(selfpos))/ItemSpace);
            if (index - 1 >= 0) {
                pos1 = flavorScrollview->getChildByTag(index - 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (fabs(pos1) < ItemSpace){
                    flavorScrollview->getChildByTag(index - 1)->setScale(ItemScale+0.2*(ItemSpace-fabs(pos1))/ItemSpace);
                    flavorScrollview->getChildByTag(index - 1)->setPositionY(STVisibleRect::getGlvisibleSize().height/2.0 + + 120*(fabs(pos1))/ItemSpace);
                }
            }
            if (index + 1 < count) {
                pos2 = flavorScrollview->getChildByTag(index + 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (fabs(pos2) < ItemSpace){
                    flavorScrollview->getChildByTag(index + 1)->setScale(ItemScale+0.2*(ItemSpace-fabs(pos2))/ItemSpace);
                    flavorScrollview->getChildByTag(index + 1)->setPositionY(STVisibleRect::getGlvisibleSize().height/2.0 + 120*(fabs(pos2))/ItemSpace);
                }
            }
        }else if (type == ScrollPage::EventType::SCROLLEDEND){
            
            int temp = 0;
            float deltax = 0;
            Vec2 pos = flavorScrollview->getInnerContainer()->getPosition();
            int index = ((int)fabs(pos.x)) / ItemSpace;
            temp = index;
            deltax = 0 - (flavorScrollview->getChildByTag(index)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0);
            
            float scale0 = flavorScrollview->getChildByTag(index)->getScale();
            if (index - 1 >= 0) {
                float scale1 = flavorScrollview->getChildByTag(index - 1)->getScale();
                float pos1 = flavorScrollview->getChildByTag(index - 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (scale1 > scale0) {
                    temp = index - 1;
                    deltax = 0 - pos1;
                }
            }
            if (index + 1 < count) {
                float scale2 = flavorScrollview->getChildByTag(index + 1)->getScale();
                float pos2 = flavorScrollview->getChildByTag(index + 1)->getPosition().x + pos.x - STVisibleRect::getGlvisibleSize().width/2.0;
                if (scale2 > scale0) {
                    temp = index + 1;
                    deltax = 0 - pos2;
                }
            }
            
            if (temp != currentIndex) {
                ((PageItem*)(flavorScrollview->getChildByTag(currentIndex)))->setCenterOrNot(false);
                ((PageItem*)(flavorScrollview->getChildByTag(currentIndex)))->setPressedActionEnabled(false);
                currentIndex = temp;
                ((PageItem*)(flavorScrollview->getChildByTag(currentIndex)))->setCenterOrNot(true);
                ((PageItem*)(flavorScrollview->getChildByTag(currentIndex)))->setPressedActionEnabled(true);
                this->scheduleOnce(schedule_selector(ChooseFlavor::changeBackGround), 0.5);
            }
            if (fabs(deltax) > 1) {
                flavorScrollview->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(std::bind(&ScrollPage::scrolltoDestinationWithDelta,flavorScrollview, Vec2(deltax, 0), 0.5, true)),DelayTime::create(0.5f), NULL));
            }
        }
        
    });
    
    
    flavorScrollview->setContentSize(Size(STVisibleRect::getGlvisibleSize().width, STVisibleRect::getGlvisibleSize().height));
    DataContainer::flavorMap::iterator it;
    float startX = STVisibleRect::getGlvisibleSize().width / 2.0;
    count = 0;
    for(it = flavorData.begin();it != flavorData.end();++it){
        PageItem* pButton = PageItem::create("make/flavor/"+it->first+".png");
        pButton->setName(it->first);
        pButton->addTouchEventListener(CC_CALLBACK_2(ChooseFlavor::onScrollItemClick, this));
        pButton->setPosition(Vec2(startX, STVisibleRect::getGlvisibleSize().height/2.0+120));
        if (count != 0) {
            pButton->setScale(ItemScale);
            pButton->setCenterOrNot(false);
            pButton->setPressedActionEnabled(false);
        }else {
            pButton->setCenterOrNot(true);
            pButton->setPressedActionEnabled(true);
        }
        startX += pButton->getContentSize().width + 60;
        flavorScrollview->addChild(pButton);
        pButton->setTag(count++);
        
    }
    startX += STVisibleRect::getGlvisibleSize().width / 2.0 - 40 - 346;
    flavorScrollview->setDirection(ScrollPage::Direction::HORIZONTAL);
    flavorScrollview->setInnerContainerSize(Size(startX, STVisibleRect::getGlvisibleSize().height));
    flavorScrollview->setPosition(Vec2(STVisibleRect::getOriginalPoint().x, STVisibleRect::getOriginalPoint().y ));
    addChild(flavorScrollview);
}

void ChooseFlavor::onScrollItemClick(cocos2d::Ref *pRef, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED ) {
        PageItem* pNode = dynamic_cast<PageItem*>(pRef);
        if (pNode->getCenterOrNot() == false) {
            return;
        }
        SwallowTouchLayout* maskLayout = SwallowTouchLayout::create();
        maskLayout->setPosition(STVisibleRect::getCenterOfScene());
        maskLayout->setBackGroundImage("ui/mask.png");
        maskLayout->setTouchEnabled(true);
        maskLayout->setBackGroundImageOpacity(0);
        this->addChild(maskLayout, kHomeBtn - 11);
        
        Sprite* pSprite = Sprite::create("make/flavor/"+pNode->getName()+".png");
        pSprite->setPosition(STVisibleRect::getCenterOfScene());
        this->addChild(pSprite);
        pNode->setVisible(false);
        flavorScrollview->runAction(EaseElasticInOut::create(MoveBy::create(1.0, Vec2(-1000, 0)), 0.3));
        
        pSprite->runAction(RepeatForever::create(EaseSineInOut::create(JumpBy::create(0.8, Vec2(0, 0), 150, 1))));
        pSprite->runAction(Sequence::create(DelayTime::create(2.0), CallFunc::create(std::bind(&ChooseFlavor::animalAction, this, pNode, maskLayout)), NULL));
        
    }
}

void ChooseFlavor::animalAction(cocos2d::Node *pNode, cocos2d::ui::Layout *pLayout){
    string type = flavorData[pNode->getName()].type;
    pLayout->setBackGroundImageOpacity(150);
    DataContainer::getInstance()->setChooseFlavor(pNode->getName());
    
    int max = DataContainer::getInstance()->getAnimalAnimationCount(type);
    Animation* panimation = createAnimation("animals/animation/"+type+"/"+type, 0, max);
    panimation->setLoops(-1);
    panimation->setDelayPerUnit(0.2);
    Sprite* animal = Sprite::create("animals/animation/"+type+"/"+type+convertIntToString(0)+".png");
    animal->setAnchorPoint(Vec2(0.5, 0));
    animal->setPosition(STVisibleRect::getPointOfSceneRightBottom() + Vec2(300, 20));
    addChild(animal, kHomeBtn - 10);
    
    animal->runAction(Sequence::create(EaseSineInOut::create(JumpTo::create(0.8, Vec2(STVisibleRect::getCenterOfScene().x, 20), 500, 1)), Animate::create(panimation),NULL));
    showNextButton(1.5f);
}

void ChooseFlavor::changeBackGround(float) {
    string name = flavorData[flavorScrollview->getChildByTag(currentIndex)->getName()].name;
    bg2->setTexture("make/bg/bg_"+name+".png");
}

void ChooseFlavor::onEnterTransitionDidFinish(){
    GameLayerBase::onEnterTransitionDidFinish();

    flavorScrollview->scrollToPercentHorizontal(100, 1.5, true);
    
    flavorScrollview->stopAllActions();
    flavorScrollview->runAction(Sequence::create(DelayTime::create(0.8), CallFunc::create(std::bind(&ScrollPage::scrollToLeft, flavorScrollview, 1.5, true)),DelayTime::create(1.0f), CallFunc::create([=]{
        Sprite* title = Sprite::create("ui/select/select_flavor.png");
        title->setPosition(STVisibleRect::getCenterOfScene().x, STVisibleRect::getPointOfSceneRightUp().y + 200);
        this->addChild(title);
        title->runAction(EaseElasticInOut::create(MoveBy::create(0.9, Vec2(0, -300)), 0.5));
    }), NULL));
    
}

void ChooseFlavor::nextClickEvent(){
    GameLayerBase::nextClickEvent();
    replaceTheScene<ChooseMilk>();
}

void ChooseFlavor::preClickEvent(){
    GameLayerBase::preClickEvent();
    replaceTheScene<HomeScene>();
}
