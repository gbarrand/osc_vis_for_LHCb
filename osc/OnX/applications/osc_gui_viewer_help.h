//
//  This file had been produced automatically
// by the obuild_wrap program from the file :
//    osc_gui_viewer.help
//
  static const char osc_gui_viewer_help_0[] = "DESCRIPTION";
  static const char osc_gui_viewer_help_1[] = "  osc-gui-viewer permits to create and visualize a GUI (Graphical User";
  static const char osc_gui_viewer_help_2[] = "  Interface) described in files at the OnX XML format.";
  static const char osc_gui_viewer_help_3[] = "";
  static const char osc_gui_viewer_help_4[] = "USAGE";
  static const char osc_gui_viewer_help_5[] = "      OS> osc-gui-viewer <options> [<OnX XML file>]";
  static const char osc_gui_viewer_help_6[] = "  (OS is for Operating System). The upper command assumes that ";
  static const char osc_gui_viewer_help_7[] = "  the setup script of OpenScientist had been executed and that";
  static const char osc_gui_viewer_help_8[] = "  the OSC_HOME_DIR environment variable points to your";
  static const char osc_gui_viewer_help_9[] = "  OpenScientist installation.";
  static const char osc_gui_viewer_help_10[] = "";
  static const char osc_gui_viewer_help_11[] = "  Some simple OnX XML files can be found under :";
  static const char osc_gui_viewer_help_12[] = "      $OSC_HOME_DIR/Resources/OnX/examples/OnX  ";
  static const char osc_gui_viewer_help_13[] = "    ( %OSC_HOME_DIR%@@back_slash@@Resources@@back_slash@@OnX@@back_slash@@examples@@back_slash@@OnX on a Windows)";
  static const char osc_gui_viewer_help_14[] = "";
  static const char osc_gui_viewer_help_15[] = "  For example on a UNIX :";
  static const char osc_gui_viewer_help_16[] = "      csh> osc-gui-viewer $OSC_HOME_DIR/Resources/OnX/examples/OnX/hello_world.onx";
  static const char osc_gui_viewer_help_17[] = "       sh> osc-gui-viewer $OSC_HOME_DIR/Resources/OnX/examples/OnX/so_examiner_viewer.onx";
  static const char osc_gui_viewer_help_18[] = "  or on a Windows under DOS :";
  static const char osc_gui_viewer_help_19[] = "      DOS> osc-gui-viewer %OSC_HOME_DIR%@@back_slash@@Resources@@back_slash@@OnX@@back_slash@@examples@@back_slash@@OnX@@back_slash@@shapes.onx";
  static const char osc_gui_viewer_help_20[] = "";
  static const char osc_gui_viewer_help_21[] = "OPTIONS";
  static const char osc_gui_viewer_help_22[] = " -version : ";
  static const char osc_gui_viewer_help_23[] = "    OnX version.";
  static const char osc_gui_viewer_help_24[] = "";
  static const char osc_gui_viewer_help_25[] = " -toolkit=[Xt, Gtk, Qt, Win, NS] :";
  static const char osc_gui_viewer_help_26[] = "    GUI toolkit to be used when creating a GUI from a .onx xml file.";
  static const char osc_gui_viewer_help_27[] = "    This assumes that the corresponding OnX GUI driver / DLL had ";
  static const char osc_gui_viewer_help_28[] = "    been installed.";
  static const char osc_gui_viewer_help_29[] = "    For example :";
  static const char osc_gui_viewer_help_30[] = "      OS> osc-gui-viewer -toolkit=Xt shapes.onx";
  static const char osc_gui_viewer_help_31[] = "";
  static const char osc_gui_viewer_help_32[] = " -Xt -xt -Xm -xm -motif -Motif :";
  static const char osc_gui_viewer_help_33[] = "    use Motif GUI toolkit. It is the default on a UNIX system.";
  static const char osc_gui_viewer_help_34[] = "";
  static const char osc_gui_viewer_help_35[] = " -Gtk -gtk :";
  static const char osc_gui_viewer_help_36[] = "    use gtk.";
  static const char osc_gui_viewer_help_37[] = "    For example :";
  static const char osc_gui_viewer_help_38[] = "      OS> osc-gui-viewer -gtk";
  static const char osc_gui_viewer_help_39[] = "";
  static const char osc_gui_viewer_help_40[] = " -Qt -qt :";
  static const char osc_gui_viewer_help_41[] = "    use Qt.";
  static const char osc_gui_viewer_help_42[] = "";
  static const char osc_gui_viewer_help_43[] = " -Win -win :";
  static const char osc_gui_viewer_help_44[] = "    use WIN32 GUI toolkit. It is the default on a Windows system.";
  static const char osc_gui_viewer_help_45[] = "";
  static const char osc_gui_viewer_help_46[] = " -stdout :";
  static const char osc_gui_viewer_help_47[] = "    redirect all output to the terminal. In this mode the session.printer, ";
  static const char osc_gui_viewer_help_48[] = "    std[out,err] and also std::[cout,cerr] write on the terminal.";
  static const char osc_gui_viewer_help_49[] = "    (In particular, there is no tentative to redirect the std channels";
  static const char osc_gui_viewer_help_50[] = "    to the @@double_quote@@UI console@@double_quote@@ ; see below).";
  static const char osc_gui_viewer_help_51[] = "";
  static const char osc_gui_viewer_help_52[] = " -defout :";
  static const char osc_gui_viewer_help_53[] = "    redirect only the session.printer to the @@double_quote@@UI console@@double_quote@@.";
  static const char osc_gui_viewer_help_54[] = "    The @@double_quote@@UI console@@double_quote@@ is the first widget named @@double_quote@@consoleText@@double_quote@@";
  static const char osc_gui_viewer_help_55[] = "    found in the GUI hierarchy. In this mode the std[out,err] and ";
  static const char osc_gui_viewer_help_56[] = "    std::[cout,cerr] are still on the terminal (no redirection";
  static const char osc_gui_viewer_help_57[] = "    is attempted).";
  static const char osc_gui_viewer_help_58[] = "     If working with Python, someone can do a : ";
  static const char osc_gui_viewer_help_59[] = "       >>> import OnX;OnX.sys2onx() ";
  static const char osc_gui_viewer_help_60[] = "    to redirect Python output to the UI console.";
  static const char osc_gui_viewer_help_61[] = "";
  static const char osc_gui_viewer_help_62[] = " -uiout :";
  static const char osc_gui_viewer_help_63[] = "    redirect all the output to the @@double_quote@@UI console@@double_quote@@";
  static const char osc_gui_viewer_help_64[] = "    It is the default of the osc-gui-viewer program.";
  static const char osc_gui_viewer_help_65[] = "    In this mode the session.printer, std[out,err] and ";
  static const char osc_gui_viewer_help_66[] = "    also std::[cout,cerr] are redirected to the UI console.";
  static const char osc_gui_viewer_help_67[] = "     If working with Python, it is better to explicitly";
  static const char osc_gui_viewer_help_68[] = "    do a : ";
  static const char osc_gui_viewer_help_69[] = "       >>> import OnX;OnX.sys2onx() ";
  static const char osc_gui_viewer_help_70[] = "    to redirect Python output to the UI console.";
  static const char osc_gui_viewer_help_71[] = "";
  static const char osc_gui_viewer_help_72[] = " [-verbose,-verbose=<manager>] :";
  static const char osc_gui_viewer_help_73[] = "    enable verbose mode. Convenient to debug.";
  static const char osc_gui_viewer_help_74[] = "    For example :";
  static const char osc_gui_viewer_help_75[] = "      OS> osc-gui-viewer -verbose -stdout";
  static const char osc_gui_viewer_help_76[] = "      OS> osc-gui-viewer -verbose=UI_Manager -stdout -gtk";
  static const char osc_gui_viewer_help_77[] = "";
  static const char osc_gui_viewer_help_78[] = " -balance : ";
  static const char osc_gui_viewer_help_79[] = " -debug_check_class : ";
  static const char osc_gui_viewer_help_80[] = "    enable object balance counting per class.";
  static const char osc_gui_viewer_help_81[] = "";
  static const char osc_gui_viewer_help_82[] = " -style=[windows,motif,cde,motifplus,platinum,sgi]";
  static const char osc_gui_viewer_help_83[] = " -style=[compact,windowsxp,aqua,macintosh] : ";
  static const char osc_gui_viewer_help_84[] = "    passed to some GUI (for exa Qt) that supports multiple GUI styles.";
  static const char osc_gui_viewer_help_85[] = "    For example :";
  static const char osc_gui_viewer_help_86[] = "      OS> osc-gui-viewer -qt -style=windows OnX.onx";
  static const char osc_gui_viewer_help_87[] = "";
  static const char osc_gui_viewer_help_88[] = " -thread : ";
  static const char osc_gui_viewer_help_89[] = "    enable multithreading in case the (GUI, scripting) drivers support it.";
  static const char osc_gui_viewer_help_90[] = "";
  static const char osc_gui_viewer_help_91[] = " -session=<file> :";
  static const char osc_gui_viewer_help_92[] = "    session parameters xml file. The default is :";
  static const char osc_gui_viewer_help_93[] = "      $OSC_HOME_DIR/Resources/OnX/scripts/OnX/Session.onx";
  static const char osc_gui_viewer_help_94[] = "";
  static const char osc_gui_viewer_help_95[] = "ENVIRONMENT";
  static const char osc_gui_viewer_help_96[] = "  OSC_HOME_DIR : directory where OpenScientist is installed.";
  static const char osc_gui_viewer_help_97[] = "  OSC_ARGS : to pass arguments.";
  static const char osc_gui_viewer_help_98[] = "  ONX_SESSION : session parameters xml file. ";
  static const char osc_gui_viewer_help_99[] = "  ONX_PS_VIEWER : PostScript viewer program.";
  static const char osc_gui_viewer_help_100[] = "  ONX_JPEG_VIEWER : JPEG viewer program.";
  static const char osc_gui_viewer_help_101[] = "  ONX_PDF_VIEWER : PDF viewer program.";
  static const char osc_gui_viewer_help_102[] = "  ONX_GIF_VIEWER : GIF file viewer program.";
  static const char osc_gui_viewer_help_103[] = "  ONX_IV_VIEWER : Inventor file viewer program.";
  static const char osc_gui_viewer_help_104[] = "  ONX_XRM : Xt/Motif driver. Permit to pass a X11 resource file.";
  static const char osc_gui_viewer_help_105[] = "  ONX_GTK_RC : gtk driver. Permit to pass a gtk @@double_quote@@rc@@double_quote@@ file.";
  static const char osc_gui_viewer_help_106[] = "  TTFPATH : to set access to TTF fonts.";
  static const char osc_gui_viewer_help_107[] = "";
  static const char osc_gui_viewer_help_108[] = "FILES ";
  static const char osc_gui_viewer_help_109[] = "  Under $OSC_HOME_DIR/Resources/OnX :";
  static const char osc_gui_viewer_help_110[] = "    examples/OnX/hello_world.onx";
  static const char osc_gui_viewer_help_111[] = "    scripts/OnX/Sessin.onx";
  static const char osc_gui_viewer_help_112[] = "    etc...";
  static const char osc_gui_viewer_help_113[] = "";
  static const char osc_gui_viewer_help_114[] = "WEB";
  static const char osc_gui_viewer_help_115[] = "  http://OpenScientist.lal.in2p3.fr";
  static const char osc_gui_viewer_help_116[] = "";
  static unsigned int osc_gui_viewer_help_linen = 117;
  static const char* osc_gui_viewer_help_lines[] = {
osc_gui_viewer_help_0
,osc_gui_viewer_help_1
,osc_gui_viewer_help_2
,osc_gui_viewer_help_3
,osc_gui_viewer_help_4
,osc_gui_viewer_help_5
,osc_gui_viewer_help_6
,osc_gui_viewer_help_7
,osc_gui_viewer_help_8
,osc_gui_viewer_help_9
,osc_gui_viewer_help_10
,osc_gui_viewer_help_11
,osc_gui_viewer_help_12
,osc_gui_viewer_help_13
,osc_gui_viewer_help_14
,osc_gui_viewer_help_15
,osc_gui_viewer_help_16
,osc_gui_viewer_help_17
,osc_gui_viewer_help_18
,osc_gui_viewer_help_19
,osc_gui_viewer_help_20
,osc_gui_viewer_help_21
,osc_gui_viewer_help_22
,osc_gui_viewer_help_23
,osc_gui_viewer_help_24
,osc_gui_viewer_help_25
,osc_gui_viewer_help_26
,osc_gui_viewer_help_27
,osc_gui_viewer_help_28
,osc_gui_viewer_help_29
,osc_gui_viewer_help_30
,osc_gui_viewer_help_31
,osc_gui_viewer_help_32
,osc_gui_viewer_help_33
,osc_gui_viewer_help_34
,osc_gui_viewer_help_35
,osc_gui_viewer_help_36
,osc_gui_viewer_help_37
,osc_gui_viewer_help_38
,osc_gui_viewer_help_39
,osc_gui_viewer_help_40
,osc_gui_viewer_help_41
,osc_gui_viewer_help_42
,osc_gui_viewer_help_43
,osc_gui_viewer_help_44
,osc_gui_viewer_help_45
,osc_gui_viewer_help_46
,osc_gui_viewer_help_47
,osc_gui_viewer_help_48
,osc_gui_viewer_help_49
,osc_gui_viewer_help_50
,osc_gui_viewer_help_51
,osc_gui_viewer_help_52
,osc_gui_viewer_help_53
,osc_gui_viewer_help_54
,osc_gui_viewer_help_55
,osc_gui_viewer_help_56
,osc_gui_viewer_help_57
,osc_gui_viewer_help_58
,osc_gui_viewer_help_59
,osc_gui_viewer_help_60
,osc_gui_viewer_help_61
,osc_gui_viewer_help_62
,osc_gui_viewer_help_63
,osc_gui_viewer_help_64
,osc_gui_viewer_help_65
,osc_gui_viewer_help_66
,osc_gui_viewer_help_67
,osc_gui_viewer_help_68
,osc_gui_viewer_help_69
,osc_gui_viewer_help_70
,osc_gui_viewer_help_71
,osc_gui_viewer_help_72
,osc_gui_viewer_help_73
,osc_gui_viewer_help_74
,osc_gui_viewer_help_75
,osc_gui_viewer_help_76
,osc_gui_viewer_help_77
,osc_gui_viewer_help_78
,osc_gui_viewer_help_79
,osc_gui_viewer_help_80
,osc_gui_viewer_help_81
,osc_gui_viewer_help_82
,osc_gui_viewer_help_83
,osc_gui_viewer_help_84
,osc_gui_viewer_help_85
,osc_gui_viewer_help_86
,osc_gui_viewer_help_87
,osc_gui_viewer_help_88
,osc_gui_viewer_help_89
,osc_gui_viewer_help_90
,osc_gui_viewer_help_91
,osc_gui_viewer_help_92
,osc_gui_viewer_help_93
,osc_gui_viewer_help_94
,osc_gui_viewer_help_95
,osc_gui_viewer_help_96
,osc_gui_viewer_help_97
,osc_gui_viewer_help_98
,osc_gui_viewer_help_99
,osc_gui_viewer_help_100
,osc_gui_viewer_help_101
,osc_gui_viewer_help_102
,osc_gui_viewer_help_103
,osc_gui_viewer_help_104
,osc_gui_viewer_help_105
,osc_gui_viewer_help_106
,osc_gui_viewer_help_107
,osc_gui_viewer_help_108
,osc_gui_viewer_help_109
,osc_gui_viewer_help_110
,osc_gui_viewer_help_111
,osc_gui_viewer_help_112
,osc_gui_viewer_help_113
,osc_gui_viewer_help_114
,osc_gui_viewer_help_115
,osc_gui_viewer_help_116
};
