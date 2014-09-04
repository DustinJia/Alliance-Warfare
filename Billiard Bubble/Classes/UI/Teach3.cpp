#include "Teach3.h"
#include "Teach2.h"
#include "Help3.h"
#include "Menu.h"
#include "World.h"
#include "Interface.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene *Teach3::scene()
{
    CCScene *scene = CCScene::create();
    Teach3 *layer = Teach3::create();
    scene->addChild(layer);
    return scene;
}


bool Teach3::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* backGround = CCSprite::create("inhesion_background.jpg");
    addChild(backGround);
    backGround->setPosition(ccp(size.width * 0.5, size.height * 0.5));
    iconInherence();

    plusSign();

    /*方向按钮*/
    CCMenuItemImage *itemImage = CCMenuItemImage::create("set_back_normal.png", "set_back_normal.png", this, menu_selector(Teach3::backMenu));
    itemImage->setScale(0.5);
    
    CCMenuItemImage* back = CCMenuItemImage::create("directionBack.png", "directionBack.png", this, menu_selector(Teach3::back));
    itemImage->setPosition(ccp(size.width - itemImage->boundingBox().size.width * 0.57, size.height - itemImage->boundingBox().size.height * 0.7));
    back->setPosition(size.width * 0.07, size.height * 0.1);
   
    /*加入菜单*/
    CCMenu *menu = CCMenu::create(itemImage, back, NULL);
    addChild(menu);
    menu->setPosition(ccp(0,0));
    
    label = CCLabelTTF::create("点击天赋图标旁的加号可以增强三人的实力哦！","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    label->setColor(ccc3(255, 255, 205));
    addChild(label);
    hero = CCSprite::create("attacker.png");
    

    return true;
}

void Teach3::iconInherence()
{
    CCSprite* iconHP1 = CCSprite::create("inherence_hp.png");
    CCSprite* iconHP2 = CCSprite::create("inherence_hp.png");
    CCSprite* iconHP3 = CCSprite::create("inherence_hp.png");
    CCSprite* iconAttack1 = CCSprite::create("inherence_tankSkill.png");
    CCSprite* iconAttack2 = CCSprite::create("inherence_attack.png");
    CCSprite* iconHeal = CCSprite::create("inherence_heal.png");
    CCSprite* iconCD1 = CCSprite::create("inherence_CD.png");
    CCSprite* iconCD2 = CCSprite::create("inherence_CD.png");
    CCSprite* iconCD3 = CCSprite::create("inherence_CD.png");
    
    addChild(iconHP1);
    addChild(iconHP2);
    addChild(iconHP3);
    addChild(iconAttack1);
    addChild(iconAttack2);
    addChild(iconHeal);
    addChild(iconCD1);
    addChild(iconCD2);
    addChild(iconCD3);
    
    iconHP1->setScale(0.32);
    iconHP2->setScale(0.32);
    iconHP3->setScale(0.32);
    iconAttack1->setScale(0.52);
    iconAttack2->setScale(0.31);
    iconHeal->setScale(0.31);
    iconCD1->setScale(0.29);
    iconCD2->setScale(0.29);
    iconCD3->setScale(0.29);
    
    iconHP1->setPosition(ccp(size.width * 0.18, size.height * 0.68));
    iconHP2->setPosition(ccp(size.width * 0.44, size.height * 0.68));
    iconHP3->setPosition(ccp(size.width * 0.695, size.height * 0.68));
    iconAttack1->setPosition(ccp(size.width * 0.18, size.height * 0.43));
    iconAttack2->setPosition(ccp(size.width * 0.44, size.height * 0.43));
    iconHeal->setPosition(ccp(size.width * 0.695, size.height * 0.43));
    iconCD1->setPosition(ccp(size.width * 0.18, size.height * 0.18));
    iconCD2->setPosition(ccp(size.width * 0.44, size.height * 0.18));
    iconCD3->setPosition(ccp(size.width * 0.695, size.height * 0.18));
}

void Teach3::plusSign()
{
    CCLabelTTF* labelPlusSign1 = CCLabelTTF::create("+", "Verdana-Bold", 60);
    CCLabelTTF* labelPlusSign2 = CCLabelTTF::create("+", "Verdana-Bold", 60);
    CCLabelTTF* labelPlusSign3 = CCLabelTTF::create("+", "Verdana-Bold", 60);
    CCLabelTTF* labelPlusSign4 = CCLabelTTF::create("+", "Verdana-Bold", 60);
    CCLabelTTF* labelPlusSign5 = CCLabelTTF::create("+", "Verdana-Bold", 60);
    CCLabelTTF* labelPlusSign6 = CCLabelTTF::create("+", "Verdana-Bold", 60);
    CCLabelTTF* labelPlusSign7 = CCLabelTTF::create("+", "Verdana-Bold", 60);
    CCLabelTTF* labelPlusSign8 = CCLabelTTF::create("+", "Verdana-Bold", 60);
    CCLabelTTF* labelPlusSign9 = CCLabelTTF::create("+", "Verdana-Bold", 60);
    
    CCMenuItemLabel* itemPlusSign1 = CCMenuItemLabel::create(labelPlusSign1, this, menu_selector(Teach3::addHP1));
    CCMenuItemLabel* itemPlusSign2 = CCMenuItemLabel::create(labelPlusSign2, this, menu_selector(Teach3::addHP2));
    CCMenuItemLabel* itemPlusSign3 = CCMenuItemLabel::create(labelPlusSign3, this, menu_selector(Teach3::addHP3));
    CCMenuItemLabel* itemPlusSign4 = CCMenuItemLabel::create(labelPlusSign4, this, menu_selector(Teach3::addAttack1));
    CCMenuItemLabel* itemPlusSign5 = CCMenuItemLabel::create(labelPlusSign5, this, menu_selector(Teach3::addAttack2));
    CCMenuItemLabel* itemPlusSign6 = CCMenuItemLabel::create(labelPlusSign6, this, menu_selector(Teach3::addHeal));
    CCMenuItemLabel* itemPlusSign7 = CCMenuItemLabel::create(labelPlusSign7, this, menu_selector(Teach3::addCD1));
    CCMenuItemLabel* itemPlusSign8 = CCMenuItemLabel::create(labelPlusSign8, this, menu_selector(Teach3::addCD2));
    CCMenuItemLabel* itemPlusSign9 = CCMenuItemLabel::create(labelPlusSign9, this, menu_selector(Teach3::addCD3));
    
    itemPlusSign1->setPosition(ccp(-size.width * 0.21, size.height * 0.188));
    itemPlusSign2->setPosition(ccp(size.width * 0.055, size.height * 0.188));
    itemPlusSign3->setPosition(ccp(size.width * 0.292, size.height * 0.188));
    itemPlusSign4->setPosition(ccp(-size.width * 0.21, -size.height * 0.064));
    itemPlusSign5->setPosition(ccp(size.width * 0.055, -size.height * 0.064));
    itemPlusSign6->setPosition(ccp(size.width * 0.292, -size.height * 0.064));
    itemPlusSign7->setPosition(ccp(-size.width * 0.21, -size.height * 0.313));
    itemPlusSign8->setPosition(ccp(size.width * 0.055, -size.height * 0.313));
    itemPlusSign9->setPosition(ccp(size.width * 0.292, -size.height * 0.313));
    
    CCMenu* menu = CCMenu::create(itemPlusSign1, itemPlusSign2, itemPlusSign3, itemPlusSign4, itemPlusSign5, itemPlusSign6, itemPlusSign7, itemPlusSign8, itemPlusSign9, NULL);
    addChild(menu);
}

void Teach3::addHP1()
{
    removeChild(label, true);
    removeChild(hero);

    hero = CCSprite::create("tank.png");
    hero->setScale(0.25);
    hero->setPosition(ccp(size.width*0.5-35*10, size.height*0.9));
    addChild(hero);
    label = CCLabelTTF::create("  : 加强哀木涕的防御力                    ","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    label->setColor(ccc3(255, 235, 205));
    addChild(label);
}

void Teach3::addHP2()
{
    removeChild(label, true);
    removeChild(hero);

    hero = CCSprite::create("attacker.png");
    hero->setScale(0.25);
    hero->setPosition(ccp(size.width*0.5-35*10, size.height*0.9));
    addChild(hero);
    label = CCLabelTTF::create(": 加强劣人的防御力                     ","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    label->setColor(ccc3(255, 235, 205));
    addChild(label);
}

void Teach3::addHP3()
{
    removeChild(label, true);
    removeChild(hero);

    hero = CCSprite::create("healer.png");
    hero->setScale(0.25);
    hero->setPosition(ccp(size.width*0.5-35*10, size.height*0.9));
    addChild(hero);
    label = CCLabelTTF::create(": 加强傻馒的防御力                     ","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    label->setColor(ccc3(255, 235, 205));
    addChild(label);
}

void Teach3::addAttack1()
{
    removeChild(label, true);
    removeChild(hero);

    hero = CCSprite::create("tank.png");
    hero->setScale(0.25);
    hero->setPosition(ccp(size.width*0.5-35*10, size.height*0.9));
    addChild(hero);
    label = CCLabelTTF::create("     : 加强哀木涕每次打击的攻击力和仇恨值","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    label->setColor(ccc3(255, 235, 205));
    addChild(label);
}

void Teach3::addAttack2()
{
    removeChild(label, true);
    removeChild(hero);

    hero = CCSprite::create("attacker.png");
    hero->setScale(0.25);
    hero->setPosition(ccp(size.width*0.5-35*10, size.height*0.9));
    addChild(hero);
    label = CCLabelTTF::create(": 加强劣人的攻击力                     ","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    label->setColor(ccc3(255, 235, 205));
    addChild(label);
}

void Teach3::addHeal()
{
    removeChild(label, true);
    removeChild(hero);
    
    hero = CCSprite::create("healer.png");
    hero->setScale(0.25);
    hero->setPosition(ccp(size.width*0.5-35*10, size.height*0.9));
    addChild(hero);
    label = CCLabelTTF::create(": 加强傻馒的治疗量                     ","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    label->setColor(ccc3(255, 235, 205));
    addChild(label);
}

void Teach3::addCD1()
{
    removeChild(label, true);
    removeChild(hero);

    hero = CCSprite::create("tank.png");
    hero->setScale(0.25);
    hero->setPosition(ccp(size.width*0.5-35*10, size.height*0.9));
    addChild(hero);                   
    label = CCLabelTTF::create(": 减少哀木涕的技能冷却时间         ","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    label->setColor(ccc3(255, 235, 205));
    addChild(label);
}

void Teach3::addCD2()
{
    removeChild(label, true);
    removeChild(hero);

    hero = CCSprite::create("attacker.png");
    hero->setScale(0.25);
    hero->setPosition(ccp(size.width*0.5-35*10, size.height*0.9));
    addChild(hero);
    label = CCLabelTTF::create(": 减少劣人的技能冷却时间            ","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    label->setColor(ccc3(255, 235, 205));
    addChild(label);
}

void Teach3::addCD3()
{
    removeChild(label, true);
    removeChild(hero);

    hero = CCSprite::create("healer.png");
    hero->setScale(0.25);
    hero->setPosition(ccp(size.width*0.5-35*10, size.height*0.9));
    addChild(hero);
    label = CCLabelTTF::create(": 减少傻馒的技能冷却时间            ","Verdana-Bold",35);
    label->setPosition(ccp(size.width*0.5, size.height*0.9));
    label->setColor(ccc3(255, 235, 205));
    addChild(label);
}

void Teach3::backMenu()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3", false);
    }
    
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("ifFirstTime",true)) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1.5, World::scene()));
    }
    else {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, Menu::scene()));
    }

}


void Teach3::back()
{
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3", false);
    }
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(1, Teach2::scene()));
}

void Teach3::onExit()
{
    CCUserDefault::sharedUserDefault()->setBoolForKey("ifFirstTime", false);
    CCUserDefault::sharedUserDefault()->flush();
    
    CCLayer::onExit();
}