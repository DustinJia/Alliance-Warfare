//
//  Teach1.h
//  Billiard Bubble
//
//  Created by 杨 跃杰 on 13-7-20.
//
//

#ifndef __Billiard_Bubble__Teach1__
#define __Billiard_Bubble__Teach1__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class Teach1 :public CCLayer
{
public:
    bool init();
    static CCScene *scene();
    CREATE_FUNC(Teach1);
    
private:
    void back();
    void addSprite();
    void circleAppear();
    void moveTogether();
    void moveAlone();
    void changeColor();
    void skil();
    void logic();
    void secondPage();
    void role();
    void absorb();
    void attck();
    void attckEffect();
    
protected:
    CCSize size;
    CCSprite *tank;
    CCSprite *finger;
    CCSprite *boss;
    CCSprite *circle;
    CCSprite *inherence;
    CCLabelTTF *label;
    int number;
    ccColor3B color;
    CCMenuItemImage* itemBack;
};

#endif /* defined(__Billiard_Bubble__Teach1__) */
