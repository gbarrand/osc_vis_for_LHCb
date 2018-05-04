
import java.awt.Window;
import java.awt.Frame;
import java.awt.Panel;
import java.awt.Button;
//import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.GridLayout;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import javax.swing.JTree;

import SoAwt.SoViewer;
import Inventor;

class OnX_AWT_test {
  class MyButtonListener implements ActionListener {
    public void actionPerformed(ActionEvent aEvent) {
      System.exit(0);
    }
  }
  class MyComponentListener implements ComponentListener {
    public void componentHidden(ComponentEvent aEvent) {}
    public void componentMoved(ComponentEvent aEvent) {}
    public void componentShown(ComponentEvent aEvent) {}
    public void componentResized(ComponentEvent aEvent) {
      //System.out.println("resized");
      Frame frame = (Frame)aEvent.getSource();
      // How to tell the frame to recompute the layout ?
      //frame.doLayout();
    }
  }
  public OnX_AWT_test() {
    GridLayout layout = new GridLayout();
        
    Frame frame = new Frame();

    frame.setLayout(layout);
    frame.setResizable(true);
    frame.addComponentListener(new MyComponentListener());
    
    Button button = new Button("Exit");
    //button.setBackground(new Color(255,255,255));
    button.addActionListener(new MyButtonListener());
    frame.add(button);
      
    SoViewer viewer = new SoViewer();
    //viewer.setBackground(new Color(255,0,0));
    viewer.setSize(new Dimension(400,400));
    frame.add(viewer);
    
    /*
    SoViewer viewer2 = new SoViewer();
    viewer2.setBackground(new Color(255,0,0));
    viewer2.setSize(new Dimension(400,400));
    frame.add(viewer2);
    */


    JTree tree = new JTree();
    frame.add(tree);
    
    frame.pack();
    frame.show();

    // Build a scene in viewer.
    // Must be done AFTER the frame.pack so that Inventor
    // is inited and the native Inventor viewer is created.
    long root = Inventor.new_SoSeparator();
    Inventor.SoNode_ref(root);
    long cube = Inventor.new_SoCube();
    Inventor.SoNode_set(cube,"width 1");
    Inventor.SoSeparator_addChild(root,cube);
    viewer.setSceneGraph(root);
    viewer.viewAll();

    /*
    long root2 = Inventor.new_SoSeparator();
    Inventor.SoNode_ref(root2);
    long cylinder = Inventor.new_SoCylinder();
    //Inventor.SoNode_set(cylinder,"width 1");
    Inventor.SoSeparator_addChild(root2,cylinder);
    viewer2.setSceneGraph(root2);
    viewer2.viewAll();
    */

  }        
  public static void main(String[] args) {
    new OnX_AWT_test();
  }        
}

