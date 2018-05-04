// this :
#include <User/Event.h>

#include <User/Track.h>

#include <stdlib.h>   //rand.
//#include <math.h>   
static double shoot() {
  // Shoot random numbers in [0,1] according a flat distribution.
  double value  = (double)::rand();
  value /= (double)RAND_MAX;
  return value;
}

User::Event::Event(){
  // Create some data.
  double s = 100; //scale.
  fTracks.push_back(User::Track(s*shoot(),s*shoot(),s*shoot()));
  fTracks.push_back(User::Track(s*shoot(),s*shoot(),s*shoot()));
  fTracks.push_back(User::Track(s*shoot(),s*shoot(),s*shoot()));
  fTracks.push_back(User::Track(s*shoot(),s*shoot(),s*shoot()));
  fTracks.push_back(User::Track(s*shoot(),s*shoot(),s*shoot()));
}
User::Event::Event(const Event& aFrom):fTracks(aFrom.fTracks){}
User::Event::~Event() {}
User::Event& User::Event::operator=(const Event& aFrom) {
  fTracks = aFrom.fTracks;
  return *this;
}
const std::vector<User::Track>& User::Event::tracks() const {
  return fTracks;
}
