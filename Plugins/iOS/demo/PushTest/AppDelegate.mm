//
//  AppDelegate.m
//  PushTest
//
//  Created by LiDong on 12-8-15.
//  Copyright (c) 2012年 HXHG. All rights reserved.
//

#import "AppDelegate.h"
#import "APService.h"
#import "APServiceCpp.h"

void setupCallback();
void closeCallback();
void Register_callback(const char *registrationID);
void Login_callback();
void ReceiveMessage_callback(message message);

void tagsAliasCallback(int iResCode, c_tags tags, const char *alias);
void tagsAliasCallback2(int iResCode, c_tags tags, const char *alias);

@implementation AppDelegate {
  UIImageView *_imageView;
  UILabel *_pushLabel;
  NSMutableArray *_messageContents;
}

@synthesize window = _window;

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  // Override point for customization after application launch.
  self.window.backgroundColor = [UIColor blackColor];

  _infoLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 90, 320, 200)];
  [_infoLabel setBackgroundColor:[UIColor clearColor]];
  [_infoLabel
      setTextColor:[UIColor colorWithRed:0.5 green:0.65 blue:0.75 alpha:1]];
  [_infoLabel setFont:[UIFont boldSystemFontOfSize:10]];
  [_infoLabel setTextAlignment:UITextAlignmentCenter];
  [_infoLabel setNumberOfLines:20];
  [_infoLabel setText:@"未连接。。。"];
  [self.window addSubview:_infoLabel];
  NSLog(@"enabledRemoteNotificationTypes: %u",
        [[UIApplication sharedApplication] enabledRemoteNotificationTypes]);

  NSLog(@"中文日志");

  _tokenLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 300, 320, 60)];
  [_tokenLabel setBackgroundColor:[UIColor clearColor]];
  [_tokenLabel
      setTextColor:[UIColor colorWithRed:0.5 green:0.7 blue:0.75 alpha:1]];
  [_tokenLabel setFont:[UIFont systemFontOfSize:14]];
  [_tokenLabel setTextAlignment:UITextAlignmentCenter];
  [_tokenLabel setNumberOfLines:3];
  [self.window addSubview:_tokenLabel];

  _udidLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 380, 320, 24)];
  [_udidLabel setBackgroundColor:[UIColor clearColor]];
  [_udidLabel
      setTextColor:[UIColor colorWithRed:0.5 green:0.7 blue:0.75 alpha:1]];
  [_udidLabel setFont:[UIFont systemFontOfSize:18]];
  [_udidLabel setTextAlignment:UITextAlignmentCenter];
  [_udidLabel
      setText:[NSString stringWithFormat:@"UDID: %@", [APService openUDID]]];
  [self.window addSubview:_udidLabel];

  _pushLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 430, 320, 24)];
  [_pushLabel setBackgroundColor:[UIColor clearColor]];
  [_pushLabel
      setTextColor:[UIColor colorWithRed:0.5 green:0.7 blue:0.75 alpha:1]];
  [_pushLabel setFont:[UIFont systemFontOfSize:18]];
  [_pushLabel setTextAlignment:UITextAlignmentCenter];
  [_pushLabel setText:[NSString stringWithFormat:@"0000"]];
  [self.window addSubview:_pushLabel];

  _messageContents = [[NSMutableArray alloc] initWithCapacity:6];
  //
  //    NSNotificationCenter *defaultCenter = [NSNotificationCenter
  // defaultCenter];
  //
  //    [defaultCenter addObserver:self selector:@selector(networkDidSetup:)
  // name:kAPNetworkDidSetupNotification object:nil];
  //    [defaultCenter addObserver:self selector:@selector(networkDidClose:)
  // name:kAPNetworkDidCloseNotification object:nil];
  //    [defaultCenter addObserver:self selector:@selector(networkDidRegister:)
  // name:kAPNetworkDidRegisterNotification object:nil];
  //    [defaultCenter addObserver:self selector:@selector(networkDidLogin:)
  // name:kAPNetworkDidLoginNotification object:nil];
  //    [defaultCenter addObserver:self
  // selector:@selector(networkDidReceiveMessage:)
  // name:kAPNetworkDidReceiveMessageNotification object:nil];
  //    [defaultCenter addObserver:self selector:@selector(serviceError:)
  // name:kAPServiceErrorNotification object:nil];

  [self.window makeKeyAndVisible];

  //    [APService
  // registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge |
  //                                                   UIRemoteNotificationTypeSound
  // |
  //                                                   UIRemoteNotificationTypeAlert)];
  //    [APService setupWithOption:launchOptions];
  APServiceCpp::registerForRemoteNotificationTypes(
      (UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound |
       UIRemoteNotificationTypeAlert));
  APServiceCpp::setupWithOption(launchOptions);

  APServiceCpp::setCallbackFunction(setupCallback, closeCallback,
                                    Register_callback, Login_callback,
                                    ReceiveMessage_callback);

  //    APServiceCpp::setTags([@"别名1" UTF8String],tagsAliasCallback);

  c_tags tags1 = new set<string>;
  tags1->insert([@"tag1" UTF8String]);
  tags1->insert([@"tag2" UTF8String]);
  tags1->insert([@"tag3" UTF8String]);

  //    APServiceCpp::setTags(tags1, [@"别名1" UTF8String], tagsAliasCallback);
  APServiceCpp::setTags(tags1, [@"别名1" UTF8String], tagsAliasCallback);
  APServiceCpp::setTags(tags1, [@"别名2" UTF8String], tagsAliasCallback2);
//    [APService setTags:[NSSet setWithObjects:@"tag1", @"tag2",
// @"tag3",@"tag4",@"tag5",@"tag6",@"tag7",@"tag8",@"tag9",nil] alias:@"别名1"];
//    SEL sel = @selector(tagsAliasCallback:tags:alias:);
//    NSSet* tags = [NSSet setWithObjects:@"tag1", @"tag2",
// @"tag3",@"tag4",@"tag5",@"tag6",@"tag7 fd5^$^$",@"tag8",@"tag9",nil];
#if 0
      [APService setAlias:@"别名2" callbackSelector:sel object:self];
      [NSTimer scheduledTimerWithTimeInterval:10.f target:self selector:@selector(doTimmerSettagalias:) userInfo:nil repeats:NO];
#endif
  //    UIButton* testFlow = [[UIButton alloc]initWithFrame:CGRectMake(20, 20,
  // 100, 100)];
  //    [testFlow setBackgroundColor:[UIColor redColor]];
  //    [testFlow addTarget:self action:@selector(testFlow)
  // forControlEvents:UIControlEventTouchUpInside];
  //    [self.window addSubview:testFlow];

  return YES;
}

/**
 *  test flow view
 */
- (void)testFlow {
  test = [[testFlowViewController alloc] init];
  test.view.backgroundColor = [UIColor redColor];
  [self.window addSubview:test.view];
}

- (void)doTimmerSettagalias:(NSDictionary *)userInfo {
  //    static int seq = 90000;
  //    seq ++;
  //    NSString *alias = [NSString stringWithFormat:@"alias_seq_%d", seq];
  //    [APService setAlias:alias
  // callbackSelector:@selector(tagsAliasCallback:tags:alias:) object:self];
}

- (void)applicationWillResignActive:(UIApplication *)application {
  //    [APService stopLogPageView:@"aa"];
  // Sent when the application is about to move from active to inactive state.
  // This can occur for certain types of temporary interruptions (such as an
  // incoming phone call or SMS message) or when the user quits the application
  // and it begins the transition to the background state.
  // Use this method to pause ongoing tasks, disable timers, and throttle down
  // OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
  // Use this method to release shared resources, save user data, invalidate
  // timers, and store enough application state information to restore your
  // application to its current state in case it is terminated later.
  // If your application supports background execution, this method is called
  // instead of applicationWillTerminate: when the user quits.

  //[[UIApplication sharedApplication] setApplicationIconBadgeNumber:1];

  [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
  [application setApplicationIconBadgeNumber:0];
  [application cancelAllLocalNotifications];
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
  [NSTimer scheduledTimerWithTimeInterval:3.f
                                   target:self
                                 selector:@selector(doTimmerSettagalias:)
                                 userInfo:nil
                                  repeats:NO];

  NSLog(@"UDID____________: %@", [APService openUDID]);
  // Restart any tasks that were paused (or not yet started) while the
  // application was inactive. If the application was previously in the
  // background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
  // Called when the application is about to terminate. Save data if
  // appropriate. See also applicationDidEnterBackground:.
}

- (void)application:(UIApplication *)application
    didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
  [_tokenLabel
      setText:[NSString stringWithFormat:@"Device Token: %@", deviceToken]];
  //    [APService registerDeviceToken:deviceToken];
  APServiceCpp::registerDeviceToken(deviceToken);
}

- (void)application:(UIApplication *)application
    didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
  NSLog(@"did Fail To Register For Remote Notifications With Error: %@", error);
}

- (void)application:(UIApplication *)application
    didReceiveRemoteNotification:(NSDictionary *)userInfo {
  [APService handleRemoteNotification:userInfo];
}

- (void)application:(UIApplication *)application
    didReceiveRemoteNotification:(NSDictionary *)userInfo
          fetchCompletionHandler:
              (void (^)(UIBackgroundFetchResult))completionHandler {
  APServiceCpp::handleRemoteNotification(userInfo);
  //    [APService handleRemoteNotification:userInfo];

  //    [APService startLogPageView:@"settag7s"];
  //    sleep(3);
  //    [APService stopLogPageView:@"settag7s"];
  //
  //    NSLog(@"didReceiveRemoteNotification: %@", userInfo);
  static int i = 1;
  _pushLabel.text = [NSString stringWithFormat:@"%d", i++];

  completionHandler(UIBackgroundFetchResultNewData);
}
#pragma mark -

void setupCallback() { [AppDelegate setup]; }

void closeCallback() { [AppDelegate close]; }

void Register_callback(const char *registrationID) { [AppDelegate register]; }

void Login_callback() { [AppDelegate login]; }

void ReceiveMessage_callback(message message) { [AppDelegate receivieMessage]; }

+ (void)setup {
  [((AppDelegate *)[UIApplication sharedApplication].delegate)
      networkDidSetup:nil];
}

+ (void)close {
  [((AppDelegate *)[UIApplication sharedApplication].delegate)
      networkDidClose:nil];
}

+ (void) register {
  [((AppDelegate *)[UIApplication sharedApplication].delegate)
      networkDidRegister:nil];
}

+ (void)login {
  [((AppDelegate *)[UIApplication sharedApplication].delegate)
      networkDidLogin:nil];
}

+ (void)receivieMessage {
  [((AppDelegate *)[UIApplication sharedApplication].delegate)
      networkDidReceiveMessage:nil];
}

- (void)networkDidSetup:(NSNotification *)notification {
  [_infoLabel setText:@"已连接"];
}

- (void)networkDidClose:(NSNotification *)notification {
  [_infoLabel setText:@"未连接。。。"];
}

- (void)networkDidRegister:(NSNotification *)notification {
  [_infoLabel setText:@"已注册"];
}

- (void)networkDidLogin:(NSNotification *)notification {
  [_infoLabel setText:@"已登录"];
}

- (void)networkDidReceiveMessage:(NSNotification *)notification {
  NSDictionary *userInfo = [notification userInfo];
  NSString *title = [userInfo valueForKey:@"title"];
  NSString *content = [userInfo valueForKey:@"content"];
  NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];

  //
  //    [APService startLogPageView:@"settag5s"];
  //    sleep(3);
  //    [APService stopLogPageView:@"settag5s"];

  [dateFormatter setDateFormat:@"yyyy-MM-dd hh:mm:ss"];

  NSString *currentContent =
      [NSString stringWithFormat:@"\ntitle:%@\ncontent:%@", title, content];

  [_messageContents addObject:currentContent];

  while ([_messageContents count] > 6) {
    [_messageContents removeObjectAtIndex:0];
  }

  NSString *allContent = [NSString
      stringWithFormat:@"%@收到消息:\n%@",
                       [NSDateFormatter
                           localizedStringFromDate:[NSDate date]
                                         dateStyle:NSDateFormatterNoStyle
                                         timeStyle:NSDateFormatterMediumStyle],
                       [_messageContents componentsJoinedByString:nil]];

  [_infoLabel setText:allContent];
  //    APLog(@"networkDidReceiveMessage: %@", userInfo);
}

- (void)serviceError:(NSNotification *)notification {
  NSDictionary *userInfo = [notification userInfo];
  NSString *error = [userInfo valueForKey:@"error"];

  NSLog(@"%@", error);
}

void tagsAliasCallback(int iResCode, c_tags tags, const char *alias) {
  [(AppDelegate *)([UIApplication sharedApplication].delegate)
      tagsAliasCallback:iResCode
                   tags:nil
                  alias:[NSString stringWithCString:alias
                                           encoding:NSUTF8StringEncoding]];
}

- (void)tagsAliasCallback:(int)iResCode
                     tags:(NSSet *)tags
                    alias:(NSString *)alias {
  if (iResCode == 6002) {
    _window.backgroundColor = [UIColor colorWithRed:rand() % 255 / 255.f
                                              green:rand() % 255 / 255.f
                                               blue:rand() % 255 / 255.f
                                              alpha:1.f];
  }
  NSLog(@"rescode: %d, \ntags: %@, \nalias: %@\n", iResCode, tags, alias);
}

void tagsAliasCallback2(int iResCode, c_tags tags, const char *alias) {
  [(AppDelegate *)([UIApplication sharedApplication].delegate)
      tagsAliasCallback2:iResCode
                    tags:nil
                   alias:[NSString stringWithCString:alias
                                            encoding:NSUTF8StringEncoding]];
}

- (void)tagsAliasCallback2:(int)iResCode
                      tags:(NSSet *)tags
                     alias:(NSString *)alias {
  if (iResCode == 6002) {
    _window.backgroundColor = [UIColor colorWithRed:rand() % 255 / 255.f
                                              green:rand() % 255 / 255.f
                                               blue:rand() % 255 / 255.f
                                              alpha:1.f];
  }
  NSLog(@"rescode: %d, \ntags: %@, \nalias: %@\n", iResCode, tags, alias);
}

@end
