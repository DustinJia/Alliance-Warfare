#include "Setup.h"
#include "SimpleAudioEngine.h"
#include "Menu.h"
#include "Interface.h"
using namespace CocosDenshion;

typedef enum
{
    slider_tag,
    ttf_tag,
}Tag;


CCScene *Setup::scene()
{
    CCScene *scene = CCScene::create();
    Setup *layer = Setup::create();
    scene->addChild(layer);
    return scene;
}

bool Setup::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    /*添加背景*/
    CCSprite *background = CCSprite::create("set.jpg");
    background->setPosition(ccp(size.width*0.5, size.height*0.5));
    addChild(background);
    
    CCMenuItemImage *pauseMusic = CCMenuItemImage::create("set_stopMusic_normal.png", "set_stopMusic_normal.png");
    CCMenuItemImage *resumeMusic = CCMenuItemImage::create("set_playMusic_normal.png", "set_playMusic_normal.png");
    pauseMusic->setScale(0.5);
    resumeMusic->setScale(0.5);
    CCMenuItemToggle *musicItem = CCMenuItemToggle::createWithTarget(this, menu_selector(Setup::resumeMusic), pauseMusic, resumeMusic, NULL);
    musicItem->setPosition(ccp(-size.width*0.13, size.height*0.2));
    
    CCMenuItemImage *pauseVolume = CCMenuItemImage::create("set_stopEffect_normal.png", "set_stopEffect_normal.png");
    CCMenuItemImage *resumeVolume = CCMenuItemImage::create("set_playEffect_normal.png", "set_playEffect_normal.png");
    pauseVolume->setScale(0.5);
    resumeVolume->setScale(0.5);
    CCMenuItemToggle *effctsItem = CCMenuItemToggle::createWithTarget(this, menu_selector(Setup::effetsVolume), pauseVolume, resumeVolume, NULL);
    effctsItem->setPosition(ccp(size.width*0.13, size.height*0.2));
    
    CCMenuItemImage *backItem = CCMenuItemImage::create("set_back_normal.png", "set_back_normal.png", this, menu_selector(Setup::backMenu));
    backItem->setPosition(ccp(0, -size.height * 0.27));
    backItem->setScale(0.5);
    
    CCMenu *menu = CCMenu::create(musicItem, effctsItem, backItem, NULL);
    addChild(menu);
    
    /*添加滑动条*/
    CCSprite *spBackground = CCSprite::create("sliderBg.png");
    CCSprite *spProgress = CCSprite::create("sliderProgress.png");
    CCSprite *spThum = CCSprite::create("sliderThumb.png");
    spThum->setScaleX(1.5);
    slider = CCControlSlider::create(spBackground, spProgress, spThum);
    addChild(slider);
    //设置滑动条属性
    slider->setPosition(ccp(size.width*0.5, size.height*0.41));
    slider->setMaximumValue(5);
    slider->setMinimumValue(1);
    slider->setTag(slider_tag);
    slider->setScaleX(0.6);
    slider->setValue(CCUserDefault::sharedUserDefault()->getFloatForKey("slider", 3));
    //设置监听，当滑动条的值发生变化后，会响应valueChanged函数
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Setup::sensitivity), CCControlEventValueChanged);
    
    //用于展示当前滑动条的滑动值
    CCSprite* slide = CCSprite::create("set_touch_normal.png");
    addChild(slide);
    slide->setPosition(ccp(size.width*0.5, size.height*0.51));
    slide->setScale(0.5);
    CCLabelTTF * ttf = CCLabelTTF::create("%.0f", "Thonburi", 40);
    ttf->setPosition(ccp(slide->getPositionX(), slide->getPositionY() - slide->boundingBox().size.height * 2.1));
    ttf->setString(CCString::createWithFormat("%.0f", slider->getValue())->getCString());
    ttf->setTag(ttf_tag);
    ttf->setColor(ccc3(41, 36, 33));
    addChild(ttf);
    
    /*初始化*/
    rusumMusic = true;
    effects = true;
    slider->setValue(CCUserDefault::sharedUserDefault()->getFloatForKey("slider", 3));
    
    return true;
}

void Setup::backMenu()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }
    CCUserDefault::sharedUserDefault()->setBoolForKey("ifPlayMusic", Interface::sharedInterface()->ifPlayMusic);
    CCUserDefault::sharedUserDefault()->setBoolForKey("ifPlayEffect", Interface::sharedInterface()->ifPlayEffect);
    CCUserDefault::sharedUserDefault()->setFloatForKey("slider", slider->getValue());
    CCUserDefault::sharedUserDefault()->flush();
    
    CCDirector::sharedDirector()->popScene();
}

void Setup::resumeMusic()
{
    if(rusumMusic){
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        Interface::sharedInterface()->ifPlayMusic = false;
    }
    else
    {
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
        Interface::sharedInterface()->ifPlayMusic = true;
    }
    rusumMusic = !rusumMusic;
}

void Setup::sensitivity()
{
    CCControlSlider* slider= (CCControlSlider*)this->getChildByTag(slider_tag);
    CCLabelTTF* ttf = (CCLabelTTF*)this->getChildByTag(ttf_tag);
    ttf->setString(CCString::createWithFormat("%.0f", slider->getValue())->getCString());
}

void Setup::effetsVolume()
{
    if(effects) {
        Interface::sharedInterface()->ifPlayEffect = false;
    }
    else {
        Interface::sharedInterface()->ifPlayEffect = true;
    }
    effects = !effects;
}