#include "Help2.h"
#include "Help1.h"
#include "Help3.h"
#include "Teach.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;


CCScene* Help2::scene()
{
    CCScene* scene = CCScene::create();
    Help2* layer = Help2::create();
    scene->addChild(layer);
    return scene;
}

bool Help2::init()
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
    CCSprite *spDPS = CCSprite::create("help_DPS.png");
    spDPS->setPosition(ccp(size.width*0.5-28*12.8, size.height*0.83));
    spDPS->setScale(0.11);
    addChild(spDPS);
    /*人物简介*/
    CCLabelTTF *label1 = CCLabelTTF::create("劣人:", "Verdana-Bold", 28);
    CCLabelTTF *label11 = CCLabelTTF::create("同为哀木涕的伙伴,有点秀逗,自大，可智商不高,", "Verdana-Bold", 28);
    CCLabelTTF *label2 = CCLabelTTF::create("天天抱着自己的爱猪不放手,有严重的恶趣味,不", "Verdana-Bold", 28);
    CCLabelTTF *label3 = CCLabelTTF::create("为人知的一面。", "Verdana-Bold", 28);
    label1->setPosition(ccp(size.width*0.5-28*9, size.height*0.91));
    label11->setPosition(ccp(size.width*0.582, size.height*0.91));
    label2->setPosition(ccp(size.width*0.577, size.height*0.86));
    label3->setPosition(ccp(size.width*0.5-28*4.1, size.height*0.81));
    label1->setColor(ccc3(128, 42, 42));
    label11->setColor(ccc3(56, 94, 15));
    label2->setColor(ccc3(56, 94, 15));
    label3->setColor(ccc3(56, 94, 15));
    addChild(label1);
    addChild(label11);
    addChild(label2);
    addChild(label3);
    
    
    /*特性说明*/
    CCLabelTTF *label4 = CCLabelTTF::create("1、团队中的输出,是攻击力最高的角色,主要负责对怪", "Verdana-Bold", 33);
    CCLabelTTF *label5 = CCLabelTTF::create("物造成伤害,血量居中。", "Verdana-Bold", 33);
    CCLabelTTF *label6 = CCLabelTTF::create("2、触发技能后会在下一击对怪物造成致命伤害。", "Verdana-Bold", 33);
    CCLabelTTF *label7 = CCLabelTTF::create("3、在傻馒开启技能的时间段内单击,可为自己补血。", "Verdana-Bold", 33);
    
    label4->setColor(ccc3(0, 0, 255));
    label5->setColor(ccc3(0, 0, 255));
    label6->setColor(ccc3(0, 0, 255));
    label7->setColor(ccc3(0, 0, 255));
    
    label4->setPosition(ccp(size.width*0.5, size.height*0.55));
    label5->setPosition(ccp(size.width*0.5-28*5.6, size.height*0.48));
    label6->setPosition(ccp(size.width*0.5-28, size.height*0.41));
    label7->setPosition(ccp(size.width*0.495, size.height*0.34));
    
    addChild(label4);
    addChild(label5);
    addChild(label6);
    addChild(label7);
    
    CCMenuItemImage* item = CCMenuItemImage::create("direction.png", "direction.png", this, menu_selector(Help2::backToTeach));
    CCMenu* menu = CCMenu::create(item, NULL);
    addChild(menu);
    menu->setPosition(size.width * 0.94, size.height * 0.9);
    
    //实现触摸
    this->setTouchEnabled(true);
    
    return true;
}

void Help2::backToTeach()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1, Teach::scene()));
}

void Help2::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool Help2::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    beginPoint = pTouch->getLocation();
    return true;
}

void Help2::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    endPoint = pTouch->getLocation();
    
    if(beginPoint.x - endPoint.x > 80)
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.8, Help3::scene()));
    }
    else if(endPoint.x - beginPoint.x > 80)
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(0.8, Help1::scene()));
    }
}