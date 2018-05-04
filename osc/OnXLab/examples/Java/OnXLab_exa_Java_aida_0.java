import hep.aida.*;

import java.util.Random;

public class OnXLab_exa_Java_aida_0 {
  public static void main(String[] args) {
    //We loop to test startup, cleanup of everythhing.
    //for(int i=0;i<100;i++) 
    example(); 
  }        
  public static void example() { 
    IAnalysisFactory aida = IAnalysisFactory.create();

    ITreeFactory tf = aida.createTreeFactory();
    ITree tree = tf.create();

    IHistogramFactory hf = aida.createHistogramFactory(tree);
    IHistogram1D h = hf.createHistogram1D("h","Rand gauss",100,-5,5);

    Random r = new Random();
    for(int i=0;i<10000;i++) {
      h.fill(r.nextGaussian(),1);
    }

    System.out.println("mean "+h.mean()+", sigma "+h.rms());

    // Get current plotter :
    IPlotterFactory plotterFactory = aida.createPlotterFactory();
    IPlotter plotter = plotterFactory.create("");
    plotter.createRegions(1,1,0);
    plotter.setTitle("aidaex0");
    IPlotterRegion region = plotter.currentRegion();
    region.setTitle("The famous normal distribution");
    region.style().xAxisStyle().setLabel("Random gauss");
    region.style().yAxisStyle().setLabel("Entries");
    region.plot(h);
    plotter.show();
    plotter.interact();
    
  }
}
