#include "Interface.h"
#include "Tank.h"
#include "Attacker.h"
#include "Healer.h"
#include "SubsidiaryLayer.h"
#include "Fish.h"

typedef enum
{
    tag_dropHP1 = 5,
    tag_dropHP2 = 6,
    tag_dropHP3 = 7,
    tag_dropHPEnemy = 8,
    tag_recoverHP1 = 9,
    tag_recoverHP2 = 10,
    tag_recoverHP3 = 11,
}Tag_Interface;

static Interface* instance = NULL;

Interface* Interface::sharedInterface()
{
    if (instance == NULL) {
        instance = new Interface();
    }
    return instance;
}


#pragma mark - 接口函数
void Interface:: setDifficulty()
{
    /*boss类型，1.综合，2.移动速度，3旋转速度，4.防御力，5.攻击力*/
    int temp;
    float BossDivisionMoveVelocity;//分量
    float BossDivisionAngularVelocity;
    float BossDivisionDefine;
    float BossDivisionAttacke;
    
    /*读取天赋数据*/
    Interface::sharedInterface()->readInhesion();
    
    /*兼容训练模式的难度设置*/
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("ifEndlessMode")) {
        level = CCUserDefault::sharedUserDefault()->getIntegerForKey("level", 1);
    }
    else {
        level = CCUserDefault::sharedUserDefault()->getIntegerForKey("level1", 1);
    }
    
    temp = arc4random()%10;
    if (temp<2)
    {
        bossType=1;
        BossDivisionMoveVelocity = 0.20;
        BossDivisionAngularVelocity = 0.20;
        BossDivisionDefine = 0.20;
        BossDivisionAttacke =0.20;
    }
    else
    {
        float residue1;
        float residue2;
        
        BossDivisionMoveVelocity = (float)(arc4random()%40)/100;
        residue1 = 0.4-BossDivisionMoveVelocity;
        BossDivisionAngularVelocity = residue1;
        BossDivisionDefine = (float)(arc4random()%40)/100;
        residue2 =0.4- BossDivisionDefine;
        BossDivisionAttacke =residue2;
        
        float max=BossDivisionMoveVelocity;
        
        if (BossDivisionAngularVelocity>max)
        {
            max = BossDivisionAngularVelocity;
        }
        if (BossDivisionDefine>max)
        {
            max = BossDivisionDefine;
        }
        if (BossDivisionAttacke>max)
        {
            max = BossDivisionAttacke;
        }
        
        if (BossDivisionMoveVelocity==max)
        {
            bossType = 2;
        }
        if (BossDivisionAngularVelocity==max)
        {
            bossType = 3;
        }
        if (BossDivisionDefine==max)
        {
            bossType = 4;
        }
        if (BossDivisionAttacke==max)
        {
            bossType = 5;
        }
        
    }
    enemyType = bossType;
    
    /*静态*/
    /*BOSS防御力*/
    bossDefine =(float)level*18.0*(0.8+BossDivisionDefine) ;
    /*BOSS攻击力*/
    bossAttack = (float)level*1.4*(0.8+BossDivisionAttacke)+8.0;
    /*积分与天赋点数的兑换比率（单位：积分/点）*/
    exchangeRate = 1000;
    /*每关用时加分（满分5000，每多用一秒扣5分）尽量与lever有关*/
    addPointsWithTime = 40;
    /*积分增加*/
    /*T普通攻击积分增加*/
    addPointsWithTank = 85;
    /*T发动吸引仇恨技能积分增加*/
    addPointsWithAbsorbing = 75;
    /*DPS普通攻击积分增加*/
    addPointsWithAttack = 60;
    /*DPS发动技能积分增加*/
    addPointsWithAttackSkill = 75;
    /*治疗发动技能积分增加*/
    addPointsWithHeal = 50;
    /*T发动吸引仇恨技能时的仇恨增量*/
    incrementTank = 2000;
    /*攻击技能的持续时间*/
    attackDuration = 5;
    
    /*半静态,关于savedInhesion这些的数据*/
    /*治疗每次加血时的仇恨增量*/
    incrementHealer = savedInhesion6*1.5 + 50*level;
    /*T每次攻击时的仇恨增量*/
    incrementTankAttack = 1 + savedInhesion4;
    /*DPS每次普通攻击时的仇恨增量*/
    incrementDPS = savedInhesion5/8+ 10;
    /*吸引仇恨技能的CD时间*/
    if (savedInhesion7) {
        absorbCoolingTime = 16 * savedInhesion7;
    }
    else {
        absorbCoolingTime = 16;
    }
    /*DPS技能的CD时间*/
    if (savedInhesion8) {
        attackCoolingTime = 16 * savedInhesion8;
    }
    else {
        attackCoolingTime = 16;
    }
    /*治疗技能的CD时间*/
    if (savedInhesion9) {
        healCoolingTime = 16 * savedInhesion9;
    }
    else {
        healCoolingTime = 16;
    }
    /*DPS用技能时的仇恨增量*/
    incrementSkillDPS = savedInhesion5/6+ 15;
    /*吸引仇恨技能的持续时间*/
    absorbDuration =  10;
    /*治疗技能的持续时间*/
    healDuration = 3;
    /*敌人每次移动速度（度）*/  //适合从200-600
    fishVelocity = 20+level*5+BossDivisionMoveVelocity*150;
    //fishVelocity = 100+level*4+BossDivisionMoveVelocity*100;
    /*敌人每次旋转速度（度）*/  //适合从5-16
    angularVelocity =(float)level*0.25+(float)BossDivisionAngularVelocity*2;
    // angularVelocity =3+(float)level*0.15+(float)BossDivisionAngularVelocity*4;
    /*精灵们实际血量*/
    hpActual = 90.0+(float)level*10.0;
    /*BOSS实际血量*/
    hpBossActual = 600.0+(float)level*100.0;
    /*敌人的实际血量与百分比的转化*/
    hpBossConversion = hpBossActual/100.0;
    /*精灵们的实际血量与百分比的转化*/
    hpConversion = hpActual/100.0;
    
    /*治疗每次治疗的加血量（百分比）*/
    hpHeal = (20 + savedInhesion6) / hpConversion;
    /*随机动态*/  //关于lever和综合以及一定的随机量的数据
    
    /*动态*/

    /*关于BOSS的防御力*/
    
    /*敌人每次被DPS普通攻击的掉血量（百分比）*/
    if(70+ savedInhesion5 - bossDefine>=2)
    {
        hpFish = (70+ savedInhesion5 - bossDefine)/hpBossConversion ;
    }
    else hpFish = 2/hpBossConversion ;
    /*敌人每次被DPS用技能攻击的掉血量（百分比）*/
    hpEnemySkill = ((70+ savedInhesion5)*2-bossDefine)/hpBossConversion;
    /*敌人每次被T攻击的掉血量（百分比）*/
    if((50+ savedInhesion4*5 - bossDefine)>=0.1)
    {
        hpEnemyTankAttack = (50+ savedInhesion4*5 - bossDefine)/hpBossConversion ;
    }
    else hpEnemyTankAttack = 1/hpBossConversion ;
    
    /*关于BOSS的攻击力和精灵们的防御力*/
    
    if (bossAttack-5 - savedInhesion1>=0.1)
    {
        /*T每次被攻击的掉血量（百分比）*/
        hpTank =  (bossAttack-5 - savedInhesion1)/hpConversion;
    }
    else hpTank =  0.1/hpConversion;
    if (bossAttack - savedInhesion2>=0.1)
    {
        /*DPS每次被攻击的掉血量（百分比）*/
        hpDPS =  (bossAttack - savedInhesion2)/hpConversion;
    }
    else hpDPS =  0.1/hpConversion;
    if (bossAttack - savedInhesion3>=0.1)
    {
        /*治疗每次被攻击的掉血量（百分比）*/
        hpHealer = (bossAttack - savedInhesion3)/hpConversion;
    }
    else hpHealer =  0.1/hpConversion;
}


#pragma mark - 仇恨相关
void Interface::augmentTankThreat()
{
    threatTank += incrementTank;
}

void Interface::augmentTankAttackThreat()
{
    threatTank += incrementTankAttack;
}

void Interface::augmentDPSThreat()
{
    threatDPS += incrementDPS;
}

void Interface::augmentDPSSkillThreat()
{
    threatDPS += incrementSkillDPS;
}

void Interface::augmentHealerThreat()
{
    threatHealer += incrementHealer;
}

/*降低仇恨*/
void Interface::lessenTankThreat()
{
    threatTank -= incrementTank;
}

void Interface::lessenDPSThreat()
{
    threatDPS -= incrementSkillDPS;
}

void Interface::readInhesion()
{
    savedInhesion1 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion1", 0);
    savedInhesion2 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion2", 0);
    savedInhesion3 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion3", 0);
    savedInhesion4 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion4", 0);
    savedInhesion5 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion5", 0);
    savedInhesion6 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion6", 0);
    savedInhesion7 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion7", 1);
    savedInhesion8 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion8", 1);
    savedInhesion9 = CCUserDefault::sharedUserDefault()->getFloatForKey("savedInhesion9", 1);
}


#pragma mark - 复用函数
/*速率监测*/
void Interface::monitorVelocity(CCNode* sprite, float delta)
{
    /*记录T之前的位置*/
    static CCPoint preLocationT = ccp(0, 0);
    /*记录DPS之前的位置*/
    static CCPoint preLocationDPS = ccp(0, 0);
    /*记录治疗之前的位置*/
    static CCPoint preLocationHealer = ccp(0, 0);
    /*记录当前的位置*/
    CCPoint currentLocation = ccp(0, 0);
    /*两点间的距离*/
    float distance = 0;
    /*监测的时间间隔*/
    float intervel = delta;
    
    if (typeid(*sprite) == typeid(*Tank::sharedTank()))
    {
        currentLocation = ((CCSprite*)sprite)->getPosition();
        distance = ccpDistance(currentLocation, preLocationT);
        preLocationT = currentLocation;
        tankVelocity = distance / intervel;
    }
    else if (typeid(*sprite) == typeid(*Attacker::sharedAttacker()))
    {
        currentLocation = ((CCSprite*)sprite)->getPosition();
        distance = ccpDistance(currentLocation, preLocationDPS);
        preLocationDPS = currentLocation;
        dpsVelocity = distance / intervel;
    }
    else if (typeid(*sprite) == typeid(*Healer::sharedHealer()))
    {
        currentLocation = ((CCSprite*)sprite)->getPosition();
        distance = ccpDistance(currentLocation, preLocationHealer);
        preLocationHealer = currentLocation;
        healerVelocity = distance / intervel;
    }
}

/*减血效果*/
void Interface::showHPDropping(CCNode *sprite)
{
    if (typeid(*sprite) == typeid(*Tank::sharedTank())) {
        CCLabelBMFont* hp = CCLabelBMFont::create("0", "font01.fnt");
        SubsidiaryLayer::sharedSubsidiaryLayer()->addChild(hp);
        hp->setColor(ccc3(255, 0, 0));
        hp->setTag(tag_dropHP1);
        CCString* string = CCString::createWithFormat("-%g", (float)(((int)(100*hpTank*hpConversion))/10)/10);
        hp->setString(string->getCString());
        hp->setPosition(ccp(Tank::sharedTank()->getPositionX(), Tank::sharedTank()->getPositionY() + 10));
        CCMoveTo* moveTo = CCMoveTo::create(0.5, ccp(hp->getPositionX(), hp->getPositionY() + 75));
        CCFadeOut* fadeOut = CCFadeOut::create(0.3);
        CCSequence* sequence = CCSequence::create(moveTo, fadeOut, NULL);
        hp->runAction(sequence);
    }
    
    else if (typeid(*sprite) == typeid(*Attacker::sharedAttacker())) {
        CCLabelBMFont* hp = CCLabelBMFont::create("0", "font01.fnt");
        SubsidiaryLayer::sharedSubsidiaryLayer()->addChild(hp);
        hp->setColor(ccc3(255, 0, 0));
        hp->setTag(tag_dropHP2);
        CCString* string = CCString::createWithFormat("-%g", (float)(((int)(100*hpDPS*hpConversion))/10)/10);
        hp->setString(string->getCString());
        hp->setPosition(ccp(Attacker::sharedAttacker()->getPositionX(), Attacker::sharedAttacker()->getPositionY() + 10));
        CCMoveTo* moveTo = CCMoveTo::create(0.5, ccp(hp->getPositionX(), hp->getPositionY() + 75));
        CCFadeOut* fadeOut = CCFadeOut::create(0.3);
        CCSequence* sequence = CCSequence::create(moveTo, fadeOut, NULL);
        hp->runAction(sequence);
    }
    
    else if (typeid(*sprite) == typeid(*Healer::sharedHealer())) {
        CCLabelBMFont* hp = CCLabelBMFont::create("0", "font01.fnt");
        SubsidiaryLayer::sharedSubsidiaryLayer()->addChild(hp);
        hp->setColor(ccc3(255, 0, 0));
        hp->setTag(tag_dropHP3);
        CCString* string = CCString::createWithFormat("-%g", (float)(((int)(100*hpHealer*hpConversion))/10)/10);
        hp->setString(string->getCString());
        hp->setPosition(ccp(Healer::sharedHealer()->getPositionX(), Healer::sharedHealer()->getPositionY() + 10));
        CCMoveTo* moveTo = CCMoveTo::create(0.5, ccp(hp->getPositionX(), hp->getPositionY() + 75));
        CCFadeOut* fadeOut = CCFadeOut::create(0.3);
        CCSequence* sequence = CCSequence::create(moveTo, fadeOut, NULL);
        hp->runAction(sequence);
    }
}

void Interface::showEnemyHPDropping(int type)
{
    CCLabelBMFont* hp = CCLabelBMFont::create("0", "testChinese.fnt");
    SubsidiaryLayer::sharedSubsidiaryLayer()->addChild(hp);
    
    hp->setScale(0.8);
    hp->setPosition(ccp(Fish::sharedFish()->getPositionX(), Fish::sharedFish()->getPositionY() + 10));
    hp->setTag(tag_dropHPEnemy);
    
    if (type == 1) {
        CCString* string = CCString::createWithFormat("-%d", (int)(hpEnemyTankAttack*hpBossConversion));
        hp->setString(string->getCString());
    }
    else if (type == 2) {
        CCString* string = CCString::createWithFormat("-%d", (int)(hpFish*hpBossConversion));
        hp->setString(string->getCString());
    }
    else if (type == 3) {
        CCString* string = CCString::createWithFormat("-%d", (int)(hpEnemySkill*hpBossConversion));
        hp->setString(string->getCString());
    }
    
    CCMoveTo* moveTo = CCMoveTo::create(0.5, ccp(hp->getPositionX(), hp->getPositionY() + 80));
    CCFadeOut* fadeOut = CCFadeOut::create(0.3);
    CCSequence* sequence = CCSequence::create(moveTo, fadeOut, NULL);
    hp->runAction(sequence);
}

/*加血效果*/
void Interface::showHPRecovering(CCNode *sprite)
{
    if (typeid(*sprite) == typeid(*Tank::sharedTank())) {
        CCLabelBMFont* hp = CCLabelBMFont::create("0", "font01.fnt");
        SubsidiaryLayer::sharedSubsidiaryLayer()->addChild(hp);
        hp->setTag(tag_recoverHP1);
        CCString* string = CCString::createWithFormat("+%d", (int)(hpHeal * hpConversion));
        hp->setString(string->getCString());
        hp->setPosition(ccp(Tank::sharedTank()->getPositionX(), Tank::sharedTank()->getPositionY() + 10));
        CCMoveTo* moveTo = CCMoveTo::create(0.5, ccp(hp->getPositionX(), hp->getPositionY() + 75));
        CCFadeOut* fadeOut = CCFadeOut::create(0.3);
        CCSequence* sequence = CCSequence::create(moveTo, fadeOut, NULL);
        hp->runAction(sequence);
    }
    
    else if (typeid(*sprite) == typeid(*Attacker::sharedAttacker())) {
        CCLabelBMFont* hp = CCLabelBMFont::create("0", "font01.fnt");
        SubsidiaryLayer::sharedSubsidiaryLayer()->addChild(hp);
        hp->setTag(tag_recoverHP2);
        CCString* string = CCString::createWithFormat("+%d", (int)(hpHeal * hpConversion));
        hp->setString(string->getCString());
        hp->setPosition(ccp(Attacker::sharedAttacker()->getPositionX(), Attacker::sharedAttacker()->getPositionY() + 10));
        CCMoveTo* moveTo = CCMoveTo::create(0.5, ccp(hp->getPositionX(), hp->getPositionY() + 75));
        CCFadeOut* fadeOut = CCFadeOut::create(0.3);
        CCSequence* sequence = CCSequence::create(moveTo, fadeOut, NULL);
        hp->runAction(sequence);
    }
    
    else if (typeid(*sprite) == typeid(*Healer::sharedHealer())) {
        CCLabelBMFont* hp = CCLabelBMFont::create("0", "font01.fnt");
        SubsidiaryLayer::sharedSubsidiaryLayer()->addChild(hp);
        hp->setTag(tag_recoverHP3);
        CCString* string = CCString::createWithFormat("+%d", (int)(hpHeal * hpConversion * 0.3));
        hp->setString(string->getCString());
        hp->setPosition(ccp(Healer::sharedHealer()->getPositionX(), Healer::sharedHealer()->getPositionY() + 10));
        CCMoveTo* moveTo = CCMoveTo::create(0.5, ccp(hp->getPositionX(), hp->getPositionY() + 75));
        CCFadeOut* fadeOut = CCFadeOut::create(0.3);
        CCSequence* sequence = CCSequence::create(moveTo, fadeOut, NULL);
        hp->runAction(sequence);
    }
}

void Interface::showLaunchedRecovering(CCNode *sprite)
{
    if (typeid(*sprite) == typeid(*Tank::sharedTank())) {
        CCLabelBMFont* hp = CCLabelBMFont::create("0", "font01.fnt");
        SubsidiaryLayer::sharedSubsidiaryLayer()->addChild(hp);
        hp->setTag(tag_recoverHP1);
        CCString* string = CCString::createWithFormat("+%d", (int)(hpHeal * hpConversion * 0.2));
        hp->setString(string->getCString());
        hp->setPosition(ccp(Tank::sharedTank()->getPositionX(), Tank::sharedTank()->getPositionY() + 10));
        CCMoveTo* moveTo = CCMoveTo::create(0.5, ccp(hp->getPositionX(), hp->getPositionY() + 75));
        CCFadeOut* fadeOut = CCFadeOut::create(0.3);
        CCSequence* sequence = CCSequence::create(moveTo, fadeOut, NULL);
        hp->runAction(sequence);
    }
    
    else if (typeid(*sprite) == typeid(*Attacker::sharedAttacker())) {
        CCLabelBMFont* hp = CCLabelBMFont::create("0", "font01.fnt");
        SubsidiaryLayer::sharedSubsidiaryLayer()->addChild(hp);
        hp->setTag(tag_recoverHP2);
        CCString* string = CCString::createWithFormat("+%d", (int)(hpHeal * hpConversion * 0.2));
        hp->setString(string->getCString());
        hp->setPosition(ccp(Attacker::sharedAttacker()->getPositionX(), Attacker::sharedAttacker()->getPositionY() + 10));
        CCMoveTo* moveTo = CCMoveTo::create(0.5, ccp(hp->getPositionX(), hp->getPositionY() + 75));
        CCFadeOut* fadeOut = CCFadeOut::create(0.3);
        CCSequence* sequence = CCSequence::create(moveTo, fadeOut, NULL);
        hp->runAction(sequence);
    }
    
    else if (typeid(*sprite) == typeid(*Healer::sharedHealer())) {
        CCLabelBMFont* hp = CCLabelBMFont::create("0", "font01.fnt");
        SubsidiaryLayer::sharedSubsidiaryLayer()->addChild(hp);
        hp->setTag(tag_recoverHP3);
        CCString* string = CCString::createWithFormat("+%d", (int)(hpHeal * hpConversion * 0.2));
        hp->setString(string->getCString());
        hp->setPosition(ccp(Healer::sharedHealer()->getPositionX(), Healer::sharedHealer()->getPositionY() + 10));
        CCMoveTo* moveTo = CCMoveTo::create(0.5, ccp(hp->getPositionX(), hp->getPositionY() + 75));
        CCFadeOut* fadeOut = CCFadeOut::create(0.3);
        CCSequence* sequence = CCSequence::create(moveTo, fadeOut, NULL);
        hp->runAction(sequence);
    }
}


/*移除血量显示*/
void Interface::removeDrop1(float delta)
{
    SubsidiaryLayer::sharedSubsidiaryLayer()->removeChildByTag(tag_dropHP1);
}

void Interface::removeDrop2(float delta)
{
    SubsidiaryLayer::sharedSubsidiaryLayer()->removeChildByTag(tag_dropHP2);
}

void Interface::removeDrop3(float delta)
{
    SubsidiaryLayer::sharedSubsidiaryLayer()->removeChildByTag(tag_dropHP3);
}

void Interface::removeEnemy(float delta)
{
    SubsidiaryLayer::sharedSubsidiaryLayer()->removeChildByTag(tag_dropHPEnemy);
}

void Interface::removeRecover1(float delta)
{
    SubsidiaryLayer::sharedSubsidiaryLayer()->removeChildByTag(tag_recoverHP1);
}

void Interface::removeRecover2(float delta)
{
    SubsidiaryLayer::sharedSubsidiaryLayer()->removeChildByTag(tag_recoverHP2);
}

void Interface::removeRecover3(float delta)
{
    SubsidiaryLayer::sharedSubsidiaryLayer()->removeChildByTag(tag_recoverHP3);
}

void Interface::gameOver(CCNode *sprite)
{
    if (typeid(*sprite) == typeid(*Fish::sharedFish())) {
        sprite = Fish::sharedFish();
    }
    else if (typeid(*sprite) == typeid(*Tank::sharedTank())) {
        sprite = Tank::sharedTank();
    }
    else if (typeid(*sprite) == typeid(*Attacker::sharedAttacker())) {
        sprite = Attacker::sharedAttacker();
    }
    else if (typeid(*sprite) == typeid(*Healer::sharedHealer())) {
        sprite = Healer::sharedHealer();
    }
    
    CCActionInterval* fadeOut = CCFadeOut::create(3);
    sprite->runAction(fadeOut);
}


#pragma mark - 生命周期相关
Interface::Interface()
{
    /*为静态对象添加实例*/
    instance = this;
    
    /*初始化*/
    threatTank = 0;
    threatDPS = 0;
    threatHealer = 0;
    tankVelocity = 0;
    dpsVelocity = 0;
    healerVelocity = 0;
    bossType = 1;
    level = CCUserDefault::sharedUserDefault()->getIntegerForKey("level", 1);
    level1 = CCUserDefault::sharedUserDefault()->getIntegerForKey("level1", 1);
    
    /*天赋控制*/
    /*T的御天赋的每点加防御*/
    inhesionHPTank = 1.5;
    /*DPS的防御天赋的每点加防御*/
    inhesionHPDPS = 1;
    /*治疗的防御天赋的每点加防御*/
    inhesionHPHealer = 1;
    /*T技能天赋的每点仇恨增加量（每次用T攻击时的仇恨增量）*/
    inhesionTankThreat = 4;
    /*DPS天赋的每点攻击力增量*/
    inhesionDPSSkill = 20;
    /*治疗天赋的每点治疗量增量（百分比）*/
    inhesionHealerHeal = 1;
    /*T的CD天赋的每点CD时间变为上一次的（百分比）*/
    inhesionCDTank = 0.95;
    /*DPS的CD天赋的每点CD时间变为上一次的（百分比）*/
    inhesionCDDPS = 0.95;
    /*治疗的CD天赋的每点CD时间变为上一次的（百分比）*/
    inhesionCDHealer = 0.95;
}