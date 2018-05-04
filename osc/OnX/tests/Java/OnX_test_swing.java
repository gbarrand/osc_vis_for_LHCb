//

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JTree;
import javax.swing.JSplitPane;
import javax.swing.JScrollPane;
import javax.swing.JPanel;
import java.awt.*;

import java.lang.Runtime;
import java.io.*;

//import bsh.Interpreter;

public class OnX_swing_test {
  static JFrame frame;
  static MyActionListener sActionListener;
  class MyMenuItem extends JMenuItem {
    public MyMenuItem(String aScript) {
      super(aScript);
      fScript = aScript;
      addActionListener(sActionListener);
    }
    public String fScript;
  }
  class MyActionListener implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      MyMenuItem item = (MyMenuItem)e.getSource();
      System.out.println(item.fScript);
      if(item.fScript=="Exit") {
        System.exit(0);
      } else {
        Runtime rt = Runtime.getRuntime();
        try {
          System.out.println("try to do dir...");
          rt.exec("dir > out");
        } catch(IOException x) {
        } finally {
        }
      }
    }
  }
  public OnX_swing_test() {
    sActionListener = new MyActionListener();

    frame = new JFrame();

    JTree tree = new JTree();
    JTree tree2 = new JTree();

    // Ok.
    //JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
    //                                  tree,tree2);
    JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
    //JSplitPane splitPane = new JSplitPane();
    frame.getContentPane().add(splitPane);

    // Ok.
    //splitPane.setLeftComponent(tree);
    //splitPane.setRightComponent(tree2);

    //splitPane.setLeftComponent(tree);
    splitPane.add(tree);
    splitPane.add(tree2);

    JMenuBar menuBar = new JMenuBar();
    frame.setJMenuBar(menuBar);

    JMenu menu = new JMenu("File");
    menuBar.add(menu);

    MyMenuItem menuItem = new MyMenuItem("Exit");
    menu.add(menuItem);
    MyMenuItem menuItem2 = new MyMenuItem("Exit2");
    menu.add(menuItem2);
    //MyActionListener myActionListener = new MyActionListener();
    //menuItem.addActionListener(myActionListener);

    MyMenuItem myMenuItem = new MyMenuItem("Hello");
    menu.add(myMenuItem);

    /*
    Interpreter i = new Interpreter();
    i.setVariable("foo",5);
    i.eval("bar = foo * 10");
    System.out.println(" bar = " + i.getVariable("bar"));
    */

    frame.pack();
    frame.show();
  }
  public static void main(String[] args) {
    System.out.println("Hello xx");
    new OnX_swing_test();
  }
}





