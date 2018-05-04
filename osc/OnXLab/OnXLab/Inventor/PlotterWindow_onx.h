//
//  This file had been produced automatically
// by the obuild_wrap program from the file :
//    PlotterWindow.onx
//
  static const char PlotterWindow_onx_0[] = "  <widget class=@@double_quote@@Window@@double_quote@@ name=@@double_quote@@@name@@window_@@@double_quote@@>";
  static const char PlotterWindow_onx_1[] = "    <geometry>500x500+0+0</geometry>";
  static const char PlotterWindow_onx_2[] = "    <title>osc-plot</title>";
  static const char PlotterWindow_onx_3[] = "    <decoration>no_close_button</decoration>";
  static const char PlotterWindow_onx_4[] = "    <destroy exec=@@double_quote@@DLD@@double_quote@@>OnX ui_exit</destroy>";
  static const char PlotterWindow_onx_5[] = "    <show>no</show>  ";
  static const char PlotterWindow_onx_6[] = "";
  static const char PlotterWindow_onx_7[] = "    <widget class=@@double_quote@@VBox@@double_quote@@> ";
  static const char PlotterWindow_onx_8[] = "      <expand>first</expand>";
  static const char PlotterWindow_onx_9[] = "      <sizeOfFixed>30</sizeOfFixed>";
  static const char PlotterWindow_onx_10[] = "";
  static const char PlotterWindow_onx_11[] = "      <widget class=@@double_quote@@PlanePageViewer@@double_quote@@ name=@@double_quote@@@name@@plotter_@@@double_quote@@>";
  static const char PlotterWindow_onx_12[] = "        <create exec=@@double_quote@@DLD@@double_quote@@>OnX ui_set_current_widget @this@</create>";
  static const char PlotterWindow_onx_13[] = "        <create exec=@@double_quote@@DLD@@double_quote@@>OnX page_set_title @this@ OnXLab</create>";
  static const char PlotterWindow_onx_14[] = "        <create exec=@@double_quote@@DLD@@double_quote@@>OnX page_title_visible @this@ false</create>";
  static const char PlotterWindow_onx_15[] = "        <create exec=@@double_quote@@DLD@@double_quote@@>OnX page_create_regions @this@ SoPlotterRegion 1 1 0</create>";
  static const char PlotterWindow_onx_16[] = "        <create exec=@@double_quote@@DLD@@double_quote@@>OnX region_set_color @this@ white</create>";
  static const char PlotterWindow_onx_17[] = "        <create exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_set_decoration @this@ false</create>";
  static const char PlotterWindow_onx_18[] = "        <create exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_set_viewing @this@ false</create>";
  static const char PlotterWindow_onx_19[] = "";
  static const char PlotterWindow_onx_20[] = "        <collect exec=@@double_quote@@DLD@@double_quote@@>OnX viewer_collect @this@</collect>";
  static const char PlotterWindow_onx_21[] = "        <popupItems>No highlighted</popupItems>";
  static const char PlotterWindow_onx_22[] = "        <variable name=@@double_quote@@Pages_popup_item_script@@double_quote@@>";
  static const char PlotterWindow_onx_23[] = "          No highlighted@@back_slash@@nOnX region_no_highlighted @this@";
  static const char PlotterWindow_onx_24[] = "        </variable>";
  static const char PlotterWindow_onx_25[] = "        <popup exec=@@double_quote@@DLD@@double_quote@@>OnX ui_execute_script DLD @next@this.value@{Pages_popup_item_script}@strip_key@</popup>";
  static const char PlotterWindow_onx_26[] = "      </widget>";
  static const char PlotterWindow_onx_27[] = "";
  static const char PlotterWindow_onx_28[] = "      <widget class=@@double_quote@@PushButton@@double_quote@@>";
  static const char PlotterWindow_onx_29[] = "        <label>Leave plotter</label>";
  static const char PlotterWindow_onx_30[] = "        <activate exec=@@double_quote@@DLD@@double_quote@@>OnX ui_exit</activate>";
  static const char PlotterWindow_onx_31[] = "      </widget>";
  static const char PlotterWindow_onx_32[] = "    </widget>";
  static const char PlotterWindow_onx_33[] = "  </widget>";
  static unsigned int PlotterWindow_onx_linen = 34;
  static const char* PlotterWindow_onx_lines[] = {
PlotterWindow_onx_0
,PlotterWindow_onx_1
,PlotterWindow_onx_2
,PlotterWindow_onx_3
,PlotterWindow_onx_4
,PlotterWindow_onx_5
,PlotterWindow_onx_6
,PlotterWindow_onx_7
,PlotterWindow_onx_8
,PlotterWindow_onx_9
,PlotterWindow_onx_10
,PlotterWindow_onx_11
,PlotterWindow_onx_12
,PlotterWindow_onx_13
,PlotterWindow_onx_14
,PlotterWindow_onx_15
,PlotterWindow_onx_16
,PlotterWindow_onx_17
,PlotterWindow_onx_18
,PlotterWindow_onx_19
,PlotterWindow_onx_20
,PlotterWindow_onx_21
,PlotterWindow_onx_22
,PlotterWindow_onx_23
,PlotterWindow_onx_24
,PlotterWindow_onx_25
,PlotterWindow_onx_26
,PlotterWindow_onx_27
,PlotterWindow_onx_28
,PlotterWindow_onx_29
,PlotterWindow_onx_30
,PlotterWindow_onx_31
,PlotterWindow_onx_32
,PlotterWindow_onx_33
};
