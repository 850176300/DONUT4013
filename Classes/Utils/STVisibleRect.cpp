#include "STVisibleRect.h"
#include "STSystemFunction.h"
#include "GameLayerBase.h"
USING_NS_CC;
USING_NS_ST;

static Size c_realSize = Size(768, 1136);
static Size c_offset = Size(0, 0);

void STVisibleRect::setupVisibleRect(Size design_size)
{
    float imageWidth = 768;        //给出的背景图大小
    float imageHeight = 1136;
    Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();  //显示器framsize
    float scaleX = (float) imageWidth / frameSize.width;
    float scaleY = (float) imageHeight / frameSize.height;
    //设置缩放后的真实size（winsize大小），相当于noborder的用法
    //使背景图的最短边能填满屏幕
    if(scaleX < scaleY){
        float realHeight = imageHeight * scaleX / scaleY;
        c_realSize = Size(imageWidth, realHeight);
    }
    else if(scaleX > scaleY){
        float realWidth = imageWidth * scaleY / scaleX;
        c_realSize = Size(realWidth, imageHeight);
    }
    //设置偏移，使操作的size（960*640）在整个屏幕中
    c_offset = Size((c_realSize.width - 768) / 2, (c_realSize.height - 1136) / 2);
}


Point STVisibleRect::getPosition(Node* node, float x, float y, bool boundTop, bool boundBottom, bool boundLeft, bool boundRight) {
    float posX = x + c_offset.width;
    float posY = y + c_offset.height;
    if(boundTop){
        posY = posY + c_offset.height;
    }
    else if(boundBottom){
        posY = posY - c_offset.height;
    }
    if(boundLeft){
        posX = posX - c_offset.width;
    }
    else if(boundRight){
        posX = posX + c_offset.width;
    }
    return Vec2(posX, posY);
}
void STVisibleRect::setPosition(Node* node, float x, float y, bool boundTop, bool boundBottom, bool boundLeft, bool boundRight)
{
    float posX = x + c_offset.width;
    float posY = y + c_offset.height;
    if(boundTop){
        posY = posY + c_offset.height;
    }
    else if(boundBottom){
        posY = posY - c_offset.height;
    }
    if(boundLeft){
        posX = posX - c_offset.width;
    }
    else if(boundRight){
        posX = posX + c_offset.width;
    }
	node->setPosition(Vec2(posX, posY));
}



Size STVisibleRect::getRealDesignSize()
{
	return c_realSize;
}


Point STVisibleRect::getDesignOffset()
{
	return c_offset;
}

Point STVisibleRect::getCenterOfScene()
{
    Point oringnalP = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    return Vec2(oringnalP.x + visibleSize.width/2.0, oringnalP.y+visibleSize.height/2.0);
}

Point STVisibleRect::getPointOfSceneRightBottom()
{
    return getOriginalPoint()+Vec2(getGlvisibleSize().width, 0);
}

Point STVisibleRect::getPointOfSceneRightUp(){
    return getOriginalPoint()+Vec2(getGlvisibleSize().width, getGlvisibleSize().height);
}

Point STVisibleRect::getPointOfSceneLeftUp()
{
    return getOriginalPoint()+ Vec2(0, getGlvisibleSize().height);
}

Size STVisibleRect::getGlvisibleSize(){
    return Director::getInstance()->getVisibleSize();
}

Rect STVisibleRect::getMovableRect(){
    float bannerSize = GameLayerBase::getBannerSize();
    return Rect(Director::getInstance()->getVisibleOrigin().x, Director::getInstance()->getVisibleOrigin().y+bannerSize, Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height-bannerSize);
    
}


bool STVisibleRect::JudgeContain(cocos2d::Rect rect1/*大矩阵*/, cocos2d::Rect rect2/*小矩阵*/) {
    return !((rect1.origin.x >= rect2.origin.x) || (rect1.origin.y >= rect2.origin.y) ||
                      ( rect1.origin.x + rect1.size.width <= rect2.origin.x + rect2.size.width) ||
                    ( rect1.origin.y + rect1.size.height <= rect2.origin.y + rect2.size.height));
}

Point STVisibleRect::getOriginalPoint(){
    return Director::getInstance()->getVisibleOrigin();
}

//p2 表示转换之前的锚点，p1 应该对应的锚点
Point STVisibleRect::conformByAnchorPoint(cocos2d::Node *pNode,cocos2d::Point p1, cocos2d::Point anchorP1,Point p2/* = Vec2(0.5f, 0.5f)*/) {
    Point delta = Vec2((anchorP1.x - p2.x)*pNode->getContentSize().width, (anchorP1.y-p2.y)*pNode->getContentSize().height);
    return p1+delta;
}




