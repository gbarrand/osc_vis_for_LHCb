
public class OnX_exa_Java_detector {
  public static void main(String[] aArgs) {
    boolean createUI = true;
    boolean showUI = true;
    OnX.Main onx_main = new OnX.Main(createUI,showUI,aArgs);

    OnX.ISession session = onx_main.session();
    OnX.IUI ui = session.ui();

    ui.currentPage().currentRegion().clear();  // To clear the OnX banner.

    OnX.SoPage soPage = ui.currentPage();
    // title SoSFString field is accessed with getTitle.
    soPage.getTitle().setValue("Java/Detector");

    OnX.SoRegion soRegion = soPage.currentRegion();
    soRegion.getColor().setValue(new OnX.SbColor(0.8f,0.8f,0.8f));
    OnX.SoSeparator soSep = soRegion.cast_SoDisplayRegion().getStaticScene();

    OnX.SoScale soScale = new OnX.SoScale();
    soScale.getScaleFactor().setValue(new OnX.SbVec3f(0.1f,0.1f,0.1f));
    soSep.addChild(soScale);

    OnX.SoDetectorExample soDet = new OnX.SoDetectorExample();
    soSep.addChild(soDet);

    ui.steer();
  }        
}
