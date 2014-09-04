#include "Fish.h"
#include "Tank.h"
#include "Interface.h"
#include "Attacker.h"
#include "Healer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

typedef enum
{
    tag_sprite,
    tag_sequence,
    TANK,
    ATTACKER,
    HEALER,
}Tag_Fish;

static Fish* instance = NULL;

Fish* Fish::sharedFish()
{
    if (instance == NULL) {
        instance = new Fish();
    }
    return instance;
}

Fish* Fish::createFish(const char *fileName)
{
    instance = new Fish();
    if (instance && instance->initWithSpriteFrameName(fileName)) {
        instance->fishInit();
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    return NULL;
}

void Fish::fishInit()
{
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("monsterWound.wav");
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*设置属性*/
    setPosition(ccp(size.width * 0.25, size.height * 0.5));
    setScale(0.5);
    SharkMoveByVelocity=Interface::sharedInterface()->fishVelocity;
    SharkRotationVelocity=Interface::sharedInterface()->angularVelocity;
    
    /*添加动画*/
    animation();
    
    /*调用移动动作*/
    schedule(schedule_selector(Fish::update));
    
    /*初始化*/
    color = getColor();
}


#pragma mark - 动作
void Fish::animation()
{
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("boss.plist");
    
    CCAnimation* animation1 = CCAnimation::create();
    CCAnimation* animation2 = CCAnimation::create();
    CCAnimation* animation3 = CCAnimation::create();
    CCAnimation* animation4 = CCAnimation::create();
    CCAnimation* animation5 = CCAnimation::create();
        
    animation1->setDelayPerUnit(0.1f);
    animation1->setLoops(-1);
    animation2->setDelayPerUnit(0.1f);
    animation2->setLoops(-1);
    animation3->setDelayPerUnit(0.1f);
    animation3->setLoops(-1);
    animation4->setDelayPerUnit(0.1f);
    animation4->setLoops(-1);
    animation5->setDelayPerUnit(0.1f);
    animation5->setLoops(-1);

    /*敌人动画*/
    if (Interface::sharedInterface()->enemyType == 1) {
        for(int i=0;i<=2;i++)
        {
            CCString *fileName = CCString::createWithFormat("boss10_1_%0d.png",i);
            CCSpriteFrame *frame = frameCache->spriteFrameByName(fileName->getCString());
            animation1->addSpriteFrame(frame);
        }
        runAction(CCAnimate::create(animation1));
        
        /*添加阴影*/
        CCString* strShadow = CCString::create("boss10_0_0.png");
        CCSprite* spriteShadow = CCSprite::createWithSpriteFrameName(strShadow->getCString());
        addChild(spriteShadow);
        spriteShadow->setPosition(ccp(190, 130));
        
        /*添加眼睛*/
        CCAnimation *animation11 = CCAnimation::create();
        for(int i=0;i<=2;i++)
        {
            CCString *fileName = CCString::createWithFormat("boss10_2_%0d.png",i);
            CCSpriteFrame *frame = frameCache->spriteFrameByName(fileName->getCString());
            animation11->addSpriteFrame(frame);
        }
        animation11->setDelayPerUnit(0.1f);
        animation11->setLoops(-1);
        CCString* strEyes = CCString::create("boss10_2_0.png");
        CCSprite* spriteEyes = CCSprite::createWithSpriteFrameName(strEyes->getCString());
        addChild(spriteEyes);
        spriteEyes->setPosition(ccp(190, 190));
        spriteEyes->runAction(CCAnimate::create(animation11));
    }
    
    else if (Interface::sharedInterface()->enemyType == 2) {
        for(int i=0;i<=3;i++)
        {
            CCString *fileName = CCString::createWithFormat("zako09_0_%0d.png",i);
            CCSpriteFrame *frame = frameCache->spriteFrameByName(fileName->getCString());
            animation2->addSpriteFrame(frame);
        }
        runAction(CCAnimate::create(animation2));
    }
    
    else if (Interface::sharedInterface()->enemyType == 3) {
        for(int i=0;i<=3;i++)
        {
            CCString *fileName = CCString::createWithFormat("boss08_1_%0d.png",i);
            CCSpriteFrame *frame = frameCache->spriteFrameByName(fileName->getCString());
            animation3->addSpriteFrame(frame);
        }
        runAction(CCAnimate::create(animation3));
        
        /*添加阴影*/
        CCString* strShadow = CCString::create("boss08_0_0.png");
        CCSprite* spriteShadow = CCSprite::createWithSpriteFrameName(strShadow->getCString());
        addChild(spriteShadow);
        spriteShadow->setPosition(ccp(170, 120));
        
        /*添加眼睛*/
        CCAnimation *animation31 = CCAnimation::create();
        for(int i=0;i<=2;i++)
        {
            CCString *fileName = CCString::createWithFormat("boss08_2_%0d.png",i);
            CCSpriteFrame *frame = frameCache->spriteFrameByName(fileName->getCString());
            animation31->addSpriteFrame(frame);
        }
        animation31->setDelayPerUnit(0.1f);
        animation31->setLoops(-1);
        CCString* strEyes = CCString::create("boss08_2_0.png");
        CCSprite* spriteEyes = CCSprite::createWithSpriteFrameName(strEyes->getCString());
        addChild(spriteEyes);
        spriteEyes->setPosition(ccp(170, 180));
        spriteEyes->runAction(CCAnimate::create(animation31));
        
        /*添加树叶*/
        CCAnimation *animation32 = CCAnimation::create();
        for(int i=0;i<=2;i++)
        {
            CCString *fileName = CCString::createWithFormat("boss08_3_%0d.png",i);
            CCSpriteFrame *frame = frameCache->spriteFrameByName(fileName->getCString());
            animation32->addSpriteFrame(frame);
        }
        animation32->setDelayPerUnit(0.1f);
        animation32->setLoops(-1);
        CCString* strLeaf = CCString::create("boss08_3_0.png");
        CCSprite* spriteLeaf = CCSprite::createWithSpriteFrameName(strLeaf->getCString());
        addChild(spriteLeaf);
        spriteLeaf->setPosition(ccp(170, 100));
        spriteLeaf->runAction(CCAnimate::create(animation32));
    }
    
    else if (Interface::sharedInterface()->enemyType == 4) {
        for(int i=0;i<=5;i++)
        {
            CCString *fileName = CCString::createWithFormat("boss06_1_%0d.png",i);
            CCSpriteFrame *frame = frameCache->spriteFrameByName(fileName->getCString());
            animation4->addSpriteFrame(frame);
        }
        runAction(CCAnimate::create(animation4));
        
        /*添加阴影*/
        CCString* strShadow = CCString::create("boss06_0_0.png");
        CCSprite* spriteShadow = CCSprite::createWithSpriteFrameName(strShadow->getCString());
        addChild(spriteShadow);
        spriteShadow->setPosition(ccp(120, 80));
    }
    
    else if (Interface::sharedInterface()->enemyType == 5) {
        for(int i=0;i<=3;i++)
        {
            CCString *fileName = CCString::createWithFormat("boss09_0_%0d.png",i);
            CCSpriteFrame *frame = frameCache->spriteFrameByName(fileName->getCString());
            animation5->addSpriteFrame(frame);
        }
        runAction(CCAnimate::create(animation5));
    }
}

void Fish::update(float delta)
{
    /*精灵当前所在点的横纵坐标*/
    float SharkSpritePointNowX;
    float SharkSpritePointNowY;
    /*精灵每帧的移动距离*/
    float SharkMoveByDistance=SharkMoveByVelocity*0.01666666666;
    /*精灵与目标点的角度*/
    float SharkRotationTo;
    /*精灵每帧移动的坐标向量*/
    float SharkMoveByNowX=SharkMoveByDistance*cos(sharkRotationNow);
    float SharkMoveByNowY=SharkMoveByDistance*sin(sharkRotationNow);
    /*精灵与目标点的向量差*/
    float SharkMoveByX;
    float SharkMoveByY;
    /*精灵与目标点的偏正角度*/
    float SharkRotationSmall;
    
    /*根据仇恨系统判断追踪目标*/
    pointTo=trackingTarget();
    
    /*获得精灵的数据*/
    SharkSpritePointNowX = getPositionX();
    SharkSpritePointNowY = getPositionY();
    SharkMoveByX=pointTo.x-SharkSpritePointNowX;
    SharkMoveByY=pointTo.y-SharkSpritePointNowY;
    /*获得精灵的角度*/
    SharkRotationTo=this->getAngle(SharkMoveByX,SharkMoveByY);
    /*让精灵的头部与运动方向同步*/
    setRotation(-sharkRotationNow);
    /*精灵的旋转方向*/
    int SharkrotationVelocityturn;
    
    /*精灵的旋转方向人性化处理*/
    bool rotOne;
    bool rotTwo;
    rotTwo=SharkRotationTo-sharkRotationNow<SharkRotationVelocity&&SharkRotationTo-sharkRotationNow>-SharkRotationVelocity;
    rotOne=(SharkRotationTo-sharkRotationNow>SharkRotationVelocity&&SharkRotationTo-sharkRotationNow<=180)||(SharkRotationTo-sharkRotationNow<-180);
    /*精灵的旋转方向小于旋转方向*/
    if(rotTwo)
    {
        SharkRotationSmall=SharkRotationTo-sharkRotationNow;
        SharkRotationNext=sharkRotationNow+SharkRotationSmall;
        SharkMoveByNowX=SharkMoveByDistance*cos(sharkRotationNow*3.1415926/180);
        SharkMoveByNowY=SharkMoveByDistance*sin(sharkRotationNow*3.1415926/180);
    }
    /*精灵的旋转方向为逆时针*/
    else  if(rotOne)
    {
        SharkrotationVelocityturn=1;
        SharkRotationNext=sharkRotationNow+SharkrotationVelocityturn*SharkRotationVelocity;
        SharkMoveByNowX=SharkMoveByDistance*cos(sharkRotationNow*3.1415926/180);
        SharkMoveByNowY=SharkMoveByDistance*sin(sharkRotationNow*3.1415926/180);
    }
    /*精灵的旋转方向为顺时针*/
    else
    {
        SharkrotationVelocityturn=-1;
        SharkRotationNext=sharkRotationNow+SharkrotationVelocityturn*SharkRotationVelocity;
        SharkMoveByNowX=SharkMoveByDistance*cos(sharkRotationNow*3.1415926/180);
        SharkMoveByNowY=SharkMoveByDistance*sin(sharkRotationNow*3.1415926/180);
    }
    /*精灵每帧的移动向量*/
    CCActionInterval * SharkMove = CCMoveBy::create(0.0166666666, ccp(SharkMoveByNowX,SharkMoveByNowY));
    /*防止坐标从-180与180突变产生无限旋转的BUG*/
    runAction(SharkMove);
    if (SharkRotationNext>180)
    {
        SharkRotationNext-=360;
    }
    if(SharkRotationNext<-180)
    {
        SharkRotationNext+=360;
    }
    sharkRotationNow = SharkRotationNext;
}

void Fish::wounded()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("monsterWound.wav", false);
    }
    
    /*执行掉血动作*/
    
    CCTintTo* tintTo1 = CCTintTo::create(0.2, 0, 201, 87);
    CCTintTo* tintTo2 = CCTintTo::create(0.2, 0, 0, 255);
    CCTintTo* tintTo3 = CCTintTo::create(0.2, 255, 0, 255);
    CCTintTo* tintTo4 = CCTintTo::create(0.2, color.r, color.g, color.b);
    CCSequence* sequence = CCSequence::create(tintTo1, tintTo2, tintTo3, tintTo4, NULL);
    runAction(sequence);
}


#pragma mark - 接口
void Fish::getMoveVelocity(float moveVelocity)
{
    /*获得精灵线速度*/
    SharkMoveByVelocity=moveVelocity;
}

void Fish::getRotationVelocity(float rotationVelocity)
{   /*获得精灵角速度*/
    SharkRotationVelocity=rotationVelocity;
}

float Fish::getAngle(float x,float y)
{
    /*将坐标转成角度，坐标右上为正，向右为0度,范围正负180*/
    float anglereturn;
    anglereturn=atan2(y,x)*180/3.1415926;
    return anglereturn;
}

CCPoint Fish::trackingTarget()
{
    float largestThreatTarget = MAX(MAX(Interface::sharedInterface()->threatDPS, Interface::sharedInterface()->threatHealer), Interface::sharedInterface()->threatTank);
    
    /*比较仇恨值大小，默认追踪治疗*/
    if (largestThreatTarget > 1 && largestThreatTarget - Interface::sharedInterface()->threatTank < 0.01) {
        return Tank::sharedTank()->getPosition();
    }
    else if (largestThreatTarget > 1 && largestThreatTarget - Interface::sharedInterface()->threatDPS < 0.01)
    {
        return Attacker::sharedAttacker()->getPosition();
    }
    else
    {
        return Healer::sharedHealer()->getPosition();
    }
}