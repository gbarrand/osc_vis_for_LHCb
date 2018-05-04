#ifndef Slash_IManager_h
#define Slash_IManager_h

/**
 * Pure abstract interface for a named manager.
 * 
 * @author G.Barrand
 */

#include <string>

namespace Slash {

namespace Core {

class IManager {
public:
  virtual ~IManager() {}
  virtual void* cast(const std::string&) const = 0;
public:
  /**
   * @return The name of the manager.
   */
  virtual std::string name() const = 0;

  /**
   * Set the verbosity of the manager.
   * @param aVerbosity The verbosity. If zero not verbose.
   */
  //virtual void setVerboseLevel(int aVerbosity) = 0;

  /**
   * @return The verbosity of the manager.
   */
  //virtual int verboseLevel() const = 0;

  /**
   * A manager handles a list of string keyed parameters.
   * It permits to set a parameter.
   * @param aKey The string key.
   * @param aValue The value.
   */
  //virtual bool setParameter(const std::string& aKey,const std::string& aValue) = 0;

  /**
   * Retreive the value of a parameter knowing its key.
   * @param aKey The string key.
   * @param aValue The value.
   * @return The status of the execution.
   */
  //virtual bool parameterValue(const std::string& aKey,std::string& aValue) = 0;

};

} //Core

} //Slash

#endif
