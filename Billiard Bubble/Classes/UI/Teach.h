//
//  Teach.h
//  Billiard Bubble
//
//  Created by 杨 跃杰 on 13-7-19.
//
//

#ifndef __Billiard_Bubble__Teach__
#define __Billiard_Bubble__Teach__
#include "cocos2d.h"
USING_NS_CC;
#include <iostream>
class Teach :public CCLayer
{
public:
    bool init();
    static CCScene *scene();
    CREATE_FUNC(Teach);
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
protected:
    CCSize size;
    CCSprite *DPS;
    CCSprite *finger;
    CCSprite *boss;
    CCSprite *circle;
    CCSprite *inherence;
    CCLabelTTF *label;
    CCMenuItemImage* itemNext;
    bool next;
};




#endif /* defined(__Billiard_Bubble__Teach__) */
