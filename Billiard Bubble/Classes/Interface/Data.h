#ifndef __Billiard_Bubble__Data__
#define __Billiard_Bubble__Data__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class Data
{
public:
    /*获取当前类的单例对象*/
    static Data* sharedData();
    
    /*保存积分*/
    void savePoints();
    
    /*当前关的积分*/
    float points;
    /*总积分*/
    float totalPoints;
    
    /*是否更新了积分*/
    bool ifRefreshed;
    
    /*每关由积分兑换的点数*/
    int severalWithPoints = 10;
    
public:
    /*增加积分的接口函数*/
    void addPointsWithAbsorbing();
    void addPointsWithTankAttack();
    void addPointsWithAttack();
    void addPointsWithAttackSkill();
    void addPointsWithHeal();
    void addPointsWithTotalHP();
    
protected:
    Data();
};

#endif /* defined(__Billiard_Bubble__Data__) */