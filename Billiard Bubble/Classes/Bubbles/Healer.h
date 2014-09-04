#ifndef __Billiard_Bubble__Healer__
#define __Billiard_Bubble__Healer__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class Healer : public CCSprite, public CCTouchDelegate
{
public:
    /*实例化对象*/
    static Healer* createHealer(const char* fileName);
    /*创建本类对象的单例*/
    static Healer* sharedHealer();
    
public:
    /*技能发动*/
    void launch();
    /*判断技能是否发动了*/
    bool ifLaunched;
    /*判断技能冷却是否结束*/
    bool ifFinishedCooling;
    /*把两个角度转换成一个0~90度的角度*/
    float getAngle_0_90(float angle1,float angle2);
    
    /*当前的旋转角度*/
    float healerRotationNow;
    /*下帧的移动角度*/
    float healerAngleNew;
    /*下帧的移动速度*/
    float healerVelocity;
    /*记录下当前速度，此数据用于数据传递，避免系统先后运行导致同一帧数据的变化*/
    float spVelocityRecord;
    /*记录下当前的运动角度，此数据用于数据传递，避免系统先后运行导致同一帧数据的变化*/
    float spRotationNowRecord;
protected:
    /*初始化对象*/
    void healerInit();
    
    /*判断是否受伤*/
    void updateWound(float delta);
    void wounded();
    
    /*开启技能后覆盖的新精灵*/
    CCSprite* coveredSprite;
    
    /*判断是否加血*/
    void updateIncreaseTank(float delta);
    void updateIncreaseDPS(float delta);
    
    /*技能发动持续时间计时*/
    void durationUpdate(float delta);
    /*开启冷却计时*/
    void startCoolingClock(float delta);
    /*停止技能效果检测*/
    void stopActionUpdate(float delta);
    
    /*判断是否为双击*/
    bool isDoubleClick();
    
    /*反弹的处理，对碰撞产生的向量进行合成，最终获得下一次的转向角度*/
    float getCollisionAngle();
    /*判断按照当前运动轨迹，下一帧是否会碰撞，并且对精灵下一帧位置坐调整*/
    bool pointNextIfOut();
    /*得到下一帧的运动轨迹，并且对精灵部分碰撞的速度进行处理*/
    float getTangentAngle(int shape,CCSprite * sp);
    /*得到反弹角度的向量*/
    CCPoint collisionBack(cocos2d::CCPoint moveIn,float k);
    /*碰撞的更新函数*/
    void updateCollision(float delta);
    /*得到碰撞位置*/
    float getContactPosition(CCSprite * sp);
    /*把向量转换成角度*/
    float getAngle(float x,float y);
    
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
    bool collisionTankTouch;
    bool collisionAttackerTouch;
    bool collisionHealerTouch;
    bool collisionLeftRightTouch;
    bool touchMove;
    bool ifAddPoints;
    bool healerCollisionTank;
    bool healerCollisionAttacker;
    /*反弹相关*/
    CCPoint healerNow;
    CCPoint healerOld;
    CCPoint healerMoveOld;
    CCPoint movePointTo;
    CCPoint pointNext;
    CCPoint pointOldNext;
    /*反弹的一些控制及判断条件*/
    bool In;
    bool InOld;
    bool finish;
    bool ifFinished;
    bool nextTurns;
    bool moveCollision;
    bool ifNext;
    
    float pointDistance;
    int collisionPoints;
    /*每帧减少的像素*/
    float drag;
    float healerAngleOld;
    /*精灵精灵的一些角度*/
    float healerRotationOld;
    float angleSpTangentX;
    float angleSpTangentY;
    int frame;
    ccColor3B color ;
    
};

#endif /* defined(__Billiard_Bubble__Healer__) */