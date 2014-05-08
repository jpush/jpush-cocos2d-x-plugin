#include "HelloWorldScene.h"
#include "MyScene.h"
#include "JPushService.h"

void setupCallback(void *p_handle);
void closeCallback(void *p_handle);
void registerCallback(void *p_handle, const char *registrationID);
void loginCallback(void *p_handle);
void receiveMessageCallback(void *p_handle, message message);
void tagsAliasCallback(void *p_handle, int iResCode, const char *alias,
                       c_tags tags);

CCScene *HelloWorld::scene() {
  // 'scene' is an autorelease object
  CCScene *scene = CCScene::create();

  // 'layer' is an autorelease object
  HelloWorld *layer = HelloWorld::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCLayer::init()) {
    return false;
  }

  CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
  CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

  /////////////////////////////
  // 2. add a menu item with "X" image, which is clicked to quit the program
  //    you may modify it.

  // add a "close" icon to exit the progress. it's an autorelease object
  CCMenuItemImage *pCloseItem =
      CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this,
                              menu_selector(HelloWorld::sceneChangeCallback));

  pCloseItem->setPosition(
      ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width / 2,
          origin.y + pCloseItem->getContentSize().height / 2));

  // create menu, it's an autorelease object
  CCMenu *pMenu = CCMenu::create(pCloseItem, NULL);
  pMenu->setPosition(CCPointZero);
  this->addChild(pMenu, 1);

  /////////////////////////////
  // 3. add your codes below...

  // add a label shows "Hello World"
  // create and initialize a label

  CCLabelTTF *pLabel = CCLabelTTF::create("Hello World", "Arial", 24);

  // position the label on the center of the screen
  pLabel->setPosition(
      ccp(origin.x + visibleSize.width / 2,
          origin.y + visibleSize.height - pLabel->getContentSize().height));

  // add the label as a child to this layer
  this->addChild(pLabel, 1);

  // add "HelloWorld" splash screen"
  CCSprite *pSprite = CCSprite::create("HelloWorld.png");

  // position the sprite on the center of the screen
  pSprite->setPosition(
      ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

  // add the sprite as a child to this layer
  this->addChild(pSprite, 0);

  /**
   *  JPUSH example
   *  include how to register your callback function to SDK
   *  include set tags example
   *  include how to use pageFlow which means sceneFlow in Cocos2d-x
   *  if use filterValidTags function, remember to delete the return set.
   */
  jpushLabel = CCLabelTTF::create("Hello Jpush", "Arial", 24);
  registerIDLabel = CCLabelTTF::create("ReigstrationID", "Arial", 24);
  receiveMessageLabel = CCLabelTTF::create("ReceiveMessage", "Arial", 24);
  // position the label on the bottom of the screen
  jpushLabel->setPosition(ccp(origin.x + visibleSize.width / 2,
                              origin.y + jpushLabel->getContentSize().height));
  registerIDLabel->setPosition(
      ccp(origin.x + visibleSize.width / 2,
          origin.y + 2 * jpushLabel->getContentSize().height));
  receiveMessageLabel->setPosition(
      ccp(origin.x + visibleSize.width / 2,
          origin.y + 3 * jpushLabel->getContentSize().height));

  // add the label as a child to this layer
  this->addChild(jpushLabel, 2);
  this->addChild(registerIDLabel, 3);
  this->addChild(receiveMessageLabel, 4);
  // register callback
  JPushService::registerCallbackFunction((void *)this, setupCallback,
                                         closeCallback, registerCallback,
                                         loginCallback, receiveMessageCallback);

  JPushService::setAlias((void *)this, "别名1", tagsAliasCallback);

  // set tags
  c_tags tags1 = new set<string>;
  tags1->insert("tag1");
  tags1->insert("tag2");
  tags1->insert("tag3");

  JPushService::setAliasAndTags((void *)this, "别名1", tags1,
                                tagsAliasCallback);
  delete tags1;

  return true;
}

void HelloWorld::menuCloseCallback(CCObject *pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
  CCMessageBox(
      "You pressed the close button. Windows Store Apps do not implement a "
      "close button.",
      "Alert");
#else
  CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif
#endif
}

#pragma mark - JPUSH Callback
void setupCallback(void *p_handle) {
  ((HelloWorld *)p_handle)->jpushLabel->setString("setup");
}

void closeCallback(void *p_handle) {
  ((HelloWorld *)p_handle)->jpushLabel->setString("close");
}

void registerCallback(void *p_handle, const char *registrationID) {
  ((HelloWorld *)p_handle)->registerIDLabel->setString(registrationID);
}

void loginCallback(void *p_handle) {
  ((HelloWorld *)p_handle)->jpushLabel->setString("login");
  const char *registration = JPushService::registrationID();
  ((HelloWorld *)p_handle)->registerIDLabel->setString(registration);
}

void receiveMessageCallback(void *p_handle, message message) {
  std::string *show_message = new std::string();
  map<string, void *>::iterator it = message->begin();
  for (; it != message->end(); ++it) {
    cout << "APkey:" << it->first << endl;
    show_message->append(" ");
    show_message->append(it->first);
    show_message->append(":");
    if (it->first.compare("extras") == 0) {
      map<string, void *> *extra = (map<string, void *> *)it->second;
      map<string, void *>::iterator it = extra->begin();
      for (; it != extra->end(); ++it) {
        cout << "APkey:" << it->first << endl;
        cout << "value:" << *((string *)it->second) << endl;
      }
    } else {
      cout << "APvalue:" << *((string *)(it->second)) << endl;
      show_message->append(*((string *)(it->second)));
    }
  }
  ((HelloWorld *)p_handle)
      ->receiveMessageLabel->setString(show_message->c_str());
  delete show_message;
}

void tagsAliasCallback(void *p_handle, int responseCode, const char *alias,
                       c_tags tags) {
  set<string>::iterator it = tags->begin();
  for (; it != tags->end(); ++it) {
    const char *p = ((string)(*it)).c_str();
    cout << "tags:" << p << endl;
  }
  cout << "alias" << alias << endl;
  cout << "iResCode" << responseCode << endl;
}

void HelloWorld::sceneChangeCallback(CCObject *pSender) {
  /**
   *  Example Scene
   */
  //实例化一个场景MyScene
  CCScene *scene = new MyScene();
  //实例化一个布景MyLayer
  CCLayer *layer = new MyLayer();
  //将布景添加到场景中
  scene->addChild(layer, 0);

  //运行新创建的这个场景,并设置过渡效果（此处为淡入淡出）
  CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(2, scene));
  //释放
  layer->release();
  scene->release();
}
void HelloWorld::onEnter() {
  CCLayer::onEnter();
  JPushService::pageStart("example");
}
void HelloWorld::onExit() {
  CCLayer::onExit();
  JPushService::pageEnd("example");
}
