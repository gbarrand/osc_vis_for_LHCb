
#import <WiiRemote/WiiRemote.h>
#import <WiiRemote/WiiRemoteDiscovery.h>

@interface MyWiiRemote : NSObject {
  NSApplication* m_app;
  WiiRemoteDiscovery* m_discovery;
  WiiRemote* m_wiimote;
}
- (id)initWithApp:(NSApplication*)a_app;
- (void)dealloc;
//- (WiiRemote*)wiimote;
- (WiiRemoteDiscovery*)discovery;
- (void)close;
- (void)expansionPortChanged:(NSNotification*)notification;
//WiiRemoteDiscoveryDelegate
- (void)WiiRemoteDiscovered:(WiiRemote*)a_wiimote;
- (void)WiiRemoteDiscoveryError:(int)a_code;
- (void)willStartWiimoteConnections;
//WiiRemoteDelegate
- (void)wiiRemoteDisconnected:(IOBluetoothDevice*)device;
//events 
- (void)buttonChanged:(WiiButtonType)a_type isPressed:(BOOL)a_isPressed;
- (void)irPointMovedX:(float)px Y:(float)py;
- (void)rawIRData:(IRData[4])irData;
- (void)accelerationChanged:(WiiAccelerationSensorType)type accX:(unsigned char)accX accY:(unsigned char)accY accZ:(unsigned char)accZ;
- (void)joyStickChanged:(WiiJoyStickType)type tiltX:(unsigned char)tiltX tiltY:(unsigned char)tiltY;
- (void)analogButtonChanged:(WiiButtonType)type amount:(unsigned)press;
@end
@implementation MyWiiRemote
- (id)initWithApp:(NSApplication*)a_app {
  m_app = a_app;
  m_discovery = 0;
  m_wiimote = 0;
  if(self = [super init]) {
    printf("debug : MyWiiRemote : init\n");
    [[NSNotificationCenter defaultCenter] 
       addObserver:self
       selector:@selector(expansionPortChanged:)
       name:@"WiiRemoteExpansionPortChangedNotification"
       object:nil];
    m_discovery = [[WiiRemoteDiscovery alloc] init];
    [m_discovery setDelegate:self];
  }
  return self;
}
- (void)dealloc{
  [m_wiimote release];
  [m_discovery release];
  [super dealloc];
}
//- (WiiRemote*)wiimote {return m_wiimote;}
- (WiiRemoteDiscovery*)discovery {return m_discovery;}
- (void)close {
  printf("debut : will terminate...\n");
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  [m_wiimote closeConnection];
}

- (void)expansionPortChanged:(NSNotification*)notification{
  WiiRemote* wiimote = (WiiRemote*)[notification object];
  if(wiimote!=m_wiimote) return;
  if([m_wiimote isExpansionPortAttached]){
    [m_wiimote setExpansionPortEnabled:YES];
    printf("Nunchuk enabled\n");
  } else {
    [m_wiimote setExpansionPortEnabled:NO];
    printf("Nunchuk disabled\n");
  }
}	

//WiiRemoteDiscoveryDelegate
- (void)WiiRemoteDiscovered:(WiiRemote*)a_wiimote {
  printf("WiiRemoteDiscovered : 003\n");
  // the wiimote must be retained because the discovery provides
  // us with an autoreleased object
  m_wiimote = [a_wiimote retain];
  [a_wiimote setDelegate:self];
	
  [a_wiimote setLEDEnabled1:YES enabled2:NO enabled3:YES enabled4:NO];

  [a_wiimote setMotionSensorEnabled:YES];
//[a_wiimote setIRSensorEnabled:YES];
}

- (void)WiiRemoteDiscoveryError:(int)a_code {
  printf("===== WiiRemoteDiscovery error.  try System Preferences > Bluetooth > Devices, delete Nintendo. (%d) =====\n",a_code);
}

- (void)willStartWiimoteConnections {
  printf("===== WiiRemote discovered.  Opening connection. =====\n");
}

//WiiRemoteDelegate
- (void)wiiRemoteDisconnected:(IOBluetoothDevice*)device {
  [m_wiimote release];
  m_wiimote = nil;
  printf("===== lost connection with WiiRemote =====\n");
}
//events :
- (void)buttonChanged:(WiiButtonType)a_type isPressed:(BOOL)a_isPressed {
  printf("debug : button %d pressed %d\n",a_type,a_isPressed);

  if(a_type == WiiRemoteAButton){
  } else if(a_type == WiiRemoteBButton){
  } else if(a_type == WiiRemoteUpButton){
  } else if(a_type == WiiRemoteDownButton){
  } else if(a_type == WiiRemoteLeftButton){
  } else if(a_type == WiiRemoteRightButton){
  } else if(a_type == WiiRemoteMinusButton){
  } else if(a_type == WiiRemotePlusButton){
  } else if(a_type == WiiRemoteHomeButton){
    [m_app terminate:nil];
  } else if(a_type == WiiRemoteOneButton){
  } else if(a_type == WiiRemoteTwoButton){
  } else if(a_type == WiiNunchukCButton){
  } else if(a_type == WiiNunchukZButton){
  }
}
- (void)irPointMovedX:(float)px Y:(float)py {
  printf("debug : irPointMovedX : dummy\n");
}
- (void)rawIRData: (IRData[4])irData {
  printf("debug : rawIRData : dummy\n");
}
- (void)accelerationChanged:(WiiAccelerationSensorType)type accX:(unsigned char)accX accY:(unsigned char)accY accZ:(unsigned char)accZ {
  //return;
  if(type==WiiRemoteAccelerationSensor) {
    //printf("debug : accelerationChanged : WiiRemoteAccelerationSensor %d %d %d\n",accX,accY,accZ);
  } else if(type==WiiNunchukAccelerationSensor){
    //printf("debug : accelerationChanged : WiiNunchukAccelerationSensor %d %d %d\n",accX,accY,accZ);
  }
}
- (void)joyStickChanged:(WiiJoyStickType)type tiltX:(unsigned char)tiltX tiltY:(unsigned char)tiltY {
  if(type==WiiNunchukJoyStick) {
    //printf("debug : joyStickChanged : WiiNunchukJoyStick %d %d\n",tiltX,tiltY);
  } else if(type==WiiClassicControllerLeftJoyStick) {
    printf("debug : joyStickChanged : WiiClassicControllerLeftJoyStick\n");
  } else if(type==WiiClassicControllerRightJoyStick) {
    printf("debug : joyStickChanged : WiiClassicControllerRightJoyStick\n");
  }
}
- (void)analogButtonChanged:(WiiButtonType)type amount:(unsigned)press {
  printf("debug : analogButtonChanged : dummy\n");
}
@end

