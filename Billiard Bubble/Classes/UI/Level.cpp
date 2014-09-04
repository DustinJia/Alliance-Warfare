#include "Level.h"
#include "Menu.h"
#include "World.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene* Level::scene()
{
    CCScene* scene = CCScene::create();
    Level* layer = Level::create();
    scene->addChild(layer);
    return scene;
}

bool Level::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("confirm.mp3");
    
    /*添加背景*/
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* spBackGround = CCSprite::create("levelBackground.png");
    spBackGround->setPosition(ccp(size.width*0.5, size.height*0.5));
    addChild(spBackGround);
    
    /*创建图片菜单项*/
    CCMenuItemImage *item1 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel1));
    CCMenuItemImage *item2 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel2));
    CCMenuItemImage *item3 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel3));
    CCMenuItemImage *item4 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel4));
    CCMenuItemImage *item5 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel5));
    
    /*创建返回菜单项*/
    CCMenuItemImage *itemBack = CCMenuItemImage::create("display_back.png", "display_back.png", this, menu_selector(Level::backToMenu));;
    
    /*设置菜单项大小*/
    item1->setScale(0.45);
    item2->setScale(0.45);
    item3->setScale(0.45);
    item4->setScale(0.45);
    item5->setScale(0.45);
    
    /*创建菜单*/
    CCMenu* menu1 = CCMenu::create(item1, item2, item3, item4, item5, NULL);
    menu1->setPosition(ccp(size.width * 0.5, size.height * 0.55));
    menu1->alignItemsHorizontallyWithPadding(70);
    
    CCMenu* menuBack = CCMenu::create(itemBack, NULL);
    addChild(menuBack);
    menuBack->setPosition(ccp(size.width * 0.36, size.height * 0.05));
    menuBack->setScale(0.7);
    menuBack->setColor(ccc3(0, 32, 240));
    
    /*创建图片菜单项*/
    CCMenuItemImage *item6 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel6));
    CCMenuItemImage *item7 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel7));
    CCMenuItemImage *item8 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel8));
    CCMenuItemImage *item9 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel9));
    CCMenuItemImage *item10 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel10));
    
    /*设置菜单项大小*/
    item6->setScale(0.45);
    item7->setScale(0.45);
    item8->setScale(0.45);
    item9->setScale(0.45);
    item10->setScale(0.45);
    
    /*创建菜单*/
    CCMenu* menu2 = CCMenu::create(item6, item7, item8, item9, item10, NULL);
    
    menu2->setPosition(ccp(size.width * 1.5, size.height * 0.55));
    menu2->alignItemsHorizontallyWithPadding(70);
    
    /*-----------------------*/
    /*创建图片菜单项*/
    CCMenuItemImage *item11 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel11));
    CCMenuItemImage *item12 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel12));
    CCMenuItemImage *item13 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel13));
    CCMenuItemImage *item14 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel14));
    CCMenuItemImage *item15 = CCMenuItemImage::create("level_normal.png", "level_selected.png", this, menu_selector(Level::chooseLevel15));
    
    /*设置菜单项大小*/
    item11->setScale(0.45);
    item12->setScale(0.45);
    item13->setScale(0.45);
    item14->setScale(0.45);
    item15->setScale(0.45);
    
    /*创建菜单*/
    CCMenu* menu3 = CCMenu::create(item11, item12, item13, item14, item15, NULL);
    
    menu3->setPosition(ccp(size.width * 2.5, size.height * 0.55));
    menu3->alignItemsHorizontallyWithPadding(70);
    
    /*添加关卡锁*/
    lock2 = CCSprite::create("lock.png");
    lock3 = CCSprite::create("lock.png");
    lock4 = CCSprite::create("lock.png");
    lock5 = CCSprite::create("lock.png");
    lock6 = CCSprite::create("lock.png");
    lock7 = CCSprite::create("lock.png");
    lock8 = CCSprite::create("lock.png");
    lock9 = CCSprite::create("lock.png");
    lock10 = CCSprite::create("lock.png");
    lock11 = CCSprite::create("lock.png");
    lock12 = CCSprite::create("lock.png");
    lock13 = CCSprite::create("lock.png");
    lock14 = CCSprite::create("lock.png");
    lock15 = CCSprite::create("lock.png");
    
    item2->addChild(lock2);
    item3->addChild(lock3);
    item4->addChild(lock4);
    item5->addChild(lock5);
    item6->addChild(lock6);
    item7->addChild(lock7);
    item8->addChild(lock8);
    item9->addChild(lock9);
    item10->addChild(lock10);
    item11->addChild(lock11);
    item12->addChild(lock12);
    item13->addChild(lock13);
    item14->addChild(lock14);
    item15->addChild(lock15);
    
    addLock();
    
    //创建层容器
    CCLayer * containLayer = CCLayer::create();
    containLayer->addChild(menu1);
    containLayer->addChild(menu2);
    containLayer->addChild(menu3);
    containLayer->setContentSize(CCSizeMake(size.width * 4, size.height));
    
    /*添加关数*/
    CCLabelTTF* level1 = CCLabelTTF::create("1st", "Georgia-Bold", 45);
    CCLabelTTF* level2 = CCLabelTTF::create("2nd", "Georgia-Bold", 45);
    CCLabelTTF* level3 = CCLabelTTF::create("3rd", "Georgia-Bold", 45);
    CCLabelTTF* level4 = CCLabelTTF::create("4th", "Georgia-Bold", 45);
    CCLabelTTF* level5 = CCLabelTTF::create("5th", "Georgia-Bold", 45);
    CCLabelTTF* level6 = CCLabelTTF::create("6th", "Georgia-Bold", 45);
    CCLabelTTF* level7 = CCLabelTTF::create("7th", "Georgia-Bold", 45);
    CCLabelTTF* level8 = CCLabelTTF::create("8th", "Georgia-Bold", 45);
    CCLabelTTF* level9 = CCLabelTTF::create("9th", "Georgia-Bold", 45);
    CCLabelTTF* level10 = CCLabelTTF::create("10th", "Georgia-Bold", 45);
    CCLabelTTF* level11 = CCLabelTTF::create("11th", "Georgia-Bold", 45);
    CCLabelTTF* level12 = CCLabelTTF::create("12th", "Georgia-Bold", 45);
    CCLabelTTF* level13 = CCLabelTTF::create("13th", "Georgia-Bold", 45);
    CCLabelTTF* level14 = CCLabelTTF::create("14th", "Georgia-Bold", 45);
    CCLabelTTF* level15 = CCLabelTTF::create("15th", "Georgia-Bold", 45);
    
    containLayer->addChild(level1);
    containLayer->addChild(level2);
    containLayer->addChild(level3);
    containLayer->addChild(level4);
    containLayer->addChild(level5);
    containLayer->addChild(level6);
    containLayer->addChild(level7);
    containLayer->addChild(level8);
    containLayer->addChild(level9);
    containLayer->addChild(level10);
    containLayer->addChild(level11);
    containLayer->addChild(level12);
    containLayer->addChild(level13);
    containLayer->addChild(level14);
    containLayer->addChild(level15);
    
    level1->setPosition(ccp(size.width * 0.18, size.height * 0.4));
    level2->setPosition(ccp(size.width * 0.34, size.height * 0.4));
    level3->setPosition(ccp(size.width * 0.5, size.height * 0.4));
    level4->setPosition(ccp(size.width * 0.657, size.height * 0.4));
    level5->setPosition(ccp(size.width * 0.82, size.height * 0.4));
    level6->setPosition(ccp(size.width * 1.18, size.height * 0.4));
    level7->setPosition(ccp(size.width * 1.34, size.height * 0.4));
    level8->setPosition(ccp(size.width * 1.5, size.height * 0.4));
    level9->setPosition(ccp(size.width * 1.657, size.height * 0.4));
    level10->setPosition(ccp(size.width * 1.82, size.height * 0.4));
    level11->setPosition(ccp(size.width * 2.18, size.height * 0.4));
    level12->setPosition(ccp(size.width * 2.34, size.height * 0.4));
    level13->setPosition(ccp(size.width * 2.5, size.height * 0.4));
    level14->setPosition(ccp(size.width * 2.657, size.height * 0.4));
    level15->setPosition(ccp(size.width * 2.82, size.height * 0.4));
    
    /*创建滚动视图*/
    scrollView = CCScrollView::create(CCSizeMake(size.width*3, size.height));
    addChild(scrollView);
    
    scrollView->setBounceable(false);//设置scrollview有弹跳效果(默认为true)
    scrollView->setViewSize(CCSizeMake(size.width*2, size.height));//设置scrollView尺寸
    scrollView->setContainer(containLayer);//设置容器
    scrollView->getViewSize();//获取视图尺寸
    
    scrollView->setTouchEnabled(true);//开启监听多触点
    scrollView->setDelegate(this);//注册监听
    
    return true;
}


#pragma mark - 关卡锁
void Level::addLock()
{
    lock2->setScale(0.09);
    lock3->setScale(0.09);
    lock4->setScale(0.09);
    lock5->setScale(0.09);
    lock6->setScale(0.09);
    lock7->setScale(0.09);
    lock8->setScale(0.09);
    lock9->setScale(0.09);
    lock10->setScale(0.09);
    lock11->setScale(0.09);
    lock12->setScale(0.09);
    lock13->setScale(0.09);
    lock14->setScale(0.09);
    lock15->setScale(0.09);
    
    lock2->setPosition(ccp(220, 40));
    lock3->setPosition(ccp(220, 40));
    lock4->setPosition(ccp(220, 40));
    lock5->setPosition(ccp(220, 40));
    lock6->setPosition(ccp(220, 40));
    lock7->setPosition(ccp(220, 40));
    lock8->setPosition(ccp(220, 40));
    lock9->setPosition(ccp(220, 40));
    lock10->setPosition(ccp(220, 40));
    lock11->setPosition(ccp(220, 40));
    lock12->setPosition(ccp(220, 40));
    lock13->setPosition(ccp(220, 40));
    lock14->setPosition(ccp(220, 40));
    lock15->setPosition(ccp(220, 40));
}

void Level::save()
{
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve2", ifLock2);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve3", ifLock3);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve4", ifLock4);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve5", ifLock5);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve6", ifLock6);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve7", ifLock7);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve8", ifLock8);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve9", ifLock9);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve10", ifLock10);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve11", ifLock11);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve12", ifLock12);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve13", ifLock13);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve14", ifLock14);
    CCUserDefault::sharedUserDefault()->setBoolForKey("leve15", ifLock15);
    
    CCUserDefault::sharedUserDefault()->flush();
}

void Level::read()
{
    ifLock2 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve2", true);
    ifLock3 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve3", true);
    ifLock4 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve4", true);
    ifLock5 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve5", true);
    ifLock6 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve6", true);
    ifLock7 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve7", true);
    ifLock8 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve8", true);
    ifLock9 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve9", true);
    ifLock10 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve10", true);
    ifLock11 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve11", true);
    ifLock12 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve12", true);
    ifLock13 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve13", true);
    ifLock14 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve14", true);
    ifLock15 = CCUserDefault::sharedUserDefault()->getBoolForKey("leve15",true);
    level = CCUserDefault::sharedUserDefault()->getIntegerForKey("level1", 1);
}

void Level::set()
{
    /*设置BOOL值*/
    switch (level) {
        case 1:
            ifLock2 = true;
            ifLock3 = true;
            ifLock4 = true;
            ifLock5 = true;
            ifLock6 = true;
            ifLock7 = true;
            ifLock8 = true;
            ifLock9 = true;
            ifLock10 = true;
            ifLock11 = true;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 2:
            ifLock2 = false;
            ifLock3 = true;
            ifLock4 = true;
            ifLock5 = true;
            ifLock6 = true;
            ifLock7 = true;
            ifLock8 = true;
            ifLock9 = true;
            ifLock10 = true;
            ifLock11 = true;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 3:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = true;
            ifLock5 = true;
            ifLock6 = true;
            ifLock7 = true;
            ifLock8 = true;
            ifLock9 = true;
            ifLock10 = true;
            ifLock11 = true;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 4:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = true;
            ifLock6 = true;
            ifLock7 = true;
            ifLock8 = true;
            ifLock9 = true;
            ifLock10 = true;
            ifLock11 = true;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 5:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = true;
            ifLock7 = true;
            ifLock8 = true;
            ifLock9 = true;
            ifLock10 = true;
            ifLock11 = true;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 6:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = false;
            ifLock7 = true;
            ifLock8 = true;
            ifLock9 = true;
            ifLock10 = true;
            ifLock11 = true;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 7:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = false;
            ifLock7 = false;
            ifLock8 = true;
            ifLock9 = true;
            ifLock10 = true;
            ifLock11 = true;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 8:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = false;
            ifLock7 = false;
            ifLock8 = false;
            ifLock9 = true;
            ifLock10 = true;
            ifLock11 = true;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 9:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = false;
            ifLock7 = false;
            ifLock8 = false;
            ifLock9 = false;
            ifLock10 = true;
            ifLock11 = true;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 10:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = false;
            ifLock7 = false;
            ifLock8 = false;
            ifLock9 = false;
            ifLock10 = false;
            ifLock11 = true;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 11:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = false;
            ifLock7 = false;
            ifLock8 = false;
            ifLock9 = false;
            ifLock10 = false;
            ifLock11 = false;
            ifLock12 = true;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 12:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = false;
            ifLock7 = false;
            ifLock8 = false;
            ifLock9 = false;
            ifLock10 = false;
            ifLock11 = false;
            ifLock12 = false;
            ifLock13 = true;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 13:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = false;
            ifLock7 = false;
            ifLock8 = false;
            ifLock9 = false;
            ifLock10 = false;
            ifLock11 = false;
            ifLock12 = false;
            ifLock13 = false;
            ifLock14 = true;
            ifLock15 = true;
            break;
        case 14:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = false;
            ifLock7 = false;
            ifLock8 = false;
            ifLock9 = false;
            ifLock10 = false;
            ifLock11 = false;
            ifLock12 = false;
            ifLock13 = false;
            ifLock14 = false;
            ifLock15 = true;
            break;
        case 15:
            ifLock2 = false;
            ifLock3 = false;
            ifLock4 = false;
            ifLock5 = false;
            ifLock6 = false;
            ifLock7 = false;
            ifLock8 = false;
            ifLock9 = false;
            ifLock10 = false;
            ifLock11 = false;
            ifLock12 = false;
            ifLock13 = false;
            ifLock14 = false;
            ifLock15 = false;
            break;
        default:
            break;
    }
    
    /*设置可见*/
    if (ifLock2) {
        lock2->setVisible(true);
    }
    else if (!ifLock2) {
        lock2->setVisible(false);
    }
    if (ifLock3) {
        lock3->setVisible(true);
    }
    else if (!ifLock3) {
        lock3->setVisible(false);
    }
    if (ifLock4) {
        lock4->setVisible(true);
    }
    else if (!ifLock4) {
        lock4->setVisible(false);
    }
    if (ifLock5) {
        lock5->setVisible(true);
    }
    else if (!ifLock5) {
        lock5->setVisible(false);
    }
    if (ifLock6) {
        lock6->setVisible(true);
    }
    else if (!ifLock6) {
        lock6->setVisible(false);
    }
    if (ifLock7) {
        lock7->setVisible(true);
    }
    else if (!ifLock7) {
        lock7->setVisible(false);
    }
    if (ifLock8) {
        lock8->setVisible(true);
    }
    else if (!ifLock8) {
        lock8->setVisible(false);
    }
    if (ifLock9) {
        lock9->setVisible(true);
    }
    else if (!ifLock9) {
        lock9->setVisible(false);
    }
    if (ifLock10) {
        lock10->setVisible(true);
    }
    else if (!ifLock10) {
        lock10->setVisible(false);
    }
    if (ifLock11) {
        lock11->setVisible(true);
    }
    else if (!ifLock11) {
        lock11->setVisible(false);
    }
    if (ifLock12) {
        lock12->setVisible(true);
    }
    else if (!ifLock12) {
        lock12->setVisible(false);
    }
    if (ifLock13) {
        lock13->setVisible(true);
    }
    else if (!ifLock13) {
        lock13->setVisible(false);
    }
    if (ifLock14) {
        lock14->setVisible(true);
    }
    else if (!ifLock14) {
        lock14->setVisible(false);
    }
    if (ifLock15) {
        lock15->setVisible(true);
    }
    else if (!ifLock15) {
        lock15->setVisible(false);
    }
}


#pragma mark - 选关
void Level::chooseLevel1()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }
    
    /*切换到游戏场景*/
    Interface::sharedInterface()->level1 = 1;
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
}

void Level::chooseLevel2()
{
    if (level >= 2) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 2;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel3()
{
    if (level >= 3) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 3;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel4()
{
    if (level >= 4) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 4;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel5()
{
    if (level >= 5) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 5;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel6()
{
    if (level >= 6) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 6;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel7()
{
    if (level >= 7) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 7;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel8()
{
    if (level >= 8) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 8;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel9()
{
    if (level >= 9) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 9;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel10()
{
    if (level >= 10) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 10;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel11()
{
    if (level >= 11) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 11;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel12()
{
    if (level >= 12) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 12;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel13()
{
    if (level >= 13) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 13;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel14()
{
    if (level >= 14) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 14;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::chooseLevel15()
{
    if (level >= 1) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
        }
        /*切换到游戏场景*/
        Interface::sharedInterface()->level1 = 15;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }
}

void Level::backToMenu()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }
    
    /*切换到主菜单场景*/
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5, Menu::scene()));
}


#pragma mark - 触摸事件
bool Level::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    return true;
}

void Level::ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    adjustScrollView();
}


#pragma mark - 滚动视图相关
void Level::scrollViewDidScroll(CCScrollView* view){
    
}

void Level::scrollViewDidZoom(CCScrollView* view){
    
}

void Level::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    adjustScrollView();
}

void Level::adjustScrollView()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // 关闭CCScrollView中的自调整
    scrollView->unscheduleAllSelectors();
    int x =scrollView->getContentOffset().x;
    int offset = (int) x % (int)(size.width);
    static bool ifSlided = false;
    // 调整位置
    CCPoint adjustPos;
    // 调整动画时间
    float adjustAnimDelay;
    //   向右滑动是正向左滑动是负
    if (offset < -100 && !ifSlided)
    {
        // 计算下一页位置，时间
        adjustPos =
        ccpSub(scrollView->getContentOffset(), ccp((int)(size.width) + offset, 0));
        adjustAnimDelay = (float) (1136 +offset) / 1500;
        ifSlided = true;
        
    }
    else if(offset < -100 && ifSlided){
        // 计算当前页位置，时间
        adjustPos =ccpSub(scrollView->getContentOffset(), ccp(offset, 0));
        // 这里要取绝对值，否则在第一页往左翻动的时，保证adjustAnimDelay为正数
        adjustAnimDelay = (float) abs(offset) /1500;
        ifSlided = false;
    }
    // 调整位置
    adjustPos.y = 0;
    scrollView->setContentOffsetInDuration(adjustPos,adjustAnimDelay);
}


#pragma mark - 生命周期相关
void Level::onEnter()
{
    read();
    set();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 10, true);
    CCLayer::onEnter();
}

void Level::onExit()
{
    save();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}