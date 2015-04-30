//
//  FillMaterialModel.cpp
//  COOK016_TacoMaker
//
//  Created by jianghai on 15-4-23.
//
//

#include "FillMaterialModel.h"
#include "CocosHelper.h"

FillMaterialModel::FillMaterialModel():
_close(nullptr),
_magnify(nullptr),
_item(nullptr),
_listener(nullptr),
_eventType(EventType::NONE),
_orignalRotate(0),
_isTipsFrameShow(true),
_enable(true),
isShotScreen(false)
{
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(FillMaterialModel::onReciveNotify), kShotScreenEvent, nullptr);
}

FillMaterialModel::~FillMaterialModel()
{
    NotificationCenter::getInstance()->removeObserver(this, kShotScreenEvent);
}

FillMaterialModel* FillMaterialModel::create(const string& ImageName)
{
    auto pRet = new FillMaterialModel();
    if(pRet->init(ImageName))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool FillMaterialModel::init(const string& ImageName)
{
    if ( !Sprite::initWithFile("ui/decorate/controlTip_bg.png"))
    {
        return false;
    }
    
    _resourceName = ImageName;
    
    _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(FillMaterialModel::onTouchBegan, this);
    _listener->onTouchMoved = CC_CALLBACK_2(FillMaterialModel::onTouchMoved, this);
    _listener->onTouchEnded = CC_CALLBACK_2(FillMaterialModel::onTouchEnded, this);
    _listener->onTouchCancelled = CC_CALLBACK_2(FillMaterialModel::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    _close = CocosHelper::getButton("ui/decorate/controlTip_close.png","ui/decorate/controlTip_close.png");
    _close->addTargetWithActionForControlEvents(this, cccontrol_selector(FillMaterialModel::deleteSelf), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    _magnify = Sprite::create("ui/decorate/controlTip_magnify.png");
    _item = Sprite::create(ImageName);
    
    _close->setPosition(0, 254);
    _magnify->setPosition(254, 0);
    _item->setPosition(127, 127);
    
    this->addChild(_item);
    this->addChild(_close);
    this->addChild(_magnify);
    
    return true;
}

void FillMaterialModel::removeCloseBtn(){
    _close->setOpacity(0);
    _close->setEnabled(false);
}

void FillMaterialModel::changeItemTexture(const string &filename){
    _resourceName = filename;
    _item->setTexture(filename);
}

void FillMaterialModel::deleteSelf(cocos2d::Ref *sender, Control::EventType type)
{
    this->removeFromParent();
}


void FillMaterialModel::onEnter()
{
    scheduleOnce(schedule_selector(FillMaterialModel::updateTipsFrameStatus), 1.5f);
    Sprite::onEnter();
}
void FillMaterialModel::onExit()
{
    isShotScreen = true;
    Sprite::onExit();
}

bool FillMaterialModel::onTouchBegan(Touch *touch, Event *unused_event)
{
    if(!_enable)
        return false;
    Point po = this->convertToNodeSpace(touch->getLocation());
    Rect rec = Rect::ZERO;
    if (_isTipsFrameShow == true) {
        rec = Rect(-27, -27, this->getContentSize().width + 54, this->getContentSize().height+ 54);
    }else {
        rec = _item->getBoundingBox();
    }
    
    if(!rec.containsPoint(po))
        return false;
    unschedule(schedule_selector(FillMaterialModel::updateTipsFrameStatus));
    _isTipsFrameShow = !_isTipsFrameShow;
    if(_isTipsFrameShow)
    {
        _close->setVisible(true);
        _magnify->setVisible(true);
        this->setSpriteFrame(Sprite::create("ui/decorate/controlTip_bg.png")->getSpriteFrame());
        _isTipsFrameShow = true;
    }
    this->getParent()->reorderChild(this, this->getLocalZOrder() + 50);
    
    if(!_magnify->getBoundingBox().containsPoint(po))
        _eventType = MOVE;
    _orignalRotate = this->getRotation();
    return true;
}
void FillMaterialModel::onTouchMoved(Touch *touch, Event *unused_event)
{
    _isTipsFrameShow = true;
    Point po = touch->getLocation();

    if (_eventType == NONE)
    {
        _eventType = SCALE;
    }
    
    
    switch (_eventType) {
        case NONE:
            break;
        case SCALE:
        {
            
            
            float distance = po.distance(this->getPosition());
            float orignalLengh = sqrt(127*127*2);
            float m_Scale = distance/orignalLengh;
            this->setScale(m_Scale);
            
            if(m_Scale < 0.5){
                m_Scale = 0.5f;
                this->setScale(0.5);
                this->setPosition(this->getPosition()+touch->getDelta());
            }else if(m_Scale >= 1.5){
                m_Scale = 1.5f;
                this->setScale(1.5);
                this->setPosition(this->getPosition()+touch->getDelta());
            }else {
                Vec2 temp1 = touch->getStartLocation() - this->getPosition();
                Vec2 temp2 = po - this->getPosition();
                float degree = temp1.getAngle(temp2);
                float theAngle = CC_RADIANS_TO_DEGREES(degree);
                this->setRotation(_orignalRotate-theAngle);
            }
            _close->setScale(1/m_Scale);
            _magnify->setScale(1/m_Scale);
        }
            break;
        case ROTATE:
        {
            Vec2 temp1 = touch->getStartLocation() - this->getPosition();
            Vec2 temp2 = po - this->getPosition();
            float theAngle = CC_RADIANS_TO_DEGREES(temp1.getAngle(temp2));
            this->setRotation(_orignalRotate-theAngle);
        }
            break;
        case MOVE:
            this->setPosition(this->getPosition()+touch->getDelta());
            break;
        default:
            break;
    }
}
void FillMaterialModel::onTouchEnded(Touch *touch, Event *unused_event)
{
    if (_isTipsFrameShow == false) {
        updateTipsFrameStatus(0);
    }else {
        scheduleOnce(schedule_selector(FillMaterialModel::updateTipsFrameStatus), 1.5f);
    }
    _eventType = NONE;
    this->getParent()->reorderChild(this, this->getLocalZOrder() - 50);
}
void FillMaterialModel::onTouchCancelled(Touch *touch, Event *unused_event)
{
    onTouchEnded(touch, unused_event);
}

void FillMaterialModel::updateTipsFrameStatus(float)
{
    _close->setVisible(false);
    _magnify->setVisible(false);
    this->setSpriteFrame(Sprite::create("ui/decorate/controlTip_bg_Clear.png")->getSpriteFrame());
    _isTipsFrameShow = false;
}

void FillMaterialModel::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags){
    if (isShotScreen == true) {
        updateTipsFrameStatus(0);
    }
    Sprite::visit(renderer, parentTransform, parentFlags);
}

void FillMaterialModel::onReciveNotify(cocos2d::Ref *pRef) {
    isShotScreen = true;
}