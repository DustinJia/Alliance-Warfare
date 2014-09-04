#ifndef __Billiard_Bubble__HPTank__
#define __Billiard_Bubble__HPTank__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class HPTank : public CCSprite
{
public:
    /*实例化对象*/
    static HPTank* createHPTank(const char* fileName);
    /*创建本类对象的单例*/
    static HPTank* sharedHPTank();
    /*血条*/
    CCProgressTimer* progressTimer;
    
public:
    /*监测头像更换*/
    void updateReplaceIcon(float delta);
    
    /*掉血*/
    void decreaseHP();
    /*加血*/
    void increaseHP(int type);
    
protected:
    /*初始化*/
    void HPTankInit();
    
    /*显示剩余血量变化*/
    void refreshHP();
    
    /*加血特效*/
    void effectIncreaseHP();
    void remove1();
    void remove2();
    void remove3();
    void remove4();
    void remove5();
    
protected:
    /*获取屏幕尺寸*/
    CCSize size;
    /*头像*/
    CCSprite* iconTank1;
    CCSprite* iconTank2;
    /*血量*/
    CCLabelAtlas* hp;
    
    /*判断是否在本关首次调用*/
    bool ifFirstTime;
};

#endif /* defined(__Billiard_Bubble__HPTank__) */