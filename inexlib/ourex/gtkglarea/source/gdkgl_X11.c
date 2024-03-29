/* 
 * Copyright (C) 1998 Janne L�f <jlof@mail.student.oulu.fi>
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

#include <gdk/gdkx.h>

#ifdef APPLE_GL
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include <GL/glx.h>
#include <string.h>

static XVisualInfo *get_xvisualinfo(GdkVisual *visual)
{
  Display *dpy;
  XVisualInfo vinfo_template;
  XVisualInfo *vi;
  int nitems_return;

  dpy = GDK_DISPLAY();

  /* TODO: is this right way to get VisualInfo from Visual ?? */
  /* AFAIK VisualID and depth should be enough to uniquely identify visual */
  vinfo_template.visual   = GDK_VISUAL_XVISUAL(visual);
  vinfo_template.visualid = XVisualIDFromVisual(vinfo_template.visual);
  vinfo_template.depth    = visual->depth;
  vi = XGetVisualInfo(dpy, VisualIDMask|VisualDepthMask, &vinfo_template, &nitems_return);

  g_assert( vi!=0  && nitems_return==1 ); /* visualinfo needs to be unique */

  /* remember to XFree returned XVisualInfo !!! */
  return vi;
}


struct _GdkGLContextPrivate {
  Display    *xdisplay;
  GLXContext glxcontext;
  guint ref_count;
};

typedef struct _GdkGLContextPrivate GdkGLContextPrivate;


gint gdk_gl_query(void)
{
  return (glXQueryExtension(GDK_DISPLAY(),NULL,NULL) == True) ? TRUE : FALSE;
}


gchar *gdk_gl_get_info()
{
  return g_strdup_printf("VENDOR     : %s\n"
			 "VERSION    : %s\n"
			 "EXTENSIONS : %s\n",
			 glXGetClientString(GDK_DISPLAY(), GLX_VENDOR),
			 glXGetClientString(GDK_DISPLAY(), GLX_VERSION),
			 glXGetClientString(GDK_DISPLAY(), GLX_EXTENSIONS));
}


GdkVisual *gdk_gl_choose_visual(int *attrlist)
{
  Display *dpy;
  XVisualInfo *vi;
  GdkVisual  *visual;

  g_return_val_if_fail(attrlist != NULL, NULL);

  dpy = GDK_DISPLAY();
  if ((vi = glXChooseVisual(dpy,DefaultScreen(dpy), attrlist)) == NULL)
    return NULL;
  
  visual = gdkx_visual_get(vi->visualid);
  XFree(vi);
  return visual;
}


int gdk_gl_get_config(GdkVisual *visual, int attrib)
{
  Display *dpy;
  XVisualInfo *vi;
  int value;
  
  g_return_val_if_fail(visual != NULL, -1);

  dpy = GDK_DISPLAY();
 
  vi = get_xvisualinfo(visual);

  if (glXGetConfig(dpy, vi, attrib, &value) == 0)
    {
      XFree(vi);
      return value;
    }
  XFree(vi);
  return -1;
}


GdkGLContext *gdk_gl_context_new(GdkVisual *visual)
{
  return gdk_gl_context_share_new(visual, NULL, FALSE);
}


GdkGLContext *gdk_gl_context_share_new(GdkVisual *visual, GdkGLContext *sharelist, gint direct)
{
  Display *dpy;
  XVisualInfo *vi;
  GLXContext glxcontext;
  GdkGLContextPrivate *ctx_private; /*G.Barrand : ctx_private for C++. */

  g_return_val_if_fail(visual != NULL, NULL);

  dpy = GDK_DISPLAY();

  //G.Barrand : NOTE :
  // a gdk_gl_choose_visual(attrlist) and here a :
  //   vi = get_xvisualinfo(visual);
  // could be short cut by doing directly here a :
  //   vi = glXChooseVisual(dpy,DefaultScreen(dpy), attrlist);
  // 

  vi = get_xvisualinfo(visual);

  if (sharelist)
    glxcontext = glXCreateContext(dpy, vi, ((GdkGLContextPrivate*)sharelist)->glxcontext, direct ? True : False);
  else
    glxcontext = glXCreateContext(dpy, vi, 0, direct ? True : False);
  
  XFree(vi);
  if (glxcontext == NULL)
    return NULL;
  
  ctx_private = g_new(GdkGLContextPrivate, 1);
  ctx_private->xdisplay = dpy;
  ctx_private->glxcontext = glxcontext;
  ctx_private->ref_count = 1;
  
  return (GdkGLContext*)ctx_private;
}

GdkGLContext *gdk_gl_attrlist_share_new(int *attrlist, GdkGLContext *sharelist, gint direct)
{
  GdkVisual *visual = gdk_gl_choose_visual(attrlist);
  if (visual)
    return gdk_gl_context_share_new(visual, sharelist, direct);
  return NULL;
}


GdkGLContext *gdk_gl_context_ref(GdkGLContext *context)
{
  GdkGLContextPrivate *ctx_private = (GdkGLContextPrivate*)context;

  g_return_val_if_fail(context != NULL, NULL);
  ctx_private->ref_count += 1;

  return context;
}

void gdk_gl_context_unref(GdkGLContext *context)
{
  GdkGLContextPrivate *ctx_private = (GdkGLContextPrivate*)context;

  g_return_if_fail(context != NULL);

  if (ctx_private->ref_count > 1)
    {
      ctx_private->ref_count -= 1;
    }
  else
    {
      if (ctx_private->glxcontext == glXGetCurrentContext()) {
	/*FIXME : G.Barrand : centos crash : glXMakeCurrent(ctx_private->xdisplay, None, NULL);*/
      }

      glXDestroyContext(ctx_private->xdisplay, ctx_private->glxcontext);

      g_free(ctx_private);
    }
}

gint gdk_gl_make_current(GdkDrawable *drawable, GdkGLContext *context)
{
  GdkGLContextPrivate *ctx_private = (GdkGLContextPrivate*)context;

  g_return_val_if_fail(drawable != NULL, FALSE);
  g_return_val_if_fail(context  != NULL, FALSE);

  return (glXMakeCurrent(ctx_private->xdisplay, GDK_WINDOW_XWINDOW(drawable), ctx_private->glxcontext) == True) ? TRUE : FALSE;
/*   if (ctx_private->glxcontext != None && ctx_private->glxcontext == glXGetCurrentContext()) */
/*     { */
/*       glFlush(); */
/*       return TRUE; */
/*     } */
/*   else */
/*     { */
/*       return (glXMakeCurrent(ctx_private->xdisplay, GDK_WINDOW_XWINDOW(drawable), ctx_private->glxcontext) == True) ? TRUE : FALSE; */
/*     } */
}

void gdk_gl_swap_buffers(GdkDrawable *drawable)
{
  g_return_if_fail(drawable != NULL);

  glXSwapBuffers(GDK_WINDOW_XDISPLAY(drawable), GDK_WINDOW_XWINDOW(drawable));
}

void gdk_gl_wait_gdk(void)
{
  glXWaitX();
}

void gdk_gl_wait_gl (void)
{
  glXWaitGL();
}


/* glpixmap stuff */

struct _GdkGLPixmapPrivate {
  Display   *xdisplay;
  GLXPixmap glxpixmap;
  GdkPixmap *front_left;
  guint     ref_count;
};

typedef struct _GdkGLPixmapPrivate GdkGLPixmapPrivate;


GdkGLPixmap *gdk_gl_pixmap_new(GdkVisual *visual, GdkPixmap *pixmap)
{
  Display *dpy;
  XVisualInfo *vi;
  Pixmap xpixmap;
  GdkGLPixmapPrivate *ctx_private;
  GLXPixmap glxpixmap;
  Window root_return;
  int x_ret, y_ret;
  unsigned int w_ret, h_ret, bw_ret, depth_ret;

  g_return_val_if_fail(pixmap != NULL, NULL);
  g_return_val_if_fail(visual != NULL, NULL);
#ifdef HAS_GTK1
  g_return_val_if_fail(gdk_window_get_type(pixmap) == GDK_WINDOW_PIXMAP, NULL);
#else
  g_return_val_if_fail(GDK_IS_PIXMAP(gdk_window_get_type(pixmap)), NULL);
#endif

  dpy = GDK_DISPLAY();
  xpixmap = (Pixmap)GDK_WINDOW_XWINDOW(pixmap);
  
  g_return_val_if_fail(XGetGeometry(dpy, xpixmap, &root_return,
				    &x_ret, &y_ret, &w_ret, &h_ret, &bw_ret, &depth_ret), NULL);

  g_return_val_if_fail((gdk_gl_get_config(visual, GDK_GL_RED_SIZE) +
			gdk_gl_get_config(visual, GDK_GL_GREEN_SIZE) +
			gdk_gl_get_config(visual, GDK_GL_BLUE_SIZE)) == (int)depth_ret, NULL);

  vi = get_xvisualinfo(visual);
  glxpixmap = glXCreateGLXPixmap(dpy, vi, xpixmap);
  XFree(vi);

  g_return_val_if_fail(glxpixmap != None, NULL);

  ctx_private = g_new(GdkGLPixmapPrivate, 1);
  ctx_private->xdisplay   = dpy;
  ctx_private->glxpixmap  = glxpixmap;
  ctx_private->front_left = gdk_pixmap_ref(pixmap);
  ctx_private->ref_count  = 1;

  return (GdkGLPixmap*)ctx_private;
}


GdkGLPixmap *gdk_gl_pixmap_ref(GdkGLPixmap *glpixmap)
{
  GdkGLPixmapPrivate *ctx_private = (GdkGLPixmapPrivate*)glpixmap;

  g_return_val_if_fail(glpixmap != NULL, NULL);
  ctx_private->ref_count += 1;

  return glpixmap;
}

void gdk_gl_pixmap_unref(GdkGLPixmap *glpixmap)
{
  GdkGLPixmapPrivate *ctx_private = (GdkGLPixmapPrivate*)glpixmap;

  g_return_if_fail(glpixmap != NULL);

  if (ctx_private->ref_count > 1)
    {
      ctx_private->ref_count -= 1;
    }
  else
    {
      glXDestroyGLXPixmap(ctx_private->xdisplay, ctx_private->glxpixmap);
      glXWaitGL();
      gdk_pixmap_unref(ctx_private->front_left);
      glXWaitX();
      memset(glpixmap, 0, sizeof(GdkGLPixmapPrivate));
      g_free(glpixmap);
    }
}

gint gdk_gl_pixmap_make_current(GdkGLPixmap *glpixmap, GdkGLContext *context)
{
  Display  *dpy;
  GLXPixmap glxpixmap;
  GLXContext glxcontext;

  g_return_val_if_fail(glpixmap != NULL, FALSE);
  g_return_val_if_fail(context  != NULL, FALSE);

  dpy        = ((GdkGLContextPrivate*)context)->xdisplay;
  glxpixmap  = ((GdkGLPixmapPrivate*)glpixmap)->glxpixmap;
  glxcontext = ((GdkGLContextPrivate*)context)->glxcontext;

  return (glXMakeCurrent(dpy, glxpixmap, glxcontext) == True) ? TRUE : FALSE;
}

/* fonts */
void gdk_gl_use_gdk_font(GdkFont *font, int first, int count, int list_base)
{
  g_return_if_fail(font != NULL);
  glXUseXFont(gdk_font_id(font), first, count, list_base);
}

