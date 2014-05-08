//
//  MyScene.cpp
//  HelloCpp
//
//  Created by 张 on 14-5-7.
//
//

#include "MyScene.h"
#include "HelloWorldScene.h"
#include "JPushService.h"
MyScene::MyScene() {}
MyScene::~MyScene() {}

void MyLayer::sceneChangeCallback(CCObject *pSender) {
  CCDirector::sharedDirector()->popScene();
}

MyLayer::MyLayer() {
  CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
  CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
  /////////////////////////////
  // 2. add a menu item with "X" image, which is clicked to quit the program
  //    you may modify it.

  // add a "close" icon to exit the progress. it's an autorelease object
  CCMenuItemImage *pCloseItem =
      CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this,
                              menu_selector(MyLayer::sceneChangeCallback));

  pCloseItem->setPosition(
      ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width / 2,
          origin.y + pCloseItem->getContentSize().height / 2));

  // create menu, it's an autorelease object
  CCMenu *pMenu = CCMenu::create(pCloseItem, NULL);
  pMenu->setPosition(CCPointZero);
  this->addChild(pMenu, 1);

  //获得屏幕的大小
  CCSize size = CCDirector::sharedDirector()->getWinSize();
  //创建一行文本并设置位置在屏幕中间
  CCLabelTTF *label = CCLabelTTF::create("This is MyScene.", "Arial", 30);
  label->setPosition(ccp(size.width / 2, size.height / 2));
  //将这行文本添加到布景中
  addChild(label);
}
void MyLayer::onEnter() {
  CCLayer::onEnter();
  JPushService::pageStart("example2");
}
void MyLayer::onExit() {
  CCLayer::onExit();
  JPushService::pageEnd("example2");
}

MyLayer::~MyLayer() {}
