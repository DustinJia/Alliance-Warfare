#include "About.h"
#include "Menu.h"
#include "SimpleAudioEngine.h"
#include "Interface.h"

using namespace CocosDenshion;

CCScene* About::scene()
{
    CCScene* scene = CCScene::create();
    About* layer = About::create();
    scene->addChild(layer);
    return scene;
}

bool About::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("confirm.mp3");
    
    size = CCDirector::sharedDirector()->getWinSize();
    /*添加背景图片*/
    CCSprite* sprite = CCSprite::create("about.png");
    sprite->setPosition(ccp(size.width*0.5, size.height*0.5));
    addChild(sprite);
    sprite->setScaleX((float)size.width / 1136);
    sprite->setScaleY((float)size.height / 640);
    
    CCMenuItemImage* itemLabel = CCMenuItemImage::create("backToMenu.png", "backToMenu.png", this, menu_selector(About::backMenu));
    itemLabel->setPosition(ccp(0, -size.height*0.4));
    itemLabel->setScale(0.65);
    CCMenu* menu = CCMenu::create(itemLabel, NULL);
    addChild(menu);
    
    return true;
}

/*返回主菜单*/
void About::backMenu()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }

    CCDirector::sharedDirector()->popScene();
}