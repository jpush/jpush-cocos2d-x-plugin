//
//  MyScene.h
//  HelloCpp
//
//  Created by 张 on 14-5-7.
//
//

#include "cocos2d.h"
#ifndef __HelloCpp__MyScene__
#define __HelloCpp__MyScene__

#include <iostream>
USING_NS_CC;
class MyScene : public CCScene {
 public:
  //构造函数
  MyScene();
  //析构函数
  ~MyScene();
};
class MyLayer : public CCLayer {
 public:
  //构造函数
  MyLayer();
  //析构函数
  ~MyLayer();
  void onEnter();
  void onExit();
  // a example to change scene
  void sceneChangeCallback(CCObject* pSender);
};

#endif /* defined(__HelloCpp__MyScene__) */
