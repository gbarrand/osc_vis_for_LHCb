*
*
>Name onxlab_kuip_def

****************************************************************
****************************************************************
****************************************************************
>Menu /AIDA/TREE
>Guidance
Interfaces to data base.

>Command DELETE
>Parameters
NAME 'Tree name' C Loop
>Keyword
delete
>Guidance
Delete current tree. Close associated store...
>Action onxlab_kuip%C

>Command CD
>Parameters
NAME 'Tree name' C Loop
CHPATH 'Path name' C D=' '
>Keyword
dir directory
>Guidance
Change the current working directory in current Tree.
If CHPATH is given make it the new CWD.
Otherwise, print the pathname of the CWD.
 Ex.  CD dir1         ; make DIR1 the new CWD
      CD ..           ; go up.
      CD              ; print the name of the CWD
>Action onxlab_kuip%C

>Command LS
>Parameters
NAME 'Tree name' C Loop
>Keyword
dir Directory
>Guidance
List objects in the current directory/container of the current tree.
>Action onxlab_kuip%C

>Menu RIO
>Guidance
RIO menu

>Command NEW
>Parameters
NAME 'Tree name' C
FNAME 'File name' C
MODE 'Mode' C D='READ'
>Keyword
RIO ROOT open
>Guidance
Open a RIO file.
>Action onxlab_kuip%C

****************************************************************
****************************************************************
****************************************************************
>Menu /AIDA/HISTOGRAM
>Keyword
histo
>Guidance
Manipulation of histograms.

* >Command ADD
* >Parameters
* NAME 'Histogram name' C Minus Loop
* NAMEH 'Histo to add' C
* >Keyword
* add channel
* >Guidance
* Add an histogram.
* >Action onxlab_kuip%C

* >Command MULTIPLY
* >Parameters
* NAME 'Histogram name' C Minus Loop
* NAMEH 'Histo to multiply' C
* >Keyword
* multiply channel
* >Guidance
* Multiply an histogram.
* >Action onxlab_kuip%C

* >Command DIVIDE
* >Parameters
* NAME 'Histogram name' C Minus Loop
* NAMEH 'Histo to divide' C
* >Keyword
* divide channel
* >Guidance
* Divide an histogram.
* >Action onxlab_kuip%C

>Command SHOW
>Parameters
NAME 'Histogram name' C Minus Loop
>Keyword
graphics draw channel axis
>Guidance
Plot an histogram.
>Action onxlab_kuip%C

>Command DELETE
>Parameters
NAME 'Histogram name' C Loop
>Keyword
delete
>Guidance
Delete histogram in memory.
>Action onxlab_kuip%C

>Menu 1D
>Guidance
Things specific to 1D histos.

>Command GET
>Parameters
NAME 'Histogram name' C
TREENAME 'Tree name' C Loop
>Keyword
Read memory disk
>Guidance
Read histogram from the current data base directory to memory.
>Action onxlab_kuip%C

>Command NEW
>Parameters
NAME 'Histogram name' C
+
LABEL 'Label' C D=' '
NCX  'Number of channels' I D=100
XMIN 'Low edge' R  D=0.
XMAX 'Upper edge' R  D=100.
>Keyword
channel
>Guidance
Create a one dimensional histogram.
The contents are set to zero.
>Action onxlab_kuip%C

>Menu ../2D
>Guidance
Things specific to 1D histos.

>Command GET
>Parameters
NAME 'Histogram name' C
TREENAME 'Tree name' C
>Keyword
Read memory disk
>Guidance
Read histogram from the current data base directory to memory.
>Action onxlab_kuip%C

>Command NEW
>Parameters
NAME 'Histogram name' C
+
LABEL 'Label' C D=' '
NCX  'X number of channels' I D=100
XMIN 'X low edge' R  D=0.
XMAX 'X upper edge' R  D=100.
NCY  'Y number of channels' I D=100
YMIN 'Y low edge' R  D=0.
YMAX 'Y upper edge' R  D=100.
>Keyword
channel
>Guidance
Create a one dimensional histogram.
The contents are set to zero.
>Action onxlab_kuip%C

****************************************************************
****************************************************************
****************************************************************
>Menu /AIDA/TUPLE
>Guidance
Tuple related operations.

>Command GET
>Parameters
NAME 'Tuple name' C
TREENAME 'Tree name' C
>Keyword
Read memory disk
>Guidance
Read tuple from the current data base directory to memory.
>Action onxlab_kuip%C

>Command DELETE
>Parameters
NAME 'Tuple name' C
>Guidance
Delete a tuple.
>Action onxlab_kuip%C

>Command SHOW
>Parameters
NAME 'Tuple name' C
>Keyword
printer plotter
>Guidance
Print a summary about a tuple.
Number of entries, variables names and limits are listed.
>Action onxlab_kuip%C

>Menu PROJECT
>Guidance
Projection of a tuple over an histogram.

>Command 1D
>Parameters
NAMET 'Tuple name' C
NAMEH 'Histo name to fill' C
EXP 'Expression of tuple variables to fill histogram' C
+
CUT 'Cuts over tuple variables' C
>Keyword
projection
>Guidance
Project a tuple onto a 1-Dim.
The histogram is not reset before filling. This allows
several PROJECTs from different tuples.
>Action onxlab_kuip%C

>Command 2D
>Parameters
NAMET 'Tuple name' C
NAMEH 'Histo name to fill' C
XEXP 'Expression of tuple variables to X fill histogram' C
YEXP 'Expression of tuple variables to Y fill histogram' C
+
CUT 'Cuts over tuple variables' C
>Keyword
projection
>Guidance
Project a tuple onto a 1-Dim.
The histogram is not reset before filling. This allows
several PROJECTs from different tuples.
>Action onxlab_kuip%C

****************************************************************
****************************************************************
****************************************************************
>Menu /AIDA/FUNCTION
>Keyword
function
>Guidance
Manipulation of functions.

>Command SHOW
>Parameters
NAME 'Function name' C Minus Loop
>Keyword
graphics draw channel axis
>Guidance
Plot a function.
>Action onxlab_kuip%C

>Command DELETE
>Parameters
NAME 'Function name' C Loop
>Keyword
delete
>Guidance
Delete function in memory.
>Action onxlab_kuip%C

>Command NEW
>Parameters
NAME 'Function name' C
+
LABEL 'Label' C D=' '
TYPE 'Type' C D='Gaussian'
>Keyword
fit
>Guidance
Create a fit function.
>Action onxlab_kuip%C

****************************************************************
****************************************************************
****************************************************************
>Menu /AIDA/FITRESULT
>Keyword
fitter
>Guidance
Manipulation of fitters.

>Command DELETE
>Parameters
NAME 'Fit result name' C Loop
>Keyword
delete
>Action onxlab_kuip%C

****************************************************************
****************************************************************
****************************************************************
>Menu /AIDA/FITTER
>Keyword
fitter
>Guidance
Manipulation of fitters.

>Command NEW
>Parameters
NAME 'Name of the fitter' C
>Keyword
new create
>Guidance
Create a fitter.
>Action onxlab_kuip%C

>Command DELETE
>Parameters
NAME 'Fitter name' C Loop
>Keyword
delete
>Action onxlab_kuip%C

>Command FIT
>Parameters
NAME 'Fitter name' C
TYPE 'Fitting type' C
NAMEH 'Histogram name' C
NAMER 'Fit result name' C
>Keyword
fit histogram
>Guidance
Fit over an histogram.
>Action onxlab_kuip%C

****************************************************************
****************************************************************
****************************************************************
>Menu /AIDA/PLOTTER
>Keyword
plotter
>Guidance
Manipulation of plotters.

>Command NEW
>Parameters
NAME 'Name of the plotter' C
+
WIDGET 'Widget name' C D=' '
>Keyword
new create
>Guidance
Create a plotter or find one in a GUI.
>Action onxlab_kuip%C

>Command DELETE
>Parameters
NAME 'plotter name' C Loop
>Keyword
delete
>Action onxlab_kuip%C

>Command CLEAR
>Parameters
>Action onxlab_kuip%C

>Command NEXT
>Parameters
>Guidance
Set current region to the next one.
>Action onxlab_kuip%C

>Command SETPARAMETER
>Parameters
NAME 'Name of the plotter' C
PARAMETER 'Parameter name' C D=' '
VALUE 'Parameter value' C D=' '
>Keyword
parameter
>Guidance
Set plotter parameter.
>Action onxlab_kuip%C

>Menu REGION
>Guidance
Things specific to 1D histos.

>Command GRID
>Parameters
NAME 'Name of the plotter' C
+
NC 'Number of columns' I D=1
NR 'Number of rows' I D=1
I 'Index of current region' I D=0
>Guidance
Create a grid of regions
>Action onxlab_kuip%C

>Command CREATE
>Parameters
NAME 'Name of the plotter' C
+
X 'x' R D=0
Y 'y' R D=0
W 'width' R D=1
H 'height' R D=1
>Guidance
Create a region
>Action onxlab_kuip%C

>Command SET
>Parameters
NAME 'Name of the plotter' C
+
N 'Region ID to set as current one' I D=0
>Guidance
Set current region in a page.
>Action onxlab_kuip%C

>Command CLEAR
>Parameters
NAME 'Name of the plotter' C
>Action onxlab_kuip%C

>Command SETPARAMETER
>Parameters
NAME 'Name of the plotter' C
PARAMETER 'Parameter name' C D=' '
VALUE 'Parameter value' C D=' '
>Keyword
parameter
>Guidance
Set plotter region parameter.
>Action onxlab_kuip%C

>Menu ../PLOT
>Guidance
Things specific to 1D histos.

>Command HISTOGRAM
>Parameters
NAME 'Name of the plotter' C
NAMEH 'Histogram to plot' C
>Keyword
plot draw histogram
>Guidance
>Action onxlab_kuip%C

>Command FUNCTION
>Parameters
NAME 'Name of the plotter' C
NAMEF 'Function to plot' C
>Keyword
plot draw function
>Guidance
>Action onxlab_kuip%C

>Command FITRESULT
>Parameters
NAME 'Name of the plotter' C
NAMER 'Fit result to plot' C
>Keyword
plot draw fit result
>Guidance
>Action onxlab_kuip%C



