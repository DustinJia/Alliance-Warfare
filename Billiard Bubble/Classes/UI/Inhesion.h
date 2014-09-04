#ifndef __Billiard_Bubble__Inhesion__
#define __Billiard_Bubble__Inhesion__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class Inhesion : public CCLayer
{
public:
    bool init();
    static CCScene* scene();
    CREATE_FUNC(Inhesion);
    
public:
    /*获取当前类的单例对象*/
    static Inhesion* sharedInhesion();
    /*是否从主菜单转换到的本场景*/
    bool ifFromMenu;
    
protected:
    /*基本内容*/
    void iconPlayer();       /*添加头像图标*/
    void iconInherence();    /*添加天赋图标*/
    void data();             /*添加数字*/
    void plusSign();         /*添加加号*/
    
    /*加点方法*/
    void addHP1();           /*增加T的血量*/
    void addHP2();           /*增加DPS的血量*/
    void addHP3();           /*增加治疗的血量*/
    void addAttack1();       /*增加T的攻击力*/
    void addAttack2();       /*增加DPS的攻击力*/
    void addHeal();          /*增加治疗的治疗量*/
    void addCD1();           /*减少T的技能的CD时间*/
    void addCD2();           /*减少DPS的技能的CD时间*/
    void addCD3();           /*减少治疗的技能的CD时间*/
    
    /*存储*/
    void save();
    /*读入*/
    void read();
    /*设置数据为读入的结果*/
    void set();
    
    /*剩余点数*/
    void residue();
    void refreshResidue();
    
    /*确定*/
    void confirm();
    /*重置*/
    void reset();
    
    void resetAll();
    
    /*所有数据清零*/
    void resetAllData();
    
    void onEnter();
    
protected:
    /*获取屏幕尺寸*/
    CCSize size;
    
    /*数据字符*/
    CCLabelAtlas* hp1;
    CCLabelAtlas* hp2;
    CCLabelAtlas* hp3;
    CCLabelAtlas* attack1;
    CCLabelAtlas* attack2;
    CCLabelAtlas* heal;
    CCLabelAtlas* cd1;
    CCLabelAtlas* cd2;
    CCLabelAtlas* cd3;
    CCLabelAtlas* labelNumber;
    
    /*数据*/
    int data1;
    int data2;
    int data3;
    int data4;
    int data5;
    int data6;
    int data7;
    int data8;
    int data9;
    
    /*剩余点数*/
    int several;
    
    /*判断是否重置*/
    bool ifReset;
    
    /*判断重置时需要恢复可加点的天赋项*/
    bool ifAdded1;
    bool ifAdded2;
    bool ifAdded3;
    bool ifAdded4;
    bool ifAdded5;
    bool ifAdded6;
    bool ifAdded7;
    bool ifAdded8;
    bool ifAdded9;
};

#endif /* defined(__Billiard_Bubble__Inhesion__) */