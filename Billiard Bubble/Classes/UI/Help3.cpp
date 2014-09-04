#include "Help3.h"
#include "Help2.h"
#include "Help4.h"
#include "Teach2.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene* Help3::scene()
{
    CCScene* scene = CCScene::create();
    Help3* layer = Help3::create();
    scene->addChild(layer);
    return scene;
}

bool Help3::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*添加背景*/
    CCSprite* spBackground = CCSprite::create("help.jpg");
    addChild(spBackground);
    spBackground->setPosition(ccp(size.width * 0.5, size.height * 0.5));
    spBackground->setScaleX((float)size.width / 1136);
    spBackground->setScaleY((float)size.height / 640);
    
    /*添加框架*/
    CCSprite *spFrame1 = CCSprite::create("frame1.png");
    CCSprite *spFrame2 = CCSprite::create("frame2.png");
    addChild(spFrame1);
    addChild(spFrame2);
    spFrame1->setScaleX(1.8);
    spFrame1->setScaleY(0.5);
    spFrame2->setScaleY(0.6);
    spFrame2->setScaleX(0.9);
    spFrame1->setPosition(ccp(size.width*0.5, size.height*0.83));
    spFrame2->setPosition(ccp(size.width*0.5, size.height*0.35));
    
    /*添加头像*/
    CCSprite *spDPS = CCSprite::create("help_Healer.png");
    spDPS->setPosition(ccp(size.width*0.5-28*13, size.height*0.83));
    spDPS->setScale(0.13);
    addChild(spDPS);
    /*人物简介*/
    CCLabelTTF *label1 = CCLabelTTF::create("傻馒:", "Verdana-Bold", 28);
    CCLabelTTF *label11 = CCLabelTTF::create("哀木涕的伙伴，一只话唠母牛，强势，自己永远", "Verdana-Bold", 28);
    CCLabelTTF *label2 = CCLabelTTF::create("是正确的，眼里容不得沙子,尤其是劣人经常被傻", "Verdana-Bold", 28);
    CCLabelTTF *label3 = CCLabelTTF::create("馒欺负,对哀木涕有意思,但哀木涕貌似不太入道。", "Verdana-Bold", 28);
    
    
    label1->setPosition(ccp(size.width*0.5-28*9, size.height*0.91));
    label11->setPosition(ccp(size.width*0.582, size.height*0.91));
    label2->setPosition(ccp(size.width*0.59, size.height*0.86));
    label3->setPosition(ccp(size.width*0.59, size.height*0.81));
    
    label1->setColor(ccc3(128, 42, 42));
    label11->setColor(ccc3(56, 94, 15));
    label2->setColor(ccc3(56, 94, 15));
    label3->setColor(ccc3(56, 94, 15));
    addChild(label1);
    addChild(label11);
    addChild(label2);
    addChild(label3);
    
    /*特性说明*/
    CCLabelTTF *label4 = CCLabelTTF::create("1、团队中的治疗，主要负责给队友补血，血量最低。", "Verdana-Bold", 33);
    CCLabelTTF *label5 = CCLabelTTF::create("2、与队友碰撞即可给队友补血。", "Verdana-Bold", 33);
    CCLabelTTF *label6 = CCLabelTTF::create("3、当技能开启时，可连续点击队友，为其多次补血。", "Verdana-Bold", 33);
    
    label4->setColor(ccc3(0, 0, 255));
    label5->setColor(ccc3(0, 0, 255));
    label6->setColor(ccc3(0, 0, 255));
    
    label4->setPosition(ccp(size.width*0.5, size.height*0.55));
    label5->setPosition(ccp(size.width*0.5-28*5, size.height*0.48));
    label6->setPosition(ccp(size.width*0.5, size.height*0.41));
    
    addChild(label4);
    addChild(label5);
    addChild(label6);
    
    CCMenuItemImage* item = CCMenuItemImage::create("direction.png", "direction.png", this, menu_selector(Help3::backToTeach));
    CCMenu* menu = CCMenu::create(item, NULL);
    addChild(menu);
    menu->setPosition(size.width * 0.94, size.height * 0.9);
    
    //实现触摸
    this->setTouchEnabled(true);
    
    return true;
}

void Help3::backToTeach()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1, Teach2::scene()));
}

void Help3::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool Help3::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    beginPoint = pTouch->getLocation();
    return true;
}

void Help3::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    endPoint = pTouch->getLocation();
    
    if(beginPoint.x - endPoint.x > 80)
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.8, Help4::scene()));
    }
    else if(endPoint.x - beginPoint.x > 80)
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(0.8, Help2::scene()));
    }
}