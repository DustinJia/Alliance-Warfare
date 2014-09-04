#ifndef __Billiard_Bubble__Menu__
#define __Billiard_Bubble__Menu__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class Menu : public CCLayer
{
public:
    bool init();
    static CCScene* scene();
    CREATE_FUNC(Menu);
    
public:
    /*获取本类的单例对象*/
    static Menu* sharedMenu();
    
    /*更新数据*/
    void refreshData();
    
protected:
    /*点击菜单触发的函数*/
    void pressPlay();
    void pressChoose();
    void pressInhesion();          
    void pressSet();
    void pressHelp();
    void pressAbout();
    
protected:
    void onEnter();
    
protected:
    CCSize size;                   /*获取屏幕尺寸*/
};

#endif /* defined(__Billiard_Bubble__Menu__) */