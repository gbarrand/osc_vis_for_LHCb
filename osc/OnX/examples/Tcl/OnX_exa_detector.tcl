
source $env(ONXROOT)/scripts/Tcl/OnX.tcl

ISession session -this [OnX_session]
IUI ui -this [session ui]

SoPage soPage -this [ui currentPage]
soPage set "title OnX/Detector"

SoRegion soRegion -this [soPage currentRegion]
soRegion set "color 0.8 0.8 0.8"

SoSeparator soSep -this [[soRegion cast_SoDisplayRegion] getStaticScene]

SoDetectorExample soDet
soSep addChild [soDet cget -this]

rename soDet ""
rename soSep ""
rename soRegion ""
rename soPage ""

rename ui ""
rename session ""
