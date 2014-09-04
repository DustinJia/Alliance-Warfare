#ifndef __Billiard_Bubble__Interface__
#define __Billiard_Bubble__Interface__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace extension;

class Interface : public CCLayer
{
public:
    /*获取当前类的单例对象*/
    static Interface* sharedInterface();
    /*boss类型，1.综合，2.移动速度，3旋转速度，4.防御力，5.攻击力*/
    int bossType;
public:
    /*读取天赋点数*/
    void readInhesion();
    /*设置关卡难度*/
    void setDifficulty();
    
    /*增加仇恨*/
    void augmentTankThreat();
    void augmentTankAttackThreat();
    void augmentDPSThreat();
    void augmentDPSSkillThreat();
    void augmentHealerThreat();
    
    /*降低仇恨*/
    void lessenTankThreat();
    void lessenDPSThreat();
    
    /*检测速度*/
    void monitorVelocity(CCNode* sprite, float delta);
    
    /*减血效果*/
    void showHPDropping(CCNode* sprite);            /*Player的减血效果*/
    void showEnemyHPDropping(int type);             /*敌人的减血效果*/
    /*加血效果*/
    void showHPRecovering(CCNode* sprite);          /*普通撞击的加血效果*/
    void showLaunchedRecovering(CCNode* sprite);    /*技能触发后的加血效果*/
    
    /*移除血量显示*/
    void removeDrop1(float delta);
    void removeDrop2(float delta);
    void removeDrop3(float delta);
    void removeEnemy(float delta);
    void removeRecover1(float delta);
    void removeRecover2(float delta);
    void removeRecover3(float delta);
    
    void gameOver(CCNode* sprite);
    
public:
    /*接口*/
    float hpConversion ;               /*精灵们的实际血量与百分比的转化*/
    float hpActual;                    /*精灵们的实际血量*/
    float hpFish;                      /*敌人的血量(每次掉血的百分比)*/
    float hpEnemySkill;                /*DPS开启技能后的掉血量*/
    float hpEnemyTankAttack;           /*T攻击时的掉血量*/
    float hpTank;                      /*T的血量*/
    float hpDPS;                       /*DPS的血量*/
    float hpHealer;                    /*治疗的血量*/
    float hpHeal;                      /*每次治疗的加血量*/
    
    float threatTank;                  /*T的仇恨值*/
    float threatDPS;                   /*DPS的仇恨值*/
    float threatHealer;                /*治疗的仇恨值*/
    float incrementTank;               /*T发动吸引仇恨技能时的仇恨增量*/
    float incrementTankAttack;         /*T每次攻击时的仇恨增量*/
    float incrementDPS;                /*DPS每次攻击的仇恨增量*/
    float incrementSkillDPS;           /*DPS发动技能后的仇恨增量*/
    float incrementHealer;             /*治疗每次加血的仇恨增量*/
    
    float absorbDuration;              /*吸引仇恨技能的持续时间*/
    float attackDuration;              /*攻击技能的持续时间*/
    float healDuration;                /*治疗技能的持续时间*/
    
    float absorbCoolingTime;           /*吸引仇恨技能的冷却时间*/
    float attackCoolingTime;           /*攻击技能的冷却时间*/
    float healCoolingTime;             /*治疗技能的冷却时间*/
    
    float tankVelocity;                /*监测到的T的瞬时速率*/
    float dpsVelocity;                 /*DPS的瞬时速率*/
    float healerVelocity;              /*治疗的瞬时速率*/
    
    float addPointsWithTank;           /*T普通攻击的积分增量*/
    float addPointsWithAbsorbing;      /*T用技能的积分增量*/
    float addPointsWithAttack;         /*DPS普通攻击的积分增量*/
    float addPointsWithAttackSkill;    /*DPS用技能攻击的积分增量*/
    float addPointsWithHeal;           /*治疗加血的积分增量*/
    float addPointsWithTime;           /*与每关用时相关的积分增量*/
    
    float inhesionHPTank;              /*T的防御天赋的每点加防御力*/
    float inhesionHPDPS;               /*DPS的防御天赋的每点加防御力*/
    float inhesionHPHealer;            /*治疗的防御天赋的每点加防御力*/
    float inhesionTankThreat;          /*T技能天赋的每点仇恨增加量（每次用T攻击时的仇恨增量）*/
    float inhesionDPSSkill;            /*DPS技能天赋的每点攻击力增量（百分比）*/
    float inhesionHealerHeal;          /*治疗技能天赋的每点治疗量增量（百分比）*/
    float inhesionCDTank;              /*T的CD天赋的每点CD时间变为上一次的（单位：百分比）*/
    float inhesionCDDPS;               /*DPS的CD天赋的每点CD时间变为上一次的（单位：百分比）*/
    float inhesionCDHealer;            /*治疗的CD天赋的每点CD时间变为上一次的（单位：百分比）*/
    
    float savedInhesion1;              /*T的防御数*/
    float savedInhesion2;              /*DPS的防御数*/
    float savedInhesion3;              /*治疗的防御数*/
    float savedInhesion4;              /*T每次攻击增加的仇恨数*/
    float savedInhesion5;              /*DPS的攻击力*/
    float savedInhesion6;              /*治疗的单次奶量*/
    float savedInhesion7;              /*T的CD天赋的每点CD时间变为原来的（单位：百分比）*/
    float savedInhesion8;              /*DPS的CD天赋的每点CD时间变为原来的（单位：百分比）*/
    float savedInhesion9;              /*治疗的CD天赋的每点CD时间变为原来的（单位：百分比）*/
    
    int level;                         /*无尽模式的关数*/
    int level1;                        /*训练模式的关数*/
    float fishVelocity;                /*敌人移动的速率*/
    float angularVelocity;             /*敌人移动的角速度*/
    float bossDefine;                  /*敌人的防御力*/
    float bossAttack;                  /*敌人的攻击力*/
    float bossComprehensive;           /*敌人的综合能力*/
    float exchangeRate;                /*积分与天赋点数的兑换比率（单位：积分/点）*/
    int enemyType;                     /*敌人的随机类型*/
    float hpBossActual;                /*敌人的实际血量*/
    float hpBossConversion ;           /*敌人的实际血量与百分比的转化*/
    
    CCControlSlider* slider;           /*滑动条*/
    
    /*设置*/
    bool ifPlayMusic;                  /*是否播放音乐*/
    bool ifPlayEffect;                 /*是否播放音效*/
    int sensitivity;                   /*触摸敏感度*/
    
protected:
    /*生命周期相关*/
    Interface();
};

#endif /* defined(__Billiard_Bubble__Interface__) */