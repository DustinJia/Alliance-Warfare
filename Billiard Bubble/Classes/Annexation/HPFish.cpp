#include "HPFish.h"
#include "Interface.h"
#include "World.h"
#include "Data.h"
#include "DisplayPoints.h"
#include "Fish.h"

typedef enum
{
    tag_progress,
}Tag_HPFish;

static HPFish* instance = NULL;

HPFish* HPFish::sharedHPFish()
{
    if (instance == NULL) {
        instance = new HPFish();
    }
    return instance;
}

 HPFish* HPFish::createHPFish(const char *fileName)
{
    instance = new HPFish();
    if (instance && instance->initWithFile(fileName)) {
        instance->HPFishInit();
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    return NULL;
}

void HPFish::HPFishInit()
{
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*设置属性*/
    setScale(0.35);
    setPosition(ccp(size.width*0.08, size.height*0.88));
    
    /*添加头像*/
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("boss.plist");
    
    /*boss类型，1.综合，2.移动速度，3旋转速度，4.防御力，5.攻击力*/
    switch (Interface::sharedInterface()->enemyType) {
        case 4:
            iconEnemy = CCSprite::createWithSpriteFrameName("boss06_1_0.png");
            addChild(iconEnemy);
            iconEnemy->setScale(1.1);
            iconEnemy->setPosition(ccp(210, 230));
            break;
        case 3:
            iconEnemy = CCSprite::createWithSpriteFrameName("boss08_1_0.png");
            addChild(iconEnemy);
            iconEnemy->setScale(1.2);
            iconEnemy->setPosition(ccp(222, 240));
            break;
        case 5:
            iconEnemy = CCSprite::createWithSpriteFrameName("boss09_0_0.png");
            addChild(iconEnemy);
            iconEnemy->setScale(0.87);
            iconEnemy->setPosition(ccp(205, 232));
            break;
        case 1:
            iconEnemy = CCSprite::createWithSpriteFrameName("boss10_1_0.png");
            addChild(iconEnemy);
            iconEnemy->setScale(0.8);
            iconEnemy->setPosition(ccp(215, 232));
            break;
        case 2:
            iconEnemy = CCSprite::createWithSpriteFrameName("zako09_0_0.png");
            addChild(iconEnemy);
            iconEnemy->setScale(1.2);
            iconEnemy->setPosition(ccp(215, 235));
            break;
        default:
            break;
    }

    /*创建血条*/
    CCSprite* progress = CCSprite::create("hpEnemy.png");
    addChild(progress);
    progress->setScaleY(1);
    progress->setScaleX(1.05);
    progress->setPosition(ccp(220, 53));
    progress->setColor(ccc3(255, 0, 0));
    progress->setTag(tag_progress);
    
    progressTimer = CCProgressTimer::create(progress);
    addChild(progressTimer);
    
    /*设置血条属性*/
    progressTimer->setScaleY(1);
    progressTimer->setScaleX(1.05);
    progressTimer->setPosition(ccp(221, 51));
    progressTimer->setPercentage(100);                  /*设置进度值范围*/
    progressTimer->setType(kCCProgressTimerTypeBar);    /*设置进度条样式*/
    progressTimer->setBarChangeRate(ccp(1, 0));         /*设置进度条的宽高起始比例*/
    progressTimer->setMidpoint(ccp(0, 1));              /*设置进度条的运动方向为从左向右*/
    
    /*展示血量*/
    hp = CCLabelAtlas::create("0", "testfont.plist");
    addChild(hp);
    CCString* string = CCString::createWithFormat("%.f",Interface::sharedInterface()->hpBossActual);
    hp->setString(string->getCString());
    hp->setScaleY(1);
    hp->setScaleX(0.5);
    hp->setAnchorPoint(ccp(1, 0.5));
    hp->setPosition(boundingBox().size.width * 4, 70);
    hp->setColor(ccc3(176, 23, 31));
        
    /*初始化*/
    ifFirstTime = true;
}

void HPFish::decreaseHP()
{
    if (ifFirstTime) {
        removeChildByTag(tag_progress);
    }
    /*设置为反向进度*/
    progressTimer->setReverseProgress(true);
    /*使进度条执行动作*/
    if (progressTimer->getPercentage() > 99) {
        CCProgressTo* progressTo = CCProgressTo::create(1, Interface::sharedInterface()->hpFish);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPFish::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));
        /*更新血量*/
        /*积分增加*/
        Data::sharedData()->addPointsWithAttack();
    }
    else {
        CCProgressTo* progressTo = CCProgressTo::create(1, progressTimer->getPercentage() + Interface::sharedInterface()->hpFish);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPFish::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));
        /*积分增加*/
        Data::sharedData()->addPointsWithAttack();
    }
    if (progressTimer->getPercentage() > 98 && !ifFirstTime) {
        /*停止进度条动作*/
        progressTimer->stopAllActions();
        /*停止怪物动作*/
        Fish::sharedFish()->SharkMoveByVelocity = 0;
        /*将血量显示为0*/
        hp->setString(CCString::createWithFormat("%d", 0)->getCString());
        /*切换到积分界面*/
        Interface::sharedInterface()->gameOver(Fish::sharedFish());
        World::sharedWorld()->ifWin = true;
        World::sharedWorld()->points();
    }
    
    ifFirstTime = false;
}

void HPFish::decreaseSkillHP()
{
    if (ifFirstTime) {
        removeChildByTag(tag_progress);
    }
    
    /*设置为反向进度*/
    progressTimer->setReverseProgress(true);
    
    /*使进度条执行动作*/
    if (progressTimer->getPercentage() > 99) {
        CCProgressTo* progressTo = CCProgressTo::create(0.5, Interface::sharedInterface()->hpEnemySkill);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPFish::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));
        /*积分增加*/
        Data::sharedData()->addPointsWithAttackSkill();
    }
    else {
        CCProgressTo* progressTo = CCProgressTo::create(0.5, progressTimer->getPercentage() + Interface::sharedInterface()->hpEnemySkill);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPFish::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));
        /*积分增加*/
        Data::sharedData()->addPointsWithAttackSkill();
    }
    if (progressTimer->getPercentage() > 98 && !ifFirstTime) {
        /*停止进度条动作*/
        progressTimer->stopAllActions();
        /*停止怪物动作*/
        Fish::sharedFish()->SharkMoveByVelocity = 0;
        /*将血量显示为0*/
        hp->setString(CCString::createWithFormat("%d", 0)->getCString());
        /*切换到积分界面*/
        Interface::sharedInterface()->gameOver(Fish::sharedFish());
        World::sharedWorld()->ifWin = true;
        World::sharedWorld()->points();
    }
    
    ifFirstTime = false;
}

void HPFish::decreaseTankAttackHP()
{
    if (ifFirstTime) {
        removeChildByTag(tag_progress);
    }
    
    /*设置为反向进度*/
    progressTimer->setReverseProgress(true);
    
    /*使进度条执行动作*/
    if (progressTimer->getPercentage() > 99) {
        CCProgressTo* progressTo = CCProgressTo::create(1, Interface::sharedInterface()->hpEnemyTankAttack);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPFish::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));
        /*积分增加*/
        Data::sharedData()->addPointsWithAttackSkill();
    }
    else {
        CCProgressTo* progressTo = CCProgressTo::create(1, progressTimer->getPercentage() + Interface::sharedInterface()->hpEnemyTankAttack);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(HPFish::refreshHP));
        progressTimer->runAction(CCSequence::create(progressTo, callFunc, NULL));
        /*积分增加*/
        Data::sharedData()->addPointsWithAttackSkill();
    }
    if (progressTimer->getPercentage() > 98 && !ifFirstTime) {
        /*停止进度条动作*/
        progressTimer->stopAllActions();
        /*停止怪物动作*/
        Fish::sharedFish()->SharkMoveByVelocity = 0;
        /*将血量显示为0*/
        hp->setString(CCString::createWithFormat("%d", 0)->getCString());
        /*切换到积分界面*/
        Interface::sharedInterface()->gameOver(Fish::sharedFish());
        World::sharedWorld()->ifWin = true;
        World::sharedWorld()->points();
    }
    
    ifFirstTime = false;
}

void HPFish::refreshHP()
{
    CCString* string = CCString::createWithFormat("%.0f",(100 - progressTimer->getPercentage()) * Interface::sharedInterface()->hpBossConversion);
    hp->setString(string->getCString());
}