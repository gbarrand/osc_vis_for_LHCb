#!/bin/sh -f

#set -x

/bin/rm -R -f ../classes/hep
/bin/mkdir -p ../classes/hep

javas=`find ../source/Java/OnXLab -name "*.java" -print`
#echo $javas

# 
# If .java code does "package OnXLab;" ; the below
# compilation will put the .class under :
#    ../classes/OnXLab 
# directory.
#

javac -d ../classes ${javas}

cd ../classes
jar cvf OnXLab.jar hep
/bin/rm -R hep
