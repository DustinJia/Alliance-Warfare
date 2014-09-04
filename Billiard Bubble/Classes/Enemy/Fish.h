#ifndef __Billiard_Bubble__Fish__
#define __Billiard_Bubble__Fish__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class Fish : public CCSprite
{
public:
    /*实例化对象*/
    static Fish* createFish(const char* fileName);
    /*创建本类对象的单例*/
    static Fish* sharedFish();

public:
    /*移动的动画*/
    void animation();
    
    /*动作控制*/
    void wounded();
    
    /*动作逻辑*/
    void update(float delta);
    
    /*鱼当前的角度*/
    float sharkRotationNow;
    
protected:
    /*初始化对象*/
    void fishInit();
    /*动作的回调函数*/
    void callBackMove();
    /*把向量转换成角度*/
    float getAngle(float x,float y);
    /*仇恨比较*/
    CCPoint trackingTarget();
    
public:
    /*接口*/
    void getMoveVelocity(float moveVelocity);            /*获得线速度*/
    void getRotationVelocity(float rotationVelocity);    /*获得角速度*/
    
    float SharkMoveByVelocity;                           /*1s所走的坐标数*/
    
protected:
    /*实例变量*/
    CCSize size;                    /*获取屏幕尺寸*/
    CCPoint pointTo;                /*仇恨目标的坐标*/
    float SharkRotationVelocity;    /*每帧转的角度*/
    float SharkRotationNow;         /*鱼当前的角度*/
    float SharkRotationNext;        /*鱼即将要旋转到的角度*/
    ccColor3B color;
};

#endif /* defined(__Billiard_Bubble__Fish__) */