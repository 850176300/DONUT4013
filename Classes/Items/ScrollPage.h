//
//  ScrollPage.h
//  DONUT4013
//
//  Created by liuwei on 4/17/15.
//
//

#ifndef __DONUT4013__ScrollPage__
#define __DONUT4013__ScrollPage__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

typedef enum
{
    ScrollPage_EVENT_SCROLL_TO_TOP,
    ScrollPage_EVENT_SCROLL_TO_BOTTOM,
    ScrollPage_EVENT_SCROLL_TO_LEFT,
    ScrollPage_EVENT_SCROLL_TO_RIGHT,
    ScrollPage_EVENT_SCROLLING,
    ScrollPage_EVENT_BOUNCE_TOP,
    ScrollPage_EVENT_BOUNCE_BOTTOM,
    ScrollPage_EVENT_BOUNCE_LEFT,
    ScrollPage_EVENT_BOUNCE_RIGHT
}ScrollPageEventType;

typedef void (Ref::*SEL_ScrollPageEvent)(Ref*, ScrollPageEventType);
#define ScrollPageeventselector(_SELECTOR) (SEL_ScrollPageEvent)(&_SELECTOR)


class ScrollPage : public Layout
{
    
    DECLARE_CLASS_GUI_INFO
    
public:
    enum class Direction
    {
        NONE,
        VERTICAL,
        HORIZONTAL,
        BOTH
    };
    
    enum class EventType
    {
        SCROLL_TO_TOP,
        SCROLL_TO_BOTTOM,
        SCROLL_TO_LEFT,
        SCROLL_TO_RIGHT,
        SCROLLING,
        SCROLLEDEND,
        BOUNCE_TOP,
        BOUNCE_BOTTOM,
        BOUNCE_LEFT,
        BOUNCE_RIGHT
    };
    typedef std::function<void(Ref*, EventType)> ccScrollPageCallback;
    
    /**
     * Default constructor
     */
    ScrollPage();
    
    /**
     * Default destructor
     */
    virtual ~ScrollPage();
    /**
     * Allocates and initializes.
     */
    static ScrollPage* create();
    
    /**
     * Changes scroll direction of ScrollPage.
     *
     * @see Direction      Direction::VERTICAL means vertical scroll, Direction::HORIZONTAL means horizontal scroll
     *
     * @param dir
     */
    virtual void setDirection(Direction dir);
    
    /**
     * Gets scroll direction of ScrollPage.
     *
     * @see Direction      Direction::VERTICAL means vertical scroll, Direction::HORIZONTAL means horizontal scroll
     *
     * @return Direction
     */
    Direction getDirection()const;
    
    /**
     * Gets inner container of ScrollPage.
     *
     * Inner container is the container of ScrollPage's children.
     *
     * @return inner container.
     */
    Layout* getInnerContainer()const;
    
    /**
     * Scroll inner container to bottom boundary of ScrollPage.
     */
    void scrollToBottom(float time, bool attenuated);
    
    /**
     * Scroll inner container to top boundary of ScrollPage.
     */
    void scrollToTop(float time, bool attenuated);
    
    /**
     * Scroll inner container to left boundary of ScrollPage.
     */
    void scrollToLeft(float time, bool attenuated);
    
    /**
     * Scroll inner container to right boundary of ScrollPage.
     */
    void scrollToRight(float time, bool attenuated);
    
    /**
     * Scroll inner container to top and left boundary of ScrollPage.
     */
    void scrollToTopLeft(float time, bool attenuated);
    
    /**
     * Scroll inner container to top and right boundary of ScrollPage.
     */
    void scrollToTopRight(float time, bool attenuated);
    
    /**
     * Scroll inner container to bottom and left boundary of ScrollPage.
     */
    void scrollToBottomLeft(float time, bool attenuated);
    
    /**
     * Scroll inner container to bottom and right boundary of ScrollPage.
     */
    void scrollToBottomRight(float time, bool attenuated);
    
    /**
     * Scroll inner container to vertical percent position of ScrollPage.
     */
    void scrollToPercentVertical(float percent, float time, bool attenuated);
    
    /**
     * Scroll inner container to horizontal percent position of ScrollPage.
     */
    void scrollToPercentHorizontal(float percent, float time, bool attenuated);
    
    /**
     * Scroll inner container to both direction percent position of ScrollPage.
     */
    void scrollToPercentBothDirection(const Vec2& percent, float time, bool attenuated);
    
    /**
     * Move inner container to bottom boundary of ScrollPage.
     */
    void jumpToBottom();
    
    /**
     * Move inner container to top boundary of ScrollPage.
     */
    void jumpToTop();
    
    /**
     * Move inner container to left boundary of ScrollPage.
     */
    void jumpToLeft();
    
    /**
     * Move inner container to right boundary of ScrollPage.
     */
    void jumpToRight();
    
    /**
     * Move inner container to top and left boundary of ScrollPage.
     */
    void jumpToTopLeft();
    
    /**
     * Move inner container to top and right boundary of ScrollPage.
     */
    void jumpToTopRight();
    
    /**
     * Move inner container to bottom and left boundary of ScrollPage.
     */
    void jumpToBottomLeft();
    
    /**
     * Move inner container to bottom and right boundary of ScrollPage.
     */
    void jumpToBottomRight();
    
    /**
     * Move inner container to vertical percent position of ScrollPage.
     */
    void jumpToPercentVertical(float percent);
    
    /**
     * Move inner container to horizontal percent position of ScrollPage.
     */
    void jumpToPercentHorizontal(float percent);
    
    /**
     * Move inner container to both direction percent position of ScrollPage.
     */
    void jumpToPercentBothDirection(const Vec2& percent);
    
    /**
     * Changes inner container size of ScrollPage.
     *
     * Inner container size must be larger than or equal ScrollPage's size.
     *
     * @param inner container size.
     */
    void setInnerContainerSize(const Size &size);
    
    /**
     * Gets inner container size of ScrollPage.
     *
     * Inner container size must be larger than or equal ScrollPage's size.
     *
     * @return inner container size.
     */
    const Size& getInnerContainerSize() const;
    
    /**
     * Add call back function called ScrollPage event triggered
     */
    CC_DEPRECATED_ATTRIBUTE void addEventListenerScrollPage(Ref* target, SEL_ScrollPageEvent selector);
    virtual void addEventListener(const ccScrollPageCallback& callback);
    
    //all of these functions are related to innerContainer.
    virtual void addChild(Node* child)override;
    virtual void addChild(Node * child, int localZOrder)override;
    virtual void addChild(Node* child, int zOrder, int tag) override;
    virtual void addChild(Node* child, int zOrder, const std::string &name) override;
    
    virtual void removeAllChildren() override;
    virtual void removeAllChildrenWithCleanup(bool cleanup) override;
    virtual void removeChild(Node* child, bool cleaup = true) override;
    
    virtual Vector<Node*>& getChildren() override;
    virtual const Vector<Node*>& getChildren() const override;
    virtual ssize_t getChildrenCount() const override;
    virtual Node * getChildByTag(int tag) const override;
    virtual Node* getChildByName(const std::string& name)const override;
    
    //handle touch event
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) override;
    
    virtual void update(float dt) override;
    
    void setBounceEnabled(bool enabled);
    
    bool isBounceEnabled() const;
    
    void setInertiaScrollEnabled(bool enabled);
    
    bool isInertiaScrollEnabled() const;
    
    /**
     * Sets LayoutType.
     *
     * @see LayoutType
     *
     * @param LayoutType
     */
    virtual void setLayoutType(Type type) override;
    
    /**
     * Gets LayoutType.
     *
     * @see LayoutType
     *
     * @return LayoutType
     */
    virtual Type getLayoutType() const override;
    
    /**
     * Returns the "class name" of widget.
     */
    virtual std::string getDescription() const override;
    
    virtual void onEnter() override;
    virtual void onExit() override;
    /**
     *  When a widget is in a layout, you could call this method to get the next focused widget within a specified direction.
     *  If the widget is not in a layout, it will return itself
     *@param dir the direction to look for the next focused widget in a layout
     *@param current  the current focused widget
     *@return the next focused widget in a layout
     */
    virtual Widget* findNextFocusedWidget(FocusDirection direction, Widget* current) override;
    
    virtual bool scrollChildren(float touchOffsetX, float touchOffsetY);
    
    Vec2 getHorizontalOffset();
    
    void scrolltoDestinationWithDelta(Vec2 delta, float duration, bool attenuated);
    
CC_CONSTRUCTOR_ACCESS:
    virtual bool init() override;
    
protected:
    virtual void initRenderer() override;
    
    virtual void onSizeChanged() override;
    virtual void doLayout() override;
    
    virtual Widget* createCloneInstance() override;
    virtual void copySpecialProperties(Widget* model) override;
    virtual void copyClonedWidgetChildren(Widget* model) override;
    
    
    void moveChildren(float offsetX, float offsetY);
    void autoScrollChildren(float dt);
    void bounceChildren(float dt);
    void checkBounceBoundary();
    bool checkNeedBounce();
    void startAutoScrollChildrenWithOriginalSpeed(const Vec2& dir, float v, bool attenuated, float acceleration);
    void startAutoScrollChildrenWithDestination(const Vec2& des, float time, bool attenuated);
    void jumpToDestination(const Vec2& des);
    void stopAutoScrollChildren();
    void startBounceChildren(float v);
    void stopBounceChildren();
    bool checkCustomScrollDestination(float* touchOffsetX, float* touchOffsetY);
    
    
    
    bool scrollChildrenVertical(float touchOffsetX, float touchOffsetY);
    bool scrollChildrenHorizontal(float touchOffsetX, float touchOffestY);
    bool scrollChildrenBoth(float touchOffsetX, float touchOffsetY);
    
    
    bool bounceScrollChildren(float touchOffsetX, float touchOffsetY);
    void startRecordSlidAction();
    virtual void endRecordSlidAction();
    
    //ScrollPageProtocol
    virtual void handlePressLogic(Touch *touch) ;
    virtual void handleMoveLogic(Touch *touch) ;
    virtual void handleReleaseLogic(Touch *touch) ;
    
    virtual void interceptTouchEvent(Widget::TouchEventType event,Widget* sender,Touch *touch) override;
    
    void recordSlidTime(float dt);
    
    void scrollToTopEvent();
    void scrollToBottomEvent();
    void scrollToLeftEvent();
    void scrollToRightEvent();
    void scrollingEvent();
    void scrollendEvent();
    
    void bounceTopEvent();
    void bounceBottomEvent();
    void bounceLeftEvent();
    void bounceRightEvent();
    
protected:
    Layout* _innerContainer;
    
    Direction _direction;
    Vec2 _autoScrollDir;
    
    float _topBoundary;
    float _bottomBoundary;
    float _leftBoundary;
    float _rightBoundary;
    
    float _bounceTopBoundary;
    float _bounceBottomBoundary;
    float _bounceLeftBoundary;
    float _bounceRightBoundary;
    
    
    bool _autoScroll;
    float _autoScrollAddUpTime;
    
    float _autoScrollOriginalSpeed;
    float _autoScrollAcceleration;
    bool _isAutoScrollSpeedAttenuated;
    bool _needCheckAutoScrollDestination;
    Vec2 _autoScrollDestination;
    
    bool _bePressed;
    float _slidTime;
    Vec2 _moveChildPoint;
    float _childFocusCancelOffset;
    
    bool _leftBounceNeeded;
    bool _topBounceNeeded;
    bool _rightBounceNeeded;
    bool _bottomBounceNeeded;
    
    bool _bounceEnabled;
    bool _bouncing;
    Vec2 _bounceDir;
    float _bounceOriginalSpeed;
    bool _inertiaScrollEnabled;
    
    Ref* _ScrollPageEventListener;
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (push)
#pragma warning (disable: 4996)
#endif
    SEL_ScrollPageEvent _ScrollPageEventSelector;
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (pop)
#endif
    ccScrollPageCallback _eventCallback;
};



#endif /* defined(__DONUT4013__ScrollPage__) */
