/*
 * Copyright (C) 1997-1998 Janne L�f <jlof@mail.student.oulu.fi>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "../gtkgl/gdkgl.h"
#include "../gtkgl/gtkglarea.h"

#ifdef APPLE_GL
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include <stdarg.h>

static void gtk_gl_area_class_init    (GtkGLAreaClass *klass);
static void gtk_gl_area_init          (GtkGLArea      *glarea);
static void gtk_gl_area_destroy       (GtkObject      *object); /* change to finalize? */

static GtkDrawingAreaClass *parent_class = NULL;


GtkType
gtk_gl_area_get_type ()
{
#ifdef HAS_GTK1
  static GtkType gl_area_type = 0;  
  if (!gl_area_type)
    {
      GtkTypeInfo gl_area_info =
      {
	(gchar*)"GtkGLArea",
	sizeof (GtkGLArea),
	sizeof (GtkGLAreaClass),
	(GtkClassInitFunc) gtk_gl_area_class_init,
	(GtkObjectInitFunc) gtk_gl_area_init,
	/* reserved_1 */ NULL,
	/* reserved_2 */ NULL,
	(GtkClassInitFunc) NULL
      };
      gl_area_type = gtk_type_unique (gtk_drawing_area_get_type (), &gl_area_info);
  }
  return gl_area_type;
#else
  static GType ttt_type = 0;
  if(!ttt_type) {
    static const GTypeInfo ttt_info = {
      sizeof (GtkGLAreaClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc)gtk_gl_area_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkGLArea),
      0,    /* n_preallocs */
      (GInstanceInitFunc) gtk_gl_area_init,
      NULL /* value_table */
    };
    ttt_type = g_type_register_static(gtk_drawing_area_get_type(),
                                      "GtkGLArea",&ttt_info,(GTypeFlags)0);
  }
  return ttt_type;
#endif
}

static void
gtk_gl_area_class_init (GtkGLAreaClass *klass)
{
  GtkObjectClass *object_class;

  /*G.Barrand : C++ : have the below cast. */
  parent_class = 
    (GtkDrawingAreaClass*)gtk_type_class(gtk_drawing_area_get_type());
  object_class = (GtkObjectClass*) klass;
  
  object_class->destroy = gtk_gl_area_destroy;
}


static void
gtk_gl_area_init (GtkGLArea *gl_area)
{
  gl_area->glcontext = NULL;
}



GtkWidget*
gtk_gl_area_new_vargs(GtkGLArea *share, ...)
{
  GtkWidget *glarea;
  va_list ap;
  int i;
  gint *attrlist;

  va_start(ap, share);
  i=1;
  while (va_arg(ap, int) != GDK_GL_NONE) /* get number of arguments */
    i++;
  va_end(ap);

  attrlist = g_new(int,i);

  va_start(ap,share);
  i=0;
  while ( (attrlist[i] = va_arg(ap, int)) != GDK_GL_NONE) /* copy args to list */
    i++;
  va_end(ap);
  
  glarea = gtk_gl_area_share_new(attrlist, share);

  g_free(attrlist);

  return glarea;
}

GtkWidget*
gtk_gl_area_new (int *attrlist)
{
  return gtk_gl_area_share_new(attrlist, NULL);
}

GtkWidget*
gtk_gl_area_share_new (int *attrlist, GtkGLArea *share)
{

#if !defined(WIN32)

  GdkVisual *visual;
  GdkGLContext *glcontext;
  GtkGLArea *gl_area;

  g_return_val_if_fail(share == NULL || GTK_IS_GL_AREA(share), NULL);

  visual = gdk_gl_choose_visual(attrlist);
  if (visual == NULL)
    return NULL;

  glcontext = gdk_gl_context_share_new(visual, share ? share->glcontext : NULL, TRUE);
  if (glcontext == NULL)
    return NULL;

  /* use colormap and visual suitable for OpenGL rendering */
#ifdef HAS_GTK1
  gtk_widget_push_colormap(gdk_colormap_new(visual,TRUE));
  gtk_widget_push_visual(visual);
#endif
  
  /*G.Barrand : C++ : have the below cast. */
  gl_area = (GtkGLArea*)gtk_type_new (gtk_gl_area_get_type());
  gl_area->glcontext = glcontext;
#ifdef HAS_GTK1
#else
  gtk_widget_set_colormap(GTK_WIDGET(gl_area),gdk_colormap_new(visual,TRUE));
  gtk_widget_set_visual(GTK_WIDGET(gl_area),visual);
#endif

  /* pop back defaults */
#ifdef HAS_GTK1
  gtk_widget_pop_visual();
  gtk_widget_pop_colormap();
#endif

  //G.Barrand : do the below, else bad rendering on virtual machines.
  //            (Done by default also in gtkglext).
  gtk_widget_set_double_buffered(GTK_WIDGET(gl_area),FALSE);

#else

  GdkGLContext *glcontext;
  GtkGLArea *gl_area;

  g_return_val_if_fail(share == NULL || GTK_IS_GL_AREA(share), NULL);

  glcontext = gdk_gl_context_attrlist_share_new(attrlist, share ? share->glcontext : NULL, TRUE);
  if (glcontext == NULL)
    return NULL;

  gl_area = gtk_type_new (gtk_gl_area_get_type());
  gl_area->glcontext = glcontext;

#endif

  return GTK_WIDGET(gl_area);
}


static void
gtk_gl_area_destroy(GtkObject *object)
{
  GtkGLArea *gl_area;

  g_return_if_fail (object != NULL);
  g_return_if_fail (GTK_IS_GL_AREA(object));
  
  gl_area = GTK_GL_AREA(object);
  gdk_gl_context_unref(gl_area->glcontext);

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    (* GTK_OBJECT_CLASS (parent_class)->destroy) (object);
}






gint gtk_gl_area_make_current(GtkGLArea *gl_area)
{
  g_return_val_if_fail(gl_area != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_GL_AREA (gl_area), FALSE);
  g_return_val_if_fail(GTK_WIDGET_REALIZED(gl_area), FALSE);

  return gdk_gl_make_current(GTK_WIDGET(gl_area)->window, gl_area->glcontext);
}

/* deprecated, use gtk_gl_area_make_current */
gint gtk_gl_area_begingl(GtkGLArea *gl_area)
{
  return gtk_gl_area_make_current(gl_area);
}

/* deprecated, use glFlush if needed */
void gtk_gl_area_endgl(GtkGLArea *gl_area)
{
  g_return_if_fail(gl_area != NULL);
  g_return_if_fail(GTK_IS_GL_AREA(gl_area));
  glFlush();
}

/* deprecated, use gtk_gl_area_swap_buffers */
void gtk_gl_area_swapbuffers(GtkGLArea *gl_area)
{
  gtk_gl_area_swap_buffers(gl_area);
}
void gtk_gl_area_swap_buffers(GtkGLArea *gl_area)
{
  g_return_if_fail(gl_area != NULL);
  g_return_if_fail(GTK_IS_GL_AREA(gl_area));
  g_return_if_fail(GTK_WIDGET_REALIZED(gl_area));

  gdk_gl_swap_buffers(GTK_WIDGET(gl_area)->window);
}

void gtk_gl_area_size (GtkGLArea *glarea, gint width, gint height)
{
  g_return_if_fail (glarea != NULL);
  g_return_if_fail (GTK_IS_GL_AREA (glarea));

  gtk_drawing_area_size(GTK_DRAWING_AREA(glarea), width, height);
}
