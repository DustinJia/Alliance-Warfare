//
//  Teach1.cpp
//  Billiard Bubble
//
//  Created by 杨 跃杰 on 13-7-20.
//
//

#include "Teach1.h"
#include "Help1.h"
#include "Teach0.h"
#include "Teach.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene *Teach1::scene()
{
    CCScene *scene = CCScene::create();
    Teach1 *layer = Teach1::create();
    scene->addChild(layer);
    return scene;
}

bool Teach1::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    number = 0;
    size = CCDirector::sharedDirector()->getWinSize();
    CCSprite *background = CCSprite::create("teach_background.jpg");
    background->setPosition(ccp(size.width*0.5, size.height*0.5));
    addChild(background);
    
    /*方向按钮*/
    itemBack = CCMenuItemImage::create("direction.png", "direction.png",this, menu_selector(Teach1::secondPage));
    CCMenuItemImage* itemReturn = CCMenuItemImage::create("directionBack.png", "directionBack.png", this, menu_selector(Teach1::back));
    itemBack->setPosition(size.width * 0.93, size.height * 0.1);
    itemReturn->setPosition(size.width * 0.07, size.height * 0.1);
    
    /*角色简介*/
    CCMenuItemImage *labelMenu = CCMenuItemImage::create("teach_menu.png", "teach_menu.png", this, menu_selector(Teach1::role));
    labelMenu->setScale(0.5);
    labelMenu->setPosition(ccp(size.width - labelMenu->boundingBox().size.width * 0.65, size.height - labelMenu->boundingBox().size.height));
    
    /*加入菜单*/
    CCMenu *menu = CCMenu::create(itemBack, itemReturn, labelMenu, NULL);
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
    
    color=boss->getColor();
    
    /*技能图标*/
    inherence = CCSprite::create("inherence_tankSkill.png");
    inherence->setPosition(ccp(size.width*0.8, size.height*0.2));
    inherence->setScale(0.55);
    addChild(inherence);
    
    addSprite();
    
    return true;
}


void Teach1::addSprite()
{
    if(number!=0)
    {
        removeChild(tank);
        removeChild(finger);
        removeChild(circle);
        removeChild(label);
        boss->setFlipX(true);
    }
    
    /*添加T精灵*/
    tank = CCSprite::create("tank.png");
    switch (number) {
        case 0:
             tank->setPosition(ccp(size.width*0.7, size.height*0.6));
            break;
          case 1:
            tank->setPosition(ccp(size.width*0.7, size.height*0.4));
            break;
        case 2:
            tank->setPosition(ccp(size.width*0.7, size.height*0.5));
            boss->setColor(color);
        default:
            break;
    }
        
    tank->setScale(0.25);
    addChild(tank);
    
    /*添加选种效果*/
    circle = CCSprite::create("choice.png");
    circle->setOpacity(0);
    circle->setScale(0.1);
    circle->setPosition(ccp(200, 200));
    tank->addChild(circle);
    
    /*添加手指*/
    finger = CCSprite::create("finger.png");
    switch (number) {
        case 0:
             finger->setPosition(ccp(size.width*0.7, size.height*0.8));
            break;
        case 1:
            finger->setPosition(ccp(size.width*0.75, size.height*0.5));
            break;
        case 2:
             finger->setPosition(ccp(size.width*0.8, size.height*0.3));
            break;
        default:
            break;
    }
        
    logic();
    finger->setRotation(30);
    finger->setScale(0.5);
    addChild(finger);

}

void Teach1::logic()
{
    /*添加手指动作演示*/
    float X;
    float Y;
    
    if(number != 2)
    {
        X =   tank->getPositionX() - finger->boundingBox().size.width*0.25;
        Y =  tank->getPositionY() + finger->boundingBox().size.height*0.15;
        CCMoveTo *move = CCMoveTo::create(2, ccp(X, Y));
        finger->runAction(move);
        
        scheduleOnce(schedule_selector(Teach1::circleAppear), 2);
        scheduleOnce(schedule_selector(Teach1::moveTogether), 3);
        scheduleOnce(schedule_selector(Teach1::moveAlone), 4.5);
    }
    else{
        scheduleOnce(schedule_selector(Teach1::absorb), 2);
    }
}

void Teach1::absorb()
{
    CCMoveTo *move = CCMoveTo::create(1, ccp(inherence->getPositionX() - finger->boundingBox().size.width*0.38, inherence->getPositionY() + finger->boundingBox().size.height*0.2));
    finger->runAction(move);
    scheduleOnce(schedule_selector(Teach1::attck), 1);
}

void Teach1::attck()
{
    CCScaleBy *scale = CCScaleBy::create(0.3, 0.9);
    CCActionInterval *back = scale->reverse();
    inherence->runAction(CCSequence::create(scale, back,NULL));
    
    CCAnimation *animation = CCAnimation::create();
    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("boss.plist");
    for(int i=0;i<=5;i++)
    {
        CCString *fileName = CCString::createWithFormat("boss06_1_%d.png",i);
        CCSpriteFrame *frame = frameCache->spriteFrameByName(fileName->getCString());
        animation->addSpriteFrame(frame);
    }
    animation->setDelayPerUnit(0.1f);
    animation->setLoops(-1);
    CCAnimate *animate = CCAnimate::create(animation);
    boss->setFlipX(false);
    boss->runAction(animate);
    CCMoveTo *move = CCMoveTo::create(1.5, ccp(size.width*0.59, size.height*0.5));
    boss->runAction(move);
    scheduleOnce(schedule_selector(Teach1::attckEffect), 1.5);
}

void Teach1::attckEffect()
{
    tank->setColor(ccc3(255, 0, 0));
    /*停止boss动作*/
    boss->stopAllActions();
    label = CCLabelTTF::create("开启技能后，对面的怪物会立即凑过来哦~！","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*1.1));
    label->setColor(ccc3(250, 255, 255));
    addChild(label);
    CCMoveBy *move = CCMoveBy::create(0.75, ccp(0, -size.height*0.3));
    CCEaseBounceOut* bounceOut = CCEaseBounceOut::create(move);
    label->runAction(bounceOut);
    
    CCScaleBy* scaleBy = CCScaleBy::create(0.25, 1.2);
    CCActionInterval* reverse = scaleBy->reverse();
    itemBack->runAction(CCRepeatForever::create(CCSequence::create(scaleBy, reverse, NULL)));
}

/*手指选中效果*/
void Teach1::circleAppear()
{
    circle->setOpacity(255);
    CCScaleBy *scale = CCScaleBy::create(0.5, 10);
    CCFadeOut *fade = CCFadeOut::create(0.3);
    CCActionInterval *back = scale->reverse();
    circle->runAction(CCSequence::create(scale,fade,back,NULL));
}

/*手指滑动T移动动作*/
void Teach1::moveTogether()
{
    CCMoveBy *moveT = CCMoveBy::create(0.5, ccp(-size.width*0.1, 0));
    CCMoveBy *moveF = CCMoveBy::create(0.5, ccp(-size.width*0.1, 0));
    CCMoveBy *moveFi = CCMoveBy::create(0.5, ccp(-size.width*0.05, size.height*0.1));
    tank->runAction(moveT);
    finger->runAction(CCSequence::create(moveF,moveFi,NULL));
    scheduleOnce(schedule_selector(Teach1::circleAppear), 0.3);
    
}
/*手指离开T自己移动*/
void Teach1::moveAlone()
{
    CCMoveBy *move = CCMoveBy::create(1, ccp(-size.width*0.21, 0));
   tank->runAction(move);
    scheduleOnce(schedule_selector(Teach1::changeColor), 1);
}

void Teach1::changeColor()
{
    if(number == 0)
    {
        tank->setColor(ccc3(225, 0, 0));
        label = CCLabelTTF::create("撞到怪物正面时，自己会受伤呢~","Verdana-Bold",35);
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
        label = CCLabelTTF::create("记住，只能攻击怪物后背！","Verdana-Bold",35);
        label->setPosition(ccp(size.width*0.5, size.height*1.1));
        label->setColor(ccc3(0, 0, 255));
        addChild(label);
        CCMoveBy *move = CCMoveBy::create(0.75, ccp(0, -size.height*0.3));
        CCEaseBounceOut* bounceOut = CCEaseBounceOut::create(move);
        label->runAction(bounceOut);
    }
    
    
    if(number != 2)
    {
        scheduleOnce(schedule_selector(Teach1::addSprite), 3);
        number++;
    }
    
}

void Teach1::role()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click2.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8, Help1::scene()));
}

void Teach1::secondPage()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(1, Teach::scene()));
}

void Teach1::back()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(1, Teach0::scene()));
}