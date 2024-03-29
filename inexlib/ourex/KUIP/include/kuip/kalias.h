/*
 * $Id: kalias.h,v 1.2 2003/11/04 08:00:39 barrand Exp $
 *
 * $Log: kalias.h,v $
 * Revision 1.2  2003/11/04 08:00:39  barrand
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/05/25 13:17:42  barrand
 * KUIP
 *
 * Revision 1.1.1.1  1996/03/08 15:32:59  mclareni
 * Kuip
 *
 */
#define ALIAS_TABLE_SIZE 97     /* should be a prime */

typedef struct {
  int         translate;        /* flag if translation wanted */
  int         substitutions;    /* how many more before recursive alarm */
  HashTable  *arg_table;        /* Argument alias table */
  HashTable  *cmd_table;        /* Command alias table */
  HashTable  *global_value;     /* global variable values */
  HashTable  *global_text;      /* global variable explanation text */
  HashTable  *var_table;        /* macro variables */
} KcAlias;
EXTERN KcAlias kc_alias;

extern int   match_paren( const char* );
extern char* repl_variable(   char*);
extern char* repl_sysfun(     char*, int);
extern char* subst_arg_alias( char*);
extern char* subst_cmd_alias( char*);
extern char* subst_var_alias( char*);
extern char* subst_sysfun(    char*, int);
extern char* var_value( const char* );


