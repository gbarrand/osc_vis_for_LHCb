// this :
#include <OnX/NextStep/NSCyclic.h>

#include <Slash/Core/ISession.h>

#import <Foundation/NSTimer.h>

@interface OnX_NSTimer : NSTimer {
  Slash::UI::ICyclic* fCyclic; 
  NSTimer* fTimer;
}
- (id)initWithCyclic:(Slash::UI::ICyclic*)aCyclic delay:(double)aDelay;
- (void)dealloc;
- (void)stop;
- (void)performAnimation:(NSTimer*)aTimer;
@end

@implementation OnX_NSTimer
- (id)initWithCyclic:(Slash::UI::ICyclic*)aCyclic delay:(double)aDelay { 
  fCyclic = 0;
  fTimer = 0;
  if(self = [super init]) {
    fCyclic = aCyclic;
    double seconds = aDelay/1000;
    fTimer = [NSTimer scheduledTimerWithTimeInterval:seconds target:self selector:@selector(performAnimation:) userInfo:nil repeats:YES];
  }
  return self;
}
- (void)dealloc {  
  [fTimer invalidate];
  [super dealloc];
}
- (void)stop {  
  [fTimer invalidate];
}
- (void)performAnimation:(NSTimer*)aTimer {
  fCyclic->execute();
}
@end

OnX::NSCyclic::NSCyclic(
 Slash::Core::ISession& aSession
,const std::string& aName
,int aDelay
,const std::string& aInterpreter
,const std::string& aScript
)
:BaseCyclic(aSession,aName,aDelay,aInterpreter,aScript)
,fTimer(0)
{}

OnX::NSCyclic::~NSCyclic(){stop();}

void OnX::NSCyclic::stop() {
  if(!fTimer) return;
  [fTimer stop];
  [fTimer release];
  fTimer = 0;
}

void OnX::NSCyclic::start(){
  stop();
  if(fDelay<=0) return;
  fTimer = [[OnX_NSTimer alloc] initWithCyclic:this delay:(double)fDelay];
}

bool OnX::NSCyclic::started() const {
  return fTimer?true:false;
}
