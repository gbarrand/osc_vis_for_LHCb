@ECHO off
SETLOCAL

.\AIDA_example_analysis.exe
.\AIDA_example_animated_fit.exe
.\AIDA_example_animated_fit_2.exe
.\AIDA_example_ascii_tuple.exe
.\AIDA_example_banner.exe
.\AIDA_example_cloud_2D.exe
.\AIDA_example_cloud_fill.exe
.\AIDA_example_cloud_slice.exe
.\AIDA_example_complex_and_func.exe
.\AIDA_example_data_point_set.exe
.\AIDA_example_fitting.exe
.\AIDA_example_func_2D.exe
.\AIDA_example_func_2D_curve.exe
.\AIDA_example_func_cut.exe
.\AIDA_example_func_to_cloud.exe
.\AIDA_example_func_write.exe
.\AIDA_example_hippo.exe
.\AIDA_example_hippo_write.exe
.\AIDA_example_hopera.exe
.\AIDA_example_jec.exe
.\AIDA_example_lego.exe
.\AIDA_example_mag.exe
.\AIDA_example_mandel.exe
.\AIDA_example_on_the_fly_func.exe
.\AIDA_example_profile.exe
.\AIDA_example_reopen.exe
.\AIDA_example_rio.exe
.\AIDA_example_rmason.exe
.\AIDA_example_root_th.exe
.\AIDA_example_store.exe
.\AIDA_example_xml.exe
.\AIDA_example_shahrokh_a_write.exe
.\AIDA_example_shahrokh_read.exe
.\AIDA_example_thread_and_auto_refresh.exe
.\AIDA_example_pure_thread.exe

.\AIDA_example_slash_cyclic.exe
.\AIDA_example_slash_thread_cyclic.exe
.\AIDA_example_slash_shape_collect_tuple.exe
.\AIDA_example_slash_shape_hits_collect.exe

.\AIDA_example_load_aida.exe BatchLabLoadAIDA


FOR /F "usebackq delims=;" %%s IN (`aida-config --implementation`) DO SET aida_imp=%%s

IF "%aida_imp%" NEQ "OnXLab" GOTO end_if_iv
.\AIDA_example_load_aida.exe
.\AIDA_example_load_plotter.exe

.\AIDA_example_slash_iv_regions.exe
.\AIDA_example_slash_iv_plot_and_det.exe
.\AIDA_example_slash_iv_plot_and_images.exe
:end_if_iv

ENDLOCAL
@ECHO on
