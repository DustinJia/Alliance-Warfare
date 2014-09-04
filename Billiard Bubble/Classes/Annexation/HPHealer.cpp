#include "HPHealer.h"
#include "Interface.h"
#include "HPHealer.h"
#include "Healer.h"
#include "World.h"
#include "SimpleAudioEngine.h"
#include "Fish.h"

using namespace CocosDenshion;

typedef enum
{
    tag_progress,
}Tag_HPHealer;

static HPHealer* instance = NULL;

HPHealer* HPHealer::sharedHPHealer()
{
    if (instance == NULL) {
        instance = new HPHealer();
    }
    return instance;
}

HPHealer* HPHealer::createHPHealer(const char *fileName)
{
    instance = new HPHealer();
    if (instance && instance->initWithFile(fileName)) {
        instance->HPHealerInit();
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    return NULL;
}

void HPHealer::HPHealerInit()
{
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("wound.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("increaseHP.mp3");
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*设置属性*/
    setScale(0.7);
    setPosition(ccp(size.width*0.08, size.height*0.15));
    
    /*添加头像*/
    iconHealer1 = CCSprite::create("boardHealer.png");
    addChild(iconHealer1);
    iconHealer1->setScale(0.08);
    iconHealer1->setPosition(ccp(100, 104));
    
    iconHealer2 = CCSprite::create("board_Healer.png");
    addChild(iconHealer2);
    iconHealer2->setScale(0.22);
    iconHealer2->setPosition(ccp(100, 106));
    
    schedule(schedule_selector(HPHealer::updateReplaceIcon));
    
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

void HPHealer::updateReplaceIcon(float delta)
{
    if (!Healer::sharedHealer()->ifLaunched) {
        iconHealer2->setVisible(false);
        iconHealer1->setVisible(true);
    }
    else {
        iconHealer1->setVisible(false);
        iconHealer2->setVisible(true);
    }
}

void HPHealer::decreaseHP()
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
        CCProgressTo* progressTo = CCProgressTo::create(1, Interface::sharedInterface()->hpHealer);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPHealer::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));      }
    else {
        CCProgressTo* progressTo = CCProgressTo::create(1, progressTimer->getPercentage() + Interface::sharedInterface()->hpHealer);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPHealer::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));
    }
    
    if (progressTimer->getPercentage() > 98 && !ifFirstTime) {
        /*停止怪物动作*/
        Fish::sharedFish()->SharkMoveByVelocity = 0;
        /*停止进度条动作*/
        progressTimer->stopAllActions();
        /*将血量显示为0*/
        hp->setString(CCString::createWithFormat("%d", 0)->getCString());
        /*切换到积分界面*/
        Interface::sharedInterface()->gameOver(Healer::sharedHealer());
        World::sharedWorld()->ifWin = false;
        World::sharedWorld()->points();
    }
    
    ifFirstTime = false;
}

void HPHealer::increaseHP()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect)
    {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.1);
        SimpleAudioEngine::sharedEngine()->playEffect("increaseHP.mp3");
    }
    
    /*执行加血特效*/
    schedule(schedule_selector(HPHealer::effectIncreaseHP), 0.3);
    
    /*设置为反向进度*/
    progressTimer->setReverseProgress(true);
    
    /*使进度条执行动作*/
    CCProgressTo* progressTo = CCProgressTo::create(0, progressTimer->getPercentage() - Interface::sharedInterface()->hpHeal * 0.3);
    CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPHealer::refreshHP));
    progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));
}

void HPHealer::refreshHP()
{
    CCString* string = CCString::createWithFormat("%.0f",(100 - progressTimer->getPercentage()) * Interface::sharedInterface()->hpConversion);
    hp->setString(string->getCString());
}


#pragma mark - 加血特效
void HPHealer::effectIncreaseHP()
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
        Healer::sharedHealer()->addChild(increase);
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
                scheduleOnce(schedule_selector(HPHealer::remove1), 1.5f);
                break;
            case 2:
                scheduleOnce(schedule_selector(HPHealer::remove2), 1.5f);
                break;
            case 3:
                scheduleOnce(schedule_selector(HPHealer::remove3), 1.5f);
                break;
            case 4:
                scheduleOnce(schedule_selector(HPHealer::remove4), 1.5f);
                break;
            case 5:
                scheduleOnce(schedule_selector(HPHealer::remove5), 1.5f);
                break;
                
            default:
                break;
        }
        
        if(t == 5)
        {
            t = 0;
            unschedule(schedule_selector(HPHealer::effectIncreaseHP));
        }
    }
}

void HPHealer::remove1()
{
    Healer::sharedHealer()->removeChildByTag(1);
}

void HPHealer::remove2()
{
    Healer::sharedHealer()->removeChildByTag(2);
}

void HPHealer::remove3()
{
    Healer::sharedHealer()->removeChildByTag(3);
}

void HPHealer::remove4()
{
    Healer::sharedHealer()->removeChildByTag(4);
}

void HPHealer::remove5()
{
    Healer::sharedHealer()->removeChildByTag(5);
}