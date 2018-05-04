//
//  This file had been produced automatically
// by the obuild_wrap program from the file :
//    osc_plot.help
//
  static const char osc_plot_help_0[] = "WEB";
  static const char osc_plot_help_1[] = "  http://OpenScientist.lal.in2p3.fr";
  static const char osc_plot_help_2[] = "";
  static const char osc_plot_help_3[] = "USAGE";
  static const char osc_plot_help_4[] = "      OS> osc-plot <options> [<data file>]";
  static const char osc_plot_help_5[] = "  (OS is for Operating System). The upper command assumes that ";
  static const char osc_plot_help_6[] = "  the setup script of OpenScientist had been executed and that";
  static const char osc_plot_help_7[] = "  the OSC_HOME_DIR environment variable points to your";
  static const char osc_plot_help_8[] = "  OpenScientist installation. (For historical reasons osc-plot";
  static const char osc_plot_help_9[] = "  can be lauched also as onxlab).";
  static const char osc_plot_help_10[] = "";
  static const char osc_plot_help_11[] = "  Some various little data file at various formats can be found under :";
  static const char osc_plot_help_12[] = "      $OSC_HOME_DIR/Resources/OnXLab/examples/data";
  static const char osc_plot_help_13[] = "    ( %OSC_HOME_DIR%@@back_slash@@Resources@@back_slash@@OnXLab@@back_slash@@examples@@back_slash@@data on a Windows)";
  static const char osc_plot_help_14[] = "";
  static const char osc_plot_help_15[] = "OPTIONS";
  static const char osc_plot_help_16[] = " -version : ";
  static const char osc_plot_help_17[] = "    osc-plot version.";
  static const char osc_plot_help_18[] = "";
  static const char osc_plot_help_19[] = " -toolkit=[Xt, Gtk, Qt, Win, NS] :";
  static const char osc_plot_help_20[] = "    GUI toolkit to be used when creating the GUI.";
  static const char osc_plot_help_21[] = "    The GUI hierarchy is described from the file :";
  static const char osc_plot_help_22[] = "      $OSC_HOME_DIR/Resources/OnXLab/scripts/OnX/main.onx";
  static const char osc_plot_help_23[] = "    This assumes that the corresponding OnX GUI driver / DLL had ";
  static const char osc_plot_help_24[] = "    been installed.";
  static const char osc_plot_help_25[] = "    For example :";
  static const char osc_plot_help_26[] = "      OS> osc-plot -toolkit=Xt";
  static const char osc_plot_help_27[] = "";
  static const char osc_plot_help_28[] = " -Xt -xt -Xm -xm -motif -Motif :";
  static const char osc_plot_help_29[] = "    use Motif GUI toolkit. It is the default on a UNIX system.";
  static const char osc_plot_help_30[] = "";
  static const char osc_plot_help_31[] = " -Gtk -gtk :";
  static const char osc_plot_help_32[] = "    use gtk.";
  static const char osc_plot_help_33[] = "    For example :";
  static const char osc_plot_help_34[] = "      OS> osc-plot -gtk";
  static const char osc_plot_help_35[] = "";
  static const char osc_plot_help_36[] = " -Qt -qt :";
  static const char osc_plot_help_37[] = "    use Qt.";
  static const char osc_plot_help_38[] = "";
  static const char osc_plot_help_39[] = " -Win -win :";
  static const char osc_plot_help_40[] = "    use WIN32 GUI toolkit. It is the default on a Windows system.";
  static const char osc_plot_help_41[] = "";
  static const char osc_plot_help_42[] = " -stdout :";
  static const char osc_plot_help_43[] = "    redirect all output to the terminal. In this mode the session.printer, ";
  static const char osc_plot_help_44[] = "    std[out,err] and also std::[cout,cerr] write on the terminal.";
  static const char osc_plot_help_45[] = "    (In particular, there is no tentative to redirect the std channels";
  static const char osc_plot_help_46[] = "    to the @@double_quote@@UI console@@double_quote@@ ; see below).";
  static const char osc_plot_help_47[] = "";
  static const char osc_plot_help_48[] = " -defout :";
  static const char osc_plot_help_49[] = "    redirect only the session.printer to the @@double_quote@@UI console@@double_quote@@.";
  static const char osc_plot_help_50[] = "    The @@double_quote@@UI console@@double_quote@@ is the first widget named @@double_quote@@consoleText@@double_quote@@";
  static const char osc_plot_help_51[] = "    found in the GUI hierarchy. In this mode the std[out,err] and ";
  static const char osc_plot_help_52[] = "    std::[cout,cerr] are still on the terminal (no redirection";
  static const char osc_plot_help_53[] = "    is attempted).";
  static const char osc_plot_help_54[] = "";
  static const char osc_plot_help_55[] = " -uiout :";
  static const char osc_plot_help_56[] = "    redirect all the output to the @@double_quote@@UI console@@double_quote@@";
  static const char osc_plot_help_57[] = "    It is the default of the program.";
  static const char osc_plot_help_58[] = "    In this mode the session.printer, std[out,err] and ";
  static const char osc_plot_help_59[] = "    also std::[cout,cerr] are redirected to the UI console.";
  static const char osc_plot_help_60[] = "";
  static const char osc_plot_help_61[] = " [-verbose,-verbose=<manager>] :";
  static const char osc_plot_help_62[] = "    enable verbose mode. Convenient to debug.";
  static const char osc_plot_help_63[] = "    For example :";
  static const char osc_plot_help_64[] = "      OS> osc-plot -verbose -stdout";
  static const char osc_plot_help_65[] = "      OS> osc-plot -verbose=UI_Manager -stdout -gtk";
  static const char osc_plot_help_66[] = "";
  static const char osc_plot_help_67[] = " -balance : ";
  static const char osc_plot_help_68[] = " -debug_check_class : ";
  static const char osc_plot_help_69[] = "    enable object balance counting per class.";
  static const char osc_plot_help_70[] = "";
  static const char osc_plot_help_71[] = " -style=[windows,motif,cde,motifplus,platinum,sgi]";
  static const char osc_plot_help_72[] = " -style=[compact,windowsxp,aqua,macintosh] : ";
  static const char osc_plot_help_73[] = "    passed to some GUI (for exa Qt) that supports multiple GUI styles.";
  static const char osc_plot_help_74[] = "    For example :";
  static const char osc_plot_help_75[] = "      OS> osc-plot -qt -style=windows";
  static const char osc_plot_help_76[] = "";
  static const char osc_plot_help_77[] = " -thread : ";
  static const char osc_plot_help_78[] = "    enable multithreading in case the (GUI, scripting) drivers support it.";
  static const char osc_plot_help_79[] = "";
  static const char osc_plot_help_80[] = " -session=<file> :";
  static const char osc_plot_help_81[] = "    session parameters xml file. The default is :";
  static const char osc_plot_help_82[] = "      $OSC_HOME_DIR/Resources/OnX/scripts/OnX/Session.onx";
  static const char osc_plot_help_83[] = "";
  static const char osc_plot_help_84[] = "ENVIRONMENT";
  static const char osc_plot_help_85[] = "  Some environment variables permits to customize applications :";
  static const char osc_plot_help_86[] = "    ONX_ARGS : to pass arguments.";
  static const char osc_plot_help_87[] = "";
  static const char osc_plot_help_88[] = "FILES ";
  static const char osc_plot_help_89[] = "  Under $OSC_HOME_DIR/Resources/OnXLab :";
  static const char osc_plot_help_90[] = "    examples/data";
  static const char osc_plot_help_91[] = "    examples/Python";
  static const char osc_plot_help_92[] = "    scripts/OnX/main.onx";
  static const char osc_plot_help_93[] = "    etc...";
  static unsigned int osc_plot_help_linen = 94;
  static const char* osc_plot_help_lines[] = {
osc_plot_help_0
,osc_plot_help_1
,osc_plot_help_2
,osc_plot_help_3
,osc_plot_help_4
,osc_plot_help_5
,osc_plot_help_6
,osc_plot_help_7
,osc_plot_help_8
,osc_plot_help_9
,osc_plot_help_10
,osc_plot_help_11
,osc_plot_help_12
,osc_plot_help_13
,osc_plot_help_14
,osc_plot_help_15
,osc_plot_help_16
,osc_plot_help_17
,osc_plot_help_18
,osc_plot_help_19
,osc_plot_help_20
,osc_plot_help_21
,osc_plot_help_22
,osc_plot_help_23
,osc_plot_help_24
,osc_plot_help_25
,osc_plot_help_26
,osc_plot_help_27
,osc_plot_help_28
,osc_plot_help_29
,osc_plot_help_30
,osc_plot_help_31
,osc_plot_help_32
,osc_plot_help_33
,osc_plot_help_34
,osc_plot_help_35
,osc_plot_help_36
,osc_plot_help_37
,osc_plot_help_38
,osc_plot_help_39
,osc_plot_help_40
,osc_plot_help_41
,osc_plot_help_42
,osc_plot_help_43
,osc_plot_help_44
,osc_plot_help_45
,osc_plot_help_46
,osc_plot_help_47
,osc_plot_help_48
,osc_plot_help_49
,osc_plot_help_50
,osc_plot_help_51
,osc_plot_help_52
,osc_plot_help_53
,osc_plot_help_54
,osc_plot_help_55
,osc_plot_help_56
,osc_plot_help_57
,osc_plot_help_58
,osc_plot_help_59
,osc_plot_help_60
,osc_plot_help_61
,osc_plot_help_62
,osc_plot_help_63
,osc_plot_help_64
,osc_plot_help_65
,osc_plot_help_66
,osc_plot_help_67
,osc_plot_help_68
,osc_plot_help_69
,osc_plot_help_70
,osc_plot_help_71
,osc_plot_help_72
,osc_plot_help_73
,osc_plot_help_74
,osc_plot_help_75
,osc_plot_help_76
,osc_plot_help_77
,osc_plot_help_78
,osc_plot_help_79
,osc_plot_help_80
,osc_plot_help_81
,osc_plot_help_82
,osc_plot_help_83
,osc_plot_help_84
,osc_plot_help_85
,osc_plot_help_86
,osc_plot_help_87
,osc_plot_help_88
,osc_plot_help_89
,osc_plot_help_90
,osc_plot_help_91
,osc_plot_help_92
,osc_plot_help_93
};
