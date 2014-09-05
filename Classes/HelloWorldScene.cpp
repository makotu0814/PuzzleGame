#include "HelloWorldScene.h"
#include "DropSprite.h"
#include <math.h>

USING_NS_CC;

using namespace std;

const int MAX_DROP_X = 6;
const int MAX_DROP_Y = 5;

float m_dropSize = 0.0f;

int layer = 1;
int tag = 0;
int swaipAction = 1;
int shrinkAction = 2;
int dropAction = 3;

map<int, std::vector<int>> deleteMap;

std::vector<DropSprite*> dropSprites;

DropSprite* touchSprite = NULL;
Vec2 oldPos;

int dropWidth;
int dropHeight;

bool checkSprites = false;

Sprite* backGround;

Scene* HelloWorld::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add "HelloWorld" splash screen"
	backGround = Sprite::create("Mainbg.png");
	backGround->setPosition(
			Vec2(visibleSize.width / 2 + origin.x,
					visibleSize.height / 2 + origin.y));

	createPuzzle(backGround);

	this->setTouchMode(kCCTouchesOneByOne);
	this->setTouchEnabled(true);

	this->addChild(backGround);
	//initForVariables();
	//showDrop();

	return true;
}

/*
 * パズル(5×6)を生成
 */
void HelloWorld::createPuzzle(Sprite* backGround) {

	for (int i = 0; i < MAX_DROP_Y; i++) {
		for (int j = 0; j < MAX_DROP_X; j++) {
			kDrop dropType = (kDrop) (rand() % DropTypeNum);

			auto dropSprite = DropSprite::createWithDropType(dropType);

			dropWidth = dropSprite->getContentSize().width;
			dropHeight = dropSprite->getContentSize().height;

			dropSprite->setAnchorPoint(ccp(0.5, 0.5));
			dropSprite->setPosition(
					Vec2(dropWidth / 2 + dropWidth * j, dropHeight * (i + 1)));
			dropSprite->setTag(tag++);
			backGround->addChild(dropSprite, layer);

			dropSprites.push_back(dropSprite);
		}
	}
}

DropSprite* HelloWorld::createDropSprite(float posX, float posY,kDrop dropType) {

	auto dropSprite = DropSprite::createWithDropType(dropType);

	dropSprite->setAnchorPoint(ccp(0.5, 0.5));
	dropSprite->setPosition(Vec2(posX, posY));
	dropSprite->setTag(tag++);

	return dropSprite;
}

/**
 * タップが開始されるとコールされる
 *
 * @return  void
 */
bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {

	if (checkSprites) {
		return true;
	}

	if (touchSprite != NULL) {
		return true;
	}

	auto sprite = getTouchSprite(touch->getLocation());
	if (sprite != NULL) {
		touchBeganSprite(sprite);

		return true;
	}

	return true;
}

/**
 * タップ開始後、移動するとコールされる
 *
 * @return  void
 */
void HelloWorld::onTouchMoved(Touch* touch, Event* event) {

	if (touchSprite != NULL && !checkSprites) {
		Vec2 movPos = touch->getLocation();
		touchSprite->setPosition(movPos);

		auto sprite = getTouchSprite(movPos);

		if (sprite != NULL && sprite->getTag() != touchSprite->getTag()) {
			// スプライトの交換処理
			if (sprite->getActionByTag(swaipAction) == NULL) {
				Vec2 tempPos = sprite->getPosition();
				MoveTo* targetMove = MoveTo::create(0.2f, oldPos);
				targetMove->setTag(swaipAction);
				sprite->runAction(targetMove);
				oldPos = tempPos;
			} else {
				// スワイプ中
			}
		}
	}
}

/**
 * タップが終了するとコールされる
 *
 * @return  void
 */
void HelloWorld::onTouchEnded(Touch* touch, Event* event) {

	if (!checkSprites && touchSprite != NULL) {
		touchEndSprite(touchSprite);
		touchSprite = NULL;

		deleteDropSprite();
	}
}

/**
 * 揃っているスプライトを削除する
 *
 * @return  void
 */
void HelloWorld::deleteDropSprite() {

	checkSprites = true;

	map<std::string, DropSprite*> spriteMap = getDropSpriteMap();
	deleteRowSprite(spriteMap);
	deleteColSprite(spriteMap);
	deleteMap = deleteAction(spriteMap);

	this->scheduleOnce(schedule_selector(HelloWorld::dropAction), 0.5);
}

void HelloWorld::checkCombo(float dm) {

	log("checkCombo");
	map<std::string, DropSprite*> spriteMap = getDropSpriteMap();
	deleteRowSprite(spriteMap);
	deleteColSprite(spriteMap);
	deleteMap = deleteAction(spriteMap);

	int count = 0;
	for (int col = 1; col < MAX_DROP_X + 1; col++) {
		std::vector<int> deleteRow = deleteMap[col];

		if (deleteRow.size() == 0) {
			count++;
		}
	}

	if (count == MAX_DROP_X) {
		log("コンボじゃない");
		checkSprites = false;
	} else {
		log("コンボ");
		deleteDropSprite();
	}
}


void HelloWorld::dropAction(float dm) {

	log("dropAction");
	map<std::string, DropSprite*> spriteMap = getDropSpriteMap();

	for (int col = 1; col < MAX_DROP_X + 1; col++) {

		std::vector<int> deleteRow = deleteMap[col];

		int deleteNum = deleteRow.size();

		if (deleteNum > 0) {
			int lastRow = deleteRow[deleteRow.size() - 1];

			for (int row = 0; row < MAX_DROP_Y + 1; row++) {
				// 下に落ちる処理
				DropSprite* sprite = spriteMap[createMapKey(row, col)];
				if (sprite != NULL) {
					int count = 0;
					for (int i = 0; i < deleteRow.size(); i++) {
						if (deleteRow[i] > row) {
							count++;
						}
					}
					Vec2 position = sprite->getPosition();
					MoveTo* dropMove = MoveTo::create(0.4f, Vec2(position.x, position.y - dropHeight * (deleteNum - count)));
					sprite->runAction(dropMove);
				}
			}

			for (int i = 0; i < deleteNum; i++) {

				kDrop dropType = (kDrop) (rand() % DropTypeNum);

				float posX = dropWidth / 2 + dropWidth * (col - 1);
				float posY = MAX_DROP_Y * dropHeight + dropHeight * (i + 1);
				auto sprite = createDropSprite(posX, posY, dropType);

				MoveTo* dropMove = MoveTo::create(0.4f, Vec2(posX, posY - deleteNum * dropHeight));

				sprite->runAction(dropMove);

				backGround->addChild(sprite, layer);

				dropSprites.push_back(sprite);
			}
		}
	}
	this->scheduleOnce(schedule_selector(HelloWorld::checkCombo), 0.6f);
}


/*
 * スプライトが消えるアニメーションが終了後に呼ばれる
 */
void HelloWorld::deleteEndAction(Node* node) {

	Sprite* targetSprite = (Sprite*) node;

	for (auto itl = dropSprites.begin(); itl != dropSprites.end();) {
		auto sprite = *itl;
		if (targetSprite->getTag() == sprite->getTag() && sprite->deleteFlg) {

			itl = dropSprites.erase(itl);

			break;
		}
		itl++;
	}

	targetSprite->removeFromParentAndCleanup(true);
}

/**
 * タップが終了するとコールされる
 *
 * @return  void
 */
map<int, std::vector<int>> HelloWorld::deleteAction(
		map<std::string, DropSprite*> spriteMap) {

	map<int, std::vector<int>> deleteMap;
	for (int col = 1; col < MAX_DROP_X + 1; col++) {
		std::vector<int> deleteVec;

		for (int row = 1; row < MAX_DROP_Y + 1; row++) {

			DropSprite* sprite = spriteMap[createMapKey(row, col)];

			if (sprite->deleteFlg) {

				deleteVec.push_back(row);

				ScaleTo* shrinkSprite = ScaleTo::create(0.2f, 0.0f);
				shrinkSprite->setTag(shrinkAction);

				CallFuncN* callDeleteEndAction = CallFuncN::create(this,
						callfuncN_selector(HelloWorld::deleteEndAction));

				sprite->runAction(Sequence::create(shrinkSprite, callDeleteEndAction, NULL));
			}
		}
		deleteMap[col] = deleteVec;
	}

	return deleteMap;
}

/*
 * 横方向に揃ったスプライトを削除する
 */
void HelloWorld::deleteRowSprite(map<std::string, DropSprite*> spriteMap) {

	for (int row = 1; row < MAX_DROP_Y + 1; row++) {
		int count = 0;
		int preType = -1;
		map<int, std::vector<DropSprite*>> deleteDropMap;
		for (int col = 1; col < MAX_DROP_X + 1; col++) {
			//log("%d行 %d列", row, col);

			DropSprite* sprite = spriteMap[createMapKey(row, col)];

			if (preType != sprite->getDropType()) {

				if (deleteDropMap[preType].size() >= 3) {
					for (auto sprite : deleteDropMap[preType]) {
						sprite->deleteFlg = true;
					}
				}

				deleteDropMap[preType].clear();
			}

			deleteDropMap[sprite->getDropType()].push_back(sprite);
			preType = sprite->getDropType();
		}

		for (int type = 0; type < DropTypeNum; type++) {
			std::vector<DropSprite*> deleteDrop = deleteDropMap[type];
			if (deleteDrop.size() >= 3) {
				//log("type  %d が3つ以上揃った", type);
				for (auto sprite : deleteDrop) {
					sprite->deleteFlg = true;
				}
			} else {
				//log("type  %d は揃ってない", type);
			}
		}
	}
}

/*
 * 縦方向に揃ったスプライトを削除する
 */
void HelloWorld::deleteColSprite(map<std::string, DropSprite*> spriteMap) {

	for (int col = 1; col < MAX_DROP_X + 1; col++) {
		//log("%d列目をチェック", col);
		int count = 0;
		int preType = -1;
		map<int, std::vector<DropSprite*>> deleteDropMap;
		for (int row = 1; row < MAX_DROP_Y + 1; row++) {
			DropSprite* sprite = spriteMap[createMapKey(row, col)];

			if (preType != sprite->getDropType()) {

				if (deleteDropMap[preType].size() >= 3) {
					for (auto sprite : deleteDropMap[preType]) {
						sprite->deleteFlg = true;
					}
				}

				deleteDropMap[preType].clear();
			}

			deleteDropMap[sprite->getDropType()].push_back(sprite);
			preType = sprite->getDropType();
		}

		for (int type = 0; type < DropTypeNum; type++) {
			std::vector<DropSprite*> deleteDrop = deleteDropMap[type];
			if (deleteDrop.size() >= 3) {
				//log("type  %d が3つ以上揃った", type);
				for (auto sprite : deleteDrop) {
					sprite->deleteFlg = true;
				}
			} else {
				//log("type  %d は揃ってない", type);
			}
		}
	}
}

/*
 * mapのキーを作成して返す
 */
std::string HelloWorld::createMapKey(int row, int col) {
	return std::to_string(row) + ":" + std::to_string(col);
}

/*
 * (行,列)をkey, valueをスプライトのMapを作成して返す
 *
 * return map<std::string, std::vector<DropSprite*>>
 */
map<std::string, DropSprite*> HelloWorld::getDropSpriteMap() {

	map<std::string, DropSprite*> spriteMap;

	for (auto sprite : dropSprites) {
		Vec2 position = sprite->getPosition();
		int row = GetRow(position);
		int col = GetCol(position);
		int kDrop = sprite->getDropType();

		std::string key = createMapKey(row, col);

		spriteMap[key] = sprite;
	}

	return spriteMap;
}

int HelloWorld::GetRow(Vec2 position) {

	for (int row = 1; row < MAX_DROP_Y + 1; row++) {
		if (position.y <= row * dropHeight) {
			return row;
		}
	}

	return 0;
}

int HelloWorld::GetCol(Vec2 position) {
	for (int col = 1; col < MAX_DROP_X + 1; col++) {
		if (position.x <= (col - 1) * dropWidth + dropWidth / 2) {
			return col;
		}
	}
	return 0;
}

/**
 * タッチされたスプライトを返す
 *
 */
DropSprite* HelloWorld::getTouchSprite(Vec2 position) {
	for (auto sprite : dropSprites) {
		if (touchSprite != NULL) {
			if (touchSprite->getTag() != sprite->getTag()) {
				if (sprite->isTapped(position)) {
					return sprite;
				}
			}
		} else {
			if (sprite->isTapped(position)) {
				return sprite;
			}
		}
	}
	return NULL;
}

/*
 * タッチされたスプライトを拡大＆透明
 */
void HelloWorld::touchBeganSprite(DropSprite* sprite) {
	int touchLayer = layer + 1;
	sprite->setScale(1.2f);
	sprite->setOpacity(128);
	sprite->setAnchorPoint(ccp(0.3, 0.3));
	sprite->setLocalZOrder(touchLayer);

	touchSprite = sprite;
	oldPos = touchSprite->getPosition();
}

/*
 * タッチされたスプライトの大きさや色を元に戻す
 */
void HelloWorld::touchEndSprite(DropSprite* sprite) {
	sprite->setScale(1.0f);
	sprite->setOpacity(255);
	sprite->setAnchorPoint(ccp(0.5, 0.5));
	sprite->setLocalZOrder(layer);
	sprite->setPosition(oldPos);
}

