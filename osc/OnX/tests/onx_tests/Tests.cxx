//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

// Example of working on picked objects.

//Slash:
#include <Slash/UI/IUI.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Data/IHandlersIterator.h>
#include <Slash/Data/IHandler.h>
#include <Slash/Data/IAccessor.h>

#include <inlib/sout>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnX_exa_picked_objects(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
// DLD version.
// See OnX/examples/Python/Onx_exa_picked_objects for a Python version.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::Core::ISession& session = aUI.session();

  Slash::Data::IProcessor* da = Slash::accessorManager(session);
  if(!da) return;

 {std::vector<std::string> args;
  args.push_back("SceneGraph");
  args.push_back("highlight==true");
  da->execute("collect",args);}

 {std::vector<std::string> args;
  args.push_back("name");
  args.push_back("");
  da->execute("filter",args);}

  std::ostream& out = session.out();
  out << "Picked objects :" << std::endl;

  Slash::Data::IHandlersIterator* hit = da->handlersIterator();
  if(hit) {
    for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
      Slash::Data::IAccessor* accessor = h->type();
      void* obj = h->object();
      //void* tag = h->tag();
      out << "Accessor " << inlib::sout(accessor->name())
          << " object " << obj
          << std::endl;
    }  
    delete hit;
  }
}

}
