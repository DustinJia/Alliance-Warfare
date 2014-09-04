#include "World.h"
#include "SimpleAudioEngine.h"
#include "Interface.h"
#include "SubsidiaryLayer.h"
#include "Tank.h"
#include "Fish.h"
#include "Attacker.h"
#include "Healer.h"
#include "Data.h"
#include "HPFish.h"
#include "HPTank.h"
#include "HPDPS.h"
#include "HPHealer.h"
#include "DisplayPoints.h"
#include "Menu.h"

using namespace CocosDenshion;

static World* instance = NULL;

typedef enum
{
    tag_sprite,
}Tag_Interface;

World* World::sharedWorld()
{
    if (instance == NULL) {
        instance = new World();
    }
    return instance;
}

CCScene* World::scene()
{
    CCScene* scene = CCScene::create();
    World* layer = World::create();
    scene->addChild(layer);
    return scene;
}

bool World::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    /*播放游戏时的音乐*/
    if (Interface::sharedInterface()->ifPlayMusic) {
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.8);
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game.mp3", true);
    }
    
    /*为静态对象添加实例*/
    instance = this;
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*设置本关难度*/
    Interface::sharedInterface()->setDifficulty();

    /*添加附属层*/
    SubsidiaryLayer* subsidiaryLayer = SubsidiaryLayer::create();
    addChild(subsidiaryLayer);
    
    /*添加鱼*/
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("boss.plist");
    Fish* fish = Fish::createFish("boss06_0_0.png");
    addChild(fish);
    
    /*添加泡泡*/
    Tank* tank = Tank::createTank("tank.png");
    addChild(tank);
    Attacker* attacker = Attacker::createAttacker("attacker.png");
    addChild(attacker);
    Healer* healer = Healer::createHealer("healer.png");
    addChild(healer);
    
    /*显示关数*/
    showLevel();
    
    /*初始化*/
    Interface::sharedInterface()->threatTank = 0;
    Interface::sharedInterface()->threatDPS = 0;
    Interface::sharedInterface()->threatHealer = 0;
    ifWin = CCUserDefault::sharedUserDefault()->getBoolForKey("ifWin", false);
    
    return true;
}


#pragma mark - 显示关数
void World::showLevel()
{
    /*添加显示板*/
    CCSprite* sprite = CCSprite::create("displayLevel.png");
    addChild(sprite);
    CCLabelBMFont* label = CCLabelBMFont::create("Level", "testChinese.fnt");
    sprite->addChild(label);
    
    CCString* string;
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("ifEndlessMode")) {
        string = CCString::createWithFormat("Level %d", Interface::sharedInterface()->level);
    }
    else {
        string = CCString::createWithFormat("Level %d", Interface::sharedInterface()->level1);
    }
    
    label->setString(string->getCString());
    label->setPosition(ccp(sprite->boundingBox().size.width * 0.5, sprite->boundingBox().size.height * 0.5));
    
    sprite->setPosition(ccp(size.width * 0.5, size.height - sprite->boundingBox().size.height * 0.3));
    sprite->setScale(0.5);
    sprite->setTag(tag_sprite);
}


#pragma mark - 切换场景
void World::points()
{
    scheduleOnce(schedule_selector(World::replaceScene), 3);
}

void World::replaceScene()
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1.5, DisplayPoints::scene()));
}


#pragma mark - 生命周期相关
void World::onEnter()
{
    if (!ifPause) {
        Data::sharedData()->points = 0;
        CCUserDefault::sharedUserDefault()->setFloatForKey("points", 0);
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    ifPause = false;
    
    CCLayer::onEnter();
}

void World::onExit()
{
    /*存储总分*/
    Data::sharedData()->savePoints();
    
    /*更新并存储关数*/
    if (ifWin && !ifPause) {
        if (CCUserDefault::sharedUserDefault()->getBoolForKey("ifEndlessMode")) {
            Interface::sharedInterface()->level += 1;
        }
        else {
            Interface::sharedInterface()->level1 += 1;
        }
    }
    else if (CCUserDefault::sharedUserDefault()->getBoolForKey("ifEndlessMode") && !ifPause) {
        /*无尽模式中失败后要初始化数据*/
        Menu::sharedMenu()->refreshData();
    }
    CCUserDefault::sharedUserDefault()->setIntegerForKey("level", Interface::sharedInterface()->level);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("level1", Interface::sharedInterface()->level1);
    
    /*如果是无尽模式，还要存储输赢*/
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("ifEndlessMode")) {
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifWin", ifWin);
    }
    
    CCUserDefault::sharedUserDefault()->flush();
    CCLayer::onExit();
}