#ifndef __Billiard_Bubble__Level__
#define __Billiard_Bubble__Level__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace extension;

class Level : public CCLayer,public CCScrollViewDelegate
{
public:
    bool init();
    static CCScene *scene();
    CREATE_FUNC(Level);
    
protected:
    /*滚动视图相关*/
    void scrollViewDidScroll(CCScrollView* view);    /*当scrollview发生移动时响应*/
    void scrollViewDidZoom(CCScrollView* view);      /*当scrollview发生缩放时响应*/
    
    void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void adjustScrollView();
    
protected:
    /*关卡锁相关*/
    void addLock();
    void save();
    void read();
    void set();
    
protected:
    /*选择关卡*/
    void chooseLevel1();
    void chooseLevel2();
    void chooseLevel3();
    void chooseLevel4();
    void chooseLevel5();
    
    void chooseLevel6();
    void chooseLevel7();
    void chooseLevel8();
    void chooseLevel9();
    void chooseLevel10();
    
    void chooseLevel11();
    void chooseLevel12();
    void chooseLevel13();
    void chooseLevel14();
    void chooseLevel15();
    
    /*返回主菜单*/
    void backToMenu();
    
protected:
    /*触摸事件相关函数*/
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    /*生命周期相关函数*/
    void onEnter();
    void onExit();
    
protected:
    CCScrollView * scrollView;
    int level;
    
    /*关卡锁*/
    CCSprite* lock2;
    CCSprite* lock3;
    CCSprite* lock4;
    CCSprite* lock5;
    CCSprite* lock6;
    CCSprite* lock7;
    CCSprite* lock8;
    CCSprite* lock9;
    CCSprite* lock10;
    CCSprite* lock11;
    CCSprite* lock12;
    CCSprite* lock13;
    CCSprite* lock14;
    CCSprite* lock15;
    
    bool ifLock2;
    bool ifLock3;
    bool ifLock4;
    bool ifLock5;
    bool ifLock6;
    bool ifLock7;
    bool ifLock8;
    bool ifLock9;
    bool ifLock10;
    bool ifLock11;
    bool ifLock12;
    bool ifLock13;
    bool ifLock14;
    bool ifLock15;
};

#endif /* defined(__Billiard_Bubble__Level__) */