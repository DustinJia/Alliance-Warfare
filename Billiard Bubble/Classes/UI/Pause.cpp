#include "Pause.h"
#include "Menu.h"
#include "SimpleAudioEngine.h"
#include "World.h"
#include "Interface.h"
#include "Data.h"
#include "SubsidiaryLayer.h"
#include "Setup.h"

using namespace CocosDenshion;

typedef enum
{
    slider_tag,
    ttf_tag,
}Tag;

static Pause* instance = NULL;

Pause* Pause::sharedPause()
{
    if (instance == NULL) {
        instance = new Pause();
    }
    return instance;
}

CCScene *Pause::scene()
{
    CCScene *scene = CCScene::create();
    Pause *layer = Pause::create();
    scene->addChild(layer);
    return scene;
}

bool Pause::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    slider = Interface::sharedInterface()->slider;
    
    /*获取本类的单例对象*/
    instance = this;
    
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("confirm.mp3");
    
    /*获取屏幕尺寸*/
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    /*添加背景*/
    CCSprite *background = CCSprite::create("pause.jpg");
    background->setPosition(ccp(size.width*0.5, size.height*0.5));
    addChild(background);
    
    CCMenuItemImage *goOnItem = CCMenuItemImage::create("set_goOn_normal.png", "set_goOn_normal.png", this, menu_selector(Pause::resume));
    goOnItem->setPosition(ccp(0, size.height*0.3));
    goOnItem->setScale(0.5);
    
    CCMenuItemImage *pauseMusic = CCMenuItemImage::create("set_stopMusic_normal.png", "set_stopMusic_normal.png");
    CCMenuItemImage *resumeMusic = CCMenuItemImage::create("set_playMusic_normal.png", "set_playMusic_normal.png");
    pauseMusic->setScale(0.5);
    resumeMusic->setScale(0.5);
    CCMenuItemToggle *musicItem = CCMenuItemToggle::createWithTarget(this, menu_selector(Pause::resumeMusic), pauseMusic, resumeMusic, NULL);
    musicItem->setPosition(ccp(-size.width*0.13, size.height*0.15));
    
    CCMenuItemImage *pauseVolume = CCMenuItemImage::create("set_stopEffect_normal.png", "set_stopEffect_normal.png");
    CCMenuItemImage *resumeVolume = CCMenuItemImage::create("set_playEffect_normal.png", "set_playEffect_normal.png");
    pauseVolume->setScale(0.5);
    resumeVolume->setScale(0.5);
    CCMenuItemToggle *effctsItem = CCMenuItemToggle::createWithTarget(this, menu_selector(Pause::effetsVolume), pauseVolume, resumeVolume, NULL);
    effctsItem->setPosition(ccp(size.width*0.13, size.height*0.15));
    
    CCMenuItemImage *backItem = CCMenuItemImage::create("set_back_normal.png", "set_back_normal.png", this, menu_selector(Pause::backMenu));
    backItem->setPosition(ccp(0, -size.height * 0.27));
    backItem->setScale(0.5);
    
    CCMenu *menu = CCMenu::create(goOnItem, musicItem, effctsItem, backItem, NULL);
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
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Pause::sensitivity), CCControlEventValueChanged);
    
    //用于展示当前滑动条的滑动值
    CCSprite* slide = CCSprite::create("set_touch_normal.png");
    addChild(slide);
    slide->setPosition(ccp(size.width*0.5, size.height*0.51));
    slide->setScale(0.5);
    CCLabelTTF * ttf = CCLabelTTF::create("%.0f", "Thonburi", 40);
    ttf->setPosition(ccp(slide->getPositionX(), slide->getPositionY() - slide->boundingBox().size.height * 2.1));
    ttf->setString(CCString::createWithFormat("%.0f", slider->getValue())->getCString());
    ttf->setTag(ttf_tag);
    ttf->setColor(ccc3(255, 255, 0));
    addChild(ttf);
    
    /*初始化*/
    rusumMusic = true;
    effects = true;
    tempPoints = 0;
    slider->setValue(CCUserDefault::sharedUserDefault()->getFloatForKey("slider", 3));
    
    return true;
}

void Pause::resume()
{
    /*累加积分*/
    Data::sharedData()->points = CCUserDefault::sharedUserDefault()->getFloatForKey("points");
    Data::sharedData()->ifRefreshed = true;
    
    /*存储设置的值*/
    CCUserDefault::sharedUserDefault()->setBoolForKey("ifPlayMusic", Interface::sharedInterface()->ifPlayMusic);
    CCUserDefault::sharedUserDefault()->setBoolForKey("ifPlayEffect", Interface::sharedInterface()->ifPlayEffect);
    CCUserDefault::sharedUserDefault()->setFloatForKey("slider", slider->getValue());
    CCUserDefault::sharedUserDefault()->flush();
    
    CCDirector::sharedDirector()->popScene();
}

void Pause::backMenu()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }
    /*存储设置的值*/
    CCUserDefault::sharedUserDefault()->setBoolForKey("ifPlayMusic", Interface::sharedInterface()->ifPlayMusic);
    CCUserDefault::sharedUserDefault()->setBoolForKey("ifPlayEffect", Interface::sharedInterface()->ifPlayEffect);
    CCUserDefault::sharedUserDefault()->setFloatForKey("slider", slider->getValue());
    CCUserDefault::sharedUserDefault()->flush();
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, Menu::scene()));
}

void Pause::resumeMusic()
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

void Pause::sensitivity()
{
    CCControlSlider* slider= (CCControlSlider*)this->getChildByTag(slider_tag);
    CCLabelTTF* ttf = (CCLabelTTF*)this->getChildByTag(ttf_tag);
    ttf->setString(CCString::createWithFormat("%.0f", slider->getValue())->getCString());
}

void Pause::effetsVolume()
{
    if(effects) {
        Interface::sharedInterface()->ifPlayEffect = false;
    }
    else {
        Interface::sharedInterface()->ifPlayEffect = true;
    }
    effects = !effects;
}