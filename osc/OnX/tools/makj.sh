#!/bin/sh -f

#set -x

/bin/rm -R -f ../classes/OnX
/bin/mkdir -p ../classes/OnX

javas=`find ../source/Java/OnX -name "*.java" -print`
#echo $javas

# 
# If .java code does "package OnX;" ; the below
# compilation will put the .class under :
#    ../classes/OnX 
# directory.
#

javac -d ../classes ${javas}

cd ../classes
jar cvf OnX.jar OnX
/bin/rm -R OnX
