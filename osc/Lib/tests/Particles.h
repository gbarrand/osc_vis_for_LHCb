#include <string>
#include <vector>
#include <ostream>

namespace Lib {

class Particle;

class Decay : public std::vector<Particle> {
public:
  Decay();
  virtual ~Decay();
  void dump(std::ostream&);
};

class Particle {
public:
  Particle(const std::string& aName,const double aMass) 
  :fName(aName),fMass(aMass){}
  Particle(const Particle& aParticle)
  :fName(aParticle.fName)
  ,fMass(aParticle.fMass)
  ,fDecays(aParticle.fDecays){}
  virtual ~Particle() {}
  Particle& operator=(const Particle& aParticle){
    fName = aParticle.fName; 
    fMass = aParticle.fMass;
    fDecays = aParticle.fDecays;
    return *this;
  }
  void addDecay(const Decay& aDecay) {
    fDecays.push_back(aDecay);
  }
  std::string name() const { return fName;}
  void dump(std::ostream& a_out) {
    a_out << fName << " -> " << std::endl;
    unsigned int number = fDecays.size();
    for(unsigned int index=0;index<number;index++) {
      Decay& decay = fDecays[index];
      a_out << "     ";
      decay.dump(a_out);
    }
  }
private:
  std::string fName;
  double fMass;
  std::vector<Decay> fDecays;
};

}

Lib::Decay::Decay(){}
Lib::Decay::~Decay(){}
void Lib::Decay::dump(std::ostream& a_out) {
  unsigned int number = size();
  for(unsigned int index=0;index<number;index++) {
    const Lib::Particle& particle = (*this)[index];
    if(index) a_out << " + ";
    a_out << particle.name();
  }
  a_out << std::endl;
}


