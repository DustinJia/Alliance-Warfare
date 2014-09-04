#ifndef __Billiard_Bubble__Help4__
#define __Billiard_Bubble__Help4__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class Help4 : public CCLayer
{
public:
    bool init();
    static CCScene* scene();
    CREATE_FUNC(Help4);
    
    /*注册触摸事件*/
    void registerWithTouchDispatcher();
    //触摸开始
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    //触摸结束
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    //返回主菜单
    void backMenu();
    
protected:
    /*获取屏幕尺寸*/
    CCSize size;
    //获取触摸初始点
    CCPoint beginPoint;
    //获取触摸结束点
    CCPoint endPoint;
};

#endif