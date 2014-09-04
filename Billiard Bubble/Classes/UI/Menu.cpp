#include "Menu.h"
#include "SimpleAudioEngine.h"
#include "Level.h"
#include "Inhesion.h"
#include "World.h"
#include "Interface.h"
#include "About.h"
#include "Setup.h"
#include "Help.h"
#include "Pause.h"
#include "Teach0.h"

using namespace CocosDenshion;

/*标签的枚举变量*/
typedef enum
{
    tag_fish,
}Tag_Menu;

static Menu* instance = NULL;

Menu* Menu::sharedMenu()
{
    if (instance == NULL) {
        instance = new Menu();
    }
    return instance;
}

CCScene* Menu::scene()
{
    CCScene* scene = CCScene::create();
    Menu* layer = Menu::create();
    scene->addChild(layer);
    return scene;
}

bool Menu::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    /*播放音乐*/
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("ifPlayMusic", true)) {
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("menu.mp3", true);
    }
    
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("confirm.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("click1.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("click2.mp3");
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*获取本类的单例对象*/
    instance = this;
    
    /*添加背景*/
    CCSprite* backGround = CCSprite::create("menu.png");
    backGround->setPosition(ccp(size.width * 0.5, size.height * 0.5));
    addChild(backGround);
    
    /*创建菜单*/
    CCMenuItemImage* itemPlay = CCMenuItemImage::create("play_normal.png", "play_selected.png", this, menu_selector(Menu::pressPlay));
    CCMenuItemImage* itemChoose = CCMenuItemImage::create("choose_normal.png", "choose_selected.png", this, menu_selector(Menu::pressChoose));
    CCMenuItemImage* itemInhesion = CCMenuItemImage::create("inhesion_normal.png", "inhesion_selected.png", this, menu_selector(Menu::pressInhesion));
    CCMenuItemImage* itemSet = CCMenuItemImage::create("set_normal.png", "set_selected.png", this, menu_selector(Menu::pressSet));
    CCMenuItemImage* itemHelp = CCMenuItemImage::create("help_normal.png", "help_selected.png", this, menu_selector(Menu::pressHelp));
    CCMenuItemImage* itemAbout = CCMenuItemImage::create("about_normal.png", "about_selected.png", this, menu_selector(Menu::pressAbout));
    
    itemPlay->setScale(0.44);
    itemChoose->setScale(0.45);
    itemInhesion->setScale(0.45);
    itemSet->setScale(0.44);
    itemAbout->setScale(0.45);
    itemHelp->setScale(0.44);
    
    CCMenu* menu1 = CCMenu::create(itemPlay, itemChoose, itemInhesion, NULL);
    CCMenu* menu2 = CCMenu::create(itemSet, itemHelp, itemAbout, NULL);
    addChild(menu1);
    addChild(menu2);
    menu1->setPosition(size.width * 0.3, size.height * 0.57);
    menu2->setPosition(size.width * 0.7, size.height * 0.57);
    menu1->alignItemsVerticallyWithPadding(50);
    menu2->alignItemsVerticallyWithPadding(50);
        
    /*初始化*/
    Interface::sharedInterface()->ifPlayMusic = CCUserDefault::sharedUserDefault()->getBoolForKey("ifPlayMusic", true);
    Interface::sharedInterface()->ifPlayEffect = CCUserDefault::sharedUserDefault()->getBoolForKey("ifPlayEffect", true);
        
    return true;
}


#pragma mark - 切换场景
void Menu::pressPlay()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3");
    }
    
    /*更新数据*/
    if (!CCUserDefault::sharedUserDefault()->getBoolForKey("ifWin")) {
        refreshData();
    }
    /*记录模式*/
    CCUserDefault::sharedUserDefault()->setBoolForKey("ifEndlessMode", true);
    CCUserDefault::sharedUserDefault()->flush();
    
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("ifFirstTime", true)) {
        /*切换到教学场景*/
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, Teach0::scene()));
    }
    else {
        /*切换到游戏场景*/
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, World::scene()));
    }

}

void Menu::pressChoose()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3");
    }
    
    /*记录模式*/
    CCUserDefault::sharedUserDefault()->setBoolForKey("ifEndlessMode", false);
    CCUserDefault::sharedUserDefault()->flush();
    
    /*切换到游戏场景*/
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, Level::scene()));
}

void Menu::pressInhesion()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }
    
    /*切换到分配点数场景*/
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(1, Inhesion::scene()));
    Inhesion::sharedInhesion()->ifFromMenu = true;
}

void Menu::pressAbout()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }
    
    /*切换到关于场景*/
    CCDirector::sharedDirector()->pushScene(CCTransitionCrossFade::create(1, About::scene()));
}

void Menu::pressSet()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }
    
    /*切换到设置场景*/
    CCDirector::sharedDirector()->pushScene(CCTransitionCrossFade::create(1, Setup::scene()));
}

void Menu::pressHelp()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }
    
    /*切换到帮助场景*/
    CCDirector::sharedDirector()->pushScene(CCTransitionCrossFade::create(1, Teach0::scene()));
}


void Menu::refreshData()
{
    Interface::sharedInterface()->level = 1;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("level", 1);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data1", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data2", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data3", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data4", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data5", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data6", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data7", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data8", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data9", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("several", 0);
    
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion1", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion2", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion3", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion4", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion5", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion6", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion7", 1);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion8", 1);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion9", 1);

    CCUserDefault::sharedUserDefault()->flush();
}

#pragma mark - 生命周期相关
void Menu::onEnter()
{
    CCLayer::onEnter();
}