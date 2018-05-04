#ifndef Slash_Core_ISession_h
#define Slash_Core_ISession_h

/**
 * Pure abstract interface for a session.
 *
 * A session manages named manager.
 * 
 * @author G.Barrand
 */

#include <string>
#include <vector>
#include <ostream>

namespace Slash {namespace Core { class IManager;}}
namespace Slash {namespace Core { class IWriter;}}

namespace Slash {

namespace Core {

class ISession {
public:
  virtual ~ISession() {};
  /**
   * Safe cast.
   */
  virtual void* cast(const std::string&) const = 0;
public:
  /**
   * @return The name of the session.
   */
  //virtual std::string name() const = 0;

  /**
   * Add a manager.
   */
  virtual void addManager(IManager*) = 0;

  /**
   * Remove a manager.
   */
  virtual void removeManager(IManager*) = 0;

  /**
   * Find a manager knowing its name.
   * @param aName Name of the manager to find.
   * @param forward true, search forward, else backward.
   * @return The manager, zero is not found.
   */
  virtual IManager* findManager(const std::string& aName,bool forward = true) const = 0;

  /**
   * Destroy a manager.
   * @param aName Name of the manager to destroy.
   */
  virtual void destroyManager(const std::string& aName) = 0;

  /**
   * Return the printer (a terminal writer). A session has always a printer.
   * @return The printer.
   */
  virtual IWriter& printer() = 0;

  /**
   * Return the printer (a terminal writer). A session has always a printer.
   * @return The printer.
   */

  virtual std::ostream& out() = 0;

  //deprecated. use out().
  virtual std::ostream& cout() = 0;

  /**
   * Load a DLL. If the aPackage argument is given, an "extern C"
   * function "aPackageInitialize(ISession&)" is searched and executed.
   * @param aFile The DLL to load.
   * @param aPackage Package name.
   * @return The status of the execution.
   */
  virtual bool load(const std::string& aFile,const std::string& aPackage) = 0;

  /**
   * Permit to redirect or not the printer output to stdout.
   * @param aRedirect If false, output are redirected to stdout.
   */
  virtual void redirectOutput(bool aRedirect) = 0;

  /**
   * A session manages a list of string keyed parameters.
   * It permits to set a parameter.
   * @param aKey The string key.
   * @param aValue The value.
   */
  virtual bool setParameter(const std::string& aKey,const std::string& aValue) = 0;

  /**
   * Retreive the value of a parameter knowing its key.
   * @param aKey The string key.
   * @param aValue The value.
   * @return The status of the execution.
   */
  virtual bool parameterValue(const std::string& aKey,std::string& aValue) = 0;

  /**
   * Remove a parameter knowing its key.
   * @param aKey The string key.
   * @return The status of the execution.
   */
  virtual bool removeParameter(const std::string& aKey) = 0;

  /**
   * Get list of the available parameters (implementation-dependent)
   */
  virtual std::vector<std::string> availableParameters() const = 0;

  /**
   * Flush the printer output.
   */
  virtual bool flush() = 0;

  /**
   * Set the verbosity of the session.
   * @param aVerbosity The verbosity. If zero not verbose.
   */
  virtual void setVerboseLevel(int aVerbosity) = 0;

  /**
   * @return The verbosity of the session.
   */
  virtual int verboseLevel() const = 0;

  virtual unsigned int numberOfManagers() const = 0;
  virtual const IManager* manager(unsigned int) const = 0;

  /**
   * @return The list of manager names.
   */
  virtual std::vector<std::string> managerNames() const = 0;

  /**
   * @return The startup arguments.
   */
  virtual std::vector< std::pair<std::string,std::string> > arguments() const = 0;
};

} //Core

} //Slash

extern "C" {
  Slash::Core::ISession* Slash_create_session(const std::vector<std::string>& args = std::vector<std::string>());
}

#endif
