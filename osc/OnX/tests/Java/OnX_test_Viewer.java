
import java.awt.Frame;

import OnX.Viewer;
import OnX.Native;

class OnX_Viewer_test {
  public   static void main(String[] args) {

    Frame frame = new Frame();
    //frame.setSize(400,400);
    Viewer viewer = new Viewer(0);
    frame.add(viewer);
    frame.pack();
    frame.show();

    long v = viewer.getNativeViewer();
    
    System.loadLibrary("OnXJavaSWIG");
    long d = Native.new_OnX_Detector();
    Native.OnX_Detector_visualize(d,v);

    /*
    long root = Native.IViewer_getRegionNodeByName(v,"static");
    long material = Native.new_SoMaterial();
    Native.SoNode_set(material,"diffuseColor 1 0 0");        
    Native.SoSeparator_addChild(root,material);
    long cube = Native.new_SoCube();
    Native.SoSeparator_addChild(root,cube);
    */

    Native.IViewer_viewAll(v);
    Native.IViewer_set(v,"viewing","3D");
    Native.IViewer_set(v,"pageTitle","Java/Detector");
    Native.IViewer_set(v,"pageColor","0.7 0.7 0.7");
    Native.IViewer_set(v,"regionColor","0.8 0.8 0.8");
    Native.IViewer_show(v);

  }        
}
