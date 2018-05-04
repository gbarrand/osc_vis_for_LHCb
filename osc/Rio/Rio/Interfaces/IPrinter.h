#ifndef Rio_IPrinter_h
#define Rio_IPrinter_h

#include <ostream>

namespace Rio {

/**
 * @class IPrinter
 *
 *  IPrinter is the interface to handle a basic printer.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IPrinter {
public:
  virtual ~IPrinter() {}

  virtual std::ostream& out() const = 0;

  /** To handle indentation. */
  virtual void indent() = 0;
  virtual void increment() = 0;
  virtual void decrement() = 0;
};

}

#endif




