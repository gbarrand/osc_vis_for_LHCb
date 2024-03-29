/*
 * $Id: kienbr.c,v 1.3 1999/11/08 11:33:19 barrand Exp $
 *
 * $Log: kienbr.c,v $
 * Revision 1.3  1999/11/08 11:33:19  barrand
 * *** empty log message ***
 *
 * Revision 1.2  1999/06/01  06:56:55  barrand
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/05/25  13:09:14  barrand
 * KUIP
 *
 * Revision 1.1.1.1  1996/03/08 15:32:57  mclareni
 * Kuip
 *
 */
/*CMZ :  2.05/17 19/09/94  14.17.23  by  Alfred Nathaniel*/
/*-- Author :    Alfred Nathaniel   19/05/92*/

#include "kuip/kuip.h"
#include "kuip/kfor.h"
#include "kuip/kcom.h"
#include "kuip/ksig.h"
#include "kuip/klink.h"
#include "kuip/kmenu.h"
#include "kuip/kflag.h"


static void keyboard_interrupt(void);


#ifndef vms

/*
 * catch exceptions and longjmp back to command input loop
 */
void signal_handler( int sig )
{
  int do_traceback = 1;

#if defined(SIGNAL_V7)
  if( sig != 0 )
    signal( sig, signal_handler ); /* has been set to SIG_DFL */
#endif

  if( sig == SIGINT && !kc_break.intr_enabled ) {
    kc_break.intr_pending = 1;
    keyboard_interrupt();
    return;
  }

#if defined(USE_EDIT_SERVER)
  if( sig == SIGUSR1 ) {        /* from edit server */
    kc_flags.editor_exit = 1;
    return;
  }
#endif

#ifdef SIGUSR2
  if( sig == SIGUSR2 ) {        /* from kxterm for a soft interrupt */
    kc_break.soft_intr = 1;
    if( kc_break.piaf_sync != NULL ) {
      /* tell the Piaf server about the interrupt */
      (*kc_break.piaf_sync)( kc_break.sockfd, "\4" );
    }
    return;
  }
#endif

  /*GB Kibres();*/                     /* cleanup Fortran I/O */

  ku_alfa();
  kprintf( "\n *** Break *** " );

  switch( sig ) {
  case 0:
    kprintf( "Simulated break" );
    do_traceback = kc_break.traceback;
    break;
  case SIGINT:
    kprintf( "Keyboard interrupt" );
    keyboard_interrupt();
    do_traceback = kc_break.traceback;
    break;
  case SIGFPE:
    kprintf( "Floating point exception" );
    break;
  case SIGILL:
    kprintf( "Illegal instruction" );
    break;
  case SIGSEGV:
    kprintf( "Segmentation violation" );
    break;
#ifdef SIGBUS
  case SIGBUS:
    kprintf( "Bus error" );
    break;
#endif
  default:
    kprintf( "Unknown signal %d\n", sig );
  }

  if( kc_flags.use_kxterm ) {
    /*
     * Send STX (start-transmission) character to tell kxterm we are
     * ready for a next command.
     */
    kprintf("\2");
  }

#ifdef HPUX
  if( do_traceback ) {
    /*GB extern void U_STACK_TRACE();*/ /* somewhere in Fortran RTL */
    /*GB U_STACK_TRACE(); */
    do_traceback = 0;
  }
#endif

  if( do_traceback ) {
    INTEGER lun = 0;
    INTEGER level = 99;
    /*GB Traceq( &lun, &level );*/     /* KERNLIB traceback */
  }

  if( kc_break.jump_set ) {
    siglongjmp( kc_break.stack, 1 );
  }
  kprintf( "Signal %d caught without longjmp target\n", sig );
  exit( 1 );
}

#else

int signal_handler( const void *sigarr, const void *mecharr )
{
  const int *sigvec = sigarr;
  int cond = $VMS_STATUS_COND_ID( sigvec[1] );
  int do_traceback = 1;

  /*GB Kibres();*/                     /* cleanup Fortran I/O */

  ku_alfa();
  if( !kc_break.trap_enabled ) {
    return SS$_RESIGNAL;
  }
  if( cond == $VMS_STATUS_COND_ID( SS$_UNWIND ) ) {
    return SS$_RESIGNAL;
  }
  if( cond == $VMS_STATUS_COND_ID( SS$_CONTROLC )
     || cond == $VMS_STATUS_COND_ID( SS$_DEBUG ) ) {
    if( kc_break.intr_enabled ) {
      keyboard_interrupt();
    }
    else {
      kc_break.intr_pending = 1;
      return SS$_CONTINUE;
    }
    if( cond == $VMS_STATUS_COND_ID( SS$_DEBUG ) )
      do_traceback = 0;
    else
      do_traceback = kc_break.traceback;
  }

  if( do_traceback ) {
    lib$signal( 0 );
  }
  else if( sigvec[1] == 0 ) {
    kprintf( "\n *** Break *** Simulated break\n" );
  }
  else if( cond == $VMS_STATUS_COND_ID( SS$_CONTROLC ) ) {
    kprintf( "\n *** Break *** Keyboard interrupt\n" );
  }
  else {
    sys$putmsg( sigarr );
  }

  if( kc_flags.use_kxterm ) {
    /*
     * Send STX (start-transmission) character to tell kxterm we are
     * ready for a next command.
     */
    kprintf("\2");
    fflush( stdout );           /* otherwise VMS wouldn't see it */
  }
  kprintf( "\n" );
  lib$sig_to_ret( sigarr, mecharr );
  return SS$_CONTINUE;
}


int control_C_ast( int which )
{
  static int installed = 0;
  int was_installed = installed;
  int istat;

  if( kc_flags.use_kxterm ) {
    /*
     * We cannot simply send a signal from kxterm to the main process.
     * Instead we create a lock with a blocking AST.
     * Upon ^C kxterm requesting the same lock triggers a call to the AST
     * from where we can signal the ^C.
     */
    static int sigint_lock[2];
    static int kxterm_lock[2];

    if( sigint_lock[1] == 0 ) {        /* create lock */
      char lock_name[32];
      struct dsc$descriptor_s lock;

      sprintf( lock_name, "SIGINT%d", getpid() );
      var_descriptor( lock, lock_name );
      istat = sys$enqw( 0, LCK$K_NLMODE, sigint_lock, 0,
                       &lock, 0, NULL, SIGINT, control_C_ast, PSL$C_USER, 0 );
      if( istat != SS$_NORMAL )
        lib$signal( istat );

      sprintf( lock_name, "KXTERM%d", getpid() );
      var_descriptor( lock, lock_name );
      istat = sys$enqw( 0, LCK$K_NLMODE, kxterm_lock, 0,
                       &lock, 0, NULL, 0, NULL, PSL$C_USER, 0 );
      if( istat != SS$_NORMAL )
        lib$signal( istat );
    }

    switch( which ) {

    case 0:                     /* release lock */
      istat = sys$enqw( 0, LCK$K_NLMODE, sigint_lock, LCK$M_CONVERT,
                       NULL, 0, NULL, SIGINT, control_C_ast, PSL$C_USER, 0 );
      if( istat != SS$_NORMAL )
        lib$signal( istat );
      break;

    case 1:                     /* grab lock */
      istat = sys$enqw( 0, LCK$K_EXMODE, sigint_lock, LCK$M_CONVERT,
                       NULL, 0, NULL, SIGINT, control_C_ast, PSL$C_USER, 0 );
      if( istat != SS$_NORMAL )
        lib$signal( istat );
      break;

    case SIGINT:
      /*
       * release lock that kxterm can get it
       * then grab it again and signal ^C
       */
      istat = sys$enqw( 0, LCK$K_NLMODE, sigint_lock, LCK$M_CONVERT,
                       NULL, 0, NULL, SIGINT, control_C_ast, PSL$C_USER, 0 );
      if( istat != SS$_NORMAL )
        lib$signal( istat );

      /*
       * If we were to convert the SIGINT lock back to exclusive access
       * immediately the kxterm process might not get it granted first.
       * In VMS 6.0 the is a new flag LCK$M_QUECVT to queue the lock
       * conversion after any other waiting request.
       * To work on earlier VMS versions we need a second lock to synchronize
       * the access with kxterm.
       */
      istat = sys$enqw( 0, LCK$K_EXMODE, kxterm_lock, LCK$M_CONVERT,
                       NULL, 0, NULL, 0, NULL, PSL$C_USER, 0 );
      if( istat != SS$_NORMAL )
        lib$signal( istat );

      istat = sys$enqw( 0, LCK$K_NLMODE, kxterm_lock, LCK$M_CONVERT,
                       NULL, 0, NULL, 0, NULL, PSL$C_USER, 0 );
      if( istat != SS$_NORMAL )
        lib$signal( istat );

      istat = sys$enqw( 0, LCK$K_EXMODE, sigint_lock, LCK$M_CONVERT,
                       NULL, 0, NULL, SIGINT, control_C_ast, PSL$C_USER, 0 );
      if( istat != SS$_NORMAL )
        lib$signal( istat );

      lib$signal( SS$_CONTROLC );
      break;
    }

    installed = which;
  }

  else if( kc_window.is_a_tty ) {

    static short ttchan = -1;

    if( ttchan == -1 ) {        /* get a channel to the terminal device */
      struct dsc$descriptor_s ttname;
      var_descriptor( ttname, "TT:" );

      istat = sys$assign( &ttname, &ttchan, 0, 0 );
      if( istat != SS$_NORMAL )
        lib$signal( istat );
    }

    switch( which ) {

    case 0:                     /* deinstall ASTs */
      if( installed ) {
        istat = sys$cancel( ttchan );
        if( istat != SS$_NORMAL )
          lib$signal( istat );
        break;
      }

    case 1:                     /* install ASTs */
      if( !installed ) {
        istat = sys$qio( 0, ttchan, IO$_SETMODE | IO$M_CTRLCAST, 0, 0, 0,
                        control_C_ast, SIGINT, PSL$C_USER, 0, 0, 0 );
        if( istat != SS$_NORMAL )
          lib$signal( istat );
      }
      break;

    case SIGINT:                /* ^C typed: reinstall AST */
      istat = sys$qio( 0, ttchan, IO$_SETMODE | IO$M_CTRLCAST, 0, 0, 0,
                      control_C_ast, SIGINT, PSL$C_USER, 0, 0, 0 );
      if( istat != SS$_NORMAL )
        lib$signal( istat );

      /* there could be a problem if we interrupted a printf() */
      freopen( "SYS$OUTPUT", "w", stdout );

      lib$signal( SS$_CONTROLC );
      break;
    }

    installed = which;
  }

  return was_installed;
}

#endif /* vms */


/*
 * Handling of Control-C
 */
static
void keyboard_interrupt()
{
  if( kc_break.intr_enabled && kc_break.piaf_sync != NULL ) {
    /* resynchronize the communication with the Piaf server */
    (*kc_break.piaf_sync)( kc_break.sockfd, "\3" );
  }

  if( ++kc_break.intr_count >= 3 ) {
    if( kjmpaddr.disp_select_C != NULL ) {
      static char *labels[] = { "Yes", "No" };
      if( (*kjmpaddr.disp_select_C)( 4,
           "You keep hitting ^C --- do you want to exit ?",
           (sizeof labels)/sizeof(char*), labels, 2 ) != 1 )
        return;
    }
    else {
      const char *answer = ku_proc(
          "You keep hitting ^C --- do you want to exit (Yes/<CR>=No)?", NULL );
      if( answer[0] != 'Y' )
        return;
    }
    kprintf( " *** Exiting ***\n" );
    exit( 1 );
  }
}


/*
 * define the synchronization routine to be called after a keyboard interrupt
 */
void ku_piaf( int sockfd,
             void (*piaf_sync)() )
{
  kc_break.sockfd = sockfd;
  kc_break.piaf_sync = piaf_sync;
}


/*
 * enable signal catching or restore default signal action
 */
void ku_trap( int enable,
             int traceback )
{
#ifndef vms

  static int siglist[] = { SIGINT
# ifdef SIGBUS
    ,SIGBUS                     /* unaligned access on many machines */
# endif
# ifdef FATAL_SIGFPE
    ,SIGFPE                     /* catch only if fatal */
# endif
# ifdef SIGUSR2
    ,SIGUSR2                    /* soft interrupt */
# endif
    ,SIGSEGV
  };

  int i;

# ifdef SIGNAL_POSIX
  struct sigaction act;

  sigemptyset( &act.sa_mask );
  act.sa_flags = 0;
  act.sa_handler = signal_handler;

#  ifdef USE_EDIT_SERVER
  sigaction( SIGUSR1, &act, NULL ); /* handler for edit server always there */
#  endif

  if( !enable )                 /* reset handler */
    act.sa_handler = SIG_DFL;

  for( i = 0; i < sizeof siglist / sizeof( int ); i++ )
    sigaction( siglist[i], &act, NULL );

# endif /* SIGNAL_POSIX */


# ifdef SIGNAL_BSD
  /* VMS provides BSD like signal semantics */
  struct sigvec vec;

  vec.sv_flags = 0;
  vec.sv_mask = 0;
  vec.sv_handler = (enable ? signal_handler : SIG_DFL);

  for( i = 0; i < sizeof siglist / sizeof( int ); i++ )
    sigvec( siglist[i], &vec, NULL );

# endif /* SIGNAL_BSD */


# ifdef SIGNAL_V7

#  ifdef IBM370
  /* C/370 provide V7 like signal semantics */

  INTEGER underflow = enable ? 0 : 1;
  Xuflow( &underflow );         /* set interrupts from floating underflows */
#  endif

  for( i = 0; i < sizeof siglist / sizeof( int ); i++ )
    signal( siglist[i], enable ? signal_handler : SIG_DFL );

# endif /* SIGNAL_V7 */

#else  /* vms */

  control_C_ast( enable );

#endif /* vms */

  kc_break.trap_enabled = enable;
  kc_break.intr_enabled = enable;
  kc_break.intr_pending = 0;
  if( traceback >= 0 )
    kc_break.traceback = traceback;
}


/*
 * allow or block trapping of keyboard interrupts
 */
int ku_intr( int enable )
{
  int old_enable = kc_break.intr_enabled;

  kc_break.intr_enabled = enable;
  if( enable && kc_break.intr_pending ) {
    kc_break.intr_pending = 0;
#ifndef vms
    raise( SIGINT );
#else
    lib$signal( SS$_CONTROLC );
#endif
  }

  return old_enable;
}


/*
 * allow SIGINT delivery
 */
void Kubron()
{
  ku_intr( 1 );
}


/*
 * block SIGINT delivery
 */
INTEGER Kubrof()
{
  return ku_intr( 0 );
}


/*
 * simulate a break
 */
void ku_sibr()
{
#ifndef vms
  signal_handler( 0 );
#else
  lib$signal( 0 );
#endif
}

void Kusibr()
{
  ku_sibr();
}


/*
 * test the soft interrupt flag
 */
INTEGER Kustop()
{
  return kc_break.soft_intr;
}


/*
 * set or test the soft interrupt flag
 */
int ku_stop( int set )
{
  if( set )
    kc_break.soft_intr = 1;
  return kc_break.soft_intr;
}


/*
 * action routine for /KUIP/SET_SHOW/BREAK
 */
int kxbreak()
{
  char *set_ON  = "ON";
  char *set_OFF = "OFF";
  char *set_TB  = "TB";
  char *value = ku_getc();

  ku_alfa();

  if( strcmp( value, "?" ) == 0 ) {
    kprintf( " Current break is %s\n", kc_value.set_break );
  }
  else {
    if( strcmp( value, set_ON ) == 0 ) {
      ku_trap( 1, 0 );
      kc_value.set_break = set_ON;
    }
    else if( strcmp( value, "TB" ) == 0 ) {
      ku_trap( 1, 1 );
      kc_value.set_break = set_TB;
    }
    else {
      ku_trap( 0, 0 );
      kc_value.set_break = set_OFF;
    }
    /*GB F77_CALL_C(Kierrf,value,strlen(value));GB*/
  }
  return 0;
}

#ifdef IBM370
/*
 * Exit routine for IBM Extended Error Handling Facility
 */
void Errrun( iretcd, ierr )
     INTEGER *iretcd;
     INTEGER *ierr;
{
  int ierno = *ierr;
  char *msg;

  switch( ierno ) {
  case 180:
    msg = "File name has more than 8 characters";
    break;
  case 203:
    msg = "Invalid implied DO-loop in READ/WRITE statement";
    break;
  case 204:
    msg = "Item size exceeds buffer length";
    break;
  case 206:
    msg = "Integer number out of range";
    break;
  case 207:
    msg = "Exponent overflow";
    break;
  case 208:
    msg = "Exponent underflow";
    break;
  case 209:
    msg = "Divide by zero";
    break;
  case 210:
    msg = "Program exception";
    break;
  case 211:
    msg = "Invalid FORMAT statement";
    break;
  case 212:
    msg = "Formatted I/O, end of record (text files must be RECFM F LRECL 80)";
#ifdef NEWLIB
    ierno = 0; /* Avoid message problems with VB card format files */
#endif
    break;
  case 213:
    msg = "I/O list greater than logical record";
    break;
  case 215:
    msg = "Illegal decimal character";
    ierno = 0;                  /* ignore */
    break;
  case 217:
    msg = "End of data during READ";
#ifdef IBMMVS
    ierno = 0; /* Avoid message problems with history */
#endif
    break;
  case 219:
    msg = "Data set with no DD statement or DD statement with incorrect DDNAME";
    break;
  case 220:
    msg = "Logical unit number out of range";
    break;
  case 225:
    msg = "Illegal hexadecimal character";
    break;
  case 226:
    msg = "Real number out of range";
    break;
  case 227:
    msg = "Invalid repeat count";
    break;
  case 228:
    msg = "Last item in I/O list has lower address than first element";
    break;
  case 251:
    msg = "Negative argument of SQRT";
    break;
  case 252:
    msg = "Argument greater than 174.673 in EXP";
    break;
  default:
    msg = "Unknown message";
  }
  if( ierno != 0 ) {
    /* cannot call kualfa for GKS if error is Fortran I/O related */
    char *line = strdup( " *** AFB" );
    line = mstricat( line, ierno );
    line = mstr2cat( line, "I: ", msg );
    if( kc_break.error_msg == NULL )
      kc_break.error_msg = line;
    else {
      kc_break.error_msg = mstr2cat( kc_break.error_msg, "\n", line );
      free( line );
    }
  }

  IQUEST(1) = ierno;
  *iretcd = 0;
}
#endif


