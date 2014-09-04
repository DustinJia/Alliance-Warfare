#include "Help1.h"
#include "Help.h"
#include "Help2.h"
#include "Teach1.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;


CCScene* Help1::scene()
{
    CCScene* scene = CCScene::create();
    Help1* layer = Help1::create();
    scene->addChild(layer);
    return scene;
}

bool Help1::init()
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
    CCSprite *spTank = CCSprite::create("help_Tank.png");
    spTank->setPosition(ccp(size.width*0.5-28*13, size.height*0.83));
    spTank->setScale(0.13);
    addChild(spTank);
    /*人物简介*/
    CCLabelTTF *label1 = CCLabelTTF::create("哀木涕:", "Verdana-Bold", 28);
    CCLabelTTF *label11 = CCLabelTTF::create("集合了呆傻蠢笨为一体,既贪财又好色,没立场,", "Verdana-Bold", 28);
    CCLabelTTF *label2 = CCLabelTTF::create("见利忘义,胆小懦弱,脑子里不知道想些什么,经", "Verdana-Bold", 28);
    CCLabelTTF *label3 = CCLabelTTF::create("常做一些常人不理解的事情,但有时也激情澎湃", "Verdana-Bold", 28);
    CCLabelTTF *label4 = CCLabelTTF::create("一下,虽然经常悲剧收场。", "Verdana-Bold", 28);
    
    label1->setPosition(ccp(size.width*0.5-28*9, size.height*0.915));
    label11->setPosition(ccp(size.width*0.582, size.height*0.915));
    label2->setPosition(ccp(size.width*0.58, size.height*0.865));
    label3->setPosition(ccp(size.width*0.582, size.height*0.815));
    label4->setPosition(ccp(size.width*0.5-28*1.2, size.height*0.765));
    label1->setColor(ccc3(128, 42, 42));
    label11->setColor(ccc3(56, 94, 15));
    label2->setColor(ccc3(56, 94, 15));
    label3->setColor(ccc3(56, 94, 15));
    label4->setColor(ccc3(56, 94, 15));
    addChild(label1);
    addChild(label11);
    addChild(label2);
    addChild(label3);
    addChild(label4);
    
    /*特性说明*/
    CCLabelTTF *label5 = CCLabelTTF::create("1、团队中的坦克，是血量与防御力最高的角色，主要", "Verdana-Bold", 33);
    CCLabelTTF *label6 = CCLabelTTF::create("负责吸引仇恨,拖住怪物,使其他队友免受攻击。", "Verdana-Bold", 33);
    CCLabelTTF *label7 = CCLabelTTF::create("2、触发技能后会在一定时间内大幅增加自身的仇恨值,", "Verdana-Bold", 33);
    CCLabelTTF *label8 = CCLabelTTF::create("使怪物锁定自己。", "Verdana-Bold", 33);
    CCLabelTTF *label9 = CCLabelTTF::create("3、有一定攻击力，每次怪物敌人会永久增加一定量自", "Verdana-Bold", 33);
    CCLabelTTF *label10 = CCLabelTTF::create("身的仇恨值。", "Verdana-Bold", 33);
    CCLabelTTF *label12 = CCLabelTTF::create("4、在傻馒开启技能的时间段内单击，可为自己补血。", "Verdana-Bold", 33);
    label5->setColor(ccc3(0, 0, 255));
    label6->setColor(ccc3(0, 0, 255));
    label7->setColor(ccc3(0, 0, 255));
    label8->setColor(ccc3(0, 0, 255));
    label9->setColor(ccc3(0, 0, 255));
    label10->setColor(ccc3(0, 0, 255));
    label12->setColor(ccc3(0, 0, 255));
    label5->setPosition(ccp(size.width*0.5-5, size.height*0.55));
    label6->setPosition(ccp(size.width*0.5, size.height*0.48));
    label7->setPosition(ccp(size.width*0.5, size.height*0.41));
    label8->setPosition(ccp(size.width*0.5-28*7, size.height*0.34));
    label9->setPosition(ccp(size.width*0.5, size.height*0.27));
    label10->setPosition(ccp(size.width*0.5-28*8, size.height*0.2));
    label12->setPosition(ccp(size.width*0.5, size.height*0.14));
    addChild(label5);
    addChild(label6);
    addChild(label7);
    addChild(label8);
    addChild(label9);
    addChild(label10);
    addChild(label12);
    
    CCMenuItemImage* item = CCMenuItemImage::create("direction.png", "direction.png", this, menu_selector(Help1::backToTeach));
    CCMenu* menu = CCMenu::create(item, NULL);
    addChild(menu);
    menu->setPosition(size.width * 0.94, size.height * 0.9);
    
    //实现触摸
    this->setTouchEnabled(true);
    
    return true;
}

void Help1::backToTeach()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1, Teach1::scene()));
}

void Help1::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool Help1::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    beginPoint = pTouch->getLocation();
    return true;
}

void Help1::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    endPoint = pTouch->getLocation();
    
    if(beginPoint.x - endPoint.x > 80)
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.8, Help2::scene()));
    }
    else if(endPoint.x - beginPoint.x > 80)
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(0.8, Help::scene()));
    }
}