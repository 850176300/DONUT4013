#ifndef _STVISIBLE_H_
#define _STVISIBLE_H_

#include "cocos2d.h"
#include "STMacro.h"

NS_ST_BEGIN
USING_NS_CC;

class STVisibleRect {
public:
	// initialization for STVisibleRect
	static void setupVisibleRect(Size designSize);
    
    static Size getRealDesignSize();

	


	// the offset between game designed size and real design size
	static Point getDesignOffset();
    
    //将给定的坐标转化成屏幕适配后的坐标
    static Point getPosition(Node* node, float x, float y, bool boundTop = false, bool boundBottom = false,bool boundLeft = false, bool boundRight = false);
    //the center coordindate of the screen
    static Point getCenterOfScene();
    static Point getPointOfSceneRightBottom();
    static Point getPointOfSceneRightUp();
    static Point getPointOfSceneLeftUp();

	// adjust Node's position(attention:top & bottom feature)
    //居右还未测
    static void setPosition(Node* node, float x, float y, bool boundTop = false, bool boundBottom = false,bool boundLeft = false, bool boundRight = false);
    
    //get cocos2dx visible size
    static Size getGlvisibleSize();
    
    //get movable Rect
    static Rect getMovableRect();
    
    //get oringinal Point
    static Point getOriginalPoint();
    //judge rect Contain other one
    static bool JudgeContain(Rect rect1, Rect rect2);
    
    //caculate point by anchor point
    static Point conformByAnchorPoint(Node *pNode ,Point p1, Point anchorP1, Point p2 = Vec2(0.5f, 0.5f));
    
    
};

NS_ST_END
#endif