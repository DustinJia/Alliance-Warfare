#ifndef __Billiard_Bubble__Tank__
#define __Billiard_Bubble__Tank__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class Tank : public CCSprite, public CCTouchDelegate
{
public:
    /*实例化对象*/
    static Tank* createTank(const char* fileName);
    /*获取当前类的单例对象*/
    static Tank* sharedTank();
    
public:
    /*得到碰撞位置*/
    float getContactPosition(CCSprite * sp);
    /*把向量转换成角度*/
    float getAngle(float x,float y);
    /*把两个角度转换成一个0~90度的角度*/
    float getAngle_0_90(float angle1,float angle2);
    
    /*技能发动*/
    void launch();
    void launchEffect(float delta);
    void removeEffect(float delta);
    
    /*判断技能是否发动了*/
    bool ifLaunched;
    /*判断技能冷却是否结束*/
    bool ifFinishedCooling;
    
    /*当前的旋转角度*/
    float tankRotationNow;
    /*下帧的移动角度*/
    float tankAngleNew;
    /*下帧的移动速度*/
    float tankVelocity;
    /*记录下当前速度，此数据用于数据传递，避免系统先后运行导致同一帧数据的变化*/
    float spVelocityRecord;
    /*记录下当前的运动角度，此数据用于数据传递，避免系统先后运行导致同一帧数据的变化*/
    float spRotationNowRecord;
    /*在碰撞时，根据当前速度和碰撞后产生的变化来对x轴的坐标进行合成*/
    float angleSpTangentX;
    /*在碰撞时，根据当前速度和碰撞后产生的变化来对y轴的坐标进行合成*/
    float angleSpTangentY;
    
protected:
    /*初始化对象*/
    void tankInit();
    
    /*受伤*/
    void wounded();
    /*判断是否掉血*/
    void updateDecreaseHP(float delta);
    
    /*技能发动持续时间计时*/
    void durationUpdate(float delta);
    /*开启冷却计时*/
    void startCoolingClock(float delta);
    
    /*反弹的处理，对碰撞产生的向量进行合成，最终获得下一次的转向角度*/
    float getCollisionAngle();
    /*判断按照当前运动轨迹，下一帧是否会碰撞，并且对精灵下一帧位置坐调整*/
    bool pointNextIfOut();
    /*得到下一帧的运动轨迹，并且对精灵部分碰撞的速度进行处理*/
    float getTangentAngle(int shape,CCSprite * sp);
    /*得到反弹角度的向量*/
    CCPoint collisionBack(cocos2d::CCPoint moveIn,float k);
    
protected:
    /*触摸*/
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    /*触摸监听*/
    void onEnter();
    void onExit();
    
protected:
    /*获取屏幕尺寸*/
    CCSize size;
    
    /*触摸相关*/
    CCPoint pointOld;
    float moveByX;
    float moveByY;
    float pointDistance;
    
    bool collisionTankTouch;
    bool collisionAttackerTouch;
    bool collisionHealerTouch;
    bool collisionLeftRightTouch;
    bool touchMove;
    bool ifNext;
    
    /*反弹相关*/
    CCPoint tankNow;
    CCPoint tankOld;
    CCPoint tankMoveOld;
    CCPoint movePointTo;
    CCPoint pointNext;
    
    /*反弹的一些控制及判断条件*/
    bool moveCollision;
    bool In;
    bool InOld;
    bool finish;
    bool ifFinished;
    bool nextTurns;
    int collisionPoints;
    /*每帧减少的像素*/
    float drag;
    /*精灵精灵的一些角度*/
    float tankAngleOld;
    float tankRotationOld;
    int frame;
    ccColor3B color;
};

#endif /* defined(__Billiard_Bubble__Tank__) */