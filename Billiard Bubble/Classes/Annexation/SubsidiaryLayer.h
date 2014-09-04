#ifndef __Billiard_Bubble__SubsidiaryLayer__
#define __Billiard_Bubble__SubsidiaryLayer__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class SubsidiaryLayer : public CCLayer
{
public:
    bool init();
    CREATE_FUNC(SubsidiaryLayer);
    
public:
    /*获取当前类的单例对象*/
    static SubsidiaryLayer* sharedSubsidiaryLayer();
    
    /*技能*/
    CCProgressTimer* skillAbsorb;
    CCProgressTimer* skillAttack;
    CCProgressTimer* skillHeal;
    
    /*技能冷却*/
    void coolingAbsorb();
    void coolingAttack();
    void coolingHeal();
    
    /*切换暂停界面*/
    void pauseScene();
    
    /*计时器秒数*/
    float time;
    
    /*积分*/
    CCLabelAtlas* points;
    
protected:
    /*技能函数*/
    void absorbSkill();
    void attackSkill();
    void healSkill();
    
    /*计时器*/
    void updateClock(float delta);
    
    /*更新积分*/
    void updateRefreshPoints(float delta);
    
protected:
    /*触摸*/
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    /*触摸监听*/
    void onEnter();
    void onExit();
    
protected:
    /*获取屏幕尺寸*/
    CCSize size;
    /*判断玩家选择了哪个技能*/
    int choice;
    /*时钟*/
    CCLabelAtlas* clock;
};

#endif /* defined(__Billiard_Bubble__SubsidiaryLayer__) */