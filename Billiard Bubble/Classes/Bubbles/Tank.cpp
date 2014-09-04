#include "Tank.h"
#include "Interface.h"
#include "Fish.h"
#include "Attacker.h"
#include "Healer.h"
#include "HPTank.h"
#include "SubsidiaryLayer.h"
#include "Data.h"
#include "HPFish.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

typedef enum
{
    tag_particle,
    tag_repeat,
}Tag_Tank;

static Tank* instance = NULL;

Tank* Tank::sharedTank()
{
    if (instance == NULL) {
        instance = new Tank();
    }
    return instance;
}

Tank* Tank::createTank(const char *fileName)
{
    instance = new Tank();
    if (instance && instance->initWithFile(fileName)) {
        instance->tankInit();
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    return NULL;
}

void Tank::tankInit()
{
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("hit.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("tankLaunch.mp3");
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*设置属性*/
    setPosition(ccp(size.width * 0.75, size.height * 0.75));
    setScale(0.25);
    
    /*初始化*/
    ifFinishedCooling = true;
    pointOld = CCPointZero;
    ifLaunched = false;
    ifFinished = false;
    nextTurns = false;
    moveCollision=false;
    collisionTankTouch=true;
    collisionAttackerTouch=true;
    collisionHealerTouch=true;
    collisionLeftRightTouch=true;
    touchMove = true;
    ifNext = false;
    
    drag = 0.5;
    collisionPoints=0;
    tankMoveOld=ccp(0,0);
    tankVelocity=0;
    tankNow=getPosition();
    tankOld=getPosition();
    tankRotationNow=90;
    tankRotationOld=90;
    spVelocityRecord = tankVelocity;
    spRotationNowRecord = tankRotationNow;
    frame = size.width/10.0;
    color = getColor();
    
    /*开启碰撞检测*/
    schedule(schedule_selector(Tank::updateDecreaseHP));
}


#pragma mark - 反弹相关
/*得到反弹角度的向量*/
CCPoint Tank::collisionBack(cocos2d::CCPoint moveIn,float k)
{
    CCPoint movePointToTemp;
    float a;
    /*根据碰撞的直线经过下面的算法来获得碰撞角度*/
    a=2*k-getAngle(moveIn.x, moveIn.y);
    /*角度转成向量，为向量合成做准备*/
    movePointToTemp=ccp(cos(a*3.1415926/180)*tankVelocity,sin(a*3.1415926/180)*tankVelocity);
    return movePointToTemp;
}

/*反弹的处理，对碰撞产生的向量进行合成，最终获得下一次的转向角度*/
float Tank::getCollisionAngle()
{
    int collisionShape=0;
    collisionPoints=0;
    CCPoint movePointToTemp=ccp(0,0);
    float movePointToAngle;
    static bool collisionTankfirst;
    static bool collisionAttackerfirst;
    static bool collisionHealerfirst;
    static bool collisionLeftRightfirst;
    static bool collisionUpDownfirst;
    
    /*左右两边Y轴的碰撞*/
    if(getPositionX()>=size.width-boundingBox().size.width*0.5-frame||getPositionX()<=boundingBox().size.width*0.5+frame)
    {
        /*进入碰撞后，等待出去才会在碰撞*/
        if(collisionLeftRightfirst)
        {
            /*播放音效*/
            if (Interface::sharedInterface()->ifPlayEffect) {
                SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
                SimpleAudioEngine::sharedEngine()->playEffect("hit.mp3");
            }
            float k;
            CCPoint movePointTo1=ccp(0,0);
            /*方法的选择*/
            collisionShape=1;
            /*记录是否有碰撞*/
            collisionPoints++;
            /*得到切线角度，或者得到精灵碰撞间的角度、速度变化*/
            k = getTangentAngle(collisionShape,NULL);
            /*用于向量合成*/
            movePointTo1 = collisionBack(tankMoveOld,k);
            movePointToTemp=ccp(movePointTo1.x+movePointToTemp.x,movePointTo1.y+movePointToTemp.y);
        }
        collisionLeftRightfirst=false;
    }
    else
    {
        collisionLeftRightfirst=true;
    }
    
    /*上下两边X轴的碰撞*/
    if(getPositionY()>=size.height-boundingBox().size.height*0.5||getPositionY()<=boundingBox().size.height*0.5)
    {
        if(collisionUpDownfirst)
        {
            /*播放音效*/
            if (Interface::sharedInterface()->ifPlayEffect) {
                SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
                SimpleAudioEngine::sharedEngine()->playEffect("hit.mp3");
            }
            float k;
            CCPoint movePointTo2=ccp(0,0);
            /*方法的选择*/
            collisionShape=2;
            /*记录是否有碰撞*/
            collisionPoints++;
            /*得到切线角度，或者得到精灵碰撞间的角度、速度变化*/
            k = getTangentAngle(collisionShape,NULL);
            /*用于向量合成*/
            movePointTo2 = collisionBack(tankMoveOld,k);
            movePointToTemp=ccp(movePointTo2.x+movePointToTemp.x,movePointTo2.y+movePointToTemp.y);
        }
        collisionUpDownfirst=false;
    }
    else
    {
        collisionUpDownfirst=true;
    }
    
    /*与BOSS的碰撞*/
    if (ccpDistance(getPosition(), Fish::sharedFish()->getPosition())<=boundingBox().size.width*0.5+Fish::sharedFish()->boundingBox().size.width*0.5)
    {
        if(tankVelocity<(Interface::sharedInterface()->fishVelocity/30))
        {
            tankAngleNew = getAngle( getPositionX()-Fish::sharedFish()->getPositionX(), getPositionY()-Fish::sharedFish()->getPositionY());
            tankVelocity = Interface::sharedInterface()->fishVelocity/30;
            tankAngleOld = tankAngleNew;
        }
        if(collisionTankfirst)
        {
            /*播放音效*/
            if (Interface::sharedInterface()->ifPlayEffect) {
                SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
                SimpleAudioEngine::sharedEngine()->playEffect("hit.mp3");
            }
            float k;
            CCPoint movePointTo3=ccp(0,0);
            /*方法的选择*/
            collisionShape=3;
            /*记录是否有碰撞*/
            collisionPoints++;
            /*得到切线角度，或者得到精灵碰撞间的角度、速度变化*/
            k = getTangentAngle(collisionShape,Fish::sharedFish());
            movePointTo3 = collisionBack(tankMoveOld,k);
            /*用于向量合成*/
            movePointToTemp=ccp(movePointTo3.x+movePointToTemp.x,movePointTo3.y+movePointToTemp.y);
        }
        collisionTankfirst=false;
    }
    else
    {
        collisionTankfirst=true;
    }
    /*与DPS间的碰撞判断*/
    if (ccpDistance(getPosition(), Attacker::sharedAttacker()->getPosition())<=boundingBox().size.width*0.5+Attacker::sharedAttacker()->boundingBox().size.width*0.5)
    {
        if(collisionAttackerfirst)
        {
            /*播放音效*/
            if (Interface::sharedInterface()->ifPlayEffect) {
                SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
                SimpleAudioEngine::sharedEngine()->playEffect("hit.mp3");
            }
            CCPoint movePointTo4=ccp(0,0);
            /*方法的选择*/
            collisionShape=4;
            /*记录是否有碰撞*/
            collisionPoints++;
            /*精灵间的碰撞的处理，并且得到运动角度的向量，改变运动速度大小*/
            getTangentAngle(collisionShape,Attacker::sharedAttacker());
            /*精灵间向量的分量*/
            movePointTo4 = ccp(angleSpTangentX,angleSpTangentY);
            movePointToTemp=ccp(movePointTo4.x+movePointToTemp.x,movePointTo4.y+movePointToTemp.y);
        }
        collisionAttackerfirst=false;
    }
    else
    {
        collisionAttackerfirst=true;
    }
    /*与奶妈间的碰撞判断*/
    if (ccpDistance(getPosition(), Healer::sharedHealer()->getPosition())<=boundingBox().size.width*0.5+Healer::sharedHealer()->boundingBox().size.width*0.5)
    {
        if(collisionHealerfirst)
        {
            /*播放音效*/
            if (Interface::sharedInterface()->ifPlayEffect) {
                SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3);
                SimpleAudioEngine::sharedEngine()->playEffect("hit.mp3");
            }
            
            CCPoint movePointTo5=ccp(0,0);
            /*方法的选择*/
            collisionShape=4;
            /*记录是否有碰撞*/
            collisionPoints++;
            /*精灵间的碰撞的处理，并且得到运动角度的向量，改变运动速度大小*/
            getTangentAngle(collisionShape,Healer::sharedHealer());
            movePointTo5 = ccp(angleSpTangentX,angleSpTangentY);
            /*精灵间向量的分量*/
            movePointToTemp=ccp(movePointTo5.x+movePointToTemp.x,movePointTo5.y+movePointToTemp.y);
        }
        collisionHealerfirst=false;
    }
    else
    {
        collisionHealerfirst=true;
    }
    /*向量转成角度*/
    movePointToAngle=getAngle(movePointToTemp.x,movePointToTemp.y);
    
    if (collisionPoints!=0)
    {
        tankAngleNew=movePointToAngle;
        tankMoveOld = ccp(cos(tankAngleNew*3.1415926/180),sin(tankAngleNew*3.1415926/180));
    }
    return tankAngleNew;
}
/*下一帧是否碰撞*/
bool Tank::pointNextIfOut()
{
    int out=0;
    /*Y轴碰撞*/
    if(pointNext.x>=size.width-boundingBox().size.width*0.5-frame||pointNext.x<=boundingBox().size.width*0.5+frame)
    {
        out++;
        /*实际碰撞的线条*/
        float leftOrRight;
        if (tankNow.x<size.width/2)
        {
            leftOrRight=boundingBox().size.width*0.5+frame;
        }
        else leftOrRight=size.width-boundingBox().size.width*0.5-frame;
        if (nextTurns)
        {
            /*偏正处理*/
            setPosition(ccp(leftOrRight,tankNow.y));
            ifNext =true ;
            
        }
    }
    /*X轴碰撞*/
    if(pointNext.y>=size.height-boundingBox().size.height*0.5||pointNext.y<=boundingBox().size.height*0.5)
    {
        out++;
        /*实际碰撞的线条*/
        float UpOrDown;
        if (tankNow.y<size.height/2)
        {
            UpOrDown=boundingBox().size.height*0.5;
        }
        else UpOrDown=size.height-boundingBox().size.height*0.5;
        if (nextTurns)
        {
            /*偏正处理*/
            setPosition(ccp(getPositionX(),UpOrDown));
            ifNext =true ;
            
        }
    }
    /*与BOSS的碰撞*/
    if (ccpDistance(pointNext, Fish::sharedFish()->getPosition())<=boundingBox().size.width*0.5+Fish::sharedFish()->boundingBox().size.width*0.5)
    {
        out++;
        if (nextTurns)
        {
            /*暂时未写内嵌处理*/
            setPosition(ccpAdd(getPosition(),ccp(tankVelocity*cos(tankAngleNew*3.1415926/180),tankVelocity*sin(tankAngleNew*3.1415926/180))));
            ifNext =true ;
            
        }
    }
    /*与dps的碰撞*/
    if (ccpDistance(pointNext, Attacker::sharedAttacker()->getPosition())<=boundingBox().size.width*0.5+Attacker::sharedAttacker()->boundingBox().size.height*0.5)
    {
        out++;
        if (nextTurns)
        {
            setPosition(ccpAdd(getPosition(),ccp(tankVelocity*cos(tankAngleNew*3.1415926/180),tankVelocity*sin(tankAngleNew*3.1415926/180))));
            ifNext =true ;
            
        }
    }
    /*与奶妈的碰撞*/
    if (ccpDistance(pointNext, Healer::sharedHealer()->getPosition())<=boundingBox().size.width*0.5+Healer::sharedHealer()->boundingBox().size.width*0.5)
    {
        out++;
        if (nextTurns)
        {
            /*暂时未写内嵌处理*/
            setPosition(ccpAdd(getPosition(),ccp(tankVelocity*cos(tankAngleNew*3.1415926/180),tankVelocity*sin(tankAngleNew*3.1415926/180))));
            ifNext =true ;
            
        }
    }
    
    if (out!=0) {
        return true;
    }
    else return false;
}

/*把两个角度转换成一个0~90度的角度*/
float Tank::getAngle_0_90(float angle1,float angle2)
{
    int angle_0_90=angle1-angle2;
    
    if (angle_0_90<0)
    {
        angle_0_90=-angle_0_90;
    }
    
    if (angle_0_90>=270)
    {
        angle_0_90=360-angle_0_90;
    }
    if (angle_0_90>=90&&angle_0_90<=180)
    {
        angle_0_90=180-angle_0_90;
    }
    if (angle_0_90>180&&angle_0_90<270)
    {
        angle_0_90=angle_0_90-180;
    }
    
    return angle_0_90;
}

/*得到下一帧的运动轨迹，并且对精灵部分碰撞的速度进行处理*/
float Tank::getTangentAngle(int shape,CCSprite * sp)
{
    switch (shape)
    {
        case 1:
            return 90;
            break;
        case 2:
            return 0;
            break;
            /*质量差很大的碰撞情况，获得切线角度*/
        case 3:
            float angle;
            float angleTangent;
            angle =getAngle(sp->getPositionX()-getPositionX(),sp->getPositionY()-getPositionY());
            if (angle>=-90&&angle<90)
            {
                angleTangent=angle+90;
            }
            else if(angle>-180&&angle<-90)
            {
                angleTangent=angle+270;
            }
            else
            {
                angleTangent=angle-90;
            }
            return angleTangent;
            break;
            
            /*发生正碰撞的碰撞情况，获得向量合成及角度，改变精灵速度*/
        case 4:
            /*发生正碰撞后本精灵的运动角度*/
            float angle1;
            /*发生正碰撞后其他精灵传过来的角度*/
            float angle2;
            /*发生正碰撞后，经过向量合成传出去的角度*/
            float angleTangent4;
            /*传出去的角度的分量大小的角度*/
            float PassToSpAngle;
            /*传进来的角度的分量大小的角度*/
            float PassInSpAngle;
            /*两精灵间的指向角度*/
            float spMoveToSprite;
            float spriteMoveToSp;
            /*传来的向量角度*/
            float spAngleNew;
            /*传来的向量速度大小*/
            float spVelocity;
            /*把传进来的精灵的数据同步*/
            if (typeid(*sp) == typeid(*Tank::sharedTank()))
            {
                spAngleNew = Tank::sharedTank()->spRotationNowRecord;
                spVelocity = Tank::sharedTank()->spVelocityRecord;
            }
            
            if (typeid(*sp) == typeid(*Attacker::sharedAttacker()))
            {
                spAngleNew = Attacker::sharedAttacker()->spRotationNowRecord;
                spVelocity = Attacker::sharedAttacker()->spVelocityRecord;
            }
            
            if (typeid(*sp) == typeid(*Healer::sharedHealer()))
            {
                spAngleNew = Healer::sharedHealer()->spRotationNowRecord;
                spVelocity = Healer::sharedHealer()->spVelocityRecord;
            }
            /*更新传来的向量速度大小和角度*/
            spVelocityRecord = tankVelocity;
            spRotationNowRecord = tankRotationNow;
            
            spriteMoveToSp = getAngle(sp->getPositionX() - getPositionX(),sp->getPositionY() - getPositionY());
            spMoveToSprite = getAngle(getPositionX()-sp->getPositionX() , getPositionY()- sp->getPositionY());
            PassToSpAngle = getAngle_0_90(spriteMoveToSp, tankAngleNew);
            PassInSpAngle = getAngle_0_90(spMoveToSprite, spAngleNew);
            
            angle1=spriteMoveToSp+90;
            
            if(( angle1-tankAngleNew>=-270&&angle1-tankAngleNew<=-90)||(angle1-tankAngleNew>=90&&angle1-tankAngleNew<=270))
            {
                angle1 = spriteMoveToSp-90;
            }
            angle2=spMoveToSprite;
            angleSpTangentX = tankVelocity*sin(PassToSpAngle*3.1415926/180)*cos(angle1*3.1415926/180)+spVelocity*cos(PassInSpAngle*3.1415926/180)*cos(angle2*3.1415926/180);
            
            angleSpTangentY = tankVelocity*sin(PassToSpAngle*3.1415926/180)*sin(angle1*3.1415926/180)+spVelocity*cos(PassInSpAngle*3.1415926/180)*sin(angle2*3.1415926/180);
            
            angleTangent4 = getAngle(angleSpTangentX, angleSpTangentY);
            tankVelocity = ccpDistance(ccp(0,0), ccp(angleSpTangentX,angleSpTangentY));                       return angleTangent4;
        default:
            return 0;//可继续添加新情况
            break;
    }
}

/*得到碰撞位置*/
float Tank::getContactPosition(CCSprite * sp)
{
    float bossMoveAngleNow;
    float attackerTOBoss;
    float collisionBody;
    /*区分传进来的精灵并同步数据*/
    if (typeid(*sp) == typeid(*Fish::sharedFish()))
    {
        bossMoveAngleNow=Fish::sharedFish()->sharkRotationNow;
    }
    
    if (typeid(*sp) == typeid(*Tank::sharedTank()))
    {
        bossMoveAngleNow=Tank::sharedTank()->tankRotationNow;
    }
    
    if (typeid(*sp) == typeid(*Attacker::sharedAttacker()))
    {
        bossMoveAngleNow=Attacker::sharedAttacker()->attackerRotationNow;
    }
    
    if (typeid(*sp) == typeid(*Healer::sharedHealer()))
    {
        bossMoveAngleNow=Healer::sharedHealer()->healerRotationNow;
    }
    
    attackerTOBoss=getAngle(sp->getPositionX()-getPositionX(),sp->getPositionY()-getPositionY());
    
    collisionBody=attackerTOBoss-bossMoveAngleNow;
    
    if(collisionBody>=-360&&collisionBody<=-180)
    {
        collisionBody+=360;
    }
    
    if(collisionBody>-180&&collisionBody<0)
    {
        collisionBody=-collisionBody;
    }
    
    if(collisionBody>180&&collisionBody<=360)
    {
        collisionBody=360-collisionBody;
    }
    collisionBody=180-collisionBody;
    
    return collisionBody;
}

/*将坐标转成角度，坐标右上为正，向右为0度,范围正负180*/
float Tank::getAngle(float x,float y)
{
    float anglereturn;
    anglereturn=atan2(y,x)*180/3.1415926;
    return anglereturn;
}


#pragma mark - 本类中的逻辑

void Tank::updateDecreaseHP(float delta)
{
    static float time1 = 0;
    static float time2 = 0;
    static bool haveAttacted1 = false;
    static bool haveAttacted2 = false;
    float contactPosition = 0;
    static bool collisionTankfirst;
    
    if (ccpDistance(getPosition(), Fish::sharedFish()->getPosition())<=boundingBox().size.width*0.5+Fish::sharedFish()->boundingBox().size.width*0.5) {
        if(collisionTankfirst) {
            contactPosition = getContactPosition(Fish::sharedFish());
            if (!haveAttacted1 && contactPosition >= 60 && contactPosition <= 180) {
                /*T撞到敌人除头部外的位置才掉血*/
                Fish::sharedFish()->wounded();                            /*敌人执行被撞动作*/
                Interface::sharedInterface()->showEnemyHPDropping(1);     /*减血效果*/
                scheduleOnce(schedule_selector(Interface::removeDrop1), 0.8);
                Data::sharedData()->addPointsWithTankAttack();            /*积分增加*/
                Interface::sharedInterface()->augmentTankAttackThreat();
                HPFish::sharedHPFish()->decreaseTankAttackHP();
                haveAttacted1 = true;
            }
            else if (!haveAttacted2 && !(contactPosition >= 60 && contactPosition <= 180)) {
                contactPosition = getContactPosition(Fish::sharedFish());
                HPTank::sharedHPTank()->decreaseHP();                     /*T掉血*/
                wounded();                                                /*掉血效果*/
                Interface::sharedInterface()->showHPDropping(this);       /*减血效果*/
                scheduleOnce(schedule_selector(Interface::removeDrop1), 0.8);
                haveAttacted2 = true;
            }
        }
        collisionTankfirst=false;
    }
    else {
        collisionTankfirst=true;
    }
    
    /*设无敌时间*/
    if (time1 > 1.2) {
        haveAttacted1 = false;
        time1 = 0;
    }
    if (time2 > 2) {
        haveAttacted2 = false;
        time2 = 0;
    }
    if (haveAttacted1) {
        time1 += delta;
    }
    if (haveAttacted2) {
        time2 += delta;
    }
    
    
    
    /*针对每帧的碰撞处理*/
    bool ifOut=false;
    ifNext =false ;
    
    tankNow=getPosition();
    /*旋转角度同步*/
    if (tankVelocity!=0) {
        tankRotationNow =getAngle(tankNow.x-tankOld.x, tankNow.y-tankOld.y);
        tankRotationOld=tankRotationNow;
    }
    else tankRotationNow=tankRotationOld;
    
    setRotation(90-tankRotationNow);
    /*下一帧的位置*/
    pointNext=ccpAdd(getPosition(),ccp(tankVelocity*cos(tankAngleNew*3.1415926/180),tankVelocity*sin(tankAngleNew*3.1415926/180)));
    /*下一帧是否会碰撞，并做偏正处理*/
    ifOut=pointNextIfOut();
    
    
    /*反弹的处理，对碰撞产生的向量进行合成，最终获得下一次的转向角度*/
    tankAngleNew=getCollisionAngle();
    if(ifNext == false)
    {
        if (ifOut==false)
        {
            /*精灵运动轨迹*/
            setPosition(ccpAdd(getPosition(),ccp(tankVelocity*cos(tankAngleNew*3.1415926/180),tankVelocity*sin(tankAngleNew*3.1415926/180))));
        }
    }
    /*避免Bug的处理*/
    if (ifOut&&tankVelocity!=0)
    {
        nextTurns=true;
    }
    else nextTurns=false;
    
    tankOld=tankNow;
    
    if(tankVelocity>=drag)
    {
        tankVelocity-=drag;
    }
    else tankVelocity=0;
}

void Tank::wounded()
{
    CCTintTo* tintTo1 = CCTintTo::create(0.4, 255, 0, 0);
    CCTintTo* tintTo2 = CCTintTo::create(0.4, color.r, color.g, color.b);
    CCSequence* sequence = CCSequence::create(tintTo1, tintTo2, NULL);
    runAction(sequence);
}

void Tank::launch()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("tankLaunch.mp3", false);
    }
    
    /*增加仇恨*/
    Interface::sharedInterface()->augmentTankThreat();
    
    /*开启技能效果*/
    launchEffect(1);
    schedule(schedule_selector(Tank::launchEffect), 1);
    schedule(schedule_selector(Tank::removeEffect), 1);
    
    /*开始技能持续时间计时*/
    schedule(schedule_selector(Tank::durationUpdate));
    /*开启冷却进度条*/
    SubsidiaryLayer::sharedSubsidiaryLayer()->coolingAbsorb();
    /*开始冷却计时*/
    schedule(schedule_selector(Tank::startCoolingClock));
    
    /*积分增加*/
    Data::sharedData()->addPointsWithAbsorbing();
    
    ifLaunched = true;
}

void Tank::launchEffect(float delta)
{
    CCSprite *sprite1 = CCSprite::create("fire.png");
    CCSprite *sprite2 = CCSprite::create("fire.png");
    CCSprite *sprite3 = CCSprite::create("fire.png");
    CCSprite *sprite4 = CCSprite::create("fire.png");
    CCSprite *sprite5 = CCSprite::create("fire.png");
    CCSprite *sprite6 = CCSprite::create("fire.png");
    CCSprite *sprite7 = CCSprite::create("fire.png");
    CCSprite *sprite8 = CCSprite::create("fire.png");
    sprite1->setColor(ccc3(124, 252, 0));
    sprite2->setColor(ccc3(124, 252, 0));
    sprite3->setColor(ccc3(124, 252, 0));
    sprite4->setColor(ccc3(124, 252, 0));
    sprite5->setColor(ccc3(124, 252, 0));
    sprite6->setColor(ccc3(124, 252, 0));
    sprite7->setColor(ccc3(124, 252, 0));
    sprite8->setColor(ccc3(124, 252, 0));
    sprite1->setScale(2.0);
    sprite2->setScale(2.0);
    sprite3->setScale(2.0);
    sprite4->setScale(2.0);
    sprite5->setScale(2.0);
    sprite6->setScale(2.0);
    sprite7->setScale(2.0);
    sprite8->setScale(2.0);
    float f = 300;
    float x = 350;
    float y = 300;
    
    sprite1->setPosition(ccp(x-f/2, y +f*2/3+9));
    sprite2->setPosition(ccp(x - f*7/6-6, y + f/3+6));
    sprite3->setPosition(ccp(x - f*3/2-9, y-f/3));
    sprite4->setPosition(ccp(x - f*7/6-6, y - f-6));
    sprite5->setPosition(ccp(x-f/2, y-f*4/3-9));
    sprite6->setPosition(ccp(x + f/6+6, y - f-6));
    sprite7->setPosition(ccp(x + f/2+9, y-f/3));
    sprite8->setPosition(ccp(x + f/6+6, y + f/3+6));
    
    sprite1->setTag(2);
    sprite2->setTag(3);
    sprite3->setTag(4);
    sprite4->setTag(5);
    sprite5->setTag(6);
    sprite6->setTag(7);
    sprite7->setTag(8);
    sprite8->setTag(9);
    
    addChild(sprite1);
    addChild(sprite2);
    addChild(sprite3);
    addChild(sprite4);
    addChild(sprite5);
    addChild(sprite6);
    addChild(sprite7);
    addChild(sprite8);
    
    CCMoveTo *move1 = CCMoveTo::create(1.0, ccp(225, 225));
    CCMoveTo *move2 = CCMoveTo::create(1.0, ccp(225, 225));
    CCMoveTo *move3 = CCMoveTo::create(1.0, ccp(225, 225));
    CCMoveTo *move4 = CCMoveTo::create(1.0, ccp(225, 225));
    CCMoveTo *move5 = CCMoveTo::create(1.0, ccp(225, 225));
    CCMoveTo *move6 = CCMoveTo::create(1.0, ccp(225, 225));
    CCMoveTo *move7 = CCMoveTo::create(1.0, ccp(225, 225));
    CCMoveTo *move8 = CCMoveTo::create(1.0, ccp(225, 225));
    
    CCScaleBy *scale1 = CCScaleBy::create(1.0, 0.5);
    CCScaleBy *scale2 = CCScaleBy::create(1.0, 0.5);
    CCScaleBy *scale3 = CCScaleBy::create(1.0, 0.5);
    CCScaleBy *scale4 = CCScaleBy::create(1.0, 0.5);
    CCScaleBy *scale5 = CCScaleBy::create(1.0, 0.5);
    CCScaleBy *scale6 = CCScaleBy::create(1.0, 0.5);
    CCScaleBy *scale7 = CCScaleBy::create(1.0, 0.5);
    CCScaleBy *scale8 = CCScaleBy::create(1.0, 0.5);
    
    CCActionInterval *easeSinIn1 = CCEaseSineIn::create(move1);
    CCActionInterval *easeSinIn2 = CCEaseSineIn::create(move2);
    CCActionInterval *easeSinIn3 = CCEaseSineIn::create(move3);
    CCActionInterval *easeSinIn4 = CCEaseSineIn::create(move4);
    CCActionInterval *easeSinIn5 = CCEaseSineIn::create(move5);
    CCActionInterval *easeSinIn6 = CCEaseSineIn::create(move6);
    CCActionInterval *easeSinIn7 = CCEaseSineIn::create(move7);
    CCActionInterval *easeSinIn8 = CCEaseSineIn::create(move8);
    
    sprite1->runAction(easeSinIn1);
    sprite1->runAction(scale1);
    sprite2->runAction(easeSinIn2);
    sprite2->runAction(scale2);
    sprite3->runAction(easeSinIn3);
    sprite3->runAction(scale3);
    sprite4->runAction(easeSinIn4);
    sprite4->runAction(scale4);
    sprite5->runAction(easeSinIn5);
    sprite5->runAction(scale5);
    sprite6->runAction(easeSinIn6);
    sprite6->runAction(scale6);
    sprite7->runAction(easeSinIn7);
    sprite7->runAction(scale7);
    sprite8->runAction(easeSinIn8);
    sprite8->runAction(scale8);
}

void Tank::removeEffect(float delta)
{
    for(int i=2;i<10;i++)
    {
        removeChildByTag(i);
    }
}

void Tank::durationUpdate(float delta)
{
    static float time = 0;
    
    if (time > Interface::sharedInterface()->absorbDuration) {
        /*技能结束*/
        Interface::sharedInterface()->lessenTankThreat();     /*使仇恨降低*/
        unschedule(schedule_selector(Tank::launchEffect));    /*使粒子效果消失*/
        unschedule(schedule_selector(Tank::removeEffect));
        unschedule(schedule_selector(Tank::durationUpdate));
        removeAllChildren();
        ifLaunched = false;
        time = 0;
    }
    
    time+=delta;
}

void Tank::startCoolingClock(float delta)
{
    static float time = 0;
    if (time < Interface::sharedInterface()->absorbCoolingTime) {
        time += delta;
        ifFinishedCooling = false;
    }
    else {
        ifFinishedCooling = true;
        time = 0;
        unschedule(schedule_selector(Tank::startCoolingClock));
    }
}


#pragma mark - 触摸事件
bool Tank::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    moveCollision = false;
    touchMove = true;
    
    float distance = ccpDistance(this->getPosition(), pTouch->getLocation());
    float radius = this->boundingBox().size.width * 0.6 + (CCUserDefault::sharedUserDefault()->getFloatForKey("slider", 3) - 3) * 30;
    pointOld = pTouch->getLocation();
    
    if (distance < radius)
    {
        pointDistance=0;
        tankVelocity=0;
        
        return true;
    }
    return false;
}

/*触摸平滑处理*/
void Tank::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (touchMove)
    {
        pointDistance = ccpDistance(pTouch->getLocation(), pointOld);
        
        moveByX=pTouch->getLocation().x-pointOld.x;
        moveByY=pTouch->getLocation().y-pointOld.y;
        
        setPosition(ccpAdd(getPosition(),ccp(moveByX/2,moveByY/2)));
        
        
        if(getPositionX()>=size.width-boundingBox().size.width*0.5-frame||getPositionX()<=boundingBox().size.width*0.5+frame)
        {
            
            /*进入碰撞后，等待出去才会再碰撞*/
            if(collisionLeftRightTouch)
            {
                tankAngleNew = getAngle(moveByX, moveByY);
                tankVelocity = pointDistance*0.15;
                tankAngleOld = tankAngleNew;
                tankMoveOld = ccp(moveByX, moveByY);
                moveCollision = true;
                touchMove = false;
                
                
            }
            collisionLeftRightTouch=false;
        }
        else
        {
            collisionLeftRightTouch=true;
        }
        
        if (ccpDistance(getPosition(), Fish::sharedFish()->getPosition())<=boundingBox().size.width*0.5+Fish::sharedFish()->boundingBox().size.width*0.5)
        {
            if(collisionTankTouch)
            {
                tankAngleNew = getAngle(moveByX, moveByY);
                tankVelocity = pointDistance*0.15;
                tankAngleOld = tankAngleNew;
                tankMoveOld = ccp(moveByX, moveByY);
                moveCollision = true;
                touchMove = false;
                
            }
            collisionTankTouch=false;
        }
        else
        {
            collisionTankTouch=true;
        }
        /*与DPS间的碰撞判断*/
        if (ccpDistance(getPosition(), Attacker::sharedAttacker()->getPosition())<=boundingBox().size.width*0.5+Attacker::sharedAttacker()->boundingBox().size.width*0.5)
        {
            if(collisionAttackerTouch)
            {
                tankAngleNew = getAngle(moveByX, moveByY);
                tankVelocity = pointDistance*0.25;
                tankAngleOld = tankAngleNew;
                tankMoveOld = ccp(moveByX, moveByY);
                moveCollision = true;
                touchMove = false;
                
            }
            collisionAttackerTouch=false;
        }
        else
        {
            collisionAttackerTouch=true;
        }
        /*与奶妈间的碰撞判断*/
        if (ccpDistance(getPosition(), Healer::sharedHealer()->getPosition())<=boundingBox().size.width*0.5+Healer::sharedHealer()->boundingBox().size.width*0.5)
        {
            if(collisionHealerTouch)
            {
                tankAngleNew = getAngle(moveByX, moveByY);
                tankVelocity = pointDistance*0.15;
                tankAngleOld = tankAngleNew;
                tankMoveOld = ccp(moveByX, moveByY);
                moveCollision = true;
                touchMove = false;
                
            }
            collisionHealerTouch=false;
        }
        else
        {
            collisionHealerTouch=true;
        }
        
        pointOld=pTouch->getLocation();
    }
}

/*触摸精灵后发射速度*/
void Tank::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (moveCollision==false)
    {
        tankAngleNew = getAngle(moveByX, moveByY);
        tankVelocity = pointDistance*0.3;
        tankAngleOld = tankAngleNew;
        tankMoveOld = ccp(moveByX, moveByY);
    }
    
    if (Healer::sharedHealer()->ifLaunched)
    {
        /*如满血则不加血*/
        if (!(HPTank::sharedHPTank()->progressTimer->getPercentage() < 100)) {
            return;
        }
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("increaseHP.mp3");
        }
        HPTank::sharedHPTank()->increaseHP(2);
        Interface::sharedInterface()->showLaunchedRecovering(this);
        scheduleOnce(schedule_selector(Interface::removeRecover1), 0.8);
    }
}

void Tank::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void Tank::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}