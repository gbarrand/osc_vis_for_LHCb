/*
  GB : To control kuip output. 
  GB : It is used when GUI is not a terminal.
*/

#include <stdio.h>

#include <kuip/kout.h>

static koutFunc sPrintV = NULL;
static void* sTag = NULL;

/***************************************************************************/
void k_set_out_func(
 koutFunc aProc
,void* aTag
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  sPrintV = aProc;
  sTag = aTag;
}
/***************************************************************************/
void kprintf(
 char* aFormat
,...
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  va_list args;
  if(aFormat==NULL) return;
  va_start(args,aFormat);
  if(sPrintV!=NULL) 
    sPrintV(sTag,aFormat,args);
  else              
    vprintf(aFormat,args);
  va_end(args);
}
