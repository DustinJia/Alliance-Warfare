#include "HPDPS.h"
#include "HPDPS.h"
#include "Interface.h"
#include "Attacker.h"
#include "World.h"
#include "SimpleAudioEngine.h"
#include "Fish.h"

using namespace CocosDenshion;

typedef enum
{
    tag_progress,
}Tag_HPAttacker;

static HPDPS* instance = NULL;

HPDPS* HPDPS::sharedHPDPS()
{
    if (instance == NULL) {
        instance = new HPDPS();
    }
    return instance;
}

HPDPS* HPDPS::createHPDPS(const char *fileName)
{
    instance = new HPDPS();
    if (instance && instance->initWithFile(fileName)) {
        instance->HPDPSInit();
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    return NULL;
}

void HPDPS::HPDPSInit()
{
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("wound.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("increaseHP.mp3");
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*设置属性*/
    setScale(0.7);
    setPosition(ccp(size.width*0.08, size.height*0.4));
    
    /*添加头像*/
    iconDPS1 = CCSprite::create("boardAttacker.png");
    addChild(iconDPS1);
    iconDPS1->setScale(0.11);
    iconDPS1->setPosition(ccp(100, 104));
    
    iconDPS2 = CCSprite::create("board_Attacker.png");
    addChild(iconDPS2);
    iconDPS2->setScale(0.11);
    iconDPS2->setPosition(ccp(100, 108));
    
    schedule(schedule_selector(HPDPS::updateReplaceIcon));
    
    /*创建血条*/
    CCSprite* progress = CCSprite::create("hpPlayer.png");
    addChild(progress);
    progress->setScaleY(0.4);
    progress->setScaleX(0.2);
    progress->setPosition(ccp(70, 29));
    progress->setTag(tag_progress);
    
    progressTimer = CCProgressTimer::create(progress);
    addChild(progressTimer);
    
    /*设置血条属性*/
    progressTimer->setScaleY(0.4);
    progressTimer->setScaleX(0.2);
    progressTimer->setPosition(ccp(70, 29));
    progressTimer->setPercentage(100);                  /*设置进度值范围*/
    progressTimer->setType(kCCProgressTimerTypeBar);    /*设置进度条样式*/
    progressTimer->setBarChangeRate(ccp(1, 0));         /*设置进度条的宽高起始比例*/
    progressTimer->setMidpoint(ccp(1, 1));              /*设置进度条的运动方向为从右向左*/
    
    /*展示血量*/
    hp = CCLabelAtlas::create("0", "testfont.plist");
    addChild(hp);
    CCString* string = CCString::createWithFormat("%g",Interface::sharedInterface()->hpActual);
    hp->setString(string->getCString());
    hp->setScaleY(0.5);
    hp->setScaleX(0.2);
    hp->setAnchorPoint(ccp(1, 0.5));
    hp->setPosition(170, 55);
    hp->setColor(ccc3(255, 215, 0));
    
    /*初始化*/
    ifFirstTime = true;
}

void HPDPS::updateReplaceIcon(float delta)
{
    if (!Attacker::sharedAttacker()->ifLaunched) {
        iconDPS2->setVisible(false);
        iconDPS1->setVisible(true);
    }
    else {
        iconDPS1->setVisible(false);
        iconDPS2->setVisible(true);
    }
}

void HPDPS::decreaseHP()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("wound.wav", false);
    }

    if (ifFirstTime) {
        removeChildByTag(tag_progress);
    }
    
    /*设置为反向进度*/
    progressTimer->setReverseProgress(true);
    
    /*使进度条执行动作*/
    if (progressTimer->getPercentage() > 99) {
        CCProgressTo* progressTo = CCProgressTo::create(1, Interface::sharedInterface()->hpDPS);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPDPS::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));    }
    else {
        CCProgressTo* progressTo = CCProgressTo::create(1, progressTimer->getPercentage() + Interface::sharedInterface()->hpDPS);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPDPS::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));       }
    
    if (progressTimer->getPercentage() > 98 && !ifFirstTime) {
        /*停止怪物动作*/
        Fish::sharedFish()->SharkMoveByVelocity = 0;
        /*停止进度条动作*/
        progressTimer->stopAllActions();
        /*将血量显示为0*/
        hp->setString(CCString::createWithFormat("%d", 0)->getCString());
        /*切换到积分界面*/
        Interface::sharedInterface()->gameOver(Attacker::sharedAttacker());
        World::sharedWorld()->ifWin = false;
        World::sharedWorld()->points();
    }
    
    ifFirstTime = false;
}

void HPDPS::increaseHP(int type)
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect)
    {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.1);
        SimpleAudioEngine::sharedEngine()->playEffect("increaseHP.mp3");
    }
    /*执行加血特效*/
    schedule(schedule_selector(HPDPS::effectIncreaseHP), 0.3);
    
    /*设置为反向进度*/
    progressTimer->setReverseProgress(true);
    
    /*使进度条执行动作*/
    if (type == 1) {
        CCProgressTo* progressTo = CCProgressTo::create(1, progressTimer->getPercentage() - Interface::sharedInterface()->hpHeal);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPDPS::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));       }
    else if (type == 2) {
        CCProgressTo* progressTo = CCProgressTo::create(0, progressTimer->getPercentage() - Interface::sharedInterface()->hpHeal * 0.2);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPDPS::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));
    }
}

void HPDPS::refreshHP()
{
    CCString* string = CCString::createWithFormat("%.0f",(100 - progressTimer->getPercentage()) * Interface::sharedInterface()->hpConversion);
    hp->setString(string->getCString());
}


#pragma mark - 加血特效
void HPDPS::effectIncreaseHP()
{
    float f = 300;
    float x = 350;
    float y = 300;
    
    static int t = 0;
    t++;
    if(t <= 5)
    {
        CCSprite *increase = CCSprite::create("heart.png");
        increase->setPosition(ccp(x - f/2, y+f*2/3+9));
        Attacker::sharedAttacker()->addChild(increase);
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
                scheduleOnce(schedule_selector(HPDPS::remove1), 1.5f);
                break;
            case 2:
                scheduleOnce(schedule_selector(HPDPS::remove2), 1.5f);
                break;
            case 3:
                scheduleOnce(schedule_selector(HPDPS::remove3), 1.5f);
                break;
            case 4:
                scheduleOnce(schedule_selector(HPDPS::remove4), 1.5f);
                break;
            case 5:
                scheduleOnce(schedule_selector(HPDPS::remove5), 1.5f);
                break;
                
            default:
                break;
        }
        
        if(t == 5)
        {
            t = 0;
            unschedule(schedule_selector(HPDPS::effectIncreaseHP));
        }
    }
}

void HPDPS::remove1()
{
    Attacker::sharedAttacker()->removeChildByTag(1);
}

void HPDPS::remove2()
{
    Attacker::sharedAttacker()->removeChildByTag(2);
}

void HPDPS::remove3()
{
    Attacker::sharedAttacker()->removeChildByTag(3);
}

void HPDPS::remove4()
{
    Attacker::sharedAttacker()->removeChildByTag(4);
}

void HPDPS::remove5()
{
    Attacker::sharedAttacker()->removeChildByTag(5);
}