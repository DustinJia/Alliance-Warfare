#ifndef __Billiard_Bubble__DisplayPoints__
#define __Billiard_Bubble__DisplayPoints__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class DisplayPoints : public CCLayer
{
public:
    bool init();
    static CCScene* scene();
    CREATE_FUNC(DisplayPoints);
    
protected:
    /*逐条展示*/
    void displayView();
    void displayPoints();
    void displayData();
    void displayExchange();
    
    /*切换到输赢界面*/
    void winChoice();
    void loseChoice();
    void loseGame();
    void inhesionScene();
    void loseScene();
    void wordScene();
    void menuScene();
    
protected:
    /*获取屏幕尺寸*/
    CCSize size;
    /*用于存储天赋点数*/
    float several;
    /*用于判断输赢*/
    bool ifWin;
};


#endif /* defined(__Billiard_Bubble__DisplayPoints__) */