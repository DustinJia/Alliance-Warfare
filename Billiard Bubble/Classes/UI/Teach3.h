

#ifndef __Billiard_Bubble__Teach3__
#define __Billiard_Bubble__Teach3__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class Teach3 :public CCLayer
{
public:
    bool init();
    static CCScene *scene();
    CREATE_FUNC(Teach3);
private:
    void backMenu();
    void back();
protected:
    CCSize size;
    void plusSign();         /*添加加号*/
    void iconInherence();

    void plusSignInfo();
    void addHP1();           /*增加T的血量*/
    void addHP2();           /*增加DPS的血量*/
    void addHP3();           /*增加治疗的血量*/
    void addAttack1();       /*增加T的攻击力*/
    void addAttack2();       /*增加DPS的攻击力*/
    void addHeal();          /*增加治疗的治疗量*/
    void addCD1();           /*减少T的技能的CD时间*/
    void addCD2();           /*减少DPS的技能的CD时间*/
    void addCD3();           /*减少治疗的技能的CD时间*/
    
    void onExit();

    CCLabelTTF* label;
    CCSprite * hero;
    
     //CCMenuItemImage* infoBackgroud;
    //CCMenu* menu1;
};

#endif /* defined(__Billiard_Bubble__Teach2__) */
