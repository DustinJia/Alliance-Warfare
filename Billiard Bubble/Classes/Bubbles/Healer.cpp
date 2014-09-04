#include "Healer.h"
#include "Tank.h"
#include "HPHealer.h"
#include "HPTank.h"
#include "Interface.h"
#include "Fish.h"
#include "SubsidiaryLayer.h"
#include "Attacker.h"
#include "HPDPS.h"
#include "Data.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

typedef enum
{
    tag_repeat,
    tag_circle,
    tag_sun,
}Tag_Healer;

static Healer* instance = NULL;

Healer* Healer::sharedHealer()
{
    if (instance == NULL) {
        instance = new Healer();
    }
    return instance;
}

Healer* Healer::createHealer(const char *fileName)
{
    instance = new Healer();
    if (instance && instance->initWithFile(fileName)) {
        instance->healerInit();
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    return NULL;
}

void Healer::healerInit()
{
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("increaseHP.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("hit.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("healerLaunch.mp3");
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*设置属性*/
    setPosition(ccp(size.width * 0.75, size.height * 0.25));
    setScale(0.25);
    
    /*开启补血检测*/
    schedule(schedule_selector(Healer::updateIncreaseTank));
    schedule(schedule_selector(Healer::updateIncreaseDPS));
    
    /*开启受伤检测*/
    schedule(schedule_selector(Healer::updateWound));
    
    /*初始化*/
    pointOld = CCPointZero;
    ifFinishedCooling = true;
    ifLaunched = false;
    ifFinished = false;
    nextTurns = false;
    ifNext = false;
    collisionTankTouch=true;
    collisionAttackerTouch=true;
    collisionHealerTouch=true;
    collisionLeftRightTouch=true;
    touchMove = true;
    drag = 0.5;
    collisionPoints=0;
    healerMoveOld=ccp(0,0);
    healerVelocity=0;
    healerNow=getPosition();
    healerOld=getPosition();
    healerRotationNow=90;
    healerRotationOld=90;
    spVelocityRecord = healerVelocity;
    spRotationNowRecord = healerRotationNow;
    frame = size.width/10.0;
    color = getColor();
    
    /*开启碰撞检测*/
    schedule(schedule_selector(Healer::updateCollision));
    
}

#pragma mark - 反弹相关
void Healer::updateCollision(float delta)
{
    /*针对每帧的碰撞处理*/
    bool ifOut=false;
    ifNext =false ;
    
    healerNow=getPosition();
    /*旋转角度同步*/
    if (healerVelocity!=0)
    {
        healerRotationNow =getAngle(healerNow.x-healerOld.x, healerNow.y-healerOld.y);
        healerRotationOld=healerRotationNow;
    }
    else healerRotationNow=healerRotationOld;
    
    setRotation(90-healerRotationNow);
    healerRotationNow =getAngle(healerNow.x-healerOld.x, healerNow.y-healerOld.y);
    /*下一帧的位置*/
    
    pointNext=ccpAdd(getPosition(),ccp(healerVelocity*cos(healerAngleNew*3.1415926/180),healerVelocity*sin(healerAngleNew*3.1415926/180)));
    
    /*下一帧是否会碰撞，并做偏正处理*/
    ifOut=pointNextIfOut();
    /*反弹的处理，对碰撞产生的向量进行合成，最终获得下一次的转向角度*/
    healerAngleNew=getCollisionAngle();
    if(ifNext == false)
    {
        if (ifOut==false)
        {
            /*精灵运动轨迹*/
            setPosition(ccpAdd(getPosition(),ccp(healerVelocity*cos(healerAngleNew*3.1415926/   180),healerVelocity*sin(healerAngleNew*3.1415926/180))));
        }
    }
    /*避免Bug的处理*/
    if (ifOut&&healerVelocity!=0)
    {
        nextTurns=true;
    }
    else nextTurns=false;
    
    healerOld=healerNow;
    
    if(healerVelocity>=drag)
    {
        healerVelocity-=drag;
    }
    else healerVelocity=0;
}

/*得到反弹角度的向量*/
CCPoint Healer::collisionBack(cocos2d::CCPoint moveIn,float k)
{
    CCPoint movePointToTemp;
    float a;
    /*根据碰撞的直线经过下面的算法来获得碰撞角度*/
    a=2*k-getAngle(moveIn.x, moveIn.y);
    /*角度转成向量，为向量合成做准备*/    movePointToTemp=ccp(cos(a*3.1415926/180)*healerVelocity,sin(a*3.1415926/180)*healerVelocity);
    return movePointToTemp;
}

/*反弹的处理，对碰撞产生的向量进行合成，最终获得下一次的转向角度*/
float Healer::getCollisionAngle()
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
            movePointTo1 = collisionBack(healerMoveOld,k);
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
            movePointTo2 = collisionBack(healerMoveOld,k);
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
        if (healerVelocity<Interface::sharedInterface()->fishVelocity/30)
        {
            healerAngleNew = getAngle( getPositionX()-Fish::sharedFish()->getPositionX(), getPositionY()-Fish::sharedFish()->getPositionY());
            healerVelocity = Interface::sharedInterface()->fishVelocity/30;
            healerAngleOld = healerAngleNew;
        }
        if(collisionHealerfirst)
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
            movePointTo3 = collisionBack(healerMoveOld,k);
            /*用于向量合成*/
            movePointToTemp=ccp(movePointTo3.x+movePointToTemp.x,movePointTo3.y+movePointToTemp.y);
        }
        collisionHealerfirst=false;
    }
    else
    {
        collisionHealerfirst=true;
    }
    
    /*与T的碰撞判断*/
    if (ccpDistance(getPosition(), Tank::sharedTank()->getPosition())<=boundingBox().size.width*0.5+Tank::sharedTank()->boundingBox().size.width*0.5)
    {
        if(collisionTankfirst)
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
            getTangentAngle(collisionShape,Tank::sharedTank());
            /*精灵间向量的分量*/
            movePointTo4 = ccp(angleSpTangentX,angleSpTangentY);
            movePointToTemp=ccp(movePointTo4.x+movePointToTemp.x,movePointTo4.y+movePointToTemp.y);
        }
        collisionTankfirst=false;
    }
    else
    {
        collisionTankfirst=true;
    }
     healerCollisionTank = collisionTankfirst;
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
            
            CCPoint movePointTo5=ccp(0,0);
            /*方法的选择*/
            collisionShape=4;
            /*记录是否有碰撞*/
            collisionPoints++;
            /*精灵间的碰撞的处理，并且得到运动角度的向量，改变运动速度大小*/
            getTangentAngle(collisionShape,Attacker::sharedAttacker());
            movePointTo5 = ccp(angleSpTangentX,angleSpTangentY);
            /*精灵间向量的分量*/
            movePointToTemp=ccp(movePointTo5.x+movePointToTemp.x,movePointTo5.y+movePointToTemp.y);
        }
        collisionAttackerfirst=false;
    }
    else
    {
        collisionAttackerfirst=true;
    }
     healerCollisionAttacker = collisionAttackerfirst;
    /*向量转成角度*/
    movePointToAngle=getAngle(movePointToTemp.x,movePointToTemp.y);
    
    if (collisionPoints!=0)
    {
        healerAngleNew=movePointToAngle;
        healerMoveOld = ccp(cos(healerAngleNew*3.1415926/180),sin(healerAngleNew*3.1415926/180));
    }
    return healerAngleNew;
}

/*下一帧是否碰撞*/
bool Healer::pointNextIfOut()
{
    int out=0;
    /*Y轴碰撞*/
    if(pointNext.x>=size.width-boundingBox().size.width*0.5-frame||pointNext.x<=boundingBox().size.width*0.5+frame)
    {
        out++;
        /*实际碰撞的线条*/
        float leftOrRight;
        if (healerNow.x<size.width/2)
        {
            leftOrRight=boundingBox().size.width*0.5+frame;
        }
        else leftOrRight=size.width-boundingBox().size.width*0.5-frame;
        if (nextTurns)
        {
            /*偏正处理*/
            setPosition(ccp(leftOrRight,getPositionY()+healerVelocity*sin(healerAngleNew*3.1415926/180)));
            ifNext =true ;
        }
    }
    /*X轴碰撞*/
    if(pointNext.y>=size.height-boundingBox().size.height*0.5||pointNext.y<=boundingBox().size.height*0.5)
    {
        out++;
        /*实际碰撞的线条*/
        float UpOrDown;
        if (healerNow.y<size.height/2)
        {
            UpOrDown=boundingBox().size.height*0.5;
        }
        else UpOrDown=size.height-boundingBox().size.height*0.5;
        if (nextTurns)
        {
            setPosition(ccp(getPositionX()+healerVelocity*cos(healerAngleNew*3.1415926/180),UpOrDown));
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
            setPosition(ccpAdd(getPosition(),ccp(healerVelocity*cos(healerAngleNew*3.1415926/180),healerVelocity*sin(healerAngleNew*3.1415926/180))));
            ifNext =true ;
            
        }
    }
    /*与T的碰撞*/
    if (ccpDistance(pointNext, Tank::sharedTank()->getPosition())<=boundingBox().size.width*0.5+Tank::sharedTank()->boundingBox().size.width*0.5)
    {
        out++;
        if (nextTurns)
        {
            setPosition(ccpAdd(getPosition(),ccp(healerVelocity*cos(healerAngleNew*3.1415926/180),healerVelocity*sin(healerAngleNew*3.1415926/180))));
            ifNext =true ;
            
        }
    }
    /*与dps的碰撞*/
    if (ccpDistance(pointNext, Attacker::sharedAttacker()->getPosition())<=boundingBox().size.width*0.5+Attacker::sharedAttacker()->boundingBox().size.width*0.5)
    {
        out++;
        if (nextTurns)
        {
            /*暂时未写内嵌处理*/
            setPosition(ccpAdd(getPosition(),ccp(healerVelocity*cos(healerAngleNew*3.1415926/180),healerVelocity*sin(healerAngleNew*3.1415926/180))));
            ifNext =true ;
            
        }
    }
    
    if (out!=0)
    {
        return true;
    }
    else return false;
}

/*得到下一帧的运动轨迹，并且对精灵部分碰撞的速度进行处理*/
float Healer::getTangentAngle(int shape,CCSprite * sp)
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
            spVelocityRecord = healerVelocity;
            spRotationNowRecord = healerRotationNow;
            
            spriteMoveToSp = getAngle(sp->getPositionX() - getPositionX(),sp->getPositionY() - getPositionY());
            spMoveToSprite = getAngle(getPositionX()-sp->getPositionX() , getPositionY()- sp->getPositionY());;
            PassToSpAngle = getAngle_0_90(spriteMoveToSp, healerAngleNew);
            PassInSpAngle = getAngle_0_90(spMoveToSprite, spAngleNew);
            
            angle1=spriteMoveToSp+90;
            
            if(( angle1-healerAngleNew>=-270&&angle1-healerAngleNew<=-90)||(angle1-healerAngleNew>=90&&angle1-healerAngleNew<=270))
            {
                angle1 = spriteMoveToSp-90;
            }
            angle2=spMoveToSprite;
            angleSpTangentX = healerVelocity*sin(PassToSpAngle*3.1415926/180)*cos(angle1*3.1415926/180)+spVelocity*cos(PassInSpAngle*3.1415926/180)*cos(angle2*3.1415926/180);
            
            angleSpTangentY = healerVelocity*sin(PassToSpAngle*3.1415926/180)*sin(angle1*3.1415926/180)+spVelocity*cos(PassInSpAngle*3.1415926/180)*sin(angle2*3.1415926/180);
            
            angleTangent4 = getAngle(angleSpTangentX, angleSpTangentY);
            healerVelocity = ccpDistance(ccp(0,0), ccp(angleSpTangentX,angleSpTangentY));                       return angleTangent4;
        default:
            return angleTangent;//可继续添加新情况
            break;
    }
}

/*把两个角度转换成一个0~90度的角度*/
float Healer::getAngle_0_90(float angle1,float angle2)
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

/*得到碰撞位置*/
float Healer::getContactPosition(CCSprite * sp)
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
float Healer::getAngle(float x,float y)
{
    float anglereturn;
    anglereturn=atan2(y,x)*180/3.1415926;
    return anglereturn;
}


#pragma mark - 本类中的逻辑
void Healer::updateWound(float delta)
{
    static float time = 0;
    static bool haveAttacted = false;
    static float temp = 0;
    
    float distance = ccpDistance(getPosition(), Fish::sharedFish()->getPosition());
    float radius = (boundingBox().size.width + Fish::sharedFish()->boundingBox().size.width) * 0.5;
    float contactPosition = 0;
    contactPosition = getContactPosition(Fish::sharedFish());
    
    if (distance <= radius&& contactPosition >= 0 && contactPosition <=60) {
        if (!haveAttacted) {
            wounded();
            HPHealer::sharedHPHealer()->decreaseHP();                   /*掉血*/
            Interface::sharedInterface()->showHPDropping(this);         /*减血效果*/
            scheduleOnce(schedule_selector(Interface::removeDrop3), 0.8);
            haveAttacted = true;
        }
        /*设无敌时间*/
        if (time > 1.2) {
            haveAttacted = false;
            time = 0;
            temp = 0;
        }
        temp = delta;
    }
    time += temp;
}

void Healer::wounded()
{
    CCTintTo* tintTo1 = CCTintTo::create(0.4, 255, 0, 0);
    CCTintTo* tintTo2 = CCTintTo::create(0.4, color.r, color.g, color.b);
    CCSequence* sequence = CCSequence::create(tintTo1, tintTo2, NULL);
    if (ifLaunched) {
        coveredSprite->runAction(sequence);
    }
    else {
        runAction(sequence);
    }
}

/*补血检测*/
void Healer::updateIncreaseTank(float delta)
{
    static float time = 0;
    static bool haveAttacted = false;
    
    float distance = ccpDistance(getPosition(), Tank::sharedTank()->getPosition());
    float radius = (boundingBox().size.width + Tank::sharedTank()->boundingBox().size.width) * 0.5;
    
    if (distance <= radius)
    {
        if (healerCollisionTank && !haveAttacted)
        {
            /*如满血则不加血*/
            if (!(HPTank::sharedHPTank()->progressTimer->getPercentage() <100) || HPTank::sharedHPTank()->progressTimer->getPercentage() < 2) {
                return;
            }
            HPTank::sharedHPTank()->increaseHP(1);                                  /*T补血*/
            Interface::sharedInterface()->showHPRecovering(Tank::sharedTank());    /*补血效果*/
            scheduleOnce(schedule_selector(Interface::removeRecover1), 0.8);
            Data::sharedData()->addPointsWithHeal();                               /*积分增加*/
            haveAttacted = true;
        }
    }
    
    /*设技能冷却时间*/
    if (time > 1)
    {
        haveAttacted = false;
        time=0;
    }
    
    if (haveAttacted)
    {
        time += delta;
    }
}

void Healer::updateIncreaseDPS(float delta)
{
    static float time = 0;
    static bool haveAttacted = false;
    
    float distance = ccpDistance(getPosition(), Attacker::sharedAttacker()->getPosition());
    float radius = (boundingBox().size.width + Attacker::sharedAttacker()->boundingBox().size.width) * 0.5;
    
    if (distance <= radius)
    {
        if (healerCollisionAttacker && !haveAttacted)
        {
            /*如满血则不加血*/
            if (!(HPDPS::sharedHPDPS()->progressTimer->getPercentage() <100) || HPDPS::sharedHPDPS()->progressTimer->getPercentage() < 2) {
                return;
            }
            /*DPS补血*/
            HPDPS::sharedHPDPS()->increaseHP(1);
            /*补血效果*/
            Interface::sharedInterface()->showHPRecovering(Attacker::sharedAttacker());
            /*积分增加*/
            scheduleOnce(schedule_selector(Interface::removeRecover2), 0.8);
            Data::sharedData()->addPointsWithHeal();
        }
        /*设技能冷却时间*/
        if (time > 1)
        {
            haveAttacted = false;
            time = 0;
        }
        if (haveAttacted) {
            time += delta;
        }
    }
}

void Healer::launch()
{
    /*播放音效*/
    if (Interface::sharedInterface()->ifPlayEffect) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        SimpleAudioEngine::sharedEngine()->playEffect("healerLaunch.mp3", false);
    }
    
    /*增加仇恨*/
    Interface::sharedInterface()->augmentHealerThreat();
    
    /*开启技能效果*/
    setOpacity(0);
    
    CCSprite *sprite1 = CCSprite::create("circle1.png");
    sprite1->setPosition(ccp(205, 205));
    addChild(sprite1);
    sprite1->setTag(tag_circle);
    sprite1->setScale(0.38);
    sprite1->setColor(ccc3(0, 255, 0));
    CCRotateBy *rotate = CCRotateBy::create(Interface::sharedInterface()->healDuration, 120 * Interface::sharedInterface()->healDuration);
    sprite1->runAction(rotate);
    
    CCSprite *sp = CCSprite::create("sun.png");
    sp->setPosition(ccp(205, 205));
    addChild(sp);
    sp->setScale(0.7);
    sp->setTag(tag_sun);
    
    coveredSprite = CCSprite::create("healer.png");
    coveredSprite->setPosition(ccp(205, 205));
    addChild(coveredSprite);
    
    /*开始技能持续时间计时*/
    schedule(schedule_selector(Healer::durationUpdate));
    /*开启冷却进度条*/
    SubsidiaryLayer::sharedSubsidiaryLayer()->coolingHeal();
    /*开始冷却计时*/
    schedule(schedule_selector(Healer::startCoolingClock));
    
    ifLaunched = true;
    ifAddPoints = true;
}

void Healer::durationUpdate(float delta)
{
    static float time = 0;
    
    if (time > Interface::sharedInterface()->healDuration) {
        /*技能结束*/
        setOpacity(255);
        removeChildByTag(tag_sun);
        removeChildByTag(tag_circle);
        removeChild(coveredSprite, false);
        unschedule(schedule_selector(Healer::durationUpdate));
        ifLaunched = false;
        time = 0;
    }
    
    time+=delta;
}

void Healer::startCoolingClock(float delta)
{
    static float time = 0;
    if (time < Interface::sharedInterface()->healCoolingTime) {
        time += delta;
        ifFinishedCooling = false;
    }
    else {
        ifFinishedCooling = true;
        time = 0;
        unschedule(schedule_selector(Healer::startCoolingClock));
    }
}

void Healer::stopActionUpdate(float delta)
{
    /*开启速度检测*/
    Interface::sharedInterface()->monitorVelocity(this, delta);
    
    if (Interface::sharedInterface()->healerVelocity < 20 && Interface::sharedInterface()->healerVelocity > 0) {
        stopActionByTag(tag_repeat);
        setScale(0.2);
    }
}

bool Healer::isDoubleClick()
{
    /*当前触摸的时刻，精确到毫秒*/
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    long currentTouch = now.tv_sec * 1000 + now.tv_sec / 1000;
    /*上次触摸的时刻*/
    static long preTouch = 0;
    
    if (abs(currentTouch - preTouch) < 250) {
        preTouch = 0;
        return true;
    }
    preTouch = now.tv_sec * 1000 + now.tv_sec / 1000;
    return false;
}


#pragma mark - 触摸事件
bool Healer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    moveCollision = false;
    touchMove = true;
    float distance = ccpDistance(this->getPosition(), pTouch->getLocation());
    float radius = this->boundingBox().size.width * 0.6 + (CCUserDefault::sharedUserDefault()->getFloatForKey("slider", 3) - 3) * 30;
    pointOld = pTouch->getLocation();
    
    if (distance < radius)
    {
        pointDistance=0;
        healerVelocity=0;
        return true;
    }
    return false;
}

/*触摸平滑处理*/
void Healer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (touchMove)
    {
        
        pointDistance = ccpDistance(pTouch->getLocation(), pointOld);
        
        moveByX=pTouch->getLocation().x-pointOld.x;
        moveByY=pTouch->getLocation().y-pointOld.y;
        
        setPosition(ccpAdd(getPosition(),ccp(moveByX/2,moveByY/2)));
        
        
        /*左右两边Y轴的碰撞*/
        if(getPositionX()>=size.width-boundingBox().size.width*0.5-frame||getPositionX()<=boundingBox().size.width*0.5+frame)
        {
            /*进入碰撞后，等待出去才会在碰撞*/
            if(collisionLeftRightTouch)
            {
                healerAngleNew = getAngle(moveByX, moveByY);
                healerVelocity = pointDistance*0.15;
                healerAngleOld = healerAngleNew;
                healerMoveOld = ccp(moveByX, moveByY);
                moveCollision = true;
                touchMove = false;
            }
            collisionLeftRightTouch=false;
        }
        else
        {
            collisionLeftRightTouch=true;
        }
        
        
        /*与BOSS的碰撞*/
        if (ccpDistance(getPosition(), Fish::sharedFish()->getPosition())<=boundingBox().size.width*0.5+Fish::sharedFish()->boundingBox().size.width*0.5)
        {
            if(collisionHealerTouch)
            {
                healerAngleNew = getAngle(moveByX, moveByY);
                healerVelocity = pointDistance*0.15;
                healerAngleOld = healerAngleNew;
                healerMoveOld = ccp(moveByX, moveByY);
                moveCollision = true;
                touchMove = false;
            }
            collisionHealerTouch=false;
        }
        else
        {
            collisionHealerTouch=true;
        }
        
        /*与T的碰撞判断*/
        if (ccpDistance(getPosition(), Tank::sharedTank()->getPosition())<=boundingBox().size.width*0.5+Tank::sharedTank()->boundingBox().size.width*0.5)
        {
            if(collisionTankTouch)
            {
                healerAngleNew = getAngle(moveByX, moveByY);
                healerVelocity = pointDistance*0.3;
                healerAngleOld = healerAngleNew;
                healerMoveOld = ccp(moveByX, moveByY);
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
                healerAngleNew = getAngle(moveByX, moveByY);
                healerVelocity = pointDistance*0.15;
                healerAngleOld = healerAngleNew;
                healerMoveOld = ccp(moveByX, moveByY);
                moveCollision = true;
                touchMove = false;
                
            }
            collisionAttackerTouch=false;
        }
        else
        {
            collisionAttackerTouch=true;
        }
        
        
        pointOld=pTouch->getLocation();
    }
}

/*触摸精灵后发射速度*/
void Healer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (ifLaunched) {
        /*如满血则不加血*/
        if (!(HPHealer::sharedHPHealer()->progressTimer->getPercentage() < 100)) {
            return;
        }
        HPHealer::sharedHPHealer()->increaseHP();                /*给自己加血*/
        Interface::sharedInterface()->showHPRecovering(this);    /*补血效果*/
        scheduleOnce(schedule_selector(Interface::removeRecover3), 0.8);
        /*防止积分重复增加*/
        if (ifAddPoints) {
            Data::sharedData()->addPointsWithHeal();             /*积分增加*/
        }
        ifAddPoints = false;
    }
    else {
        if (moveCollision==false)
        {
            healerAngleNew = getAngle(moveByX, moveByY);
            healerVelocity = pointDistance*0.15;
            healerAngleOld = healerAngleNew;
            healerMoveOld = ccp(moveByX, moveByY);
        }
    }
}

void Healer::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void Healer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}