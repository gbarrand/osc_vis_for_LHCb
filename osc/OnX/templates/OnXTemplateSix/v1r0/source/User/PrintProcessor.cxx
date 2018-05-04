// this :
#include <User/PrintProcessor.h>

#include <User/Event.h>
#include <iostream>

User::PrintProcessor::PrintProcessor(Framework& aFramework)
:Processor(aFramework){}

User::PrintProcessor::PrintProcessor(const PrintProcessor& aFrom)
:Processor(aFrom){}

User::PrintProcessor::~PrintProcessor() {}

User::PrintProcessor& User::PrintProcessor::operator=(const PrintProcessor&) {
  return *this;
}

void User::PrintProcessor::processEvent(const Event& aEvent) {
  const std::vector<Track>& tracks = aEvent.tracks();
  unsigned int number = tracks.size();
  std::cout << " Event : number of tracks = " << number << std::endl;
  for(unsigned int index=0;index<number;index++) {
    const Track& track = tracks[index];
    std::cout << " px = " << track.px() 
              << ",py = " << track.py() 
              << ",pz = " << track.pz() 
              << std::endl;
  }
}
