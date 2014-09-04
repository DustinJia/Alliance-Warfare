#include "Data.h"
#include "Interface.h"
#include "HPFish.h"
#include "HPTank.h"
#include "HPDPS.h"
#include "HPHealer.h"
#include "SubsidiaryLayer.h"

static Data* instance = NULL;

Data* Data::sharedData()
{
    if (instance == NULL) {
        instance = new Data();
    }
    return instance;
}

Data::Data()
{
    ifRefreshed = false;
}

void Data::savePoints()
{
    totalPoints = CCUserDefault::sharedUserDefault()->getFloatForKey("totalPoints", 0);
    totalPoints += points;
    CCUserDefault::sharedUserDefault()->setFloatForKey("totalPoints", totalPoints);
    CCUserDefault::sharedUserDefault()->flush();
}


#pragma mark - 增加积分的接口函数
void Data::addPointsWithAbsorbing()
{
    points += Interface::sharedInterface()->addPointsWithAbsorbing;
    ifRefreshed = true;
}

void Data::addPointsWithTankAttack()
{
    points += Interface::sharedInterface()->addPointsWithTank;
    ifRefreshed = true;
}

void Data::addPointsWithAttack()
{
    points += Interface::sharedInterface()->addPointsWithAttack;
    ifRefreshed = true;
}

void Data::addPointsWithAttackSkill()
{
    points += Interface::sharedInterface()->addPointsWithAttackSkill;
    ifRefreshed = true;
}

void Data::addPointsWithHeal()
{
    points += Interface::sharedInterface()->addPointsWithHeal;
    ifRefreshed = true;
}

void Data::addPointsWithTotalHP()
{
    points += ((100 - HPFish::sharedHPFish()->progressTimer->getPercentage()) + (100 - HPTank::sharedHPTank()->progressTimer->getPercentage()) + (100 - HPDPS::sharedHPDPS()->progressTimer->getPercentage()) + (100 - HPHealer::sharedHPHealer()->progressTimer->getPercentage())) * 5;
    ifRefreshed = true;
}