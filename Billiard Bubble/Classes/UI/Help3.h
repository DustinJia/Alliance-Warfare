#ifndef __Billiard_Bubble__Help3__
#define __Billiard_Bubble__Help3__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class Help3 : public CCLayer
{
public:
    bool init();
    static CCScene* scene();
    CREATE_FUNC(Help3);
    
    void backToTeach();
    
    /*注册触摸事件*/
    void registerWithTouchDispatcher();
    //触摸开始
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    //触摸结束
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    /*获取屏幕尺寸*/
    CCSize size;
    //获取触摸初始点
    CCPoint beginPoint;
    //获取触摸结束点
    CCPoint endPoint;
};

#endif