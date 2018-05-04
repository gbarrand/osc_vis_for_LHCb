
source $env(ONXROOT)/scripts/Tcl/OnX.tcl

ISession session -this [OnX_session]
IUI ui -this [session ui]

ui executeScript DLD "OnX OnX_shape_initialize"

session setColor red

IAccessorManager da -this [session accessorManager]
da collect OnX_Shape "type=='Cube'"
da visualize

rename da ""
rename ui ""
rename session ""
