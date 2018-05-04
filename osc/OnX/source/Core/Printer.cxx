// this :
#include <OnX/Core/Printer.h>

#include <Slash/UI/IUI.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/UI.h>

#include <stdio.h>
#include <stdarg.h>

OnX::Printer::Printer()
:fSession(0)
,fEnabled(true)
,fFILE(0)
{
  //fFILE = ::fopen("OnX_debug","wb");
}

OnX::Printer::~Printer() {if(fFILE) ::fclose((FILE*)fFILE);}

void OnX::Printer::setSession(Slash::Core::ISession* aSession){fSession = aSession;}

bool OnX::Printer::write(const std::string& aString) {
  if(fFILE) {
    ::fprintf((FILE*)fFILE,"%s",aString.c_str());
  } else {
    Slash::UI::IUI* ui = 0;
    if(fEnabled && fSession && (ui = Slash::find_UI(*fSession)) ) {
      ui->put(aString);
    } else {
      ::printf("%s",aString.c_str());
    }
  }
  return true;
}
void OnX::Printer::disable() {fEnabled = false;}
void OnX::Printer::enable() {fEnabled = true;}
bool OnX::Printer::enabled() const {return fEnabled;}
bool OnX::Printer::flush() {return true;}
