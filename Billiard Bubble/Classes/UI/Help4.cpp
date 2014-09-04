#include "Help4.h"
#include "Help3.h"
#include "Menu.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene* Help4::scene()
{
    CCScene* scene = CCScene::create();
    Help4* layer = Help4::create();
    scene->addChild(layer);
    return scene;
}

bool Help4::init()
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
    
    /*天赋说明*/
    CCLabelTTF *label = CCLabelTTF::create("天赋说明", "Verdana-Bold", 35);
    label->setColor(ccc3(135, 38, 87));
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    addChild(label);
    /*哀木涕*/
    CCLabelTTF *labelT = CCLabelTTF::create("哀木涕:", "Verdana-Bold", 30);
    CCLabelTTF *labelT1 = CCLabelTTF::create("第一天赋:在此分配点数可增加哀木涕的防御力。", "Verdana-Bold", 30);
    CCLabelTTF *labelT2 = CCLabelTTF::create("第二天赋:在此分配点数可增加哀木涕每次攻击力与仇恨增量。", "Verdana-Bold", 30);
    CCLabelTTF *labelT3 = CCLabelTTF::create("第三天赋:在此分配点数可减少哀木涕每次发动技能的冷却时间。", "Verdana-Bold", 30);
    labelT->setColor(ccc3(160, 32, 240));
    labelT1->setColor(ccc3(160, 32, 240));
    labelT2->setColor(ccc3(160, 32, 240));
    labelT3->setColor(ccc3(160, 32, 240));
    labelT->setPosition(ccp(size.width*0.5-30*13, size.height*0.8));
    labelT1->setPosition(ccp(size.width*0.5-30*2, size.height*0.75));
    labelT2->setPosition(ccp(size.width*0.5+30*1, size.height*0.7));
    labelT3->setPosition(ccp(size.width*0.5+30*1.5, size.height*0.65));
    addChild(labelT);
    addChild(labelT1);
    addChild(labelT2);
    addChild(labelT3);
    
    
    /*劣人*/
    CCLabelTTF *labelD = CCLabelTTF::create("劣  人:", "Verdana-Bold", 30);
    CCLabelTTF *labelD1 = CCLabelTTF::create("第一天赋:在此分配点数可增加劣人的防御力。", "Verdana-Bold", 30);
    CCLabelTTF *labelD2 = CCLabelTTF::create("第二天赋:在此分配点数可增加劣人的攻击力。", "Verdana-Bold", 30);
    CCLabelTTF *labelD3 = CCLabelTTF::create("第三天赋:在此分配点数可减少劣人每次发动技能的冷却时间。", "Verdana-Bold", 30);
    labelD->setColor(ccc3(160, 32, 240));
    labelD1->setColor(ccc3(160, 32, 240));
    labelD2->setColor(ccc3(160, 32, 240));
    labelD3->setColor(ccc3(160, 32, 240));
    labelD->setPosition(ccp(size.width*0.5-30*13, size.height*0.55));
    labelD1->setPosition(ccp(size.width*0.5-30*2.5, size.height*0.5));
    labelD2->setPosition(ccp(size.width*0.5-30*2.5, size.height*0.45));
    labelD3->setPosition(ccp(size.width*0.5+30, size.height*0.4));
    
    addChild(labelD);
    addChild(labelD1);
    addChild(labelD2);
    addChild(labelD3);
    
    /*傻馒*/
    CCLabelTTF *labelH = CCLabelTTF::create("傻  馒:", "Verdana-Bold", 30);
    CCLabelTTF *labelH1 = CCLabelTTF::create("第一天赋:在此分配点数可增加傻馒的防御力。", "Verdana-Bold", 30);
    CCLabelTTF *labelH2 = CCLabelTTF::create("第二天赋:在此分配点数可增加傻馒的治疗量。", "Verdana-Bold", 30);
    CCLabelTTF *labelH3 = CCLabelTTF::create("第三天赋:在此分配点数可减少傻馒每次发动技能的冷却时间。", "Verdana-Bold", 30);
    labelH->setColor(ccc3(160, 32, 240));
    labelH1->setColor(ccc3(160, 32, 240));
    labelH2->setColor(ccc3(160, 32, 240));
    labelH3->setColor(ccc3(160, 32, 240));
    labelH->setPosition(ccp(size.width*0.5-30*13, size.height*0.3));
    labelH1->setPosition(ccp(size.width*0.5-30*2.5, size.height*0.25));
    labelH2->setPosition(ccp(size.width*0.5-30*2.5, size.height*0.2));
    labelH3->setPosition(ccp(size.width*0.5+30, size.height*0.15));
    
    addChild(labelH);
    addChild(labelH1);
    addChild(labelH2);
    addChild(labelH3);
    
    /*返回菜单*/
    CCMenuItemImage *itemBack = CCMenuItemImage::create("set_back_normal.png", "set_back_normal.png", this, menu_selector(Help4::backMenu));
    itemBack->setScale(0.5);
    itemBack->setPosition(ccp(size.width*0.39, size.height*0.42));
    CCMenu *menu = CCMenu::create(itemBack, NULL);
    addChild(menu);
    
    //实现触摸
    this->setTouchEnabled(true);
    
    
    return true;
}



void Help4::backMenu()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("inherence_add.wav", false);
    }
    CCScene *menu = Menu::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1.5, menu));
}


/*注册触摸事件*/
void Help4::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool Help4::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    beginPoint = pTouch->getLocation();
    return true;
}

void Help4::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    endPoint = pTouch->getLocation();
    
     if(endPoint.x - beginPoint.x > 80) {
         CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(0.8, Help3::scene()));
     }
}