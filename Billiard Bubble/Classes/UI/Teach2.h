//
//  Teach2.h
//  Billiard Bubble
//
//  Created by 杨 跃杰 on 13-7-20.
//
//

#ifndef __Billiard_Bubble__Teach2__
#define __Billiard_Bubble__Teach2__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class Teach2 :public CCLayer
{
public:
    bool init();
    static CCScene *scene();
    CREATE_FUNC(Teach2);
private:
    void back();
    void secondPage();
    void addSprite();
    void logic();
    void attckEffect();
    void role();
    void change();
    void move();
    void addBlood();
    void changeEffect();
    void addCicler();
    void moveTogether();
    void moveHeal();
    void ciclerAddBlood();
    void effectIncreaseHP();
    void remove1();
    void remove2();
    void remove3();
    void remove4();
    void remove5();
    
protected:
    CCSize size;
    CCSprite *heal;
    CCSprite *attacker;
    CCSprite *finger;
    CCSprite *boss;
    CCSprite *circle;
    CCSprite *inherence;
    CCLabelTTF *label;
    CCMenuItemImage* itemNext;
    int number;
};

#endif /* defined(__Billiard_Bubble__Teach2__) */