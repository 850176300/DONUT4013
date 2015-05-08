//
//  FavorteScene.cpp
//  DONUT4013
//
//  Created by liuwei on 5/6/15.
//
//

#include "SceneHead.h"
#include "FavManager.h"
#include "ShareScene.h"
#include "ShareFrameItem.h"

Scene* FavoriteScene::scene(){
    Scene* pScene = Scene::create();
    
    FavoriteScene* player = FavoriteScene::create();
    
    pScene->addChild(player);
    
    return pScene;
}


bool FavoriteScene::init(){
    if (GameLayerBase::initWithBgFileName("make/bg_favorite1136.jpg", false)) {
        Sprite* title = Sprite::create("ui/share/font_favorite_recipe.png");
        title->setPosition(Vec2(STVisibleRect::getCenterOfScene().x, STVisibleRect::getPointOfSceneLeftUp().y - 100) + Vec2(-STVisibleRect::getGlvisibleSize().width, 0));
        addChild(title);
        title->runAction(Sequence::create(DelayTime::create(1.0), EaseElasticInOut::create(MoveBy::create(1.5, Vec2(STVisibleRect::getGlvisibleSize().width, 0)), 1.2), NULL));
        addScrollview();
        showPreviousBtn(2.0f);
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
    pScrollView->setContentSize(Size(STVisibleRect::getGlvisibleSize().width, STVisibleRect::getGlvisibleSize().height - 150));
    int totalColums = favItems->count() / 2 + favItems->count() % 2;
    innerContentHeight = itemSpace * totalColums;
    if (innerContentHeight < pScrollView->getContentSize().height) {
        innerContentHeight = STVisibleRect::getGlvisibleSize().height - 150;
    }
    float starty = 0;
    pScrollView->getInnerContainer()->setBackGroundImageScale9Enabled(true);
    pScrollView->setInnerContainerSize(Size(STVisibleRect::getGlvisibleSize().width, innerContentHeight));
    for (int i = 0; i < totalColums; ++i) {
        int index1 = i * 2;
        int index2 = i * 2 + 1;
        float startx = (STVisibleRect::getGlvisibleSize().width - 276 * 2) / 3.0;
        FavItem* pItem = (FavItem*)favItems->getObjectAtIndex(index1);

        Button* itemBtn = Button::create(pItem->getIconSprite());
        itemBtn->setAnchorPoint(Vec2(0, 0.5f));
        itemBtn->setPosition(Vec2(startx, innerContentHeight - starty - itemBtn->getContentSize().height / 2.0 ));
        itemBtn->addTouchEventListener(CC_CALLBACK_2(FavoriteScene::onfavoriteItemClicked, this));
        pScrollView->addChild(itemBtn);
        itemBtn->setUserObject(pItem);
        itemBtn->setTag(index1);
        allItemVecPos.push_back(itemBtn->getPosition());
        if (index2 < favItems->count()) {
            FavItem* pItem2 = (FavItem*)favItems->getObjectAtIndex(index2);
            startx += itemBtn->getContentSize().width + startx;
            Button* itemBtn2 = Button::create(pItem2->getIconSprite());
            itemBtn2->setAnchorPoint(Vec2(0, 0.5f));
            itemBtn2->setPosition(Vec2(startx, innerContentHeight - starty - itemBtn2->getContentSize().height /2.0 ));
            itemBtn2->addTouchEventListener(CC_CALLBACK_2(FavoriteScene::onfavoriteItemClicked, this));
            pScrollView->addChild(itemBtn2);
            starty += itemBtn2->getContentSize().height + 20;
            itemBtn2->setUserObject(pItem2);
            allItemVecPos.push_back(itemBtn2->getPosition());
            itemBtn2->setTag(index2);
        }
    }
    pScrollView->setPosition(STVisibleRect::getOriginalPoint() + Vec2(-STVisibleRect::getGlvisibleSize().width, 0));
    pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    addChild(pScrollView);
    
    pScrollView->scrollToPercentVertical(100, 1.5, true);
    pScrollView->runAction(Sequence::create(DelayTime::create(1.5), EaseElasticInOut::create(MoveBy::create(1.2, Vec2(STVisibleRect::getGlvisibleSize().width, 0)), 1.0f), CallFunc::create([=]{
        pScrollView->scrollToPercentVertical(0, 1.0, true);
    }), NULL));
}

void FavoriteScene::onfavoriteItemClicked(cocos2d::Ref *pref, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        Node* pNode = dynamic_cast<Node*>(pref);
        FavItem* tempItem = dynamic_cast<FavItem*>(pNode->getUserObject());
        ShareFrameItem* pItem = ShareFrameItem::create(tempItem->getImage(), true);
        this->addChild(pItem, kPrompt + 10);
        pItem->setDeleteFavCallback([=]{
            int index = favItems->getIndexOfObject(tempItem);
            if (index != CC_INVALID_INDEX) {
                FavManager::getInstance()->removeFavByIndex(index);
                pScrollView->removeChild(pNode);
                Vector<Node*> childrens = pScrollView->getChildren();
                vector<Vec2>::size_type i = allItemVecPos.size() - 1;
                for (; i > index ; --i) {
                    allItemVecPos.at(i) = allItemVecPos.at(i - 1);
                }
                allItemVecPos.erase(allItemVecPos.begin()+index);
                if (favItems != nullptr) {
                    favItems->release();
                    favItems = nullptr;
                }
                favItems = FavManager::getInstance()->getAllFavs();
                favItems->retain();
//                int totalColums = favItems->count() / 2 + favItems->count() % 2;
//                innerContentHeight = itemSpace * totalColums;
//                if (innerContentHeight < pScrollView->getContentSize().height) {
//                    innerContentHeight = STVisibleRect::getGlvisibleSize().height - 150;
//                }
//                pScrollView->setInnerContainerSize(Size(STVisibleRect::getGlvisibleSize().width, innerContentHeight));
                
                for (int i = 0; i < childrens.size(); ++i) {
                    childrens.at(i)->setPosition(allItemVecPos.at(i));
                }
//                pScrollView->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
//                    pScrollView->scrollToPercentVertical(100, 1.5f, true);
//                }), NULL));
            }
        });
    }
}


void FavoriteScene::preClickEvent(){
    GameLayerBase::preClickEvent();
    Director::getInstance()->popScene();
}
