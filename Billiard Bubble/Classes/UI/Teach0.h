#ifndef __Billiard_Bubble__Teach0__
#define __Billiard_Bubble__Teach0__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class Teach0 : public CCLayer
{
public:
    bool init();
    static CCScene* scene();
    CREATE_FUNC(Teach0);
    
protected:
    void nextPage();
    
protected:
    CCSize size;
};

#endif /* defined(__Billiard_Bubble__Teach0__) */
