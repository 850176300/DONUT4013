//
//  ShareFrameItem.cpp
//  DONUT4013
//
//  Created by liuwei on 5/5/15.
//
//

#include "ShareFrameItem.h"
#include "STVisibleRect.h"
USING_NS_ST;

ShareFrameItem* ShareFrameItem::create(cocos2d::Image *pImage) {
    ShareFrameItem* pRet = new ShareFrameItem();
    
    if (pRet && pRet->init(pImage)) {
        pRet->autorelease();
        return pRet;
    }else {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

bool ShareFrameItem::init(cocos2d::Image *pImage) {
    if (SwallowTouchLayout::init()) {
        setBackGroundImage("ui/mask.png");
        setBackGroundImageOpacity(150);
     
        frame = Sprite::create("ui/share/share_box.png");
        frame->setPosition(STVisibleRect::getCenterOfScene());
        addChild(frame, 1);
        
        Texture2D* ptexture = new Texture2D();
        ptexture->initWithImage(pImage);
        
        Sprite* spriteinFrame = Sprite::createWithTexture(ptexture);
        spriteinFrame->setAnchorPoint(Vec2(0, 0.5));
        spriteinFrame->setScale(0.45);
        spriteinFrame->setPosition(Vec2(15, frame->getContentSize().height/2.0));
        frame->addChild(spriteinFrame);
        
        float startx = (frame->getContentSize().width - 3 * 94.0 - 20 * 2) / 2.0;

        Button* closeBtn = Button::create("ui/share/share_btn_close.png");
        Button* emailBtn = Button::create("ui/share/share_btn_email.png");
        Button* saveBtn = Button::create("ui/share/share_btn_save.png");
        Button* deleteBtn = Button::create("ui/share/share_btn_delete.png");
        closeBtn->setPosition(Vec2(frame->getBoundingBox().getMaxX(), frame->getBoundingBox().getMaxY()));
        deleteBtn->setPosition(Vec2(20 + 94 / 2.0, frame->getBoundingBox().getMinY() + 94 / 2.0 + 20));
        emailBtn->setPosition(Vec2(94 / 2.0 + startx, 0)+deleteBtn->getPosition());
        saveBtn->setPosition(emailBtn->getPosition() + Vec2(94/2.0 + startx, 0));
        closeBtn->addTouchEventListener(CC_CALLBACK_2(ShareFrameItem::onButtonsClicked, this));
        deleteBtn->addTouchEventListener(CC_CALLBACK_2(ShareFrameItem::onButtonsClicked, this));
        emailBtn->addTouchEventListener(CC_CALLBACK_2(ShareFrameItem::onButtonsClicked, this));
        saveBtn->addTouchEventListener(CC_CALLBACK_2(ShareFrameItem::onButtonsClicked, this));
        
        addChild(closeBtn, 2);
        addChild(deleteBtn, 2);
        addChild(emailBtn, 2);
        addChild(saveBtn, 2);
        
        closeBtn->setTag(kCloseTags);
        deleteBtn->setTag(kDeleteTags);
        emailBtn->setTag(kEmailTags);
        saveBtn->setTag(kSaveTag);
        
        return true;
    }
    return false;
}


void ShareFrameItem::onButtonsClicked(cocos2d::Ref *pRef, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        Node* pNode = dynamic_cast<Node* >(pRef);
        switch (pNode->getTag()) {
            case kCloseTags:
            {
                
            }
                break;
            case kDeleteTags:
            {
                
            }
                break;
            case kEmailTags:
            {
                
            }
                break;
            case kSaveTag:
            {
                
            }
                break;
            default:
                break;
        }
    }
}
