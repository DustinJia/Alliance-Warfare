#ifndef __Billiard_Bubble__World__
#define __Billiard_Bubble__World__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class World : public CCLayer
{
public:
    bool init();
    static CCScene* scene();
    CREATE_FUNC(World);
    
public:
    /*获取当前类的单例对象*/
    static World* sharedWorld();
    
    /*切换场景*/
    void points();
    void replaceScene();
    
    /*获取屏幕尺寸*/
    CCSize size;
    /*判断输赢*/
    bool ifWin;
    /*判断是否由暂停菜单切换的场景*/
    bool ifPause;
        
protected:
    /*游戏开始前先显示关数*/
    void showLevel();
    
protected:
    void onEnter();
    void onExit();
};

#endif /* defined(__Billiard_Bubble__World__) */