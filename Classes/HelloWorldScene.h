#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "DropSprite.h"
#include "Config.h"

using namespace std;


class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // タッチ関係のイベント
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void createPuzzle(cocos2d::Sprite* background);

    DropSprite* createDropSprite(float posX, float posY, kDrop dropType);

    DropSprite* getTouchSprite(cocos2d::Vec2 position);

    void touchBeganSprite(DropSprite* sprite);
    void touchEndSprite(DropSprite* sprite);

    int GetRow(cocos2d::Vec2 position);

    int GetCol(cocos2d::Vec2 position);

    void dropAction(float dm);

    void checkCombo(float dm);

    std::string createMapKey(int row, int col);

    std::map<std::string, DropSprite*> getDropSpriteMap();

    void deleteDropSprite();

    void deleteRowSprite(map<std::string, DropSprite*>);

    void deleteColSprite(map<std::string, DropSprite*>);

    map<int, std::vector<int>> deleteAction(map<std::string, DropSprite*>);

    void deleteEndAction(Node* node);

    CREATE_FUNC(HelloWorld);
};


#endif // __HELLOWORLD_SCENE_H__
