//
//  About.h
//  Billiard Bubble
//
//  Created by ldic on 13-7-8.
//
//

#ifndef __Billiard_Bubble__About__
#define __Billiard_Bubble__About__
#include "cocos2d.h"
#include <iostream>
USING_NS_CC;

class About : public CCLayer
{
public:
    static CCScene *scene();
    bool init();
    CREATE_FUNC(About);
    
private:
    void backMenu();
    
private:
    CCSize size;
    
};

#endif