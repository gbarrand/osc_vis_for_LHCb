*
*
>Name onx_kuip_def

****************************************************************
****************************************************************
****************************************************************
>Menu /SLASH

>Command WGET
>Parameters
FILE 'File' C
>Guidance
get a file on the web.
>Action onx_kuip%C

>Command LOAD
>Parameters
LIB 'Librairie' C
PACK 'Package' C
>Guidance
Load a KUIP DLL.
>Action onx_kuip%C

****************************************************************
****************************************************************
****************************************************************
>Menu /SLASH/UI
>Guidance
Interface to the Slash::UI::IUI class.

>Command SYNCHRONIZE
>Guidance
Flush and wait execution of screen manager events.
>Action onx_kuip%C

>Command LOAD
>Parameters
FILE 'File' C D=' '
>Keyword
GUI file
>Guidance
Load a GUI file
 Ex.  /SLASH/UI/LOAD $ONXROOT/scripts/OnX/InputPrint.onx
>Action onx_kuip%C

>Command SHOW
>Parameters
WIDGET 'Widget' C D=' '
>Keyword
widget
>Guidance
Show a shell widget.
 Ex.  /SLASH/UI/SHOW OnX_InputPrint_dialog
>Action onx_kuip%C

>Command DIALOG
>Parameters
NAME 'Name of the dialog' C D=' '
>Keyword
GUI dialog
>Guidance
 Map a dialog box.
  Exa : /SLASH/UI/DIALOG print
>Action onx_kuip%C

>Command SET_PARAMETER
>Parameters
WHAT 'param name' C D=' '
VALUE 'Field value' C D=' '
>Keyword
field
>Guidance
Set UI parameter value.
>Action onx_kuip%C

****************************************************************
****************************************************************
****************************************************************
>Menu /SLASH/VIEWER
>Guidance
To work on a viewer widget.

>Command VIEW_ALL
>Parameters
NAME 'Name of the viewer' C D='@current@'
>Guidance
>Action onx_kuip%C

>Command RENDER
>Parameters
NAME 'Name of the viewer' C D='@current@'
>Guidance
Force a redraw. By default it works on the "current" widget.
>Action onx_kuip%C

>Command SET_DECORATION
>Parameters
NAME 'Name of the viewer' C D='@current@'
VALUE 'param value' C D='TRUE'
>Guidance
Set or not the decorations of the viewer.
>Action onx_kuip%C

>Command SET_VIEWING
>Parameters
NAME 'Name of the viewer' C D='@current@'
VALUE 'param value' C D='TRUE'
>Guidance
Set or not the viewing mode.
>Action onx_kuip%C

>Command SET_FEEDBACK_VISIBILITY
>Parameters
NAME 'Name of the viewer' C D='@current@'
VALUE 'param value' C D='TRUE'
>Guidance
Set or not the feedback visiblity. Available only for examiner viewers.
>Action onx_kuip%C

>Command READ_SCENE
>Parameters
NAME 'Name of the viewer' C D='@current@'
FILE 'file' C D=' '
+
FORMAT 'file' C D='guessed'
PLACEMENT 'file' C D='staticScene'
>Guidance
Read a file at various format (typically an Inventor file).
>Action onx_kuip%C

****************************************************************
****************************************************************
****************************************************************
>Menu /SLASH/PAGE
>Guidance
To manipulate an SoPage.

>Command REFRESH
>Parameters
NAME 'Name of the page' C D='@current@'
>Action onx_kuip%C

>Command CLEAR
>Parameters
NAME 'Name of the page' C D='@current@'
>Action onx_kuip%C

>Command CREATE_REGION
>Parameters
+
NAME 'Name of the page' C D='@current@'
CLASS 'Class of region to create' C D='SoDisplayRegion'
X 'x' C D='0.25'
Y 'x' C D='0.25'
W 'x' C D='0.5'
H 'x' C D='0.5'
SET_CURRENT 'file' C D='TRUE'
>Guidance
Create a region.
>Action onx_kuip%C

>Command SET_CURRENT_REGION
>Parameters
NAME 'Name of the page' C D='@current@'
+
N 'Region ID to set as current one' I D=0
>Guidance
Set current region in a page.
>Action onx_kuip%C

>Command SET_PARAMETER
>Parameters
NAME 'Name of the page' C D='@current@'
WHAT 'param name' C D=' '
VALUE 'param value' C D=' '
>Keyword
field
>Guidance
Set page field value.
 Ex.  PAGE/SET_PARAMETER titleVisible TRUE
 Ex.  PAGE/SET_PARAMETER title Hello
>Action onx_kuip%C

>Command SET_REGIONS_HIGHLIGHTABLE
>Parameters
NAME 'Name of the page' C D='@current@'
VALUE 'param value' C D='TRUE'
>Action onx_kuip%C

****************************************************************
****************************************************************
****************************************************************
>Menu /SLASH/REGION
>Guidance
To manipulate an SoRegion.

>Command CLEAR
>Parameters
NAME 'Name of the region' C D='@current@'
>Action onx_kuip%C

>Command SET_PARAMETER
>Parameters
NAME 'Name of the region' C D='@current@'
WHAT 'param name' C D=' '
VALUE 'param value' C D=' '
>Keyword
field
>Guidance
Set page field value.
 Ex.  REGION/SET_PARAMETER ! infosRegionVisible TRUE
>Action onx_kuip%C

