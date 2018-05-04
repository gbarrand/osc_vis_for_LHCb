
source $env(ONXROOT)/scripts/Tcl/OnX.tcl

ISession session -this [OnX_session]
IUI ui -this [session ui]

SoPage soPage -this [ui currentPage]
[soPage cget -title] setValue "Tcl/Inventor"

SoRegion soRegion -this [soPage currentRegion]
[soRegion cget -color] setValue 0.8 0.8 0.8
soRegion clear

SoSeparator soSep -this [[soRegion cast_SoDisplayRegion] getStaticScene]

SoCube soCube
[soCube cget -width] setValue 0.1
[soCube cget -height] setValue 0.2
soSep addChild [soCube cget -this]

SoAxes3D soAxes
SoAxis soAxisX -this [soAxes getXAxis]
[soAxisX cget -width] setValue 1
[soAxisX cget -minimumValue] setValue 0
[soAxisX cget -maximumValue] setValue 1
SoAxis soAxisY -this [soAxes getYAxis]
[soAxisY cget -width] setValue 1
[soAxisY cget -minimumValue] setValue 0
[soAxisY cget -maximumValue] setValue 1
soSep addChild [soAxes cget -this]

rename soCube ""
rename soAxisX ""
rename soAxisY ""
rename soAxes ""
rename soSep ""
rename soRegion ""
rename soPage ""

rename ui ""
rename session ""
