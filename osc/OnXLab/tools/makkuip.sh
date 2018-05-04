#!/bin/sh -f

LD_LIBRARY_PATH=$KUIPROOT/$KUIPCONFIG;export LD_LIBRARY_PATH

cp ../source/KUIP/CDF.cdf .
$KUIPROOT/$KUIPCONFIG/KUIP_kuipcc.exe -c CDF.cdf
mv CDF.c ../source/KUIP/KuipWrapCDF.ic
touch ../source/KUIP/KuipWrapCDF.c
/bin/rm -f CDF.cdf
