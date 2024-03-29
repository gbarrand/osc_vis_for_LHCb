/*----------------------------------------------------------------------
 * (C) 1999 Spacetec IMC Corporation ("Spacetec").
 *
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby granted provided that this copyright
 * notice appears in all copies. Permission to modify this software is granted
 * and Spacetec will support such modifications only if said modifications are
 * approved by Spacetec.
 *
 */

#ifndef COIN_SPWINPUT_H
#define COIN_SPWINPUT_H

#define SPW_INPUT_MAJOR    7
#define SPW_INPUT_MINOR    6
#define SPW_INPUT_UPDATE   0
#define SPW_INPUT_BUILD    1
#define SPW_INPUT_VERSION  "7.6.0.1"
#define SPW_INPUT_DATE     _T("January 18, 1999")

#define SPW_MAXBUF 80

typedef enum {
  SPW_InputMotionEvent=1, /* Event type is a motion event */
  SPW_InputButtonPressEvent=2, /* Event type is a button press event */
  SPW_InputButtonReleaseEvent=3 /* Event type is a button release event */
} SPW_InputEventType;

typedef struct {
  unsigned int old; /* The last state of the buttons */
  unsigned int current; /* The current state of the buttons */
  unsigned int pressed; /* Which buttons were pressed */
  unsigned int released; /* Which buttons were released */
} SPW_InputButtonTable;

typedef struct {
  short int type; /* The type of event */
  float fData[7]; /* The float Spaceball data */
  short int sData[7]; /* The integer Spaceball data */
  int buttonNumber; /* The button number for this event */
  SPW_InputButtonTable buttonState; /* The state of the buttons */
  char string[SPW_MAXBUF]; /* Extra data area */
} SPW_InputEvent;

/* These two methods should only be used under X */ 
int SPW_CheckForSpaceballX11(void * display, int winid, char * product);
int SPW_TranslateEventX11(void * display, void * xevent, SPW_InputEvent * sbEvent);
/* FIXME: add two similar methods for WIN32. pederb, 20001114 */

#endif /* COIN_SPWINPUT_H */
