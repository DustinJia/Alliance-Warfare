#ifndef __Billiard_Bubble__Pause__
#define __Billiard_Bubble__Pause__

#include "cocos2d.h"
#include <iostream>
#include "cocos-ext.h"

using namespace cocos2d;
using namespace extension;

class Pause : public CCLayer
{
public:
    bool init();
    static CCScene *scene();
    CREATE_FUNC(Pause);
    
public:
    /*获取本类单例对象*/
    static Pause* sharedPause();
    
    /*滑动条*/
    CCControlSlider* slider;
    
    /*存储积分*/
    float tempPoints;
    
protected:
    void resume();
    void resumeMusic();
    void backMenu();
    void effetsVolume();
    void sensitivity();
    
protected:
    /*判定执行暂停播放音乐*/
    bool rusumMusic;
    /*判定暂停播放音效*/
    bool effects;
};

#endif /* defined(__Billiard_Bubble__Pause__) */