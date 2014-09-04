#include "Inhesion.h"
#include "Data.h"
#include "Menu.h"
#include "Interface.h"
#include "World.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

static Inhesion* instance = NULL;

Inhesion* Inhesion::sharedInhesion()
{
    if (instance == NULL) {
        instance = new Inhesion();
    }
    return instance;
}

CCScene* Inhesion::scene()
{
    CCScene* scene = CCScene::create();
    Inhesion* layer = Inhesion::create();
    scene->addChild(layer);
    return scene;
}

bool Inhesion::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("confirm.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("click1.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("inherence_add.wav");

    /*为静态对象添加实例*/
    instance = this;
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*添加背景*/
    CCSprite* backGround = CCSprite::create("inhesion_background.jpg");
    addChild(backGround);
    backGround->setPosition(ccp(size.width * 0.5, size.height * 0.5));
    
    /*添加基本内容*/
    iconPlayer();
    iconInherence();
    data();
    plusSign();
    
    /*添加剩余点数*/
    residue();
    
    /*确定与重置*/
    confirm();
    reset();
    
    /*设置数据*/
    set();
    
    /*初始化*/
    ifReset = false;
    
    return true;
}


#pragma mark - 基本内容
void Inhesion::iconPlayer()
{
    CCSprite* iconTank = CCSprite::create("tank.png");
    CCSprite* iconDPS = CCSprite::create("attacker.png");
    CCSprite* iconHealer = CCSprite::create("healer.png");
    
    addChild(iconTank);
    addChild(iconDPS);
    addChild(iconHealer);
    
    iconTank->setScale(0.25);
    iconDPS->setScale(0.25);
    iconHealer->setScale(0.25);
    
    iconTank->setPosition(ccp(size.width * 0.25, size.height * 0.85));
    iconDPS->setPosition(ccp(size.width * 0.5, size.height * 0.85));
    iconHealer->setPosition(ccp(size.width * 0.75, size.height * 0.85));
}

void Inhesion::iconInherence()
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
    
    iconHP1->setPosition(ccp(size.width * 0.15, size.height * 0.68));
    iconHP2->setPosition(ccp(size.width * 0.41, size.height * 0.68));
    iconHP3->setPosition(ccp(size.width * 0.665, size.height * 0.68));
    iconAttack1->setPosition(ccp(size.width * 0.15, size.height * 0.43));
    iconAttack2->setPosition(ccp(size.width * 0.41, size.height * 0.43));
    iconHeal->setPosition(ccp(size.width * 0.665, size.height * 0.43));
    iconCD1->setPosition(ccp(size.width * 0.15, size.height * 0.18));
    iconCD2->setPosition(ccp(size.width * 0.41, size.height * 0.18));
    iconCD3->setPosition(ccp(size.width * 0.665, size.height * 0.18));
}

void Inhesion::data()
{
    hp1 = CCLabelAtlas::create("0", "testfont.plist");
    hp2 = CCLabelAtlas::create("0", "testfont.plist");
    hp3 = CCLabelAtlas::create("0", "testfont.plist");
    attack1 = CCLabelAtlas::create("0", "testfont.plist");
    attack2 = CCLabelAtlas::create("0", "testfont.plist");
    heal = CCLabelAtlas::create("0", "testfont.plist");
    cd1 = CCLabelAtlas::create("0", "testfont.plist");
    cd2 = CCLabelAtlas::create("0", "testfont.plist");
    cd3 = CCLabelAtlas::create("0", "testfont.plist");
    
    addChild(hp1);
    addChild(hp2);
    addChild(hp3);
    addChild(attack1);
    addChild(attack2);
    addChild(heal);
    addChild(cd1);
    addChild(cd2);
    addChild(cd3);
    
    hp1->setScale(0.5);
    hp2->setScale(0.5);
    hp3->setScale(0.5);
    attack1->setScale(0.5);
    attack2->setScale(0.5);
    heal->setScale(0.5);
    cd1->setScale(0.5);
    cd2->setScale(0.5);
    cd3->setScale(0.5);
    
    hp1->setAnchorPoint(ccp(1, 0));
    hp2->setAnchorPoint(ccp(1, 0));
    hp3->setAnchorPoint(ccp(1, 0));
    attack1->setAnchorPoint(ccp(1, 0));
    attack2->setAnchorPoint(ccp(1, 0));
    heal->setAnchorPoint(ccp(1, 0));
    cd1->setAnchorPoint(ccp(1, 0));
    cd2->setAnchorPoint(ccp(1, 0));
    cd3->setAnchorPoint(ccp(1, 0));
    
    hp1->setPosition(ccp(size.width * 0.265, size.height * 0.65));
    hp2->setPosition(ccp(size.width * 0.52, size.height * 0.65));
    hp3->setPosition(ccp(size.width * 0.775, size.height * 0.65));
    attack1->setPosition(ccp(size.width * 0.265, size.height * 0.4));
    attack2->setPosition(ccp(size.width * 0.52, size.height * 0.4));
    heal->setPosition(ccp(size.width * 0.775, size.height * 0.4));
    cd1->setPosition(ccp(size.width * 0.265, size.height * 0.15));
    cd2->setPosition(ccp(size.width * 0.52, size.height * 0.15));
    cd3->setPosition(ccp(size.width * 0.775, size.height * 0.15));
}

void Inhesion::plusSign()
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
    
    CCMenuItemLabel* itemPlusSign1 = CCMenuItemLabel::create(labelPlusSign1, this, menu_selector(Inhesion::addHP1));
    CCMenuItemLabel* itemPlusSign2 = CCMenuItemLabel::create(labelPlusSign2, this, menu_selector(Inhesion::addHP2));
    CCMenuItemLabel* itemPlusSign3 = CCMenuItemLabel::create(labelPlusSign3, this, menu_selector(Inhesion::addHP3));
    CCMenuItemLabel* itemPlusSign4 = CCMenuItemLabel::create(labelPlusSign4, this, menu_selector(Inhesion::addAttack1));
    CCMenuItemLabel* itemPlusSign5 = CCMenuItemLabel::create(labelPlusSign5, this, menu_selector(Inhesion::addAttack2));
    CCMenuItemLabel* itemPlusSign6 = CCMenuItemLabel::create(labelPlusSign6, this, menu_selector(Inhesion::addHeal));
    CCMenuItemLabel* itemPlusSign7 = CCMenuItemLabel::create(labelPlusSign7, this, menu_selector(Inhesion::addCD1));
    CCMenuItemLabel* itemPlusSign8 = CCMenuItemLabel::create(labelPlusSign8, this, menu_selector(Inhesion::addCD2));
    CCMenuItemLabel* itemPlusSign9 = CCMenuItemLabel::create(labelPlusSign9, this, menu_selector(Inhesion::addCD3));
    
    itemPlusSign1->setPosition(ccp(-size.width * 0.19, size.height * 0.188));
    itemPlusSign2->setPosition(ccp(size.width * 0.065, size.height * 0.188));
    itemPlusSign3->setPosition(ccp(size.width * 0.322, size.height * 0.188));
    itemPlusSign4->setPosition(ccp(-size.width * 0.19, -size.height * 0.064));
    itemPlusSign5->setPosition(ccp(size.width * 0.065, -size.height * 0.064));
    itemPlusSign6->setPosition(ccp(size.width * 0.322, -size.height * 0.064));
    itemPlusSign7->setPosition(ccp(-size.width * 0.19, -size.height * 0.313));
    itemPlusSign8->setPosition(ccp(size.width * 0.065, -size.height * 0.313));
    itemPlusSign9->setPosition(ccp(size.width * 0.322, -size.height * 0.313));
    
    CCMenu* menu = CCMenu::create(itemPlusSign1, itemPlusSign2, itemPlusSign3, itemPlusSign4, itemPlusSign5, itemPlusSign6, itemPlusSign7, itemPlusSign8, itemPlusSign9, NULL);
    addChild(menu);
}


#pragma mark - 加点方法
void Inhesion::addHP1()
{
    if (several > 0 && !CCUserDefault::sharedUserDefault()->getBoolForKey("ifAdded1", false)) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            SimpleAudioEngine::sharedEngine()->playEffect("inherence_add.wav", false);
        }
        
        /*改变数字*/
        CCString* string = CCString::createWithFormat("%d", ++data1);
        hp1->setString(string->getCString());
        
        /*更新数据*/
        Interface::sharedInterface()->savedInhesion1 += Interface::sharedInterface()->inhesionHPTank;
        
        /*更新剩余点数*/
        refreshResidue();
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded1", true);
        CCUserDefault::sharedUserDefault()->flush();
        
        ifAdded1 = true;
    }
}

void Inhesion::addHP2()
{
    if (several > 0 && !CCUserDefault::sharedUserDefault()->getBoolForKey("ifAdded2", false)) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            SimpleAudioEngine::sharedEngine()->playEffect("inherence_add.wav", false);
        }

        /*改变数字*/
        CCString* string = CCString::createWithFormat("%d", ++data2);
        hp2->setString(string->getCString());
        
        /*更新数据*/
        Interface::sharedInterface()->savedInhesion2 += Interface::sharedInterface()->inhesionHPDPS;
        /*更新剩余点数*/
        refreshResidue();
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded2", true);
        CCUserDefault::sharedUserDefault()->flush();
        
        ifAdded2 = true;
    }
}

void Inhesion::addHP3()
{
    if (several > 0 && !CCUserDefault::sharedUserDefault()->getBoolForKey("ifAdded3", false)) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            SimpleAudioEngine::sharedEngine()->playEffect("inherence_add.wav", false);
        }

        /*改变数字*/
        CCString* string = CCString::createWithFormat("%d", ++data3);
        hp3->setString(string->getCString());
        
        /*更新数据*/
        Interface::sharedInterface()->savedInhesion3 += Interface::sharedInterface()->inhesionHPHealer;
        
        /*更新剩余点数*/
        refreshResidue();
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded3", true);
        CCUserDefault::sharedUserDefault()->flush();
        
        ifAdded3 = true;
    }
}

void Inhesion::addAttack1()
{
    if (several > 0 && !CCUserDefault::sharedUserDefault()->getBoolForKey("ifAdded4", false)) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            SimpleAudioEngine::sharedEngine()->playEffect("inherence_add.wav", false);
        }

        /*改变数字*/
        CCString* string = CCString::createWithFormat("%d", ++data4);
        attack1->setString(string->getCString());
        
        /*更新数据*/
        Interface::sharedInterface()->savedInhesion4 += Interface::sharedInterface()->inhesionTankThreat;
        
        /*更新剩余点数*/
        refreshResidue();
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded4", true);
        CCUserDefault::sharedUserDefault()->flush();
        
        ifAdded4 = true;
    }
}

void Inhesion::addAttack2()
{
    if (several > 0 && !CCUserDefault::sharedUserDefault()->getBoolForKey("ifAdded5", false)) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            SimpleAudioEngine::sharedEngine()->playEffect("inherence_add.wav", false);
        }

        /*改变数字*/
        CCString* string = CCString::createWithFormat("%d", ++data5);
        attack2->setString(string->getCString());
        
        /*更新数据*/
        Interface::sharedInterface()->savedInhesion5 += Interface::sharedInterface()->inhesionDPSSkill;
        
        /*更新剩余点数*/
        refreshResidue();
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded5", true);
        CCUserDefault::sharedUserDefault()->flush();
        
        ifAdded5 = true;
    }
}

void Inhesion::addHeal()
{
    if (several > 0 && !CCUserDefault::sharedUserDefault()->getBoolForKey("ifAdded6", false)) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            SimpleAudioEngine::sharedEngine()->playEffect("inherence_add.wav", false);
        }

        /*改变数字*/
        CCString* string = CCString::createWithFormat("%d", ++data6);
        heal->setString(string->getCString());
        
        /*更新数据*/
        Interface::sharedInterface()->savedInhesion6 += Interface::sharedInterface()->inhesionHealerHeal;
        
        /*更新剩余点数*/
        refreshResidue();
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded6", true);
        CCUserDefault::sharedUserDefault()->flush();
        
        ifAdded6 = true;
    }
}

void Inhesion::addCD1()
{
    if (several > 0 && !CCUserDefault::sharedUserDefault()->getBoolForKey("ifAdded7", false)) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            SimpleAudioEngine::sharedEngine()->playEffect("inherence_add.wav", false);
        }

        /*改变数字*/
        CCString* string = CCString::createWithFormat("%d", ++data7);
        cd1->setString(string->getCString());
        
        /*更新数据*/
        Interface::sharedInterface()->savedInhesion7 *= Interface::sharedInterface()->inhesionCDTank;

        /*更新剩余点数*/
        refreshResidue();
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded7", true);
        CCUserDefault::sharedUserDefault()->flush();
        
        ifAdded7 = true;
    }
}

void Inhesion::addCD2()
{
    if (several > 0 && !CCUserDefault::sharedUserDefault()->getBoolForKey("ifAdded8", false)) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            SimpleAudioEngine::sharedEngine()->playEffect("inherence_add.wav", false);
        }

        /*改变数字*/
        CCString* string = CCString::createWithFormat("%d", ++data8);
        cd2->setString(string->getCString());
        
        /*更新数据*/
        Interface::sharedInterface()->savedInhesion8 *= Interface::sharedInterface()->inhesionCDDPS;
        
        /*更新剩余点数*/
        refreshResidue();
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded8", true);
        CCUserDefault::sharedUserDefault()->flush();
        
        ifAdded8 = true;
    }
}

void Inhesion::addCD3()
{
    if (several > 0 && !CCUserDefault::sharedUserDefault()->getBoolForKey("ifAdded9", false)) {
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            SimpleAudioEngine::sharedEngine()->playEffect("inherence_add.wav", false);
        }

        /*改变数字*/
        CCString* string = CCString::createWithFormat("%d", ++data9);
        cd3->setString(string->getCString());
        
        /*更新数据*/
        Interface::sharedInterface()->savedInhesion9 *= Interface::sharedInterface()->inhesionCDHealer;
        
        /*更新剩余点数*/
        refreshResidue();
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded9", true);
        CCUserDefault::sharedUserDefault()->flush();
        
        ifAdded9 = true;
    }
}


#pragma mark - 存储与读取
void Inhesion::save()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->playEffect("click1.mp3");
    }
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey("data1", data1);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("data2", data2);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("data3", data3);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("data4", data4);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("data5", data5);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("data6", data6);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("data7", data7);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("data8", data8);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("data9", data9);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("several", several);

    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion1", Interface::sharedInterface()->savedInhesion1);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion2", Interface::sharedInterface()->savedInhesion2);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion3", Interface::sharedInterface()->savedInhesion3);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion4", Interface::sharedInterface()->savedInhesion4);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion5", Interface::sharedInterface()->savedInhesion5);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion6", Interface::sharedInterface()->savedInhesion6);
    if (Interface::sharedInterface()->savedInhesion7) {
        CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion7", Interface::sharedInterface()->savedInhesion7);
    }
    if (Interface::sharedInterface()->savedInhesion8) {
        CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion8", Interface::sharedInterface()->savedInhesion8);
    }
    if (Interface::sharedInterface()->savedInhesion9) {
        CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion9", Interface::sharedInterface()->savedInhesion9);
    }
    
    CCUserDefault::sharedUserDefault()->flush();
    /*切换场景*/
    if (ifFromMenu) {
        CCDirector::sharedDirector()->popScene();
    }
    else {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5, World::scene()));
    }
}

void Inhesion::read()
{
    data1 = CCUserDefault::sharedUserDefault()->getIntegerForKey("data1", 0);
    data2 = CCUserDefault::sharedUserDefault()->getIntegerForKey("data2", 0);
    data3 = CCUserDefault::sharedUserDefault()->getIntegerForKey("data3", 0);
    data4 = CCUserDefault::sharedUserDefault()->getIntegerForKey("data4", 0);
    data5 = CCUserDefault::sharedUserDefault()->getIntegerForKey("data5", 0);
    data6 = CCUserDefault::sharedUserDefault()->getIntegerForKey("data6", 0);
    data7 = CCUserDefault::sharedUserDefault()->getIntegerForKey("data7", 0);
    data8 = CCUserDefault::sharedUserDefault()->getIntegerForKey("data8", 0);
    data9 = CCUserDefault::sharedUserDefault()->getIntegerForKey("data9", 0);
    several = CCUserDefault::sharedUserDefault()->getIntegerForKey("several", Data::sharedData()->severalWithPoints);
    
    Interface::sharedInterface()->savedInhesion1 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion1");
    Interface::sharedInterface()->savedInhesion2 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion2");
    Interface::sharedInterface()->savedInhesion3 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion3");
    Interface::sharedInterface()->savedInhesion4 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion4");
    Interface::sharedInterface()->savedInhesion5 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion5");
    Interface::sharedInterface()->savedInhesion6 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion6");
    Interface::sharedInterface()->savedInhesion7 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion7");
    Interface::sharedInterface()->savedInhesion8 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion8");
    Interface::sharedInterface()->savedInhesion9 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion9");
}

void Inhesion::set()
{
    /*读入数据*/
    read();
    
    /*设置数值*/
    CCString* string1 = CCString::createWithFormat("%d",data1);
    CCString* string2 = CCString::createWithFormat("%d",data2);
    CCString* string3 = CCString::createWithFormat("%d",data3);
    CCString* string4 = CCString::createWithFormat("%d",data4);
    CCString* string5 = CCString::createWithFormat("%d",data5);
    CCString* string6 = CCString::createWithFormat("%d",data6);
    CCString* string7 = CCString::createWithFormat("%d",data7);
    CCString* string8 = CCString::createWithFormat("%d",data8);
    CCString* string9 = CCString::createWithFormat("%d",data9);
    CCString* string10 = CCString::createWithFormat("%d",several);
    
    hp1->setString(string1->getCString());
    hp2->setString(string2->getCString());
    hp3->setString(string3->getCString());
    attack1->setString(string4->getCString());
    attack2->setString(string5->getCString());
    heal->setString(string6->getCString());
    cd1->setString(string7->getCString());
    cd2->setString(string8->getCString());
    cd3->setString(string9->getCString());
    labelNumber->setString(string10->getCString());
    
    /*重置是否可加点*/
    if (ifReset) {
        if (ifAdded1) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded1", false);
            ifAdded1 = false;
        }
        if (ifAdded2) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded2", false);
            ifAdded2 = false;
        }
        if (ifAdded3) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded3", false);
            ifAdded3 = false;
        }
        if (ifAdded4) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded4", false);
            ifAdded4 = false;
        }
        if (ifAdded5) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded5", false);
            ifAdded5 = false;
        }
        if (ifAdded6) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded6", false);
            ifAdded6 = false;
        }if (ifAdded7) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded7", false);
            ifAdded7 = false;
        }if (ifAdded8) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded8", false);
            ifAdded8 = false;
        }if (ifAdded9) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("ifAdded9", false);
            ifAdded9 = false;
        }
        
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    ifReset = true;
}

void Inhesion::resetAllData()
{
    /*写入数据*/
    CCUserDefault::sharedUserDefault()->setFloatForKey("data1", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data2", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data3", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data4", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data5", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data6", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data7", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data8", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("data9", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("several", Data::sharedData()->severalWithPoints);
    
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion1", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion2", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion3", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion4", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion5", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion6", 0);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion7", 1);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion8", 1);
    CCUserDefault::sharedUserDefault()->setFloatForKey("savedInhesion9", 1);
    
    CCUserDefault::sharedUserDefault()->flush();
    
    ifAdded1 = false;
    ifAdded2 = false;
    ifAdded3 = false;
    ifAdded4 = false;
    ifAdded5 = false;
    ifAdded6 = false;
    ifAdded7 = false;
    ifAdded8 = false;
    ifAdded9 = false;
    
    /*设置数据*/
    set();
}


#pragma mark - 剩余点数
void Inhesion::residue()
{
    /*文字*/
    CCSprite* residue = CCSprite::create("residue.png");
    addChild(residue);
    residue->setScaleX(0.4);
    residue->setScaleY(0.55);
    residue->setPosition(ccp(residue->boundingBox().size.width * 0.5 + 12, size.height - residue->boundingBox().size.height * 0.5 - 10));
    
    /*数值*/
    labelNumber = CCLabelAtlas::create("0", "testfont.plist");
    addChild(labelNumber);
    labelNumber->setAnchorPoint(ccp(0.45, 0));
    labelNumber->setScale(0.5);
    labelNumber->setPosition(ccp(residue->getPositionX(), residue->getPositionY() - labelNumber->boundingBox().size.height - 20));
    CCString* string = CCString::createWithFormat("%d", several);
    labelNumber->setString(string->getCString());
}

void Inhesion::refreshResidue()
{
    CCString* string = CCString::createWithFormat("%d", --several);
    labelNumber->setString(string->getCString());
}


#pragma mark - 确定与重置
void Inhesion::confirm()
{
    CCMenuItemImage* item = CCMenuItemImage::create("confirm_normal.png", "confirm_selected.png", this, menu_selector(Inhesion::save));
    CCMenu* menu = CCMenu::create(item, NULL);
    addChild(menu);
    menu->setScaleX(0.4);
    menu->setScaleY(0.6);
    menu->setPosition(ccp(size.width - menu->boundingBox().size.width * 0.97, -menu->boundingBox().size.height * 0.18));
}

void Inhesion::reset()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }
    
    CCMenuItemImage* item = CCMenuItemImage::create("reset_normal.png", "reset_selected.png", this, menu_selector(Inhesion::set));
    CCMenu* menu = CCMenu::create(item, NULL);
    addChild(menu);
    menu->setScaleX(0.4);
    menu->setScaleY(0.6);
    menu->setPosition(ccp(size.width - menu->boundingBox().size.width * 0.97, size.height - menu->boundingBox().size.height * 0.46));
}

void Inhesion::resetAll()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->playEffect("confirm.mp3");
    }
    
    CCLabelTTF* label = CCLabelTTF::create("清零", "Georgia-Bold", 45);
    CCMenuItemLabel* item = CCMenuItemLabel::create(label, this, menu_selector(Inhesion::resetAllData));
    CCMenu* menu = CCMenu::create(item, NULL);
    addChild(menu);
    menu->setPosition(ccp(size.width - item->boundingBox().size.width * 0.5 - 30, size.height * 0.5));
}

void Inhesion::onEnter()
{
    ifReset = true;
    
    ifAdded1 = false;
    ifAdded2 = false;
    ifAdded3 = false;
    ifAdded4 = false;
    ifAdded5 = false;
    ifAdded6 = false;
    ifAdded7 = false;
    ifAdded8 = false;
    ifAdded9 = false;
    
    CCLayer::onEnter();
}