//
// Example to produce a :
//   AIDA_example_A2R.[aida,root,hdf5,hbook]
// data file used by :
//   examples/ROOT/A2R.C
//
// Only the ROOT class TRandom is used here, the rest is done
// by using AIDA and C/C++ standard libs.
//

int A2R_write() {

  std::string format("aida"); //or "xml"
  //std::string format("hbook");
  //std::string format("hdf5"); //with an OpenScientist AIDA implementation.
  //std::string format("root");

  bool compress = true;

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) return 1;

  AIDA::ITreeFactory* trf = aida->createTreeFactory();
  if(!trf) return 1;

  std::string file("AIDA_example_A2R");
  file += "."+format;

  std::string opts;
  if(format=="root") {
    //if(opts.size()) opts += ";";
    //opts += "streaming=native";
  }
  if(compress) {
    if(opts.size()) opts += ";";
    opts += "compress=yes";
  }

  AIDA::ITree* tree = trf->create(file,format,false,true,opts);
  delete trf;
  if(!tree) return 1;

  std::cout << "file " << file << " created." << std::endl;

  AIDA::IHistogramFactory* hf = aida->createHistogramFactory(*tree);
  if(!hf) return 1;

  TRandom r;

 {std::string name = "histo_1D";
  if(format=="hbook") name = "10";
  AIDA::IHistogram1D* h1D = hf->createHistogram1D(name,100,-5,5);
  if(!h1D) return 1;      
  for(int i=0;i<10000;i++) h1D->fill(r.Gaus());}
      
 {std::string name = "histo_2D";
  if(format=="hbook") name = "20";
  AIDA::IHistogram2D* h2D = hf->createHistogram2D(name,100,-5,5,100,-5,5);
  if(!h2D) return 1;      
  for(int i=0;i<10000;i++) h2D->fill(r.Gaus(),r.BreitWigner());}

  //NOTE : IProfile1D ROOT streaming available only from a 16.11.2.
  if(format!="root") {
   {std::string name = "profile_1D";
    if(format=="hbook") name = "30";
    AIDA::IProfile1D* p1D = hf->createProfile1D(name,100,-5,5);
    if(!p1D) return 1;
    for(int i=0;i<10000;i++) {
      p1D->fill(r.Gaus(),r.BreitWigner());
    }}

   {std::string name = "profile_2D";
    if(format=="hbook") name = "40";
    AIDA::IProfile2D* p2D = hf->createProfile2D(name,100,-5,5,100,-5,5);
    if(!p2D) return 1;
    for(int i=0;i<10000;i++) {
      p2D->fill(r.Gaus(),r.Gaus(),r.BreitWigner());
    }}

  }
  
  tree->commit();

  delete hf;
  delete aida;

  return 0; //Yeee...
}
