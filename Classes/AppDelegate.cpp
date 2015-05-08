#include "AppDelegate.h"
#include "SuperGlobal.h"
#include "SceneHead.h"
#include "STVisibleRect.h"
#include "FavManager.h"
#include "PurchaseManager.h"
#include "STAds.h"
#include "AdsImpl.h"
USING_NS_CC;
USING_NS_ST;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    STVisibleRect::setupVisibleRect(Size(640, 960));
    glview->setDesignResolutionSize(STVisibleRect::getRealDesignSize().width, STVisibleRect::getRealDesignSize().height, ResolutionPolicy::NO_BORDER);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FavManager::getInstance()->loadFavdata();

    PurchaseManager::getInstance()->preloadAll();
    
    STAds ads;
    ads.setAdsDelegate(AdsImpl::getInstance());
    
    replaceTheScene<LoadingScene>();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    FavManager::getInstance()->saveData();
    SoundPlayer::getInstance()->stopBackGroundMusic();
    SoundPlayer::getInstance()->pauseEffect();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    SoundPlayer::getInstance()->resumeBackGroundMusic();
    SoundPlayer::getInstance()->resumeEffect();
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    if (PurchaseManager::getInstance()->getRemoveAd() == false) {
        STAds ads;
        ads.requestInterstitialAds();
    }

}
