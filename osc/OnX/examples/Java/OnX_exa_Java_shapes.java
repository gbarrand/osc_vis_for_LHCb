
public class OnX_exa_Java_shapes {
  public static void main(String[] aArgs) {
    boolean createUI = true;
    boolean showUI = true;
    OnX.Main onx_main = new OnX.Main(createUI,showUI,aArgs);

    OnX.ISession session = onx_main.session();
    OnX.IUI ui = session.ui();

    OnX.IProcessor da = session.accessorManager(); // data accessor.

    ui.executeScript("DLD","OnX OnX_shape_initialize");

    ui.currentPage().currentRegion().clear();  // To clear the OnX banner.

    OnX.SoPage soPage = ui.currentPage();
    // title SoSFString field is accessed with getTitle.
    soPage.getTitle().setValue("Java/Shapes");

    OnX.SoRegion soRegion = soPage.currentRegion();
    soRegion.getColor().setValue(new OnX.SbColor(0.8f,0.8f,0.8f));

    session.setParameter("modeling.transparency","0.4");
    session.setColor("red");
    da.collect("OnX_Shape","type==\"Cube\"");
    da.visualize();

    session.setParameter("modeling.transparency","0");
    session.setColor("yellow");
    da.collect("OnX_Shape","type==\"Hole\"");
    da.visualize();

    session.setColor("blue");
    session.setParameter("modeling.modeling","texts");
    //da.collect("OnX_Shape","type==\"Sphere\" && size==1");
    da.collect("OnX_Shape","type==\"Sphere\"");
    da.visualize();

    OnX.IWidget widget = ui.currentWidget();
    OnX.ISoViewer soViewer = widget.cast_ISoViewer();
    //FIXME : test return value
    soViewer.setFeedbackVisibility(true);

    ui.steer();
  }        
}
