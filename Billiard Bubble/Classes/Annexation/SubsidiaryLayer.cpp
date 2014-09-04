#include "SubsidiaryLayer.h"
#include "HPFish.h"
#include "HPTank.h"
#include "HPDPS.h"
#include "HPHealer.h"
#include "Interface.h"
#include "Tank.h"
#include "Attacker.h"
#include "Healer.h"
#include "Data.h"
#include "Pause.h"
#include "World.h"

/*标签的枚举变量*/
typedef enum
{
    tag_spriteAbsorb,
    tag_spriteAttack,
    tag_spriteHeal,
}Tag_SubsidiaryLayer;

/*发动的技能的枚举变量*/
typedef enum
{
    absorb,
    attack,
    heal,
}LaunchSkill_SubsidiaryLayer;

static SubsidiaryLayer* instance = NULL;

SubsidiaryLayer* SubsidiaryLayer::sharedSubsidiaryLayer()
{
    if (instance == NULL) {
        instance = new SubsidiaryLayer();
    }
    return instance;
}

bool SubsidiaryLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    /*为静态对象添加实例*/
    instance = this;
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*添加背景*/
    if (Interface::sharedInterface()->level >=1 && Interface::sharedInterface()->level < 6) {
        CCSprite* backGround = CCSprite::create("map1.jpg");
        backGround->setPosition(ccp(size.width * 0.5, size.height * 0.5));
        addChild(backGround);
    }
    else if (Interface::sharedInterface()->level >=6 && Interface::sharedInterface()->level < 11) {
        CCSprite* backGround = CCSprite::create("map2.jpg");
        backGround->setPosition(ccp(size.width * 0.5, size.height * 0.5));
        addChild(backGround);
    }
    else if (Interface::sharedInterface()->level >=11) {
        CCSprite* backGround = CCSprite::create("map3.jpg");
        backGround->setPosition(ccp(size.width * 0.5, size.height * 0.5));
        addChild(backGround);
    }
    
    /*添加鱼的血量*/
    HPFish* hpFish = HPFish::createHPFish("board_Enemy.png");
    addChild(hpFish);
    
    /*添加T的血量*/
    HPTank* hpTank = HPTank::createHPTank("board.png");
    addChild(hpTank);
    
    /*添加DPS的血量*/
    HPDPS* hpDPS = HPDPS::createHPDPS("board.png");
    addChild(hpDPS);
    
    /*添加治疗的血量*/
    HPHealer* hpHealer = HPHealer::createHPHealer("board.png");
    addChild(hpHealer);

    /*添加技能*/
    absorbSkill();
    attackSkill();
    healSkill();
    
    /*添加计时器*/
    clock = CCLabelAtlas::create("0:0", "testfont.plist");
    addChild(clock);
    clock->setAnchorPoint(ccp(0.5, 0.5));
    clock->setScaleY(0.6);
    clock->setScaleX(0.4);
    clock->setPosition(ccp(size.width - clock->boundingBox().size.width, size.height - clock->boundingBox().size.height * 0.5));
    clock->setColor(ccc3(255, 235, 205));
    schedule(schedule_selector(SubsidiaryLayer::updateClock), 0.1);
    
    /*添加积分*/
    points = CCLabelAtlas::create("0", "testfont.plist");
    addChild(points);
    points->setAnchorPoint(ccp(0.5, 0.5));
    points->setScale(0.6);
    points->setPosition(ccp(size.width * 0.5, size.height - points->boundingBox().size.height * 1.3));
    points->setColor(ccc3(0, 255, 255));
    schedule(schedule_selector(SubsidiaryLayer::updateRefreshPoints));
    
    /*添加菜单按钮*/
    CCMenuItemImage* itemPause = CCMenuItemImage::create("pause.png", "pause.png", this, menu_selector(SubsidiaryLayer::pauseScene));
    CCMenu* menu = CCMenu::create(itemPause, NULL);
    addChild(menu);
    menu->setScale(0.7);
    menu->setPosition(ccp(size.width - menu->boundingBox().size.width * 0.293, size.height * 0.61));

    /*初始化*/
    time = 0;
    
    return true;
}

void SubsidiaryLayer::pauseScene()
{
    CCUserDefault::sharedUserDefault()->setFloatForKey("points", Data::sharedData()->points);
    CCUserDefault::sharedUserDefault()->flush();
    World::sharedWorld()->ifPause = true;
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5, Pause::scene(), ccc3(0, 0 , 0)));
}


#pragma mark - 添加技能图标
void SubsidiaryLayer::absorbSkill()
{
    /*添加进度条背景*/
    CCSprite* sprite = CCSprite::create("skill_ground.png");
    addChild(sprite);
    sprite->setScale(0.6);
    sprite->setPosition(ccp(size.width - sprite->boundingBox().size.width * 0.5 - 20, size.height * 0.45));
    
    /*初始化进度条*/
    skillAbsorb = CCProgressTimer::create(CCSprite::create("inherence_tankSkill.png"));
    addChild(skillAbsorb);
    skillAbsorb->setScale(0.6);
    skillAbsorb->setPosition(sprite->getPosition());
    skillAbsorb->setPercentage(100);
    skillAbsorb->setType(kCCProgressTimerTypeRadial);    
}

void SubsidiaryLayer::attackSkill()
{
    /*添加进度条背景*/
    CCSprite* sprite = CCSprite::create("skill_ground.png");
    addChild(sprite);
    sprite->setScale(0.6);
    sprite->setPosition(ccp(size.width - sprite->boundingBox().size.width * 0.5 - 20, size.height * 0.27));
    
    /*初始化进度条*/
    skillAttack = CCProgressTimer::create(CCSprite::create("inherence_attack.png"));
    addChild(skillAttack);
    skillAttack->setScale(0.35);
    skillAttack->setPosition(sprite->getPosition());
    skillAttack->setPercentage(100);
    skillAttack->setType(kCCProgressTimerTypeRadial);
}

void SubsidiaryLayer::healSkill()
{
    /*添加进度条背景*/
    CCSprite* sprite = CCSprite::create("skill_ground.png");
    addChild(sprite);
    sprite->setScale(0.6);
    sprite->setPosition(ccp(size.width - sprite->boundingBox().size.width * 0.5 - 20, size.height * 0.09));
    
    /*初始化进度条*/
    skillHeal = CCProgressTimer::create(CCSprite::create("inherence_heal.png"));
    addChild(skillHeal);
    skillHeal->setScale(0.35);
    skillHeal->setPosition(sprite->getPosition());
    skillHeal->setPercentage(100);
    skillHeal->setType(kCCProgressTimerTypeRadial);
}


#pragma mark - 技能冷却
void SubsidiaryLayer::coolingAbsorb()
{
    CCProgressTo* progressTo = CCProgressTo::create(Interface::sharedInterface()->absorbCoolingTime, 100);
    skillAbsorb->runAction(progressTo);
}

void SubsidiaryLayer::coolingAttack()
{
    CCProgressTo* progressTo = CCProgressTo::create(Interface::sharedInterface()->attackCoolingTime, 100);
    skillAttack->runAction(progressTo);
}

void SubsidiaryLayer::coolingHeal()
{
    CCProgressTo* progressTo = CCProgressTo::create(Interface::sharedInterface()->healCoolingTime, 100);
    skillHeal->runAction(progressTo);
}


#pragma mark - 计时器与积分
void SubsidiaryLayer::updateClock(float delta)
{
    float seconds = 0;
    int minutes = 0;
    
    if (time < 60) {
        seconds = time;
    }
    else {
        minutes = (int)time / 60;
        seconds = (int)time % 60;
    }
    
    CCString* string = CCString::createWithFormat("%d : %d", minutes, (int)seconds);
    clock->setString(string->getCString());
    
    time += delta;
}

void SubsidiaryLayer::updateRefreshPoints(float delta)
{
    if (Data::sharedData()->ifRefreshed) {
        /*执行效果*/
        CCScaleTo* enlarge = CCScaleTo::create(0.1, 1.3);
        CCScaleTo* shrink = CCScaleTo::create(0.1, 0.5);
        points->runAction(CCSequence::create(enlarge, shrink, NULL));
        points->setScale(0.6);
        /*更新数值*/
        float temp = Pause::sharedPause()->tempPoints;
        CCString* string = CCString::createWithFormat("%d", (int)(Data::sharedData()->points + temp));
        points->setString(string->getCString());
        Data::sharedData()->ifRefreshed = false;
    }
}


#pragma mark - 触摸事件
bool SubsidiaryLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    float radius = skillAbsorb->boundingBox().size.width * 0.5;
    float distance1 = ccpDistance(skillAbsorb->getPosition(), pTouch->getLocation());
    float distance2 = ccpDistance(skillAttack->getPosition(), pTouch->getLocation());
    float distance3 = ccpDistance(skillHeal->getPosition(), pTouch->getLocation());
    
    if (distance1 <= radius) {
        choice = absorb;
        return true;
    }
    else if (distance2 <= radius) {
        choice = attack;
        return true;
    }
    else if (distance3 <= radius) {
        choice = heal;
        return true;
    }
    else {
        return false;
    }
}

void SubsidiaryLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    switch (choice) {
        case absorb:
            if (Tank::sharedTank()->ifFinishedCooling) {
                Tank::sharedTank()->launch();
            }
            break;
        case attack:
            if (Attacker::sharedAttacker()->ifFinishedCooling) {
                Attacker::sharedAttacker()->launch();
            }
            break;
        case heal:
            if (Healer::sharedHealer()->ifFinishedCooling) {
                Healer::sharedHealer()->launch();
            }
            break;
        default:
            break;
    }
}


#pragma mark - 生命周期相关
void SubsidiaryLayer::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();
}

void SubsidiaryLayer::onExit()
{
    /*存储时间*/
    CCUserDefault::sharedUserDefault()->setFloatForKey("time", time);
    CCUserDefault::sharedUserDefault()->flush();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}