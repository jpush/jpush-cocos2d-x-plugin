//
//  AppDelegate.h
//  PushTest
//
//  Created by LiDong on 12-8-15.
//  Copyright (c) 2012年 HXHG. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "testFlowViewController.h"

@interface AppDelegate : NSObject<UIApplicationDelegate> {
  UILabel *_infoLabel;
  UILabel *_tokenLabel;
  UILabel *_udidLabel;
  testFlowViewController *test;
}

@property(retain, nonatomic) UIWindow *window;

@end
