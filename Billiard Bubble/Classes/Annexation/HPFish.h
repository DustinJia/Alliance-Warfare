#ifndef __Billiard_Bubble__HPFish__
#define __Billiard_Bubble__HPFish__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class HPFish : public CCSprite
{
public:
    /*实例化对象*/
    static HPFish* createHPFish(const char* fileName);
    /*创建本类对象的单例*/
    static HPFish* sharedHPFish();
    /*血条*/
    CCProgressTimer* progressTimer;
    
public:
    /*普通掉血*/
    void decreaseHP();
    /*DPS发动技能后掉血*/
    void decreaseSkillHP();
    
    /*T攻击时掉血*/
    void decreaseTankAttackHP();
    
protected:
    /*初始化*/
    void HPFishInit();
    
    /*显示剩余血量变化*/
    void refreshHP();
    
protected:
    /*获取屏幕尺寸*/
    CCSize size;
    /*头像*/
    CCSprite* iconEnemy;
    /*血量*/
    CCLabelAtlas* hp;
    
    /*判断是否为首次调用*/
    bool ifFirstTime;
};

#endif /* defined(__Billiard_Bubble__HPFish__) */