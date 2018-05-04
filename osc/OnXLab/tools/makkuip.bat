
COPY ..\source\KUIP\CDF.cdf .
%KUIPROOT%\%KUIPCONFIG%\KUIP_kuipcc.exe -c CDF.cdf
move CDF.c ..\source\KUIP\KuipWrapCDF.ic
del CDF.cdf
