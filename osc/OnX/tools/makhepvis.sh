#!/bin/sh -f
#set -x
dir=`pwd`
hepvisroot=/projects/HEPVis/v5r1p7
# 
# Produce hepvis.i :
cd ${hepvisroot}/include/HEPVis/nodes
ls *.h > tmp0
cd ../nodekits
mv ../nodes/tmp0 .
ls *.h >> tmp0
cd $dir
mv $hepvisroot/include/HEPVis/nodekits/tmp0 .
# rm some messy things  :
grep -v SoIdealTrackKit.h tmp0 | grep -v SoIdealBeamTrackKit.h | grep -v SoDblSiStripDetKit.h | grep -v SoDblSiStripWedgeDetKit.h | grep -v SoSiStripDetKit.h | grep -v SoSiStripWedgeDetKit.h | grep -v SoStereoSiStripDetKit.h | grep -v So3DErrorBar | grep -v SoCrystalHit | grep -v SoPcon | grep -v SoPolyVol | grep -v SoSiWedgeStrips | grep -v SoWedge > tmp1
sed -e 's:\.h::g' tmp1 > tmp2
awk '{print "class " $0 " : public SoNode {public: " $0 "();};"}' tmp2 > ../source/hepvis.i
/bin/rm -f tmp0 tmp1 tmp2 tmp3
#
#
# Produce hepvis.h :
cd ${hepvisroot}/include
ls HEPVis/nodes/*.h > tmp0
ls HEPVis/nodekits/*.h >> tmp0
cd $dir
mv $hepvisroot/include/tmp0 .
grep -v SoIdealTrackKit.h tmp0 | grep -v SoIdealBeamTrackKit.h | grep -v SoDblSiStripDetKit.h | grep -v SoDblSiStripWedgeDetKit.h | grep -v SoSiStripDetKit.h | grep -v SoSiStripWedgeDetKit.h | grep -v SoStereoSiStripDetKit.h | grep -v So3DErrorBar | grep -v SoCrystalHit | grep -v SoPcon | grep -v SoPolyVol | grep -v SoSiWedgeStrips | grep -v SoWedge > tmp1
awk '{print "#include <" $0 ">";}' tmp1 > ../source/hepvis.h
/bin/rm -f tmp0 tmp1 tmp2 tmp3
#
