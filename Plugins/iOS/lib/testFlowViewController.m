//
//  testFlowViewController.m
//  PushTest
//
//  Created by 张 on 14-4-8.
//
//

#import "testFlowViewController.h"
#import "AppDelegate.h"
#import "APService.h"
#import "testFlowTwoViewController.h"

@interface testFlowViewController ()

@end

@implementation testFlowViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
        UIButton* close = [[UIButton alloc]initWithFrame:CGRectMake(0, 0, 330, 100)];
        close.backgroundColor = [UIColor blackColor];
        [close addTarget:self action:@selector(removeFlow) forControlEvents:UIControlEventTouchUpInside];
        [close setTitle:@"主界面" forState:UIControlStateNormal];

        [self.view addSubview:close];
        
        UIButton* new = [[UIButton alloc]initWithFrame:CGRectMake(0, 400, 330, 100)];
        new.backgroundColor = [UIColor blackColor];
        [new setTitle:@"跳转下一个addveiw" forState:UIControlStateNormal];
        [new addTarget:self action:@selector(new) forControlEvents:UIControlEventTouchUpInside];
        [self.view addSubview:new];
        
       
    }
    return self;
}

- (void)new{
    testFlowTwoViewController* test = [[testFlowTwoViewController alloc]init];
    [self.view addSubview:test.view];
}

- (void)removeFlow{
    [self.view removeFromSuperview];
}

- (void)viewWillAppear:(BOOL)animated{
    NSLog(@"viewwillAppear");
    [APService startLogPageView:@"页面page1"];
    // 获取系统当前时间
    NSDate * date = [NSDate date];
    NSTimeInterval sec = [date timeIntervalSinceNow];
    NSDate * currentDate = [[NSDate alloc] initWithTimeIntervalSinceNow:sec];
    
    //设置时间输出格式：
    NSDateFormatter * df = [[NSDateFormatter alloc] init ];
    [df setDateFormat:@"yyyy年MM月dd日 HH小时mm分ss秒"];
    NSString * a = [df stringFromDate:currentDate];
    
    NSLog(@"page1star当前时间为：%@",a);
    
}

- (void)viewWillDisappear:(BOOL)animated{
    NSLog(@"viewwillDisAppear");
    [APService stopLogPageView:@"页面page1"];
    // 获取系统当前时间
    NSDate * date = [NSDate date];
    NSTimeInterval sec = [date timeIntervalSinceNow];
    NSDate * currentDate = [[NSDate alloc] initWithTimeIntervalSinceNow:sec];
    
    //设置时间输出格式：
    NSDateFormatter * df = [[NSDateFormatter alloc] init ];
    [df setDateFormat:@"yyyy年MM月dd日 HH小时mm分ss秒"];
    NSString * b = [df stringFromDate:currentDate];
    
    NSLog(@"page1stop当前时间为：%@",b);

}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
