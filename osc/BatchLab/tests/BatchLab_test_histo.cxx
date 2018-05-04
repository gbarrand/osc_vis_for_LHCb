
#include <BatchLab/Core/Histogram.h>

#include <iostream>
#include <cstdio>

int main(int,char**) {

  typedef unsigned long ul;

  BatchLab::Histogram1D* bh1d = 
    new BatchLab::Histogram1D(std::cout,"h1d","test 1d",50,-3,3);

  ::printf("BatchLab::Histogram1D (BH1D) : %lu\n",(ul)bh1d);

  ::printf("BH1D::cast(BH1D) : %lu\n",(ul)bh1d->cast("BatchLab::Histogram1D"));

  AIDA::IBaseHistogram* abh = static_cast<AIDA::IBaseHistogram*>(bh1d);
  ::printf("static_cast<ABaseH>(BH1D) : %lu\n",(ul)abh);

  AIDA::IHistogram1D* ah1d = 
    (AIDA::IHistogram1D*)abh->cast("AIDA::IHistogram1D");
  ::printf("ABaseH::cast(AH1D) : %lu\n",(ul)ah1d);

  ::printf("AH1D::cast(BH1D) : %lu (BH1D %lu)\n",
	   (ul)ah1d->cast("BatchLab::Histogram1D"),(ul)bh1d);

  return 0;
}
