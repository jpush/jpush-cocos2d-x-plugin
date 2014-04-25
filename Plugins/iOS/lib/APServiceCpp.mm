//
//  APServiceCpp.mm
//  PushSDK
//
//  Created by 张 on 14-4-16.
//
//

#import "APServiceCpp.h"
#import "objc/runtime.h"

#pragma - mark transfer function
/*
*   convert NSString to CString
**/
string *convert_string_to_c(NSString *oc_string) {
  string *c_string = new string([oc_string UTF8String]);
  return c_string;
}

/**
 *  convert set<string> to NSSet
 */
NSMutableSet *convert_to_oc(set<string> *tags) {
  set<string>::iterator it;
  if (tags->empty()) {
    return nil;
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
 *  convert NSSet to set<string>
 */
set<string> *convert_to_c(NSMutableSet *tags) {
  set<string> *result = new set<string>;

  [tags enumerateObjectsUsingBlock:^(NSString *oc_string, BOOL *stop) {
      string *c_string = convert_string_to_c(oc_string);
      //        std::string *c_string = new std::string([string UTF8String]);
      result->insert(*c_string);
  }];
  if (result->empty()) {
    return result;
  }
  return result;
}

/**
 *  analyse message extra value.
 */
map<string, void *> analyse_extra(NSString *key, id extra) {
  map<string, void *> *result = new map<string, void *>;
  if ([extra isKindOfClass:[NSString class]]) {
    string *str_value = new string([extra UTF8String]);
    result->insert(
        pair<string, void *>([key UTF8String], static_cast<void *>(str_value)));

    delete str_value;
    return *result;
  }
  if (![extra isKindOfClass:[NSDictionary class]]) {
    return *result;
  }

  map<string, void *> *map_value = new map<string, void *>;
  [extra enumerateKeysAndObjectsUsingBlock:^(NSString *subkey, id object,
                                             BOOL *stop) {
      map<string, void *> sub_value = analyse_extra(subkey, object);
      void *sub_value_address = &sub_value;
      map_value->insert(
          pair<string, void *>([subkey UTF8String], sub_value_address));
  }];
  void *map_value_address = &map_value;
  result->insert(pair<string, void *>([key UTF8String], map_value_address));

  delete map_value;
  return *result;
}

/*
*    transfer NSDictionary to map<string,void>
* */
map<string, void *> convert_map_to_c(NSDictionary *oc_map) {
  map<string, void *> *result = new map<string, void *>;
  NSArray *keys = [oc_map allKeys];
  for (NSString *key in keys) {
    if (![key isEqualToString:@"extras"]) {
      NSString *value = [oc_map objectForKey:key];
      string *str_value = new string([value UTF8String]);
      result->insert(pair<string, void *>([key UTF8String],
                                          static_cast<void *>(str_value)));

      delete str_value;
    } else {
      id value = [oc_map objectForKey:key];
      map<string, void *> map_value = analyse_extra(key, value);
      void *map_value_address = &map_value;
      result->insert(pair<string, void *>([key UTF8String], map_value_address));
    }
  }
  return *result;
}

#pragma - mark OC CallbackController Class
@interface APCallBackController : NSObject {
  NSUInteger _idx;
}
//@property (nonatomic, assign) tags_alias_callback tagsAliasCallback;
@property(nonatomic, assign) networkDidSetup_callback setupCallback;
@property(nonatomic, assign) networkDidClose_callback closeCallback;
@property(nonatomic, assign) networkDidRegister_callback registerCallback;
@property(nonatomic, assign) networkDidLogin_callback loginCallback;
@property(nonatomic, assign)
    networkDidReceiveMessage_callback receiveMessageCallback;
+ (APCallBackController *)sharedService;
- (id)init;
@end

@implementation APCallBackController
static APCallBackController *_sharedService = nil;

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
 *  callback notification.
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
 *  set Alias & tags
 *  call APService function.
 */
- (void)setTags:(NSSet *)tags
          alias:(NSString *)alias
       callback:(tags_alias_callback)tagsAliasCallback {
  //    __block  tags_alias_callback callback  = tagsAliasCallback;
  /*
   *   create callback method
   */
  IMP callBackIMP = imp_implementationWithBlock(
      ^(id _self, int iResCode, NSSet *tags, NSString *alias) {
          c_tags ctags = convert_to_c([[NSMutableSet alloc] initWithSet:tags]);
          tagsAliasCallback(iResCode, ctags, [alias UTF8String]);
      });
  NSString *callbackMethodName =
      [NSString stringWithFormat:@"callback%d:tags:alias:)", _idx++];
  SEL callback_function = sel_registerName([callbackMethodName UTF8String]);
  //    class_addMethod([self class], callback_function, callBackIMP, "v@i@@");

  class_addMethod([APCallBackController class], callback_function,
                  (IMP)callBackIMP, "v@:i@@");

  // call SDK API
  //    [APService setAlias:@"别名2"
  // callbackSelector:@selector(tagsAliasCallback:tags:alias:) object:self];
  [APService setTags:tags
                 alias:alias
      callbackSelector:callback_function
                target:self];
}

- (void)setTags:(NSSet *)tags callback:(tags_alias_callback)tagsAliasCallback {
  __block tags_alias_callback callback = tagsAliasCallback;
  /*
   *   create callback method
   */
  IMP callBackIMP = imp_implementationWithBlock(
      ^(int iResCode, NSSet *tags, NSString *alias) {
          callback(iResCode,
                   convert_to_c([[NSMutableSet alloc] initWithSet:tags]), 0);
      });
  NSString *callbackMethodName =
      [NSString stringWithFormat:@"callback%d:tags:alias:)", _idx++];
  class_addMethod([self class], NSSelectorFromString(callbackMethodName),
                  callBackIMP, "vi:@:@");

  // call SDK API
  [APService setTags:tags
      callbackSelector:NSSelectorFromString(callbackMethodName)
                object:self];
}

- (void)setAlias:(NSString *)alias
        callback:(tags_alias_callback)tagsAliasCallback {
  __block tags_alias_callback callback = tagsAliasCallback;
  /*
   *   create callback method
   */
  IMP callBackIMP = imp_implementationWithBlock(
      ^(int iResCode, NSSet *tags, NSString *alias) {
          //        set<string> null_result;
          //        callback(iResCode, null_result,[alias UTF8String]);
      });
  NSString *callbackMethodName =
      [NSString stringWithFormat:@"callback%d:tags:alias:)", _idx++];
  class_addMethod([self class], NSSelectorFromString(callbackMethodName),
                  callBackIMP, "vi:@:@");

  // call SDK API
  [APService setAlias:alias
      callbackSelector:NSSelectorFromString(callbackMethodName)
                object:self];
}

- (void)test {
}

#pragma - mark Callback Function
///**
// *  callback from APService.
// *  call C++ function.
// */
//- (void)Callback:(int)iResCode tags:(NSSet*)tags alias:(NSString*)alias {
//    self.tagsAliasCallback(iResCode,convert_to_c([[NSMutableSet
// alloc]initWithSet:tags]),[alias UTF8String]);
//}

/**
 *  callback function
 */
- (void)networkDidSetup:(NSNotification *)notification {
  if (_setupCallback == NULL) {
    return;
  }
  _setupCallback();
}

- (void)networkDidClose:(NSNotification *)notification {
  if (_closeCallback == NULL) {
    return;
  }
  _closeCallback();
}

- (void)networkDidRegister:(NSNotification *)notification {
  if (_registerCallback == NULL) {
    return;
  }
  NSString *registrationId =
      [[notification userInfo] objectForKey:@"RegistrationID"];
  _registerCallback([registrationId UTF8String]);
}

- (void)networkDidLogin:(NSNotification *)notification {
  if (_loginCallback == NULL) {
    return;
  }
  _loginCallback();
}

- (void)networkDidReceiveMessage:(NSNotification *)notification {
  if (_receiveMessageCallback == NULL) {
    return;
  }
  _receiveMessageCallback(convert_map_to_c([notification userInfo]));
}
@end

#pragma - mark C++ function
static APCallBackController *callbackController =
    [APCallBackController sharedService];

// 初始化
void APServiceCpp::setupWithOption(NSDictionary *launchingOption) {
  [APService setupWithOption:launchingOption];
  //    callbackController = [APCallBackController sharedService];
}

// 注册APNS类型
void APServiceCpp::registerForRemoteNotificationTypes(int types) {
  [APService registerForRemoteNotificationTypes:types];
}

// 注册SDK回调函数
void APServiceCpp::setCallbackFunction(
    networkDidSetup_callback setup_callback,
    networkDidClose_callback close_callback,
    networkDidRegister_callback register_callback,
    networkDidLogin_callback login_callback,
    networkDidReceiveMessage_callback message_callback) {
  callbackController.setupCallback = setup_callback;
  callbackController.closeCallback = close_callback;
  callbackController.registerCallback = register_callback;
  callbackController.loginCallback = login_callback;
  callbackController.receiveMessageCallback = message_callback;
}

// 向服务器上报Device Token
void APServiceCpp::registerDeviceToken(NSData *deviceToken) {
  [APService registerDeviceToken:deviceToken];
}

// 处理收到的APNS消息，向服务器上报收到
void APServiceCpp::handleRemoteNotification(NSDictionary *remoteInfo) {
  [APService handleRemoteNotification:remoteInfo];
}

/**
 *  set Tags & Alias.
 */
void APServiceCpp::setTags(c_tags tags, const char *alias,
                           tags_alias_callback callback) {
  if (callbackController) {
    //        callbackController.tagsAliasCallback = callback;
    [callbackController setTags:convert_to_oc(tags)
                          alias:[NSString stringWithUTF8String:alias]
                       callback:callback];
  }
}

void APServiceCpp::setTags(c_tags tags, tags_alias_callback callback) {
  if (callbackController) {
    [callbackController setTags:convert_to_oc(tags) callback:callback];
    //        callbackController.tagsAliasCallback = callback;
    //        [callbackController setTags:convert_to_oc(*tags)];
  }
}

void APServiceCpp::setTags(const char *alias, tags_alias_callback callback) {
  if (callbackController) {
    [callbackController setAlias:[NSString stringWithUTF8String:alias]
                        callback:callback];
    //        callbackController.tagsAliasCallback = callback;
    //        [callbackController setAlias:[NSString
    // stringWithUTF8String:alias]];
  }
}

c_tags APServiceCpp::filterValidTags(c_tags *tags) {
  NSSet *vaildSet = [APService filterValidTags:convert_to_oc(*tags)];
  return convert_to_c([[NSMutableSet alloc] initWithSet:vaildSet]);
}

/**
 *  Set Page Record
 */
void APServiceCpp::pageStart(const char *pageName) {
  [APService startLogPageView:[NSString stringWithUTF8String:pageName]];
}

void APServiceCpp::pageEnd(const char *pageName) {
  [APService stopLogPageView:[NSString stringWithUTF8String:pageName]];
}

void APServiceCpp::pageDefine(const char *pageName, int duration) {
  [APService beginLogPageView:[NSString stringWithUTF8String:pageName]
                     duration:duration];
}

/**
 *  get the UDID
 */
const char *APServiceCpp::openUDID() {
  return [[APService openUDID] UTF8String];
}

/**
 *  get RegistionID
 */
const char *APServiceCpp::registrionID() {
  return [[APService registrionID] UTF8String];
}
