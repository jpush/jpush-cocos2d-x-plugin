#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"

USING_NS_CC;
class HelloWorld : public cocos2d::CCLayer {
 public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of
  // returning 'id' in cocos2d-iphone
  virtual bool init();

  // there's no 'id' in cpp, so we recommend returning the class instance
  // pointer
  static cocos2d::CCScene* scene();

  // a selector callback
  void menuCloseCallback(CCObject* pSender);

  // a example to change scene
  void sceneChangeCallback(CCObject* pSender);

  // implement the "static node()" method manually
  CREATE_FUNC(HelloWorld);
  //

  void onEnter();
  void onExit();

  CCLabelTTF* jpushLabel;
  CCLabelTTF* registerIDLabel;
  CCLabelTTF* receiveMessageLabel;
};

#endif  // __HELLOWORLD_SCENE_H__
