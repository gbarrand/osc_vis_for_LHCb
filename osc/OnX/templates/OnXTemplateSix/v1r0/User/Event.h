#ifndef User_Event_h
#define User_Event_h

//
// Example of simple "user" event data.
//

#include <vector>

#include <User/Track.h>

namespace User {

class Event {
public:
  Event();
  Event(const Event&);
  virtual ~Event();
  Event& operator=(const Event&);
  const std::vector<User::Track>& tracks() const;
private:
  std::vector<User::Track> fTracks;
};

}

#endif
