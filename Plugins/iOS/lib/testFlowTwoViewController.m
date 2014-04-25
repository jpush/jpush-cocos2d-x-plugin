//
//  testFlowTwoViewController.m
//  170-0410-pm1
//
//  Created by JPush-lingz on 4/10/14.
//
//

#import "testFlowTwoViewController.h"
#import "AppDelegate.h"
#import "APService.h"

@interface testFlowTwoViewController ()

@end

@implementation testFlowTwoViewController

- (id)init
{
    self = [super init];
    if (self) {
        UIButton* close = [[UIButton alloc]initWithFrame:CGRectMake(0, 400, 330, 100)];
        close.backgroundColor = [UIColor blackColor];
        [close addTarget:self action:@selector(removeFlow) forControlEvents:UIControlEventTouchUpInside];
        [self.view addSubview:close];
        // Custom initialization
        [close setTitle:@"返回第一个addveiw" forState:UIControlStateNormal];

        self.view.backgroundColor = [UIColor whiteColor];
        
    }
    return self;
}

- (void)removeFlow{
    [self.view removeFromSuperview];
}


- (void)viewWillAppear:(BOOL)animated{
    NSLog(@"viewwillAppear");
    [APService startLogPageView:@"page2"];
    // 获取系统当前时间
    NSDate * date = [NSDate date];
    NSTimeInterval sec = [date timeIntervalSinceNow];
    NSDate * currentDate = [[NSDate alloc] initWithTimeIntervalSinceNow:sec];
    
    //设置时间输出格式：
    NSDateFormatter * df = [[NSDateFormatter alloc] init ];
    [df setDateFormat:@"yyyy年MM月dd日 HH小时mm分ss秒"];
    NSString * a = [df stringFromDate:currentDate];
    
    NSLog(@"page2star当前时间为：%@",a);
    
}

- (void)viewWillDisappear:(BOOL)animated{
    NSLog(@"viewwillDisAppear");
    [APService stopLogPageView:@"page2"];
    // 获取系统当前时间
    NSDate * date = [NSDate date];
    NSTimeInterval sec = [date timeIntervalSinceNow];
    NSDate * currentDate = [[NSDate alloc] initWithTimeIntervalSinceNow:sec];
    
    //设置时间输出格式：
    NSDateFormatter * df = [[NSDateFormatter alloc] init ];
    [df setDateFormat:@"yyyy年MM月dd日 HH小时mm分ss秒"];
    NSString * b = [df stringFromDate:currentDate];
    
    NSLog(@"page2stop当前时间为：%@",b);

    
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

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
