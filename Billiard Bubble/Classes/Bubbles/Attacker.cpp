#include "Attacker.h"
#include "Fish.h"
#include "HPFish.h"
#include "Interface.h"
#include "HPDPS.h"
#include "SubsidiaryLayer.h"
#include "World.h"
#include "SimpleAudioEngine.h"
#include "Healer.h"
#include "Tank.h"

using namespace CocosDenshion;

typedef enum
{
    tag_repeat,
    tag_light,
    tag_lightning,
    tag_circle,
    tag_newAttacker,
}Tag_Attacker;

static Attacker* instance = NULL;

Attacker* Attacker::sharedAttacker()
{
    if (instance == NULL) {
        instance = new Attacker();
    }
    return instance;
}

Attacker* Attacker::createAttacker(const char *fileName)
{
    instance = new Attacker();
    if (instance && instance->initWithFile(fileName)) {
        instance->attackerInit();
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    return NULL;
}

void Attacker::attackerInit()
{
    /*预加载音效*/
    SimpleAudioEngine::sharedEngine()->preloadEffect("hit.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("dpsSkill.mp3");
    
    /*获取屏幕尺寸*/
    size = CCDirector::sharedDirector()->getWinSize();
    
    /*设置属性*/
    setPosition(ccp(size.width * 0.75, size.height * 0.5));
    setScale(0.25);
    
    /*初始化*/
    pointOld = CCPointZero;
    ifFinishedCooling = true;
    ifLaunched = false;
    ifFinished = false;
    nextTurns = false;
    collisionTankTouch=true;
    collisionAttackerTouch=true;
    collisionHealerTouch=true;
    collisionLeftRightTouch=true;
    touchMove = true;
    ifNext = false;
    
    drag = 0.5;
    collisionPoints=0;
    attackerMoveOld=ccp(0,0);
    attackerVelocity=0;
    attackerNow=getPosition();
    attackerOld=getPosition();
    attackerRotationNow=90;
    attackerRotationOld=90;
    spVelocityRecord = attackerVelocity;
    spRotationNowRecord = attackerAngleNew;
    frame = size.width/10;
    color = getColor();
    
    /*添加逻辑*/
    schedule(schedule_selector(Attacker::updateAttacking));
}


#pragma mark - 反弹相关

/*得到反弹角度的向量*/
CCPoint Attacker::collisionBack(cocos2d::CCPoint moveIn,float k)
{
    CCPoint movePointToTemp;
    float a;
    /*根据碰撞的直线经过下面的算法来获得碰撞角度*/
    a=2*k-getAngle(moveIn.x, moveIn.y);
    /*角度转成向量，为向量合成做准备*/
    movePointToTemp=ccp(cos(a*3.1415926/180)*attackerVelocity,sin(a*3.1415926/180)*attackerVelocity);
    return movePointToTemp;
}

/*反弹的处理，对碰撞产生的向量进行合成，最终获得下一次的转向角度*/
float Attacker::getCollisionAngle()
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
            movePointTo1 = collisionBack(attackerMoveOld,k);
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
            movePointTo2 = collisionBack(attackerMoveOld,k);
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
        if (attackerVelocity<Interface::sharedInterface()->fishVelocity/30)
        {
            attackerAngleNew = getAngle( getPositionX()-Fish::sharedFish()->getPositionX(), getPositionY()-Fish::sharedFish()->getPositionY());
            attackerVelocity = Interface::sharedInterface()->fishVelocity/30;
            attackerAngleOld = attackerAngleNew;
        }
        if(collisionAttackerfirst)
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
            movePointTo3 = collisionBack(attackerMoveOld,k);
            /*用于向量合成*/
            movePointToTemp=ccp(movePointTo3.x+movePointToTemp.x,movePointTo3.y+movePointToTemp.y);
        }
        collisionAttackerfirst=false;
    }
    
    else
    {
        collisionAttackerfirst=true;
    }
    
    /*与MT间的碰撞判断*/
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
        attackerAngleNew=movePointToAngle;
        attackerMoveOld = ccp(cos(attackerAngleNew*3.1415926/180),sin(attackerAngleNew*3.1415926/180));
        
    }
    return attackerAngleNew;
}

/*下一帧是否碰撞*/
bool Attacker::pointNextIfOut()
{
    int out=0;
    /*Y轴碰撞*/
    if(pointNext.x>=size.width-boundingBox().size.width*0.5-frame||pointNext.x<=boundingBox().size.width*0.5+frame)
    {
        out++;
        /*实际碰撞的线条*/
        float leftOrRight;
        if (attackerNow.x<size.width/2)
        {
            leftOrRight=boundingBox().size.width*0.5+frame;
        }
        else leftOrRight=size.width-boundingBox().size.width*0.5-frame;
        if (nextTurns)
        {
            /*偏正处理*/
            setPosition(ccp(leftOrRight,attackerNow.y));
            ifNext =true ;
            
        }
    }
    /*X轴碰撞*/
    if(pointNext.y>=size.height-boundingBox().size.height*0.5||pointNext.y<=boundingBox().size.height*0.5)
    {
        out++;
        /*实际碰撞的线条*/
        float UpOrDown;
        if (attackerNow.y<size.height/2)
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
        if (attackerVelocity<Interface::sharedInterface()->fishVelocity/30)
        {
            attackerAngleNew = getAngle( getPositionX()-Fish::sharedFish()->getPositionX(), getPositionY()-Fish::sharedFish()->getPositionY());
            attackerVelocity = Interface::sharedInterface()->fishVelocity/30;
            attackerAngleOld = attackerAngleNew;
        }
        out++;
        /*实际碰撞的线条*/
        if (nextTurns)
        {
            /*暂时未写内嵌处理*/
            setPosition(ccpAdd(getPosition(),ccp(attackerVelocity*cos(attackerAngleNew*3.1415926/180),attackerVelocity*sin(attackerAngleNew*3.1415926/180))));
            ifNext =true ;
            
        }
    }
    
    /*与T的碰撞*/
    if (ccpDistance(pointNext, Tank::sharedTank()->getPosition())<=boundingBox().size.width*0.5+Tank::sharedTank()->boundingBox().size.height*0.5)
    {
        out++;
        if (nextTurns)
        {
            setPosition(ccpAdd(getPosition(),ccp(attackerVelocity*cos(attackerAngleNew*3.1415926/180),attackerVelocity*sin(attackerAngleNew*3.1415926/180))));
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
            setPosition(ccpAdd(getPosition(),ccp(attackerVelocity*cos(attackerAngleNew*3.1415926/180),attackerVelocity*sin(attackerAngleNew*3.1415926/180))));
            ifNext =true ;
            
        }
    }
    if (out!=0) {
        return true;
    }
    else return false;
}

/*得到下一帧的运动轨迹，并且对精灵部分碰撞的速度进行处理*/
float Attacker::getTangentAngle(int shape,CCSprite * sp)
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
            spVelocityRecord = attackerVelocity;
            spRotationNowRecord = attackerRotationNow;
            
            spriteMoveToSp = getAngle(sp->getPositionX() - getPositionX(),sp->getPositionY() - getPositionY());
            spMoveToSprite = getAngle(getPositionX()-sp->getPositionX() , getPositionY()- sp->getPositionY());;
            PassToSpAngle = getAngle_0_90(spriteMoveToSp, attackerAngleNew);
            PassInSpAngle = getAngle_0_90(spMoveToSprite, spAngleNew);
            
            angle1=spriteMoveToSp+90;
            
            if(( angle1-attackerAngleNew>=-270&&angle1-attackerAngleNew<=-90)||(angle1-attackerAngleNew>=90&&angle1-attackerAngleNew<=270))
            {
                angle1 = spriteMoveToSp-90;
            }
            angle2=spMoveToSprite;
            angleSpTangentX = attackerVelocity*sin(PassToSpAngle*3.1415926/180)*cos(angle1*3.1415926/180)+spVelocity*cos(PassInSpAngle*3.1415926/180)*cos(angle2*3.1415926/180);
            angleSpTangentY = attackerVelocity*sin(PassToSpAngle*3.1415926/180)*sin(angle1*3.1415926/180)+spVelocity*cos(PassInSpAngle*3.1415926/180)*sin(angle2*3.1415926/180);
            
            angleTangent4 = getAngle(angleSpTangentX, angleSpTangentY);
            attackerVelocity = ccpDistance(ccp(0,0), ccp(angleSpTangentX,angleSpTangentY));
            return angleTangent4;
        default:
            return angleTangent;//可继续添加新情况
            break;
    }
}
/*把两个角度转换成一个0~90度的角度*/
float Attacker::getAngle_0_90(float angle1,float angle2)
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
float Attacker::getContactPosition(CCSprite * sp)
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
float Attacker::getAngle(float x,float y)
{
    float anglereturn;
    anglereturn=atan2(y,x)*180/3.1415926;
    return anglereturn;
}


#pragma mark - 本类中的逻辑
void Attacker::updateAttacking(float delta)
{
    
    static float time1 = 0;
    static float time2 = 0;
    static bool haveAttacted1 = false;
    static bool haveAttacted2 = false;
    float contactPosition = 0;
    static bool collisionAttackerfirst;
    if (ccpDistance(getPosition(), Fish::sharedFish()->getPosition())<=boundingBox().size.width*0.5+Fish::sharedFish()->boundingBox().size.width*0.5) {
        if(collisionAttackerfirst) {
            contactPosition = getContactPosition(Fish::sharedFish());
            if (!haveAttacted1 && contactPosition >= 60 && contactPosition <= 180) {
                /*DPS撞到敌人除头部外的位置才掉血*/
                Fish::sharedFish()->wounded();                           /*敌人执行被撞动作*/
                if (!ifLaunched) {                                       /*技能未发动时敌人掉血*/
                    HPFish::sharedHPFish()->decreaseHP();
                    Interface::sharedInterface()->showEnemyHPDropping(2);    /*减血效果*/
                    scheduleOnce(schedule_selector(Interface::removeEnemy), 0.8);
                    Interface::sharedInterface()->augmentDPSThreat();        /*DPS增加仇恨*/
                }
                else {                                                   /*技能发动后敌人掉血*/
                    /*播放音效*/
                    if (Interface::sharedInterface()->ifPlayEffect) {
                        SimpleAudioEngine::sharedEngine()->playEffect("dpsSkill.mp3", false);
                    }
                    
                    /*开启技能效果*/
                    CCSprite* light = CCSprite::create("fire.png");
                    World::sharedWorld()->addChild(light);
                    light->setPosition(ccp(World::sharedWorld()->size.width * 0.5, World::sharedWorld()->size.height * 0.5));
                    light->setScale(50);
                    light->setOpacity(160);
                    light->setTag(tag_light);
                    
                    CCSprite* lightning = CCSprite::create("lightning.png");
                    World::sharedWorld()->addChild(lightning);
                    lightning->setPosition(ccp(World::sharedWorld()->size.width * 0.5, World::sharedWorld()->size.height * 0.5));
                    lightning->setTag(tag_lightning);
                    CCFadeIn* fadeIn = CCFadeIn::create(0.2);
                    lightning->runAction(fadeIn);
                    
                    CCMoveBy *up = CCMoveBy::create(0.1f, ccp(0, 20));
                    CCMoveBy *down = CCMoveBy::create(0.1f, ccp(0, -40));
                    CCMoveBy *back = CCMoveBy::create(0.1f, ccp(0, 20));
                    World::sharedWorld()->runAction(CCSequence::create(up, down, back, NULL));
                    scheduleOnce(schedule_selector(Attacker::disappear), 0.2);
                    
                    /*掉血*/
                    HPFish::sharedHPFish()->decreaseSkillHP();
                    /*减血效果*/
                    Interface::sharedInterface()->showEnemyHPDropping(3);
                    scheduleOnce(schedule_selector(Interface::removeEnemy), 0.8);
                    
                    /*技能结束*/
                    Interface::sharedInterface()->lessenDPSThreat();
                    setOpacity(255);                                     /*使粒子效果消失*/
                    removeChildByTag(tag_circle);
                    removeChildByTag(tag_newAttacker);
                    removeChild(coveredSprite, false);
                    unschedule(schedule_selector(Attacker::durationUpdate));
                    
                    ifLaunched = false;
                }
                haveAttacted1 = true;
            }
            else if (!haveAttacted2 && !(contactPosition >= 60 && contactPosition <= 180))
            {
                /*DPS受伤*/
                wounded();
                Interface::sharedInterface()->showHPDropping(this);           /*减血效果*/
                scheduleOnce(schedule_selector(Interface::removeDrop2), 0.8);
                HPDPS::sharedHPDPS()->decreaseHP();
                haveAttacted2 = true;
            }
        }
        collisionAttackerfirst=false;
    }
    
    else {
        collisionAttackerfirst=true;
    }
    
    /*设无敌时间*/
    if (time1 > 1.2) {
        haveAttacted1 = false;
        time1 = 0;
    }
    if (time2 > 1.2) {
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
    
    attackerNow=getPosition();
    /*旋转角度同步*/
    
    if (attackerVelocity!=0)
    {
        attackerRotationNow =getAngle(attackerNow.x-attackerOld.x, attackerNow.y-attackerOld.y);
        attackerRotationOld=attackerRotationNow;
    }
    else attackerRotationNow=attackerRotationOld;
    
    setRotation(90-attackerRotationNow);
    /*下一帧的位置*/
    pointNext=ccpAdd(getPosition(),ccp(attackerVelocity*cos(attackerAngleNew*3.1415926/180),attackerVelocity*sin(attackerAngleNew*3.1415926/180)));
    attackerRotationNow =getAngle(attackerNow.x-attackerOld.x, attackerNow.y-attackerOld.y);
    /*下一帧是否会碰撞，并做偏正处理*/
    ifOut=pointNextIfOut();
    
    /*反弹的处理，对碰撞产生的向量进行合成，最终获得下一次的转向角度*/
    attackerAngleNew=getCollisionAngle();
    if(ifNext == false)
    {
        if (ifOut==false)
        {
            /*精灵运动轨迹*/
            setPosition(ccpAdd(getPosition(),ccp(attackerVelocity*cos(attackerAngleNew*3.1415926/180),attackerVelocity*sin(attackerAngleNew*3.1415926/180))));
        }
    }
    /*避免Bug的处理*/
    if (ifOut&&attackerVelocity!=0)
    {
        nextTurns=true;
    }
    else nextTurns=false;
    
    attackerOld=attackerNow;
    
    if(attackerVelocity>=drag)
    {
        attackerVelocity-=drag;
    }
    else attackerVelocity=0;
}

void Attacker::wounded()
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

void Attacker::launch()
{
    /*增加仇恨*/
    Interface::sharedInterface()->augmentDPSSkillThreat();
    
    /*开启技能效果*/
    setOpacity(0);
    CCSprite *sprite = CCSprite::create("circle2.png");
    sprite->setPosition(ccp(205, 205));
    sprite->setTag(tag_circle);
    addChild(sprite);
    CCRotateBy *rotateBy = CCRotateBy::create(Interface::sharedInterface()->attackDuration, 360 * Interface::sharedInterface()->attackDuration);
    sprite->runAction(rotateBy);
    
    coveredSprite = CCSprite::create("attacker.png");
    coveredSprite->setPosition(ccp(205, 205));
    addChild(coveredSprite);
    
    /*开启冷却进度条*/
    SubsidiaryLayer::sharedSubsidiaryLayer()->coolingAttack();
    /*开始技能持续时间计时*/
    schedule(schedule_selector(Attacker::durationUpdate));
    /*开始冷却计时*/
    schedule(schedule_selector(Attacker::startCoolingClock));
    
    ifLaunched = true;
}

void Attacker::durationUpdate(float delta)
{
    static float time = 0;
    
    if (time > Interface::sharedInterface()->attackDuration) {
        /*技能结束*/
        Interface::sharedInterface()->lessenDPSThreat();     /*使仇恨降低*/
        setOpacity(255);                                     /*使粒子效果消失*/
        removeChildByTag(tag_circle);
        removeChildByTag(tag_newAttacker);
        removeChild(coveredSprite, false);
        unschedule(schedule_selector(Attacker::durationUpdate));
        ifLaunched = false;
        time = 0;
    }
    
    time+=delta;
}

void Attacker::startCoolingClock(float delta)
{
    static float time = 0;
    if (time < Interface::sharedInterface()->attackCoolingTime) {
        time += delta;
        ifFinishedCooling = false;
    }
    else {
        ifFinishedCooling = true;
        time = 0;
        unschedule(schedule_selector(Attacker::startCoolingClock));
    }
}

void Attacker::disappear()
{
    World::sharedWorld()->removeChildByTag(tag_light);
    World::sharedWorld()->removeChildByTag(tag_lightning);
}

#pragma mark - 触摸事件
bool Attacker::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    moveCollision = false;
    touchMove = true;
    float distance = ccpDistance(this->getPosition(), pTouch->getLocation());
    float radius = boundingBox().size.width * 0.6 + (CCUserDefault::sharedUserDefault()->getFloatForKey("slider", 3) - 3) * 30;
    pointOld = pTouch->getLocation();
    
    if (distance < radius)
    {
        pointDistance=0;
        attackerVelocity=0;
        return true;
    }
    return false;
}

/*触摸平滑处理*/
void Attacker::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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
                attackerAngleNew = getAngle(moveByX, moveByY);
                attackerVelocity = pointDistance*0.15;
                attackerAngleOld = attackerAngleNew;
                attackerMoveOld = ccp(moveByX, moveByY);
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
            if(collisionAttackerTouch)
            {
                attackerAngleNew = getAngle(moveByX, moveByY);
                attackerVelocity = pointDistance*0.15;
                attackerAngleOld = attackerAngleNew;
                attackerMoveOld = ccp(moveByX, moveByY);
                moveCollision = true;
                touchMove = false;
            }
            collisionAttackerTouch=false;
        }
        
        else
        {
            collisionAttackerTouch=true;
        }
        
        /*与MT间的碰撞判断*/
        if (ccpDistance(getPosition(), Tank::sharedTank()->getPosition())<=boundingBox().size.width*0.5+Tank::sharedTank()->boundingBox().size.width*0.5)
        {
            if(collisionTankTouch)
            {
                attackerAngleNew = getAngle(moveByX, moveByY);
                attackerVelocity = pointDistance*0.15;
                attackerAngleOld = attackerAngleNew;
                attackerMoveOld = ccp(moveByX, moveByY);
                moveCollision = true;
                touchMove = false;
            }
            collisionTankTouch=false;
        }
        else
        {
            collisionTankTouch=true;
        }
        /*与奶妈间的碰撞判断*/
        if (ccpDistance(getPosition(), Healer::sharedHealer()->getPosition())<=boundingBox().size.width*0.5+Healer::sharedHealer()->boundingBox().size.width*0.5)
        {
            if(collisionHealerTouch)
            {
                attackerAngleNew = getAngle(moveByX, moveByY);
                attackerVelocity = pointDistance*0.15;
                attackerAngleOld = attackerAngleNew;
                attackerMoveOld = ccp(moveByX, moveByY);
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
void Attacker::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (moveCollision == false)
    {
        attackerAngleNew = getAngle(moveByX, moveByY);
        attackerVelocity = pointDistance*0.3;
        attackerAngleOld = attackerAngleNew;
        attackerMoveOld = ccp(moveByX, moveByY);
    }
    if (Healer::sharedHealer()->ifLaunched) {
        /*如满血则不加血*/
        if (!(HPDPS::sharedHPDPS()->progressTimer->getPercentage() <100)) {
            return;
        }
        /*播放音效*/
        if (Interface::sharedInterface()->ifPlayEffect) {
            SimpleAudioEngine::sharedEngine()->playEffect("increaseHP.mp3");
        }
        HPDPS::sharedHPDPS()->increaseHP(2);
        Interface::sharedInterface()->showLaunchedRecovering(this);
        scheduleOnce(schedule_selector(Interface::removeRecover2), 0.8);
    }
}


#pragma mark - 触摸监听
void Attacker::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void Attacker::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}