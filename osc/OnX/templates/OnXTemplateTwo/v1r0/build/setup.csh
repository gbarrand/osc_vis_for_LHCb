
set env=`printenv PYTHONPATH`
if ( "${env}" == "" ) then
  setenv PYTHONPATH ".:../scripts/Python"
else
  setenv PYTHONPATH "${PYTHONPATH}:.:../scripts/Python"
endif


