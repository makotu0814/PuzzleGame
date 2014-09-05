#ifndef __DROP_SPRITE_H__
#define __DROP_SPRITE_H__

#include "cocos2d.h"
#include "Config.h"
#include "Box2d/Box2D.h"

class DropSprite : public cocos2d::Sprite
{
protected:
    const char* getDropImageFile(kDrop dropType);
    Sprite* collision;

public:
    CC_SYNTHESIZE_READONLY(kDrop, m_dropType, DropType);
    CC_SYNTHESIZE(kStatus, status, Status);

    DropSprite();
    virtual ~DropSprite();

    bool deleteFlg = false;

    virtual bool initWithDropType(kDrop dropType);
    static DropSprite* createWithDropType(kDrop dropType);

    cocos2d::Rect getRect();
    bool isTapped(cocos2d::Point);
};

#endif // __DROP_SPRITE_H__
