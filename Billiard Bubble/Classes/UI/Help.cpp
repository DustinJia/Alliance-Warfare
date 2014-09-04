#include "Help.h"
#include "Help1.h"

CCScene* Help::scene()
{
    CCScene* scene = CCScene::create();
    Help* layer = Help::create();
    scene->addChild(layer);
    return scene;
}

bool Help::init()
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

    CCLabelTTF *label1 = CCLabelTTF::create("基本操作说明:", "Verdana-Bold", 35);
    CCLabelTTF *label2 = CCLabelTTF::create("1、操作方式:本游戏基本操作方式为单击与滑动。", "Verdana-Bold", 30);
    CCLabelTTF *label3 = CCLabelTTF::create("2、攻击说明:只有攻击怪物的两侧与尾部时才有效,当撞到","Verdana-Bold", 30);
    CCLabelTTF *label4 = CCLabelTTF::create("怪物头部时自身会受伤。(傻馒没有攻击力)", "Verdana-Bold", 30);
    CCLabelTTF *label5 = CCLabelTTF::create("3、触摸精度：为了配合不同玩家的操作习惯，玩家可在设","Verdana-Bold"  ,30);
    CCLabelTTF *label6 = CCLabelTTF::create("置界面调节触摸精度,以调节对目标操作的范围大小。", "Verdana-Bold", 30);
    label1->setColor(ccc3(160, 32, 240));
    label2->setColor(ccc3(160, 32, 240));
    label3->setColor(ccc3(160, 32, 240));
    label4->setColor(ccc3(160, 32, 240));
    label5->setColor(ccc3(160, 32, 240));
    label6->setColor(ccc3(160, 32, 240));
    
    label1->setPosition(ccp(size.width*0.5, size.height*0.82));
    label2->setPosition(ccp(size.width*0.5-76, size.height*0.7));
    label3->setPosition(ccp(size.width*0.5-28, size.height*0.6));
    label4->setPosition(ccp(size.width*0.5-75, size.height*0.5));
    label5->setPosition(ccp(size.width*0.5-26, size.height*0.4));
    label6->setPosition(ccp(size.width*0.5-14, size.height*0.3));
    
    addChild(label1);
    addChild(label2);
    addChild(label3);
    addChild(label4);
    addChild(label5);
    addChild(label6);
    
    //实现触摸
    this->setTouchEnabled(true);
    
    
    return true;
}
//注册触摸事件
void Help::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool Help::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    beginPoint = pTouch->getLocation();
    return true;
}

void Help::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    endPoint = pTouch->getLocation();
    
    if(beginPoint.x - endPoint.x > 80)
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.8, Help1::scene()));
    }
}