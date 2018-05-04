
/* trash 
  //NSScreen* screen = [NSScreen mainScreen];
  //printf("debug : mainScreen %lu\n",screen);

  rect.origin.x = 0;
  rect.origin.y = 0;
  rect.size.width = device_width;
  rect.size.height = device_height;

  //NSRect rect = [screen visibleFrame];

 {NSArray* scrs = [NSScreen screens];
  int number = [scrs count];
  int index;
  for(index=0;index<number;index++) {
    NSScreen* screen = (NSScreen*)[scrs objectAtIndex:index];
    NSRect rect = [screen frame];
    printf("debug : screen %g %g %g %g\n",
	   rect.origin.x,
	   rect.origin.y,
	   rect.size.width,
	   rect.size.height);
  }}

  //#ifdef FULL_SCREEN 
  CGDirectDisplayID  display_id = kCGDirectMainDisplay;
  printf("debug : %d\n",display_id);

  int monitor = 1;
  CGDirectDisplayID activeDspys[3];
  CGDisplayCount dspyCnt;
  CGGetActiveDisplayList(3,activeDspys,&dspyCnt);
  printf("debug : displays : %d\n",dspyCnt);
  if( monitor >= 0 && monitor < dspyCnt ) {
    display_id = activeDspys[monitor];
    printf("debug : 000 : %d\n",display_id);
  }

  CFDictionaryRef save_mode = CGDisplayCurrentMode(display_id);

  unsigned int device_width,device_height,device_bpp;
  CFNumberGetValue(CFDictionaryGetValue(save_mode,kCGDisplayBitsPerPixel),
                   kCFNumberSInt32Type,&device_bpp);
  CFNumberGetValue(CFDictionaryGetValue(save_mode,kCGDisplayWidth),
                   kCFNumberSInt32Type,&device_width);
  CFNumberGetValue(CFDictionaryGetValue(save_mode,kCGDisplayHeight),
                   kCFNumberSInt32Type,&device_height);
  printf("debug : %d %d %d\n",device_width,device_height,device_bpp);
  //current->pixels = (Uint32*) CGDisplayBaseAddress (display_id);
  //current->pitch  = CGDisplayBytesPerRow (display_id);


  //CGDisplayCapture(display_id);
  //CGCaptureAllDisplays ();

  boolean_t exact_match = 0;
  CFDictionaryRef mode = 
    CGDisplayBestModeForParameters(display_id,device_bpp,device_width,
                                   device_height,&exact_match);
  //if(CGDisplaySwitchToMode(display_id,mode)!=CGDisplayNoErr) {
  //  exit(0);
  //}
  //ShowMenuBar ();
  //HideMenuBar ();

  //exit(0);
  //CGDisplayCanSetPalette(display_id);
//#endif

#ifdef FULL_SCREEN 
  //CGDisplayFadeReservationToken fade_token = kCGDisplayFadeReservationInvalidToken;
  //if(CGAcquireDisplayFadeReservation (5, &fade_token) == kCGErrorSuccess) {
  //  CGDisplayFade (fade_token, 0.3, kCGDisplayBlendNormal, kCGDisplayBlendSolidColor, 0.0, 0.0, 0.0, TRUE);
  //}
  //CGDisplayFade (fade_token, 0.5, kCGDisplayBlendSolidColor, kCGDisplayBlendNormal, 0.0, 0.0, 0.0, FALSE);
  //CGReleaseDisplayFadeReservation (fade_token);

  //QZ_TearDownOpenGL (this);
  //CGLSetFullScreen (NULL);

  //CGDisplaySwitchToMode(display_id, save_mode);
  //CGReleaseAllDisplays();

#endif

- (BOOL)enterFullScreenMode:(NSScreen*)screen withOptions:(NSDictionary*)options {
  printf("debug : enter full screen\n");
}
- (void)exitFullScreenModeWithOptions:(NSDictionary*)options {
  printf("debug : exit full screen\n");
}

*/
