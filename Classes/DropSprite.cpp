#include "DropSprite.h"

USING_NS_CC;


DropSprite::DropSprite()
{

}

DropSprite::~DropSprite()
{

}

DropSprite* DropSprite::createWithDropType(kDrop dropType)
{
    DropSprite *pRet = new DropSprite();
    if(pRet && pRet->initWithDropType(dropType)){
        pRet->autorelease();
        return pRet;
    } else {
    	log("失敗");
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool DropSprite::initWithDropType(kDrop dropType)
{
    if(!Sprite::initWithFile(getDropImageFile(dropType))){
        return false;
    }

    m_dropType = dropType;

    //collision = Sprite::create();
    //collision->setTextureRect(getRect());
    //collision->setColor(Color3B::WHITE);
    //collision->setOpacity(50);


    //this->addChild(collision);
    return true;
}

const char* DropSprite::getDropImageFile(kDrop dropType)
{
    switch (dropType) {
        case kDropBlue:
            return "drop_blue.png";

        case kDropGreen:
            return "drop_green.png";

        case kDropPink:
            return "drop_pink.png";

        case kDropPurple:
            return "drop_purple.png";

        case kDropRed:
            return "drop_red.png";

        case kDropYellow:
        	return  "drop_yellow.png";
        default:
            CCASSERT(false, "invalid dropType");
            return "";
    }
}


/**
 * タップされたかどうか
 *
 * @param   cocos2d::Point  point   座標点
 * @return  boolean
 */
bool DropSprite::isTapped(Point point)
{
    Rect rect = this->getRect();
    bool tapped = rect.containsPoint(point);
    if (tapped) {
        //log("ok");
    } else {
        //log("ng");
    }
    return tapped;
}


/**
 * Spriteの座標点の範囲を取得
 *
 * @return  cocos2d::Rect
 */
Rect DropSprite::getRect()
{
    Point point = this->getPosition();
    int w = this->getContentSize().width * this->getScaleX();
    int h = this->getContentSize().height * this->getScaleY();

   /* log("point.x : %f", point.x);
    log("point.y : %f", point.y);
    log("w : %d ", w);
    log("h : %d ", h);*/

    return CCRectMake(point.x - (w / 2), point.y - h, w, h);
}

