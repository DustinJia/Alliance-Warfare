//
//  Teach2.cpp
//  Billiard Bubble
//
//  Created by 杨 跃杰 on 13-7-20.
//
//

#include "Teach2.h"
#include "Teach.h"
#include "Help3.h"
#include "Menu.h"
#include "Teach3.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene *Teach2::scene()
{
    CCScene *scene = CCScene::create();
    Teach2 *layer = Teach2::create();
    scene->addChild(layer);
    return scene;
}


bool Teach2::init()
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
    itemNext = CCMenuItemImage::create("direction.png", "direction.png",this, menu_selector(Teach2::secondPage));
    CCMenuItemImage* back = CCMenuItemImage::create("directionBack.png", "directionBack.png", this, menu_selector(Teach2::back));
    itemNext->setPosition(size.width * 0.93, size.height * 0.1);
    back->setPosition(size.width * 0.07, size.height * 0.1);
    
    /*角色简介*/
    CCMenuItemImage *labelMenu = CCMenuItemImage::create("teach_menu.png", "teach_menu.png", this, menu_selector(Teach2::role));
    labelMenu->setScale(0.5);
    labelMenu->setPosition(ccp(size.width - labelMenu->boundingBox().size.width * 0.65, size.height - labelMenu->boundingBox().size.height));
    /*加入菜单*/
    CCMenu *menu = CCMenu::create(itemNext, back, labelMenu, NULL);
    addChild(menu);
    menu->setPosition(ccp(0,0));
    
    /*技能图标*/
    inherence = CCSprite::create("inherence_heal.png");
    inherence->setPosition(ccp(size.width*0.8, size.height*0.2));
    inherence->setScale(0.35);
    addChild(inherence);
    
    scheduleOnce(schedule_selector(Teach2::addSprite), 0.2);
    
    return true;
}


void Teach2::addSprite()
{
    if(number!=0)
    {
        removeChild(boss);
        removeChild(heal);
        removeChild(label);
        removeChild(finger);
        removeChild(attacker);
    }
    else
    {
        removeChild(circle);
    }
    
    /*添加Boss*/
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("boss.plist");
    boss = CCSprite::createWithSpriteFrameName("boss06_1_0.png");
    boss->setPosition(ccp(size.width*0.3, size.height*0.5));
    boss->setScale(0.5);
    addChild(boss);
    
    attacker = CCSprite::create("attacker.png");
    attacker->setPosition(ccp(size.width*0.2, size.height*0.3));
    addChild(attacker);
    attacker->setScale(0.25);
    
    /*添加奶妈*/
    heal = CCSprite::create("healer.png");
    heal->setPosition(ccp(size.width*0.7, size.height*0.5));
    heal->setScale(0.25);
    addChild(heal);
    
    /*添加选种效果*/
    circle = CCSprite::create("choice.png");
    circle->setOpacity(0);
    circle->setScale(0.1);
    circle->setPosition(ccp(200, 200));
    attacker->addChild(circle);
    
    if(number == 0)
    {
        /*添加boss动作*/
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
        boss->runAction(animate);
        CCMoveTo *move = CCMoveTo::create(1.5, ccp(size.width*0.59, size.height*0.5));
        boss->runAction(move);
        /*攻击后效果*/
        scheduleOnce(schedule_selector(Teach2::attckEffect), 1.5);
    }
    else
    {
        logic();
    }
    
}

/*奶妈受到攻击动画*/
void Teach2::attckEffect()
{
    heal->setColor(ccc3(255, 0, 0));
    /*停止boss动作*/
    boss->stopAllActions();
    label = CCLabelTTF::create("注意了！怪物总会先攻击脆弱的傻馒呢，小心小心~~","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*1.1));
    label->setColor(ccc3(227, 23, 13));
    addChild(label);
    CCMoveBy *move = CCMoveBy::create(0.75, ccp(0, -size.height*0.3));
    CCEaseBounceOut* bounceOut = CCEaseBounceOut::create(move);
    label->runAction(bounceOut);
    
    if(number==0)
    {
        scheduleOnce(schedule_selector(Teach2::addSprite), 3);
        number++;
    }
}

/*手指移动到技能位置*/
void Teach2::logic()
{
    if(number==2)
    {
        circle = CCSprite::create("choice.png");
        circle->setOpacity(0);
        circle->setScale(0.1);
        circle->setPosition(ccp(200, 200));
        heal->addChild(circle);
    }
    finger = CCSprite::create("finger.png");
    finger->setPosition(ccp(size.width*0.78, size.height*0.3));
    finger->setRotation(30);
    finger->setScale(0.5);
    addChild(finger);
    CCMoveTo *move = CCMoveTo::create(1.5, ccp(size.width*0.73, size.height*0.24));
    if (number != 2) {
        finger->runAction(move);
    }
    else {
        finger->setAnchorPoint(ccp(1, 1));
        finger->setPosition(heal->getPosition());
    }
    scheduleOnce(schedule_selector(Teach2::change), 1.5);
}

/*技能选中效果*/
void Teach2::change()
{
    if(number==1)
    {
        CCScaleBy *scale = CCScaleBy::create(0.3, 0.9);
        CCActionInterval *back = scale->reverse();
        inherence->runAction(CCSequence::create(scale, back,NULL));
        
        scheduleOnce(schedule_selector(Teach2::move), 0.3);
    }
    else if(number==2)
    {
        changeEffect();
    }
}

/*手指移动到attacker*/
void Teach2::move()
{
    CCMoveTo *move = CCMoveTo::create(1.5, ccp(size.width*0.14, size.height*0.33));
    finger->runAction(move);
    scheduleOnce(schedule_selector(Teach2::changeEffect), 1.5);
}

void Teach2::changeEffect()
{
    circle->setOpacity(255);
    CCScaleBy *scale = CCScaleBy::create(0.5, 10);
    CCFadeOut *fade = CCFadeOut::create(0.3);
    CCActionInterval *back = scale->reverse();
    circle->runAction(CCSequence::create(scale,fade,back,NULL));
    if(number == 2)
    {
        scheduleOnce(schedule_selector(Teach2::moveTogether), 0.8);
    }
    else if(number==3)
    {
        scheduleOnce(schedule_selector(Teach2::moveHeal), 1);
    }
    else
    {
        scheduleOnce(schedule_selector(Teach2::addBlood), 0.5);
    }
    
}
/*点击加血*/
void Teach2::addBlood()
{
    label = CCLabelTTF::create("傻馒的技能开启后，可连续点击哀木涕和劣人为其补血哦~","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*1.1));
    label->setColor(ccc3(255, 255, 255));
    addChild(label);
    CCMoveBy *move = CCMoveBy::create(0.75, ccp(0, -size.height*0.3));
    CCEaseBounceOut* bounceOut = CCEaseBounceOut::create(move);
    label->runAction(bounceOut);
    
    CCLabelTTF *addLabel = CCLabelTTF::create("+", "Verdana-Bold", 35);
    addLabel->setColor(ccc3(255, 0, 0));
    addChild(addLabel);
    addLabel->setPosition(ccp(attacker->getPositionX(), attacker->getPositionY()+50));
    CCMoveBy *moveBy = CCMoveBy::create(1, ccp(0, 100));
    CCFadeOut *fade = CCFadeOut::create(0.3);
    addLabel->runAction(CCSequence::create(moveBy, fade, NULL));
    
    number++;
    /*启动第三次动画*/
    scheduleOnce(schedule_selector(Teach2::addSprite), 3);
}

/*手指与heal移动*/
void Teach2::moveTogether()
{
    CCMoveBy *moveT = CCMoveBy::create(0.5, ccp(-size.width*0.1, -size.height*0.05));
    CCMoveBy *moveF = CCMoveBy::create(0.5, ccp(-size.width*0.1, -size.height*0.05));
    CCMoveBy *moveFi = CCMoveBy::create(0.5, ccp(-size.width*0.05, size.height*0.2));
    heal->runAction(moveT);
    finger->runAction(CCSequence::create(moveF,moveFi,NULL));
    number++;
    scheduleOnce(schedule_selector(Teach2::changeEffect), 0.5);
    
}

/*heal单独移动*/
void Teach2::moveHeal()
{
    CCMoveBy *moveBy = CCMoveBy::create(0.7, ccp(-size.width*0.3, -size.height*0.15));
    heal->runAction(moveBy);
    scheduleOnce(schedule_selector(Teach2::ciclerAddBlood), 0.7);
}

/*label移动*/
void Teach2::ciclerAddBlood()
{
    label = CCLabelTTF::create("当傻馒直接撞到劣人或哀木涕时，也会给他们补血呦~","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*1.1));
    label->setColor(ccc3(255, 255, 255));
    addChild(label);
    CCMoveBy *move = CCMoveBy::create(0.75, ccp(0, -size.height*0.3));
    CCEaseBounceOut* bounceOut = CCEaseBounceOut::create(move);
    label->runAction(bounceOut);
    schedule(schedule_selector(Teach2::effectIncreaseHP), 0.3);
    
    CCScaleBy* scaleBy = CCScaleBy::create(0.25, 1.2);
    CCActionInterval* reverse = scaleBy->reverse();
    itemNext->runAction(CCRepeatForever::create(CCSequence::create(scaleBy, reverse, NULL)));
}

/*加血特效*/
void Teach2::effectIncreaseHP()
{
    float f = attacker->boundingBox().size.height-40;
    float x = attacker->getPositionX()+30;
    float y = attacker->getPositionY()+20;
    static int t = 0;
    t++;
    if(t <= 5)
    {
        
        CCSprite *increase = CCSprite::create("heart.png");
        increase->setScale(0.2);
        increase->setPosition(ccp(x - f/2, y+f*2/3+9));
        addChild(increase);
        
        increase->setColor(ccc3(255, 0, 0));
        increase->setTag(t);
        CCPointArray *array = CCPointArray::create(20);
        array->addControlPoint(ccp(x-f/2, y +f*2/3+9));
        array->addControlPoint(ccp(x - f*7/6-6, y + f/3+6));
        array->addControlPoint(ccp(x - f*3/2-9, y-f/3));
        array->addControlPoint(ccp(x - f*7/6-6, y - f-6));
        array->addControlPoint(ccp(x-f/2, y-f*4/3-9));
        array->addControlPoint(ccp(x + f/6+6, y - f-6));
        array->addControlPoint(ccp(x + f/2+9, y-f/3));
        array->addControlPoint(ccp(x + f/6+6, y + f/3+6));
        array->addControlPoint(ccp(x-f/2, y + f*2/3+9));
        
        CCCardinalSplineTo *spline = CCCardinalSplineTo::create(1.5, array, 0);
        increase->runAction(spline);
        
        switch (t) {
            case 1:
                scheduleOnce(schedule_selector(Teach2::remove1), 1.5f);
                break;
            case 2:
                scheduleOnce(schedule_selector(Teach2::remove2), 1.5f);
                break;
            case 3:
                scheduleOnce(schedule_selector(Teach2::remove3), 1.5f);
                break;
            case 4:
                scheduleOnce(schedule_selector(Teach2::remove4), 1.5f);
                break;
            case 5:
                scheduleOnce(schedule_selector(Teach2::remove5), 1.5f);
                break;
                
            default:
                break;
        }
        
        
        if(t == 5)
        {
            t = 0;
            unschedule(schedule_selector(Teach2::effectIncreaseHP));
        }
    }
    
}

void Teach2::remove1()
{
    removeChildByTag(1);
    
}

void Teach2::remove2()
{
    removeChildByTag(2);
}

void Teach2::remove3()
{
    removeChildByTag(3);
}

void Teach2::remove4()
{
    removeChildByTag(4);
}

void Teach2::remove5()
{
    removeChildByTag(5);
}

/*角色简介界面*/
void Teach2::role()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click2.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, Help3::scene()));
}

void Teach2::secondPage()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(1, Teach3::scene()));
}

void Teach2::back()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(1, Teach::scene()));
}