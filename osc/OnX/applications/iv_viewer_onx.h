//
//  This file had been produced automatically
// by the obuild_wrap program from the file :
//    iv_viewer.onx
//
  static const char iv_viewer_onx_0[] = "<OnX>";
  static const char iv_viewer_onx_1[] = "";
  static const char iv_viewer_onx_2[] = "  <!-- NextStep, Qt : if dragging a data file on the icon .app -->";
  static const char iv_viewer_onx_3[] = "  <openFile exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_read_scene @current@ @{session.openFile}@ guessed staticScene</openFile>";
  static const char iv_viewer_onx_4[] = "";
  static const char iv_viewer_onx_5[] = "  <widget class=@@double_quote@@Window@@double_quote@@>";
  static const char iv_viewer_onx_6[] = "    <geometry>400x400</geometry>";
  static const char iv_viewer_onx_7[] = "    <title>OnX/IvViewer</title>";
  static const char iv_viewer_onx_8[] = "    <show>yes</show> ";
  static const char iv_viewer_onx_9[] = "    <destroy exec=@@double_quote@@DLD@@double_quote@@>OnX ui_exit</destroy>";
  static const char iv_viewer_onx_10[] = "";
  static const char iv_viewer_onx_11[] = "    <!-- A simple menu bar with a ";
  static const char iv_viewer_onx_12[] = "         @@double_quote@@File@@double_quote@@ and @@double_quote@@Scene@@double_quote@@ pull down menus (PDM). -->";
  static const char iv_viewer_onx_13[] = "";
  static const char iv_viewer_onx_14[] = "    <widget class=@@double_quote@@MenuBar@@double_quote@@>";
  static const char iv_viewer_onx_15[] = "";
  static const char iv_viewer_onx_16[] = "      <!-- A File PDM.  -->";
  static const char iv_viewer_onx_17[] = "      <widget class=@@double_quote@@CascadeButton@@double_quote@@>";
  static const char iv_viewer_onx_18[] = "        <label>File</label>";
  static const char iv_viewer_onx_19[] = "        <widget class=@@double_quote@@Menu@@double_quote@@>";
  static const char iv_viewer_onx_20[] = "          <widget class=@@double_quote@@MenuItem@@double_quote@@>";
  static const char iv_viewer_onx_21[] = "            <label>Open...</label>";
  static const char iv_viewer_onx_22[] = "            <variable name=@@double_quote@@OnX_Scene_read_iv.script@@double_quote@@>OnX viewer_read_scene @current@ @this.value@ @suffix@this.value@ staticScene</variable>";
  static const char iv_viewer_onx_23[] = "            <activate exec=@@double_quote@@DLD@@double_quote@@>OnX ui_set_fileChooser . *.iv;*.hiv;*.wrl open DLD @{OnX_Scene_read_iv.script}@</activate>";
  static const char iv_viewer_onx_24[] = "            <activate exec=@@double_quote@@DLD@@double_quote@@>OnX ui_show fileChooser</activate>";
  static const char iv_viewer_onx_25[] = "          </widget>";
  static const char iv_viewer_onx_26[] = "          <widget class=@@double_quote@@MenuItem@@double_quote@@>";
  static const char iv_viewer_onx_27[] = "            <label>Print...</label>";
  static const char iv_viewer_onx_28[] = "            <activate exec=@@double_quote@@DLD@@double_quote@@>OnX ui_show_dialog $ONXROOT/scripts/OnX/InputPrint.onx OnX_InputPrint_dialog</activate>";
  static const char iv_viewer_onx_29[] = "          </widget>";
  static const char iv_viewer_onx_30[] = "          <widget class=@@double_quote@@MenuItem@@double_quote@@>";
  static const char iv_viewer_onx_31[] = "            <label>Exit</label>";
  static const char iv_viewer_onx_32[] = "            <accelerator>Ctrl+q</accelerator>";
  static const char iv_viewer_onx_33[] = "            <activate exec=@@double_quote@@DLD@@double_quote@@>OnX ui_exit</activate>";
  static const char iv_viewer_onx_34[] = "          </widget>";
  static const char iv_viewer_onx_35[] = "        </widget>";
  static const char iv_viewer_onx_36[] = "      </widget>";
  static const char iv_viewer_onx_37[] = "";
  static const char iv_viewer_onx_38[] = "      <!-- A Scene PDM.  -->";
  static const char iv_viewer_onx_39[] = "      <widget class=@@double_quote@@CascadeButton@@double_quote@@>";
  static const char iv_viewer_onx_40[] = "        <label>Scene</label>";
  static const char iv_viewer_onx_41[] = "        <widget class=@@double_quote@@Menu@@double_quote@@>";
  static const char iv_viewer_onx_42[] = "          <widget class=@@double_quote@@MenuItem@@double_quote@@>";
  static const char iv_viewer_onx_43[] = "            <label>Clear</label>";
  static const char iv_viewer_onx_44[] = "            <activate exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_clear @current@ all</activate>";
  static const char iv_viewer_onx_45[] = "          </widget>";
  static const char iv_viewer_onx_46[] = "";
  static const char iv_viewer_onx_47[] = "          <widget class=@@double_quote@@MenuItem@@double_quote@@>";
  static const char iv_viewer_onx_48[] = "            <label>View all</label>";
  static const char iv_viewer_onx_49[] = "            <activate exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_view_all @current@</activate>";
  static const char iv_viewer_onx_50[] = "          </widget>";
  static const char iv_viewer_onx_51[] = "";
  static const char iv_viewer_onx_52[] = "          <widget class=@@double_quote@@MenuItem@@double_quote@@>";
  static const char iv_viewer_onx_53[] = "            <label>Black back</label>";
  static const char iv_viewer_onx_54[] = "            <activate exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_set_color @current@ black</activate>";
  static const char iv_viewer_onx_55[] = "          </widget>";
  static const char iv_viewer_onx_56[] = "          <widget class=@@double_quote@@MenuItem@@double_quote@@>";
  static const char iv_viewer_onx_57[] = "            <label>White back</label>";
  static const char iv_viewer_onx_58[] = "            <activate exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_set_color @current@ white</activate>";
  static const char iv_viewer_onx_59[] = "          </widget>";
  static const char iv_viewer_onx_60[] = "";
  static const char iv_viewer_onx_61[] = "        </widget>";
  static const char iv_viewer_onx_62[] = "      </widget>";
  static const char iv_viewer_onx_63[] = "";
  static const char iv_viewer_onx_64[] = "    </widget>";
  static const char iv_viewer_onx_65[] = "";
  static const char iv_viewer_onx_66[] = "    <widget class=@@double_quote@@SoExaminerViewer@@double_quote@@>";
  static const char iv_viewer_onx_67[] = "      <create exec=@@double_quote@@DLD@@double_quote@@>OnX set_variable tmp @session@command_files@</create>";
  static const char iv_viewer_onx_68[] = "<!--  <create exec=@@double_quote@@DLD@@double_quote@@>OnX set_variable tmp @filter@*iv@{tmp}@</create>";
  static const char iv_viewer_onx_69[] = "      <create exec=@@double_quote@@DLD@@double_quote@@>OnX ui_echo @{tmp}@</create> -->";
  static const char iv_viewer_onx_70[] = "      <create exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_read_scene @this@ @{tmp}@  guessed staticScene</create>";
  static const char iv_viewer_onx_71[] = "";
  static const char iv_viewer_onx_72[] = "      <create exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_view_all @this@</create>";
  static const char iv_viewer_onx_73[] = "<!--  <create exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_set_background_color @this@ 0.3 0.2 0.2</create> -->";
  static const char iv_viewer_onx_74[] = "      <create exec=@@double_quote@@DLD@@double_quote@@>OnX ui_set_current_widget @this@</create>";
  static const char iv_viewer_onx_75[] = "    </widget>";
  static const char iv_viewer_onx_76[] = "";
  static const char iv_viewer_onx_77[] = "";
  static const char iv_viewer_onx_78[] = "  <widget class=@@double_quote@@FileSelection@@double_quote@@ name=@@double_quote@@fileChooser@@double_quote@@>";
  static const char iv_viewer_onx_79[] = "    <title>File selection</title>";
  static const char iv_viewer_onx_80[] = "    <show>no</show>";
  static const char iv_viewer_onx_81[] = "  </widget>";
  static const char iv_viewer_onx_82[] = "";
  static const char iv_viewer_onx_83[] = "  </widget>";
  static const char iv_viewer_onx_84[] = "</OnX>";
  static unsigned int iv_viewer_onx_linen = 85;
  static const char* iv_viewer_onx_lines[] = {
iv_viewer_onx_0
,iv_viewer_onx_1
,iv_viewer_onx_2
,iv_viewer_onx_3
,iv_viewer_onx_4
,iv_viewer_onx_5
,iv_viewer_onx_6
,iv_viewer_onx_7
,iv_viewer_onx_8
,iv_viewer_onx_9
,iv_viewer_onx_10
,iv_viewer_onx_11
,iv_viewer_onx_12
,iv_viewer_onx_13
,iv_viewer_onx_14
,iv_viewer_onx_15
,iv_viewer_onx_16
,iv_viewer_onx_17
,iv_viewer_onx_18
,iv_viewer_onx_19
,iv_viewer_onx_20
,iv_viewer_onx_21
,iv_viewer_onx_22
,iv_viewer_onx_23
,iv_viewer_onx_24
,iv_viewer_onx_25
,iv_viewer_onx_26
,iv_viewer_onx_27
,iv_viewer_onx_28
,iv_viewer_onx_29
,iv_viewer_onx_30
,iv_viewer_onx_31
,iv_viewer_onx_32
,iv_viewer_onx_33
,iv_viewer_onx_34
,iv_viewer_onx_35
,iv_viewer_onx_36
,iv_viewer_onx_37
,iv_viewer_onx_38
,iv_viewer_onx_39
,iv_viewer_onx_40
,iv_viewer_onx_41
,iv_viewer_onx_42
,iv_viewer_onx_43
,iv_viewer_onx_44
,iv_viewer_onx_45
,iv_viewer_onx_46
,iv_viewer_onx_47
,iv_viewer_onx_48
,iv_viewer_onx_49
,iv_viewer_onx_50
,iv_viewer_onx_51
,iv_viewer_onx_52
,iv_viewer_onx_53
,iv_viewer_onx_54
,iv_viewer_onx_55
,iv_viewer_onx_56
,iv_viewer_onx_57
,iv_viewer_onx_58
,iv_viewer_onx_59
,iv_viewer_onx_60
,iv_viewer_onx_61
,iv_viewer_onx_62
,iv_viewer_onx_63
,iv_viewer_onx_64
,iv_viewer_onx_65
,iv_viewer_onx_66
,iv_viewer_onx_67
,iv_viewer_onx_68
,iv_viewer_onx_69
,iv_viewer_onx_70
,iv_viewer_onx_71
,iv_viewer_onx_72
,iv_viewer_onx_73
,iv_viewer_onx_74
,iv_viewer_onx_75
,iv_viewer_onx_76
,iv_viewer_onx_77
,iv_viewer_onx_78
,iv_viewer_onx_79
,iv_viewer_onx_80
,iv_viewer_onx_81
,iv_viewer_onx_82
,iv_viewer_onx_83
,iv_viewer_onx_84
};
