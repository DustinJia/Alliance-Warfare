//
//  Teach.cpp
//  Billiard Bubble
//
//  Created by 杨 跃杰 on 13-7-19.
//
//

#include "Teach.h"
#include "Help2.h"
#include "Teach1.h"
#include "Teach2.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene *Teach::scene()
{
    CCScene *scene = CCScene::create();
    Teach *layer = Teach::create();
    scene->addChild(layer);
    return scene;
}

bool Teach::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    next = false;
    size = CCDirector::sharedDirector()->getWinSize();
    CCSprite *background = CCSprite::create("teach_background.jpg");
    background->setPosition(ccp(size.width*0.5, size.height*0.5));
    addChild(background);
    
    /*方向按钮*/
    itemNext = CCMenuItemImage::create("direction.png", "direction.png",this, menu_selector(Teach::secondPage));
    CCMenuItemImage* back = CCMenuItemImage::create("directionBack.png", "directionBack.png", this, menu_selector(Teach::back));
    itemNext->setPosition(size.width * 0.93, size.height * 0.1);
    back->setPosition(size.width * 0.07, size.height * 0.1);
    
    /*角色简介*/
    CCMenuItemImage *labelMenu = CCMenuItemImage::create("teach_menu.png", "teach_menu.png", this, menu_selector(Teach::role));
    labelMenu->setScale(0.5);
    labelMenu->setPosition(ccp(size.width - labelMenu->boundingBox().size.width * 0.65, size.height - labelMenu->boundingBox().size.height));
    /*加入菜单*/
    CCMenu *menu = CCMenu::create(itemNext, back, labelMenu, NULL);
    addChild(menu);
    menu->setPosition(ccp(0,0));
    
    /*添加Boss*/
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("boss.plist");
    boss = CCSprite::createWithSpriteFrameName("boss06_1_0.png");
    boss->setPosition(ccp(size.width*0.3, size.height*0.5));
    boss->setScale(0.5);
    boss->setFlipX(false);
    addChild(boss);
    
    /*技能图标*/
    inherence = CCSprite::create("inherence_attack.png");
    inherence->setPosition(ccp(size.width*0.8, size.height*0.2));
    inherence->setScale(0.35);
    addChild(inherence);
    
    addSprite();
    
    return true;
}

void Teach::addSprite()
{
    if(next)
    {
        removeChild(DPS);
        removeChild(finger);
        removeChild(circle);
        removeChild(label);
        boss->setFlipX(true);
    }
    
    /*添加D精灵*/
    DPS = CCSprite::create("attacker.png");
    if(!next)
    {
        DPS->setPosition(ccp(size.width*0.7, size.height*0.6));
    }
    else
    {
        DPS->setPosition(ccp(size.width*0.7, size.height*0.4));
    }
    DPS->setScale(0.25);
    addChild(DPS);
    
    /*添加选种效果*/
    circle = CCSprite::create("choice.png");
    circle->setOpacity(0);
    circle->setScale(0.1);
    circle->setPosition(ccp(200, 200));
    DPS->addChild(circle);
    
    /*添加手指*/
    finger = CCSprite::create("finger.png");
    if(!next)
    {
    finger->setPosition(ccp(size.width*0.7, size.height*0.8));
        logic();
        finger->setRotation(30);
        finger->setScale(0.5);
        addChild(finger);
    }
    else
    {
        /*手指选中技能*/
        finger->setPosition(ccp(size.width*0.75, size.height*0.3));
        finger->setRotation(30);
        finger->setScale(0.5);
        addChild(finger);
        CCMoveTo *move = CCMoveTo::create(1, ccp(inherence->getPositionX() - finger->boundingBox().size.width*0.38, inherence->getPositionY() + finger->boundingBox().size.height*0.2));
        finger->runAction(move);
        scheduleOnce(schedule_selector(Teach::logic), 1);
    }
}

void Teach::logic()
{
    /*添加手指动作演示*/
    float X;
    float Y;
    
   if(!next)
   {
     X =  DPS->getPositionX() - finger->boundingBox().size.width*0.25;
     Y =  DPS->getPositionY() + finger->boundingBox().size.height*0.15;
       CCMoveTo *move = CCMoveTo::create(2, ccp(X, Y));
       finger->runAction(move);
   }
    if(next)
    {
        X =   DPS->getPositionX() - finger->boundingBox().size.width*0.37;
        Y =  DPS->getPositionY() + finger->boundingBox().size.height*0.17;
        CCScaleBy *scale = CCScaleBy::create(0.3, 0.9);
        CCActionInterval *back = scale->reverse();
        inherence->runAction(CCSequence::create(scale, back,NULL));
        CCDelayTime *delay = CCDelayTime::create(0.5);
        CCMoveTo *move = CCMoveTo::create(2, ccp(X, Y));
        finger->runAction(CCSequence::create(delay,move,NULL));
    }

    scheduleOnce(schedule_selector(Teach::circleAppear), 2);
    scheduleOnce(schedule_selector(Teach::moveTogether), 3);
    scheduleOnce(schedule_selector(Teach::moveAlone), 4.5);
}

/*手指选中效果*/
void Teach::circleAppear()
{
    circle->setOpacity(255);
    CCScaleBy *scale = CCScaleBy::create(0.5, 10);
    CCFadeOut *fade = CCFadeOut::create(0.3);
    CCActionInterval *back = scale->reverse();
    circle->runAction(CCSequence::create(scale,fade,back,NULL));
    
}

/*手指滑动T移动动作*/
void Teach::moveTogether()
{
    CCMoveBy *moveT = CCMoveBy::create(0.5, ccp(-size.width*0.1, 0));
    CCMoveBy *moveF = CCMoveBy::create(0.5, ccp(-size.width*0.1, 0));
    CCMoveBy *moveFi = CCMoveBy::create(0.5, ccp(-size.width*0.05, size.height*0.1));
    DPS->runAction(moveT);
    finger->runAction(CCSequence::create(moveF,moveFi,NULL));
    scheduleOnce(schedule_selector(Teach::circleAppear), 0.3);
}

/*手指离开T自己移动*/
void Teach::moveAlone()
{
   CCMoveBy *move = CCMoveBy::create(1, ccp(-size.width*0.21, 0));
   DPS->runAction(move);
    scheduleOnce(schedule_selector(Teach::changeColor), 1);
}

void Teach::changeColor()
{
    if(!next)
    {
        DPS->setColor(ccc3(225, 0, 0));
        label = CCLabelTTF::create("要小心不要让劣人被怪物正面袭击哦~！","Verdana-Bold",35);
        label->setPosition(ccp(size.width*0.5, -35));
        label->setColor(ccc3(227, 23, 13));
        addChild(label);
        CCMoveBy *move = CCMoveBy::create(0.75, ccp(0, size.height*0.3));
        CCEaseBounceOut* bounceOut = CCEaseBounceOut::create(move);
        label->runAction(bounceOut);
    }
    else
    {
        boss->setColor(ccc3(255, 0, 0));
        label = CCLabelTTF::create("劣人开启技能后向怪物背后攻击，会造成大量伤害呢！","Verdana-Bold",35);
        label->setPosition(ccp(size.width*0.5, size.height*1.1));
        label->setColor(ccc3(0, 0, 255));
        addChild(label);
        CCMoveBy *move = CCMoveBy::create(0.75, ccp(0, -size.height*0.3));
        CCEaseBounceOut* bounceOut = CCEaseBounceOut::create(move);
        label->runAction(bounceOut);
        
        CCScaleBy* scaleBy = CCScaleBy::create(0.25, 1.2);
        CCActionInterval* reverse = scaleBy->reverse();
        itemNext->runAction(CCRepeatForever::create(CCSequence::create(scaleBy, reverse, NULL)));
    }
   
    if(!next)
    {
      scheduleOnce(schedule_selector(Teach::addSprite), 3);
      next = true;
    }
}

void Teach::role()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click2.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, Help2::scene()));
}

void Teach::secondPage()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(1, Teach2::scene()));
}

void Teach::back()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(1, Teach1::scene()));
}