/***************************************************************************/
/*                                                                         */
/*  ftsystem.c                                                             */
/*                                                                         */
/*    Unix-specific FreeType low-level system interface (body).            */
/*                                                                         */
/*  Copyright 1996-2001, 2002, 2004 by                                     */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


#include "../../include/ourex_ft2build.h"
  /* we use our special ftconfig.h file, not the standard one */
#include <ftconfig.h>
#include FT_INTERNAL_DEBUG_H
#include FT_SYSTEM_H
#include FT_ERRORS_H
#include FT_TYPES_H
#include FT_INTERNAL_OBJECTS_H

  /* memory-mapping includes and definitions */
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/mman.h>
#ifndef MAP_FILE
#define MAP_FILE  0x00
#endif

#ifdef MUNMAP_USES_VOIDP
#define MUNMAP_ARG_CAST  void *
#else
#define MUNMAP_ARG_CAST  char *
#endif

#ifdef NEED_MUNMAP_DECL

#ifdef __cplusplus
  extern "C"
#else
  extern
#endif
  int
  munmap( char*  addr,
          int    len );

#define MUNMAP_ARG_CAST  char *

#endif /* NEED_DECLARATION_MUNMAP */


#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


  /*************************************************************************/
  /*                                                                       */
  /*                       MEMORY MANAGEMENT INTERFACE                     */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    ft_alloc                                                           */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The memory allocation function.                                    */
  /*                                                                       */
  /* <Input>                                                               */
  /*    memory :: A pointer to the memory object.                          */
  /*                                                                       */
  /*    size   :: The requested size in bytes.                             */
  /*                                                                       */
  /* <Return>                                                              */
  /*    The address of newly allocated block.                              */
  /*                                                                       */

/*G.Barrand*/
/*
#define INLIB_MEM
*/

#ifdef INLIB_MEM
extern void* ft_alloc(FT_Memory,long);
#else
  FT_CALLBACK_DEF( void* )
  ft_alloc( FT_Memory  memory,
            long       size )
  {
    FT_UNUSED( memory );

    return malloc( size );
  }
#endif

  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    ft_realloc                                                         */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The memory reallocation function.                                  */
  /*                                                                       */
  /* <Input>                                                               */
  /*    memory   :: A pointer to the memory object.                        */
  /*                                                                       */
  /*    cur_size :: The current size of the allocated memory block.        */
  /*                                                                       */
  /*    new_size :: The newly requested size in bytes.                     */
  /*                                                                       */
  /*    block    :: The current address of the block in memory.            */
  /*                                                                       */
  /* <Return>                                                              */
  /*    The address of the reallocated memory block.                       */
  /*                                                                       */

#ifdef INLIB_MEM
extern void* ft_realloc(FT_Memory,long,long,void*);
#else
  FT_CALLBACK_DEF( void* )
  ft_realloc( FT_Memory  memory,
              long       cur_size,
              long       new_size,
              void*      block )
  {
    FT_UNUSED( memory );
    FT_UNUSED( cur_size );

    return realloc( block, new_size );
  }
#endif

  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    ft_free                                                            */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The memory release function.                                       */
  /*                                                                       */
  /* <Input>                                                               */
  /*    memory :: A pointer to the memory object.                          */
  /*                                                                       */
  /*    block  :: The address of block in memory to be freed.              */
  /*                                                                       */
#ifdef INLIB_MEM
extern void ft_free(FT_Memory,void*);
#else
  FT_CALLBACK_DEF( void )
  ft_free( FT_Memory  memory,
           void*      block )
  {
    FT_UNUSED( memory );

    free( block );
  }
#endif

  /*************************************************************************/
  /*                                                                       */
  /*                     RESOURCE MANAGEMENT INTERFACE                     */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* The macro FT_COMPONENT is used in trace mode.  It is an implicit      */
  /* parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log  */
  /* messages during execution.                                            */
  /*                                                                       */
#undef  FT_COMPONENT
#define FT_COMPONENT  trace_io

  /* We use the macro STREAM_FILE for convenience to extract the       */
  /* system-specific stream handle from a given FreeType stream object */
#define STREAM_FILE( stream )  ( (FILE*)stream->descriptor.pointer )


  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    ft_close_stream_by_munmap                                          */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The function to close a stream which is opened by mmap.            */
  /*                                                                       */
  /* <Input>                                                               */
  /*    stream :: A pointer to the stream object.                          */
  /*                                                                       */
  FT_CALLBACK_DEF( void )
  ft_close_stream_by_munmap( FT_Stream  stream )
  {
    munmap( (MUNMAP_ARG_CAST)stream->descriptor.pointer, stream->size );

    stream->descriptor.pointer = NULL;
    stream->size               = 0;
    stream->base               = 0;
  }


  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    ft_close_stream_by_free                                            */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The function to close a stream which is created by ft_alloc.       */
  /*                                                                       */
  /* <Input>                                                               */
  /*    stream :: A pointer to the stream object.                          */
  /*                                                                       */
  FT_CALLBACK_DEF( void )
  ft_close_stream_by_free( FT_Stream  stream )
  {
    ft_free( NULL, stream->descriptor.pointer );

    stream->descriptor.pointer = NULL;
    stream->size               = 0;
    stream->base               = 0;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stream_Open( FT_Stream    stream,
                  const char*  filepathname )
  {
    int          file;
    struct stat  stat_buf;


    if ( !stream )
      return FT_Err_Invalid_Stream_Handle;

    /* open the file */
    file = open( filepathname, O_RDONLY );
    if ( file < 0 )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " could not open `%s'\n", filepathname ));
      return FT_Err_Cannot_Open_Resource;
    }

    /* Here we ensure that a "fork" will _not_ duplicate   */
    /* our opened input streams on Unix.  This is critical */
    /* since it avoids some (possible) access control      */
    /* issues and cleans up the kernel file table a bit.   */
    /*                                                     */
#ifdef F_SETFD
#ifdef FD_CLOEXEC
    (void)fcntl( file, F_SETFD, FD_CLOEXEC );
#else
    (void)fcntl( file, F_SETFD, 1 );
#endif /* FD_CLOEXEC */
#endif /* F_SETFD */

    if ( fstat( file, &stat_buf ) < 0 )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " could not `fstat' file `%s'\n", filepathname ));
      goto Fail_Map;
    }

    stream->size = stat_buf.st_size;
    stream->pos  = 0;
    stream->base = (unsigned char *)mmap( NULL,
                                          stream->size,
                                          PROT_READ,
                                          MAP_FILE | MAP_PRIVATE,
                                          file,
                                          0 );

    if ( (long)stream->base != -1 )
      stream->close = ft_close_stream_by_munmap;
    else
    {
      ssize_t  total_read_count;
    

      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " could not `mmap' file `%s'\n", filepathname ));
      
      stream->base = (unsigned char*)ft_alloc( NULL, stream->size );
      
      if ( !stream->base )
      {
        FT_ERROR(( "FT_Stream_Open:" ));
        FT_ERROR(( " could not `alloc' memory\n" ));
        goto Fail_Map;
      }
      
      total_read_count = 0;
      do {
        ssize_t  read_count;


        read_count = read( file, 
                           stream->base + total_read_count, 
                           stream->size - total_read_count );

        /*GB if ( ( read_count == -1 ) ) */
        if (read_count == -1) /*GB : rm clang warning.*/
        {
          if ( errno == EINTR )
            continue;

          FT_ERROR(( "FT_Stream_Open:" ));
          FT_ERROR(( " error while `read'ing file `%s'\n", filepathname ));
          goto Fail_Read;
        }

        total_read_count += read_count;

      } while ( total_read_count != (ssize_t)stream->size ); /*GB : cast*/

      stream->close = ft_close_stream_by_free;
    }

    close( file );

    stream->descriptor.pointer = stream->base;
    stream->pathname.pointer   = (char*)filepathname;

    stream->read = 0;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%d bytes) successfully\n",
                filepathname, stream->size ));

    return FT_Err_Ok;

  Fail_Read:
    ft_free( NULL, stream->base );

  Fail_Map:
    close( file );

    stream->base = NULL;
    stream->size = 0;
    stream->pos  = 0;

    return FT_Err_Cannot_Open_Stream;
  }


#ifdef FT_DEBUG_MEMORY

  extern FT_Int
  ft_mem_debug_init( FT_Memory  memory );

  extern void
  ft_mem_debug_done( FT_Memory  memory );

#endif


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Memory )
  FT_New_Memory( void )
  {
    FT_Memory  memory;


    /*G.Barrand : memory = (FT_Memory)malloc( sizeof ( *memory ) ); */
    memory = (FT_Memory)ft_alloc(NULL, sizeof ( *memory ) ); /*G.Barrand*/
    if ( memory )
    {
      memory->user    = 0;
      memory->alloc   = ft_alloc;
      memory->realloc = ft_realloc;
      memory->free    = ft_free;
#ifdef FT_DEBUG_MEMORY
      ft_mem_debug_init( memory );
#endif
    }

    return memory;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( void )
  FT_Done_Memory( FT_Memory  memory )
  {
#ifdef FT_DEBUG_MEMORY
    ft_mem_debug_done( memory );
#endif
    memory->free( memory, memory );
  }


/* END */
