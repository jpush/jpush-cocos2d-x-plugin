//  JPushService.mm
//  PushSDK
//
//  Created by zhanghao on 14-4-16.
//  Copyright (c) 2012年 HXHG. All rights reserved.

#import "JPushService.h"
#import "objc/runtime.h"
#import "APService.h"
#pragma - mark transfer function
/**
 *  convert NSString to CString
 *
 *  @param oc_string Objective C NSString
 *  @return C plus plus std:string
 */
string convert_string_to_c(NSString *oc_string) {
  string c_string = [oc_string UTF8String];
  return c_string;
}

/**
 *  convert set<string> to NSSet
 *
 *  @param tags C plus plus std:set
 *
 *  @return Objective C NSSet
 */
NSSet *convert_to_oc(set<string> *tags) {
  if (tags == NULL) {
    return nil;
  }
  set<string>::iterator it;
  if (tags->empty()) {
    return [NSSet set];
  }

  NSMutableSet *result = [NSMutableSet set];
  for (it = tags->begin(); it != tags->end(); ++it) {
    NSString *str = [NSString stringWithCString:(*it).c_str()
                                       encoding:NSUTF8StringEncoding];
    [result addObject:str];
  }
  return result;
}

/**
 *  convert const char* to NSString
 *
 *  @param c_str C plus plus const char*
 *
 *  @return Objective C NSString
 */
NSString *convert_to_oc(const char *c_str) {
  if (c_str == NULL) {
    return nil;
  }
  return [NSString stringWithUTF8String:c_str];
}

/**
 *  convert const NSString to char*
 *
 *  @param oc_string Objective C NSString
 *
 *  @return C plus plus const char*
 */
const char *convert_to_c(NSString *oc_string) {
  if (oc_string == nil) {
    return NULL;
  }
  return [oc_string UTF8String];
}

/**
 *
 */
/**
 *  convert NSSet to set<string>
 *
 *  @param source_tags Objective C NSSet
 *  @param target_tags C plus plus std:set
 */
void convert_to_c(NSSet *source_tags, set<string> *target_tags) {
  if (![source_tags count]) {
    target_tags = NULL;
  }
  for (NSString *oc_string in source_tags) {
    string c_string = convert_string_to_c(oc_string);
    target_tags->insert(c_string);
  }
  if (target_tags->empty()) {
  }
  source_tags = nil;
}

/**
 *  analyse message extras value.
 *  notification message has two key:content & extras.
 *  extras key is used to contains all customise key-value.
 *  this function is used to get all key-value from message into
 *  NSDictionary(Objective C object)
 *
 *  @param extra      extras message(NSDictionary)
 *  @param extra_map  extras message(std:map)
 */
void analyse_extra(NSDictionary *extra, message extra_map) {
  for (NSString *key in [extra allKeys]) {
    NSString *value = extra[key];
    if (![value isKindOfClass:[NSString class]]) {
      return;
    }
    string *str_value = new string([value UTF8String]);
    extra_map->insert(
        pair<string, void *>([key UTF8String], static_cast<void *>(str_value)));
  }
}

/**
 *  transfer NSDictionary to map<string,void>
 *
 *  @param oc_map  Received message(NSDictionary)
 *  @param result  Received message(std:map)
 */
void convert_map_to_c(NSDictionary *oc_map, message result) {
  NSArray *keys = [oc_map allKeys];
  for (NSString *key in keys) {
    if (![key isEqualToString:@"extras"]) {
      NSString *value = [oc_map objectForKey:key];
      string *str_value = new string([value UTF8String]);
      result->insert(pair<string, void *>([key UTF8String],
                                          static_cast<void *>(str_value)));
      continue;
    } else {
      // if no extras, set extras value equals ''
      if (![oc_map objectForKey:key]) {
      }
      // get all extras key-value
      map<string, void *> *map_value = new map<string, void *>;
      analyse_extra(oc_map[key], map_value);
      result->insert(pair<string, void *>([key UTF8String],
                                          static_cast<void *>(map_value)));
      continue;
    }
  }
}

/**
 *  delete map after send to API function.
 *  this function used to delete All message map pointers
 *  (include string pointer) which generate when change NSDictionary
 *  (Objecive C object from OC API) to map after send to callback
 *  function
 *
 *  @param message   received message(std:map)
 */
void delete_receive_message(map<string, void *> *message) {
  // delete
  map<string, void *>::iterator it = message->begin();
  for (; it != message->end(); ++it) {
    if ((it->first).compare("extras") == 0) {
      // delete extra value map and all value string.
      map<string, void *> *extra_map = (map<string, void *> *)it->second;
      map<string, void *>::iterator it = extra_map->begin();
      for (; it != extra_map->end(); ++it) {
        string *extra_value = (string *)it->second;
        if (extra_value->empty()) {
          continue;
        }
        delete extra_value;
      }
      delete extra_map;
    } else {
      // delete other value string
      string *value = (string *)it->second;
      if (value->empty()) {
        continue;
      }
      delete value;
    }
  }
  // delete message map
  delete message;
}

/**
 *  delete tags
 *  this function is used to delete ctags which is changed from
 *  NSSet(Objecive C object frome OC API) after send to callback
 *  function
 *
 *  @param ctags tags(std:set)
 */
void delete_tags_convert(c_tags ctags) { delete ctags; }

#pragma - mark OC CallbackController Class
/**
 *  Objective C Callback Controller
 *  this function has two purpose:
 *  1.save all callback function pointer & p_handle(this pointer point to the
 *    class you used to deal with from the callback function)
 *  2.receive the Objecitve C SDK API callback function, then call the C++
 *    callback.
 */
@interface APCallBackController : NSObject {
  NSUInteger _idx;
}
/**
 *  callback function pointer save
 */
@property(nonatomic, assign) APTagAliasCallback tagsAliasCallback;
@property(nonatomic, assign) APNetworkDidSetup_callback setupCallback;
@property(nonatomic, assign) APNetworkDidClose_callback closeCallback;
@property(nonatomic, assign) APNetworkDidRegister_callback registerCallback;
@property(nonatomic, assign) APNetworkDidLogin_callback loginCallback;
@property(nonatomic, assign)
    APNetworkDidReceiveMessage_callback receiveMessageCallback;
+ (APCallBackController *)sharedService;
- (id)init;
@end

@implementation APCallBackController
static APCallBackController *_sharedService = nil;
/**
 *  p_handler save
 */
static void *setupHandle = nil;
static void *closeHandle = nil;
static void *registerHandle = nil;
static void *loginHandle = nil;
static void *receiveMessageHandle = nil;
static void *setAliasTagsHandle = nil;

+ (APCallBackController *)sharedService {
  static dispatch_once_t onceAPService;
  dispatch_once(&onceAPService, ^{
      _sharedService = [[APCallBackController alloc] init];
      [_sharedService p_addNotification];
  });
  return _sharedService;
}

- (id)init {
  if (self = [super init]) {
  }
  return self;
}

/**
 *  reigster callback notification observer to Objecitve C runtime.
 */
- (void)p_addNotification {
  NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
  [defaultCenter addObserver:self
                    selector:@selector(networkDidSetup:)
                        name:kAPNetworkDidSetupNotification
                      object:nil];
  [defaultCenter addObserver:self
                    selector:@selector(networkDidClose:)
                        name:kAPNetworkDidCloseNotification
                      object:nil];
  [defaultCenter addObserver:self
                    selector:@selector(networkDidRegister:)
                        name:kAPNetworkDidRegisterNotification
                      object:nil];
  [defaultCenter addObserver:self
                    selector:@selector(networkDidLogin:)
                        name:kAPNetworkDidLoginNotification
                      object:nil];
  [defaultCenter addObserver:self
                    selector:@selector(networkDidReceiveMessage:)
                        name:kAPNetworkDidReceiveMessageNotification
                      object:nil];
  [defaultCenter addObserver:self
                    selector:@selector(serviceError:)
                        name:kAPServiceErrorNotification
                      object:nil];
}

/**
 *  set Alias & tags callback
 *  send Tags&Alias to Objective C SDK
 *  notice:callback only save the last register function,All register before
 *  will be remove.
 */
- (void)setAliasAndTags:(NSSet *)tags
                  alias:(NSString *)alias
               callback:(APTagAliasCallback)tagsAliasCallback {
  _tagsAliasCallback = tagsAliasCallback;
  // call SDK API
  [APService setTags:tags
                 alias:alias
      callbackSelector:@selector(tagsAliasCallback:tags:alias:)
                target:self];
}

- (void)setTags:(NSSet *)tags callback:(APTagAliasCallback)tagsAliasCallback {
  _tagsAliasCallback = tagsAliasCallback;
  // call SDK API
  [APService setTags:tags
      callbackSelector:@selector(tagsAliasCallback:tags:alias:)
                object:self];
}

- (void)setAlias:(NSString *)alias
        callback:(APTagAliasCallback)tagsAliasCallback {
  _tagsAliasCallback = tagsAliasCallback;
  // call SDK API
  [APService setAlias:alias
      callbackSelector:@selector(tagsAliasCallback:tags:alias:)
                object:self];
}

#pragma - mark Callback Function
/**
 *  callback function from Objective C SDK
 *  tagsAliasCallback invoke the tags&alias C++ callback function.
 *  networkDidSetup           invoke the C++ setup callback
 *  networkDidClose           invoke the C++ close callbcak
 *  networkDidRegister        invoke the C++ reigister callback
 *  networkDidLogin           invoke the C++ login callback
 *  networkDidReceiveMessage  invoke the C++ receiveMessage callback
 */

- (void)tagsAliasCallback:(int)iResCode
                     tags:(NSSet *)tags
                    alias:(NSString *)alias {
  c_tags ctags = new set<string>;
  convert_to_c(tags, ctags);
  _tagsAliasCallback(setAliasTagsHandle, iResCode, convert_to_c(alias), ctags);
  delete_tags_convert(ctags);
}

- (void)networkDidSetup:(NSNotification *)notification {
  if (_setupCallback == NULL) {
    return;
  }
  _setupCallback(setupHandle);
}

- (void)networkDidClose:(NSNotification *)notification {
  if (_closeCallback == NULL) {
    return;
  }
  _closeCallback(closeHandle);
}

- (void)networkDidRegister:(NSNotification *)notification {
  if (_registerCallback == NULL) {
    return;
  }
  NSString *registrationId =
      [[notification userInfo] objectForKey:@"RegistrationID"];
  _registerCallback(registerHandle, [registrationId UTF8String]);
}

- (void)networkDidLogin:(NSNotification *)notification {
  if (_loginCallback == NULL) {
    return;
  }
  _loginCallback(loginHandle);
}

- (void)networkDidReceiveMessage:(NSNotification *)notification {
  if (_receiveMessageCallback == NULL) {
    return;
  }
  map<string, void *> *result = new map<string, void *>;
  convert_map_to_c([notification userInfo], result);
  _receiveMessageCallback(receiveMessageHandle, result);
  delete_receive_message(result);
}
@end

#pragma - mark C++ function
static APCallBackController *callbackController =
    [APCallBackController sharedService];
/**
 *  register all callback function
 *
 *  @param p_handle          callback handle
 *  @param setup_callback    setup callback function pointers
 *  @param close_callback    close callback function pointers
 *  @param register_callback register callback function pointers
 *  @param login_callback     login callback function pointers
 *  @param message_callback  message receive function pointers
 */
void JPushService::registerCallbackFunction(
    void *p_handle, APNetworkDidSetup_callback setup_callback,
    APNetworkDidClose_callback close_callback,
    APNetworkDidRegister_callback register_callback,
    APNetworkDidLogin_callback login_callback,
    APNetworkDidReceiveMessage_callback message_callback) {
  callbackController.setupCallback = setup_callback;
  callbackController.closeCallback = close_callback;
  callbackController.registerCallback = register_callback;
  callbackController.loginCallback = login_callback;
  callbackController.receiveMessageCallback = message_callback;
  /**
   *  set callback handle.
   */
  setupHandle = p_handle;
  closeHandle = p_handle;
  registerHandle = p_handle;
  loginHandle = p_handle;
  receiveMessageHandle = p_handle;
}

/*
 *   set callback function
 */
void JPushService::registerSetupCallbackFunction(
    void *p_handle, APNetworkDidSetup_callback setup_callback) {
  callbackController.setupCallback = setup_callback;
  setupHandle = p_handle;
};

/*
 *   set close callback function
 */
void JPushService::registerCloseCallbackFunction(
    void *p_handle, APNetworkDidClose_callback close_callback) {
  callbackController.closeCallback = close_callback;
  closeHandle = p_handle;
}
/*
 *   set callback callback function
 */
void JPushService::registerRegisterCallbackFunction(
    void *p_handle, APNetworkDidRegister_callback register_callback) {
  callbackController.registerCallback = register_callback;
  registerHandle = p_handle;
}
/*
 *   set login callback function
 */
void JPushService::registerLoginCallbackFunction(
    void *p_handle, APNetworkDidLogin_callback login_callback) {
  callbackController.loginCallback = login_callback;
  loginHandle = p_handle;
}
/*
 *   set message callback function
 */
void JPushService::registerCallbackFunction(
    void *p_handle, APNetworkDidReceiveMessage_callback message_callback) {
  callbackController.receiveMessageCallback = message_callback;
  receiveMessageHandle = p_handle;
}

/**
 *  set Tags & Alias C++ API.
 */
void JPushService::setAliasAndTags(void *p_handle, const char *alias,
                                   set<string> *tags,
                                   APTagAliasCallback callback) {
  if (callbackController) {
    setAliasTagsHandle = p_handle;
    [callbackController setAliasAndTags:convert_to_oc(tags)
                                  alias:convert_to_oc(alias)
                               callback:callback];
  }
}

void JPushService::setTags(void *p_handle, set<string> *tags,
                           APTagAliasCallback callback) {
  if (callbackController) {
    setAliasTagsHandle = p_handle;
    [callbackController setTags:convert_to_oc(tags) callback:callback];
  }
}

void JPushService::setAlias(void *p_handle, const char *alias,
                            APTagAliasCallback callback) {
  if (callbackController) {
    setAliasTagsHandle = p_handle;
    [callbackController setAlias:convert_to_oc(alias) callback:callback];
  }
}

/*
 * this function used to check whether tags valid.
 *
 */
c_tags JPushService::filterValidTags(c_tags tags, set<string> *result) {
  if (result == NULL) {
    NSLog(@"Warning:the set you send to get filterValidTags is NULL!");
    return NULL;
  }
  if (!result->empty()) {
    NSLog(
        @"Warning:You need to send a empty set to get filterValidTags result!");
    return NULL;
  }
  NSSet *vaildSet = [APService filterValidTags:convert_to_oc(tags)];
  c_tags ctags = new set<string>;
  convert_to_c(vaildSet, ctags);
  return ctags;
}

/**
 *  Set Page Record
 *  pageStart & pageEnd is a pair function.
 *  add pageStart when the scene begin & pageEnd when scene remove or pop.
 *  (e.g.,add pageStart in CCLayer::onEnter() & add pageEnd in
 *  CCLayer::onExit())
 *  remember to ensure the funciton must be called when scene start or end.
 *
 *  pageDefine is a customize method,you can use it to add a cutomize page
 *  contains duration time already.(no place limited)
 *
 */
void JPushService::pageStart(const char *pageName) {
  [APService startLogPageView:[NSString stringWithUTF8String:pageName]];
}

void JPushService::pageEnd(const char *pageName) {
  [APService stopLogPageView:[NSString stringWithUTF8String:pageName]];
}

void JPushService::pageDefine(const char *pageName, int duration) {
  [APService beginLogPageView:[NSString stringWithUTF8String:pageName]
                     duration:duration];
}

/**
 *  get the UDID
 */
const char *JPushService::openUDID() {
  return [[APService openUDID] UTF8String];
}

/**
 *  get RegistionID
 */
const char *JPushService::registrationID() {
  return [[APService registrionID] UTF8String];
}
