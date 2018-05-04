@ECHO off
SETLOCAL

REM #/////////////////////////////////////////////////////////////
REM # In memory manipulations (no IO) :
REM #/////////////////////////////////////////////////////////////
ECHO "AIDA_example_ChainedAndFilteredTuplesExample.exe..."
AIDA_example_ChainedAndFilteredTuplesExample.exe

ECHO "AIDA_example_CloudCreate.exe..."
AIDA_example_CloudCreate.exe

ECHO "AIDA_example_ProfileCreate.exe..."
AIDA_example_ProfileCreate.exe

ECHO "AIDA_example_ProfileFill.exe..."
AIDA_example_ProfileFill.exe

ECHO "AIDA_example_Tree.exe..."
AIDA_example_Tree.exe

ECHO "AIDA_example_TupleCreate.exe..."
AIDA_example_TupleCreate.exe

ECHO "AIDA_example_anaphe_b1..."
AIDA_example_anaphe_b1.exe

ECHO "AIDA_example_anaphe_b3..."
AIDA_example_anaphe_b3.exe

REM #/////////////////////////////////////////////////////////////
REM # In memory + plotting :
REM #/////////////////////////////////////////////////////////////
ECHO "AIDA_example_Histogram.exe..."
AIDA_example_Histogram.exe

ECHO "AIDA_example_PlotExample.exe..."
AIDA_example_PlotExample.exe

ECHO "AIDA_example_PlotWithStyles.exe..."
AIDA_example_PlotWithStyles.exe

ECHO "AIDA_example_HistogramArithmetic.exe..."
AIDA_example_HistogramArithmetic.exe

ECHO "AIDA_example_Chi2FitToHistogram.exe..."
AIDA_example_Chi2FitToHistogram.exe

ECHO "AIDA_example_Cloud.exe..."
AIDA_example_Cloud.exe

ECHO "AIDA_example_ComplexFit.exe..."
AIDA_example_ComplexFit.exe

ECHO "AIDA_example_CreateAndFitDataPointSet.exe..."
AIDA_example_CreateAndFitDataPointSet.exe

ECHO "AIDA_example_DataPointSetCreateAndFill.exe..."
AIDA_example_DataPointSetCreateAndFill.exe

ECHO "AIDA_example_FilterAndEvaluatorExample.exe..."
AIDA_example_FilterAndEvaluatorExample.exe

ECHO "AIDA_example_FitExample.exe..."
AIDA_example_FitExample.exe

ECHO "AIDA_example_ProfileFitAndPlot.exe..."
AIDA_example_ProfileFitAndPlot.exe

ECHO "AIDA_example_ScanAndContour.exe..."
AIDA_example_ScanAndContour.exe

ECHO "AIDA_example_SimpleFit.exe..."
AIDA_example_SimpleFit.exe

ECHO "AIDA_example_Tuple.exe..."
AIDA_example_Tuple.exe

ECHO "AIDA_example_createAndFillTupleWithComplexStructure.exe..."
AIDA_example_createAndFillTupleWithComplexStructure.exe

ECHO "AIDA_example_anaphe_b2..."
AIDA_example_anaphe_b2.exe

ECHO "AIDA_example_anaphe_b6..."
AIDA_example_anaphe_b6.exe

REM #/////////////////////////////////////////////////////////////
REM # Examples dealing with data files :
REM #/////////////////////////////////////////////////////////////

REM # write myFile.aida :
ECHO "AIDA_example_Store.exe..."
AIDA_example_Store.exe

REM # read myFile.aida :
ECHO "AIDA_example_Restore.exe..."
AIDA_example_Restore.exe

REM # write histo.aida :
ECHO "AIDA_example_anaphe_b4..."
AIDA_example_anaphe_b4.exe

REM # read histo.aida :
REM # write histo.xml :
ECHO "AIDA_example_anaphe_b5..."
AIDA_example_anaphe_b5.exe

REM # write tuple.aida :
ECHO "AIDA_example_anaphe_b7..."
AIDA_example_anaphe_b7.exe

REM # read tuple.aida :
ECHO "AIDA_example_anaphe_b8..."
AIDA_example_anaphe_b8.exe

REM # write dps.aida :
ECHO "AIDA_example_anaphe_b11..."
AIDA_example_anaphe_b11.exe

REM # read aidaStore.aida (no plotting) :
REM #FIXME : where is the aidaStore.aida file ?
ECHO "AIDA_example_DataPointSetCreateFromData.exe..."
AIDA_example_DataPointSetCreateFromData.exe

:return
ENDLOCAL
@ECHO on
