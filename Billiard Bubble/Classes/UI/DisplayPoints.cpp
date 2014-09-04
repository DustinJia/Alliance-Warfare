#include "DisplayPoints.h"
#include "Data.h"
#include "SubsidiaryLayer.h"
#include "World.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"
#include "Inhesion.h"
#include "Menu.h"

using namespace CocosDenshion;

CCScene* DisplayPoints::scene()
{
    CCScene* scene = CCScene::create();
    DisplayPoints* layer = DisplayPoints::create();
    scene->addChild(layer);
    return scene;
}

bool DisplayPoints::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("display.mp3");
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*添加背景*/
    CCSprite* backGround = CCSprite::create("pointsBackground.jpg");
    addChild(backGround);
    backGround->setPosition(ccp(size.width * 0.5, size.height * 0.5));
    
    /*展示标题*/
    scheduleOnce(schedule_selector(DisplayPoints::displayView), 1.5);
    
    /*初始化*/
    several = CCUserDefault::sharedUserDefault()->getFloatForKey("several", 0);
    ifWin = World::sharedWorld()->ifWin;
    return true;
}


#pragma mark - 逐条展示
void DisplayPoints::displayView()
{
    if(ifWin) {
        /*播放音乐*/
        if (Interface::sharedInterface()->ifPlayMusic) {
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.7);
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("success.mp3", false);
        }
        
        CCSprite *sprite = CCSprite::create("win.jpg");
        sprite->setScale(0.01);
        sprite->setPosition(ccp(size.width*0.5, size.height*0.5));
        addChild(sprite);
        CCScaleTo *scale1 = CCScaleTo::create(0.2, 1);
        CCDelayTime *delay = CCDelayTime::create(1.6);
        CCScaleTo *scale2 = CCScaleTo::create(0.2, 0.001);
        sprite->runAction(CCSequence::create(scale1, delay, scale2, NULL));
        
        /*开始下一关后才可以继续加同一点天赋*/
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded1", false);
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded2", false);
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded3", false);
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded4", false);
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded5", false);
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded6", false);
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded7", false);
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded8", false);
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded9", false);
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    else {
        /*播放音乐*/
        if (Interface::sharedInterface()->ifPlayMusic) {
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.7);
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("fail.mp3", false);
        }
        
        CCSprite *sprite = CCSprite::create("lose.png");
        sprite->setScale(0.01);
        sprite->setPosition(ccp(size.width*0.5, size.height*0.5));
        addChild(sprite);
        CCScaleTo *scale1 = CCScaleTo::create(0.2, 1);
        CCDelayTime *delay = CCDelayTime::create(1.6);
        CCScaleTo *scale2 = CCScaleTo::create(0.2, 0.001);
        sprite->runAction(CCSequence::create(scale1, delay, scale2, NULL));
    }
    
    /*展示本关得分*/
    scheduleOnce(schedule_selector(DisplayPoints::displayPoints), 2);
}

void DisplayPoints::displayPoints()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("display.mp3");
    }
    
    /*标题*/
    CCSprite* points = CCSprite::create("point1.png");
    CCSprite* pointsWithTime = CCSprite::create("point2.png");
    CCSprite* totalPoints = CCSprite::create("point3.png");
    
    addChild(points);
    addChild(pointsWithTime);
    addChild(totalPoints);
    
    /*设置属性*/
    points->setPosition(ccp(size.width * 0.3, size.height * 0.75));
    pointsWithTime->setPosition(ccp(size.width * 0.3, size.height * 0.65));
    totalPoints->setPosition(ccp(size.width * 0.3, size.height * 0.52));
    
    points->setScale(0.7);
    pointsWithTime->setScale(0.7);
    totalPoints->setScale(0.7);
    
    points->setColor(ccc3(240, 32, 0));
    pointsWithTime->setColor(ccc3(240, 32, 0));
    totalPoints->setColor(ccc3(240, 32, 0));
    
    /*动作*/
    CCFadeIn* fadeIn1 = CCFadeIn::create(0.5);
    CCFadeIn* fadeIn2 = CCFadeIn::create(0.5);
    CCFadeIn* fadeIn3 = CCFadeIn::create(0.5);
    points->runAction(fadeIn1);
    pointsWithTime->runAction(fadeIn2);
    totalPoints->runAction(fadeIn3);
    
    /*展示数据*/
    scheduleOnce(schedule_selector(DisplayPoints::displayData), 1);
}

void DisplayPoints::displayData()
{
    float time = CCUserDefault::sharedUserDefault()->getFloatForKey("time", 0);
    
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("display.mp3");
    }
    
    /*标题*/
    CCLabelTTF* points = CCLabelTTF::create(CCString::createWithFormat("%d", (int)Data::sharedData()->points)->getCString(), "Georgia-Bold", 45);
    CCLabelTTF* pointsWithTime = CCLabelTTF::create(CCString::createWithFormat("%d", (int)(5000 - time * Interface::sharedInterface()->addPointsWithTime))->getCString(), "Georgia-Bold", 45);
    CCLabelTTF* totalPoints = CCLabelTTF::create(CCString::createWithFormat("%d", (int)(Data::sharedData()->points + 5000 - time * Interface::sharedInterface()->addPointsWithTime))->getCString(), "Georgia-Bold", 50);
    addChild(points);
    addChild(pointsWithTime);
    addChild(totalPoints);
    
    /*设置属性*/
    points->setPosition(ccp(0, size.height * 0.75));
    pointsWithTime->setPosition(ccp(0, size.height * 0.65));
    totalPoints->setPosition(ccp(0, size.height * 0.47));
    
    points->setColor(ccc3(41, 36, 255));
    pointsWithTime->setColor(ccc3(41, 36, 255));
    totalPoints->setColor(ccc3(41, 36, 255));
    
    /*动作*/
    CCMoveTo* moveTo1 = CCMoveTo::create(0.5, ccp(size.width * 0.75, size.height * 0.75));
    CCMoveTo* moveTo2 = CCMoveTo::create(0.5, ccp(size.width * 0.75, size.height * 0.65));
    CCMoveTo* moveTo3 = CCMoveTo::create(0.5, ccp(size.width * 0.75, size.height * 0.52));
    CCEaseElasticOut* easeElastic1 = CCEaseElasticOut::create(moveTo1);
    CCEaseElasticOut* easeElastic2 = CCEaseElasticOut::create(moveTo2);
    CCEaseElasticOut* easeElastic3 = CCEaseElasticOut::create(moveTo3);
    points->runAction(easeElastic1);
    pointsWithTime->runAction(easeElastic2);
    totalPoints->runAction(easeElastic3);
    
    /*当训练模式超过15关后不得继续游戏*/
    if (!CCUserDefault::sharedUserDefault()->getBoolForKey("ifEndlessMode") && ifWin && Interface::sharedInterface()->level1 >= 15) {
        scheduleOnce(schedule_selector(DisplayPoints::loseChoice), 1.5);
    }
    
    if (ifWin) {
        /*展示兑换天赋点数*/
        scheduleOnce(schedule_selector(DisplayPoints::displayExchange), 1.5);
    }
    else {
        if (CCUserDefault::sharedUserDefault()->getBoolForKey("ifEndlessMode")) {
            /*退出游戏*/
            scheduleOnce(schedule_selector(DisplayPoints::loseGame), 1.5);
        }
        else {
            /*重玩本关和退出游戏*/
            scheduleOnce(schedule_selector(DisplayPoints::loseChoice), 1.5);
        }
    }
}

void DisplayPoints::displayExchange()
{
    float time = CCUserDefault::sharedUserDefault()->getFloatForKey("time", 0);
    if (time > 125) {
        time = 125;
    }
    several += (Data::sharedData()->points + 5000 - time * Interface::sharedInterface()->addPointsWithTime) / Interface::sharedInterface()->exchangeRate;
    
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("display.mp3");
    }
    
    /*标题*/
    CCLabelTTF* label = CCLabelTTF::create("兑换天赋点数：", "Verdana-Bold", 45);
    CCLabelTTF* data = CCLabelTTF::create(CCString::createWithFormat("%d", (int)several)->getCString(), "Georgia-Bold", 45);
    addChild(label);
    addChild(data);
    
    /*设置属性*/
    label->setPosition(ccp(size.width * 0.35, size.height*0.1));
    data->setPosition(ccp(size.width * 0.75, size.height*0.1));
    
    label->setColor(ccc3(11, 23, 70));
    data->setColor(ccc3(255, 0, 0));
    
    /*动作*/
    CCMoveTo* moveTo1 = CCMoveTo::create(0.5, ccp(size.width * 0.35, size.height*0.4));
    CCMoveTo* moveTo2 = CCMoveTo::create(0.5, ccp(size.width * 0.75, size.height*0.4));
    CCEaseElasticOut* easeElastic1 = CCEaseElasticOut::create(moveTo1);
    CCEaseElasticOut* easeElastic2 = CCEaseElasticOut::create(moveTo2);
    label->runAction(easeElastic1);
    data->runAction(easeElastic2);
    
    /*更新天赋点数*/
    CCUserDefault::sharedUserDefault()->setFloatForKey("several", several);
    
    scheduleOnce(schedule_selector(DisplayPoints::winChoice), 1.5);
}

void DisplayPoints::winChoice()
{
    CCMenuItemImage *itemTalent = CCMenuItemImage::create("goInhesion.png", "goInhesion.png", this, menu_selector(DisplayPoints::inhesionScene));
    CCMenuItemImage *itemBack = CCMenuItemImage::create("display_back.png", "display_back.png", this, menu_selector(DisplayPoints::menuScene));
    
    CCMenu *menu = CCMenu::create(itemTalent, itemBack, NULL);
    addChild(menu);
    menu->setScale(0.65);
    menu->setPosition(size.width * 0.34, size.height * 0.05);
    menu->alignItemsHorizontallyWithPadding(150);
    menu->setColor(ccc3(0, 32, 240));
}

void DisplayPoints::loseChoice()
{
    CCMenuItemImage *itemReplay = CCMenuItemImage::create("replay.png", "replay.png", this, menu_selector(DisplayPoints::wordScene));
    CCMenuItemImage *itemBack = CCMenuItemImage::create("display_back.png", "display_back.png", this, menu_selector(DisplayPoints::menuScene));
    CCMenu *menu = CCMenu::create(itemReplay, itemBack, NULL);
    addChild(menu);
    menu->setScale(0.65);
    menu->setPosition(size.width * 0.34, size.height * 0.1);
    menu->alignItemsHorizontallyWithPadding(150);
    menu->setColor(ccc3(0, 32, 240));
}

void DisplayPoints::loseGame()
{
    CCMenuItemImage *itemBack = CCMenuItemImage::create("display_back.png", "display_back.png", this, menu_selector(DisplayPoints::menuScene));
    CCMenu *menu = CCMenu::create(itemBack, NULL);
    addChild(menu);
    menu->setScale(0.65);
    menu->setPosition(size.width * 0.34, size.height * 0.1);
    menu->setColor(ccc3(0, 32, 240));
}

void DisplayPoints::inhesionScene()
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1, Inhesion::scene()));
}

void DisplayPoints::wordScene()
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1, World::scene()));
}

void DisplayPoints::menuScene()
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1, Menu::scene()));
}