#!/bin/sh -f
#set -x
dir=`pwd`
ivroot=/projects/SoFree/v3r1
#
#
# Produce iv.i :
cd ${ivroot}/include/Inventor/nodes
ls *.h > tmp0
cd ../nodekits
mv ../nodes/tmp0 .
ls *.h >> tmp0
cd $dir
mv $ivroot/include/Inventor/nodekits/tmp0 .
# Remove things that are not nodes in fact :
grep -v SoNode.h tmp0 | grep -v SoNodes.h | grep -v SoSubNode.h | grep -v SoSeparator.h | grep -v SoCamera.h | grep -v SoIndexedShape.h | grep -v SoLight.h | grep -v SoNonIndexedShape.h | grep -v SoShape.h | grep -v SoVertexShape.h | grep -v SoTransformation.h | grep -v SoNodeKits.h | grep -v SoSubKit.h | grep -v SoInteractionKit.h | grep -v SoNodekitParts | grep -v SoMarkerSet.h > tmp1
sed -e 's:\.h::g' tmp1 > tmp2
awk '{print "class " $0 " : public SoNode {public: " $0 "();};"}' tmp2 > ../source/iv.i
/bin/rm -f tmp0 tmp1 tmp2 tmp3
#exit
#
# Produce iv.h :
cd ${ivroot}/include
ls Inventor/nodes/*.h > tmp0
ls Inventor/nodekits/*.h >> tmp0
cd $dir
mv $ivroot/include/tmp0 .
grep -v SoNode.h tmp0 | grep -v SoNodes.h | grep -v SoSubNode.h | grep -v SoSeparator.h | grep -v SoCamera.h | grep -v SoIndexedShape.h | grep -v SoLight.h | grep -v SoNonIndexedShape.h | grep -v SoShape.h | grep -v SoVertexShape.h | grep -v SoTransformation.h | grep -v SoNodeKits.h | grep -v SoSubKit.h | grep -v SoInteractionKit.h | grep -v SoNodekitParts | grep -v SoMarkerSet.h > tmp1
awk '{print "#include <" $0 ">";}' tmp1 > ../source/iv.h
/bin/rm -f tmp0 tmp1 tmp2 tmp3
#
