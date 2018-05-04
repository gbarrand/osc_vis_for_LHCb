//
//  All functions here should be Session callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::Core::ISession&);
//     void callback_with_arguments(Slash::Core::ISession&,
//                                  const std::vector<std::string>&);
//   }
//

#include <Slash/Core/ISession.h>

#include <Slash/Tools/names.h>

#include <OnXLab/Core/AnalysisFactory.h>

extern "C" {

/**
 * OnXLab_createAIDA
 * @aSession the Slash::Core::ISession to manage the AnalysisFactory.
 * 
 * Used by G4Lab::Manager::findAIDA.
 **/

void OnXLab_createAIDA(Slash::Core::ISession& aSession) {
  if(!aSession.findManager(Slash_AnalysisFactory)) {
    aSession.addManager(new OnXLab::AnalysisFactory(aSession));
    aSession.setParameter("OnXLab.tree","none");
  }
}

}
