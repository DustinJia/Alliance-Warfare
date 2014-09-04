#ifndef __Billiard_Bubble__Setup__
#define __Billiard_Bubble__Setup__

#include "cocos2d.h"
#include <iostream>
#include "cocos-ext.h"

USING_NS_CC;
using namespace extension;

class Setup : public CCLayer
{
public:
    static CCScene *scene();
    bool init();
    CREATE_FUNC(Setup);
public:
    /*滑动条*/
    CCControlSlider* slider;
    
protected:
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

#endif /* defined(__Billiard_Bubble__Setup__) */