#include "Teach0.h"
#include "Teach1.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene* Teach0::scene()
{
    CCScene* scene = CCScene::create();
    Teach0* layer = Teach0::create();
    scene->addChild(layer);
    return scene;
}

bool Teach0::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    size = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* background = CCSprite::create("teach.png");
    addChild(background);
    background->setPosition(ccp(size.width * 0.5, size.height * 0.5));
    background->setScaleX((float)size.width / 1136);
    background->setScaleY((float)size.height / 640);
    
    CCMenuItemImage* item = CCMenuItemImage::create("direction.png", "direction.png", this, menu_selector(Teach0::nextPage));
    CCMenu* menu = CCMenu::create(item, NULL);
    addChild(menu);
    menu->setPosition(size.width * 0.93, size.height * 0.1);
    
    return true;
}

void Teach0::nextPage()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(1, Teach1::scene()));
}