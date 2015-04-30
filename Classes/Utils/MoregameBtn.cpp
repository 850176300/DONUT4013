//
//  MoregameBtn.cpp
//  JapaneseFood
//
//  Created by youjinchao1 on 15-3-5.
//
//

#include "MoregameBtn.h"
#include "STSystemFunction.h"
#include "STAds.h"

MoregameBtn::MoregameBtn(){
    _direction = HORIZONTAL;
    _viewModel = HomeView;
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MoregameBtn::onLoadmoregameIconSucceed), kMoreGameLoadSucceed, nullptr);
}

MoregameBtn::~MoregameBtn(){
    NotificationCenter::getInstance()->removeObserver(this, kMoreGameLoadSucceed);
}

bool MoregameBtn::init(){
    if (Layer::init()) {
        Sprite* moreGameFrame = Sprite::create("ui/home/home_btn_more.png");
        container = Node::create();
        container->setContentSize(moreGameFrame->getContentSize());
        this->setContentSize(moreGameFrame->getContentSize());


        container->addChild(moreGameFrame, 10);
        moreGameFrame->setPosition(Point(moreGameFrame->getContentSize().width/2.0, moreGameFrame->getContentSize().height/2.0));
        container->setPosition(Point(0,0));

        this->addChild(container);
        return true;
    }
    return false;
}

void MoregameBtn::addtoParentLayer(cocos2d::Layer *layer) {
    if (this->getParent()) {
        log("has been added to other layer!!!");
        return;
    }
    Vec2 oringnalPP = Director::getInstance()->getVisibleOrigin();

    this->setAnchorPoint(Point(0, 0));
    this->setPosition(oringnalPP + Point(0, -20 + getBannerHeight()));
    originalPoint = this->getPosition();
    layer->addChild(this, 100);
    this->setVisible(false);
}

void MoregameBtn::startLoading(){
    MoreGameLoader::getInstance()->loadIcon();
}

void MoregameBtn::moregameClicked(cocos2d::Ref *pObj) {
    STSystemFunction st;
    st.showMoreGame();
}

void MoregameBtn::onEnter(){
   
    Layer::onEnter();
}

void MoregameBtn::onExit(){
    Layer::onExit();
}

void MoregameBtn::onLoadmoregameIconSucceed(cocos2d::Ref *pRef) {
    Image* img = dynamic_cast<Image*>(pRef);
    
    Texture2D* texture = new Texture2D();
    texture->initWithImage(img);
    Sprite* currentSprite = Sprite::createWithTexture(texture);

    Sprite* spriteGray = Sprite::createWithTexture(texture);
    spriteGray->setColor(Color3B::GRAY);

    log("==============加载moregame图标＝＝＝＝＝＝＝＝＝＝＝＝");

    MenuItemSprite* moregameBtn = MenuItemSprite::create(currentSprite, spriteGray);
    moregameBtn->setCallback(CC_CALLBACK_1(MoregameBtn::moregameClicked, this));
    moregameBtn->setAnchorPoint(Point(1.0, 1.0));
    moregameBtn->setPosition(Point(container->getContentSize().width, container->getContentSize().height));
    moregameBtn->setPosition(moregameBtn->getPosition() + Point(-25, -45));

    Menu* menu = Menu::create(moregameBtn, NULL);
    menu->setAnchorPoint(Point(0, 0));
    menu->setPosition(Point(0, 0));
    container->addChild(menu, 5);

//    img->release();
    texture->release();

    switch (_direction) {
        case HORIZONTAL:
        {
            container->runAction(Sequence::create(MoveBy::create(0, Vec2(-300, 0)),CallFunc::create([=]{
                this->setVisible(true);
            }), MoveBy::create(0.3f, Point(300, 0)),NULL));
        }
            break;
        case VERTICAL:
        {
            container->runAction(Sequence::create(MoveBy::create(0, Vec2(0, -300)),CallFunc::create([=]{
                this->setVisible(true);
            }), MoveBy::create(0.3f, Point(0, 300)),NULL));
        }
        default:
            break;
    }
}

float MoregameBtn::getBannerHeight(){
    if (_viewModel == HomeView) {
        return 0;
    }
    float size[2] = {0,0};
    STAds ads;
    ads.getBannerSize(size);
    float bannerSize = size[1];
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    STSystemFunction st;
    bool istablet = st.isTabletAvailable();
    if (istablet == true) {
        bannerSize = 50;
    }else {
        bannerSize = 90;
        
    }
#endif
    return bannerSize;
}