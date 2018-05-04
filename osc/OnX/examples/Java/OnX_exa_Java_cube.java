
public class OnX_exa_Java_cube {
  public static void main(String[] aArgs) {
    boolean createUI = true;
    boolean showUI = true;
    OnX.Main onx_main = new OnX.Main(createUI,showUI,aArgs);

    OnX.ISession session = onx_main.session();
    OnX.IUI ui = session.ui();

    ui.currentPage().currentRegion().clear();  // To clear the OnX banner.

    OnX.SoPage soPage = ui.currentPage();
    // title SoSFString field is accessed with getTitle.
    soPage.getTitle().setValue("OnX/Cube");

    OnX.SoRegion soRegion = soPage.currentRegion();
    soRegion.getColor().setValue(new OnX.SbColor(0,0,0));
    OnX.SoSeparator soSep = soRegion.cast_SoDisplayRegion().getStaticScene();

    OnX.SoCube soCube = new OnX.SoCube();
    soCube.getWidth().setValue(4);
    soSep.addChild(soCube);

    System.out.println("managers :");
    String[] mgrs = session.managerNames();
    for(int index=0;index<mgrs.length;index++) {
      System.out.println(" "+mgrs[index]);
    }

    ui.steer();
  }        
}
