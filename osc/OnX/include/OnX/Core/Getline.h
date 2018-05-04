/* 
 * G.Barrand : copied from the pain.
 *             remove R__EXTERN and #ifndef __CINT__
 */

#ifndef ONX_GETLINE_
#define ONX_GETLINE_

/* 
 * namespace protect to avoid clash with the pain :
 */
#define Getline OnX_Term_Getline
#define Getlinem OnX_Term_Getlinem
#define Gl_config OnX_Term_Gl_config
#define Gl_eof OnX_Term_Gl_eof
#define Gl_histadd OnX_Term_Gl_histadd
#define Gl_histinit OnX_Term_Gl_histinit
#define Gl_in_hook OnX_Term_Gl_in_hook
#define Gl_in_key OnX_Term_Gl_in_key
#define Gl_out_hook OnX_Term_Gl_out_hook
#define Gl_setwidth OnX_Term_Gl_setwidth
#define Gl_tab_hook OnX_Term_Gl_tab_hook
#define Gl_windowchanged OnX_Term_Gl_windowchanged
#define Gl_erase_line OnX_Term_Gl_erase_line /*G.Barrand*/

/*G.Barrand*/
#define Gl_is_there_stdin_input OnX_Gl_is_there_stdin_input

#ifdef __cplusplus
extern "C" {
#endif

/*typedef enum { kInit = -1, kLine1, kOneChar, kCleanUp } EGetLineMode;*/

char *Getline(const char *prompt);
char *Getlinem(int mode, const char *prompt);
void Gl_config(const char *which, int value);
void Gl_setwidth(int width);
void Gl_windowchanged();
void Gl_histinit(char *file);
void Gl_histadd(char *buf);
int  Gl_eof();

int Gl_is_there_stdin_input(int*); /*G.Barrand*/
void Gl_erase_line(); /*G.Barrand*/

extern int (*Gl_in_hook)(char *buf);
extern int (*Gl_out_hook)(char *buf);
extern int (*Gl_tab_hook)(char *buf, int prompt_width, int *cursor_loc);
extern int (*Gl_in_key)(int key);

#ifdef __cplusplus
}
#endif

#endif   /* ONX_GETLINE_ */
