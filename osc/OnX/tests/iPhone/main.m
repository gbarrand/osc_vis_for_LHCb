
#import <UIKit/UIKit.h>

@interface MyApp : UIApplication {
  UIProgressView* m_progress;
}
- (id)init;
- (void)dealloc;
- (void)create_ui;
@end

@implementation MyApp
- (id)init {
  m_progress = 0;
  if(self = [super init]) {
  }
  return self;
}
- (void)dealloc {[super dealloc];}
- (void)applicationDidFinishLaunching:(UIApplication *)application {
  [self create_ui];    
}
- (void)create_ui {
  CGRect aframe = [[UIScreen mainScreen] applicationFrame]; //0 20 320 460

  UIWindow* window = [[UIWindow alloc] initWithFrame:aframe];
  [window setBackgroundColor:[UIColor whiteColor]];
 
  float yw = aframe.size.height*0.4F;
  {    
  UIButton* widget = [UIButton buttonWithType:UIButtonTypeRoundedRect];
  float w = 100;  
  float h = 50;  
  CGRect rect = CGRectMake(aframe.size.width*0.5F-w*0.5F,yw,w,h);
  yw += 50;  
  [widget setFrame:rect];  
  [widget setTitle:@"iOnX_2" forState:UIControlStateNormal];  
  [widget addTarget:(id)self action:@selector(button_action:) forControlEvents:UIControlEventTouchDown];
  [window addSubview:widget];
  }
  
 {UIProgressView* widget = 
    [[UIProgressView alloc] 
     initWithProgressViewStyle:UIProgressViewStyleDefault];  
  float w = 100;  
  CGRect rect = CGRectMake(aframe.size.width*0.5F-w*0.5F,yw,w,50);
  [widget setFrame:rect];  
  [widget setProgress:0.75F];  
  m_progress = widget;
  [window addSubview:widget];
 }
   
  [window makeKeyAndVisible];
}
- (void)button_action:(id)aSender {
  //NSLog(@"debug : button_exit\n");
  //exit(0);
  system("pwd");
  system("ls");
  float value = [m_progress progress];
  value += 0.2F;
  if(value>1) value = 0;
  [m_progress setProgress:value];
}
@end

int main(int argc, char *argv[]) {
  //NSLog(@"debug : main : begin : 001\n");
  NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
  int retVal = UIApplicationMain(argc, argv, @"MyApp", nil);
  //NOTE : it seems that we never pass here.
  [pool release];
  return retVal;
}
