@ECHO off
SETLOCAL

CALL .\build_one ChainedAndFilteredTuplesExample
@ECHO OFF

CALL .\build_one Chi2FitToHistogram
@ECHO OFF

CALL .\build_one Cloud
@ECHO OFF

CALL .\build_one CloudCreate
@ECHO OFF

CALL .\build_one ComplexFit
@ECHO OFF

CALL .\build_one CreateAndFitDataPointSet
@ECHO OFF

CALL .\build_one DataPointSetCreateAndFill
@ECHO OFF

CALL .\build_one create_aidaStore_aida
@ECHO OFF

CALL .\build_one DataPointSetCreateFromData
@ECHO OFF

CALL .\build_one FilterAndEvaluatorExample
@ECHO OFF

CALL .\build_one FitExample
@ECHO OFF

CALL .\build_one Histogram
@ECHO OFF

CALL .\build_one HistogramArithmetic
@ECHO OFF

CALL .\build_one PlotExample
@ECHO OFF

CALL .\build_one PlotWithStyles
@ECHO OFF

CALL .\build_one ProfileCreate
@ECHO OFF

CALL .\build_one ProfileFill
@ECHO OFF

CALL .\build_one ProfileFitAndPlot
@ECHO OFF

CALL .\build_one create_myFile_aida
@ECHO OFF

CALL .\build_one Restore
@ECHO OFF

CALL .\build_one ScanAndContour
@ECHO OFF

CALL .\build_one SimpleFit
@ECHO OFF

CALL .\build_one Store
@ECHO OFF

CALL .\build_one StoreTuple
@ECHO OFF

CALL .\build_one Tree
@ECHO OFF

CALL .\build_one Tuple
@ECHO OFF

CALL .\build_one TupleCreate
@ECHO OFF

CALL .\build_one createAndFillTupleWithComplexStructure
@ECHO OFF

ENDLOCAL
@ECHO on
