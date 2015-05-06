//
//  ShareFrameItem.cpp
//  DONUT4013
//
//  Created by liuwei on 5/5/15.
//
//

#include "ShareFrameItem.h"
#include "STVisibleRect.h"
#include "STSystemFunction.h"
#include "FavManager.h"
#include "STFileUtility.h"
#include "AlumnAdapter.h"
USING_NS_ST;

ShareFrameItem* ShareFrameItem::create(cocos2d::Image *pImage, bool withDelete) {
    ShareFrameItem* pRet = new ShareFrameItem();
    
    if (pRet && pRet->init(pImage, withDelete)) {
        pRet->autorelease();
        return pRet;
    }else {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

bool ShareFrameItem::init(cocos2d::Image *pImage, bool withDelete) {
    if (Layout::init()) {
        SwallowTouchLayout* pLayout = SwallowTouchLayout::create();
        pLayout->setBackGroundImage("ui/mask.png");
        pLayout->setBackGroundImageOpacity(200);
        pLayout->setTouchEnabled(true);
        pLayout->setPosition(STVisibleRect::getCenterOfScene());
        addChild(pLayout, 0);
        
        
        shareImage = pImage;
        shareImage->retain();
        
        
     
        frame = Sprite::create("ui/share/share_box.png");
        frame->setPosition(STVisibleRect::getCenterOfScene());
        addChild(frame, 1);
        
        Texture2D* ptexture = new Texture2D();
        ptexture->initWithImage(pImage);
        
        Sprite* spriteinFrame = Sprite::createWithTexture(ptexture);
        spriteinFrame->setAnchorPoint(Vec2(0.5, 0.5));
        spriteinFrame->setScale(0.8f);
        spriteinFrame->setPosition(Vec2(frame->getContentSize().width/2.0, frame->getContentSize().height/2.0 + 35));
        frame->addChild(spriteinFrame);

        frame->setScale(0);
        
        Sprite* clip = Sprite::create("ui/share/icon_clip.png");
        clip->setPosition(Vec2(399, 682));
        frame->addChild(clip, 1);
        hasDelete = withDelete;
        return true;
    }
    return false;
}

void ShareFrameItem::onEnter(){
    Layout::onEnter();
    
    Button* closeBtn = Button::create("ui/share/share_btn_close.png");
    Button* emailBtn = Button::create("ui/share/share_btn_email.png");
    Button* saveBtn = Button::create("ui/share/share_btn_save.png");
    Button* deleteBtn = Button::create("ui/share/share_btn_delete.png");
    closeBtn->setPosition(Vec2(frame->getContentSize().width, frame->getContentSize().height));
    deleteBtn->setAnchorPoint(Vec2(0, 0));
    deleteBtn->setPosition(Vec2(20, 25));
    emailBtn->setAnchorPoint(Vec2(0.5, 0));
    emailBtn->setPosition(Vec2(frame->getContentSize().width/ 2.0 , 25));
    saveBtn->setAnchorPoint(Vec2(1.0, 0));
    saveBtn->setPosition(Vec2(frame->getContentSize().width-20, 25));
    closeBtn->addTouchEventListener(CC_CALLBACK_2(ShareFrameItem::onButtonsClicked, this));
    deleteBtn->addTouchEventListener(CC_CALLBACK_2(ShareFrameItem::onButtonsClicked, this));
    emailBtn->addTouchEventListener(CC_CALLBACK_2(ShareFrameItem::onButtonsClicked, this));
    saveBtn->addTouchEventListener(CC_CALLBACK_2(ShareFrameItem::onButtonsClicked, this));
    
    frame->addChild(closeBtn, 2);
    frame->addChild(deleteBtn, 2);
    frame->addChild(emailBtn, 2);
    frame->addChild(saveBtn, 2);
    
    closeBtn->setTag(kCloseTags);
    deleteBtn->setTag(kDeleteTags);
    emailBtn->setTag(kEmailTags);
    saveBtn->setTag(kSaveTag);
    
    if (hasDelete == false) {
        
        emailBtn->setPosition(deleteBtn->getPosition() + Vec2(94/2.0, 0));
        deleteBtn->removeFromParent();
        deleteBtn = nullptr;
    }

}

void ShareFrameItem::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
    frame->runAction(EaseExponentialInOut::create(ScaleTo::create(1.0, 1.0)));
}

void ShareFrameItem::onButtonsClicked(cocos2d::Ref *pRef, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        Node* pNode = dynamic_cast<Node* >(pRef);
        switch (pNode->getTag()) {
            case kCloseTags:
            {
                frame->runAction(Sequence::create(EaseElasticInOut::create(ScaleTo::create(0.5, 0), 0.2), CallFunc::create([=]{
                    this->removeFromParent();
                }),NULL));
            }
                break;
            case kDeleteTags:
            {
                
            }
                break;
            case kEmailTags:
            {
                STSystemFunction st;
                st.sendEmailAndFilePic("Cereal Maker", "Fantastic. I just made the delicious Cereal. Download this app, Let's have fun playing together.", STFileUtility::getStoragePath()+"shotscreen.png");
            }
                break;
            case kSaveTag:
            {
                AlumnAdapter::saveToAlumn(shareImage);
            }
                break;
            default:
                break;
        }
    }
}
