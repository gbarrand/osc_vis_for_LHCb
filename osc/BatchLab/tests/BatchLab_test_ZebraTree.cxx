#include <Lib/Printer.h>
#include <Lib/Session.h>

#include <BatchLab/Zebra/ZebraManager.h>
#include <BatchLab/Zebra/ZebraTree.h>

int main () {{

  Lib::Printer printer;
  Lib::Session session(printer);

  BatchLab::ZebraManager zebraManager(session);

  BatchLab::ZebraTree zebraTree(session,"histo.hbook",true,false,zebraManager,1);

  } return 0;
}
