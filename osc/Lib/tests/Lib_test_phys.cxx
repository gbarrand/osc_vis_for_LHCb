
#include <inlib/math>
#include <cmath>
#include <iostream>

namespace Lib {
  /////////
  // CSG :
  /////////
  static const double gr = 1;
  static const double cm = 1;
  static const double sec = 1;
  static const double erg = gr * (cm*cm)/(sec*sec);

  // Particle Data Booklet 1994.
  static const double c = 2.99792458e+10 * (cm/sec);
  static const double hbar = 1.05457266e-27 * (gr*cm*cm/sec);
  static const double kgrav = 6.67259e-8 * (cm*cm*cm)/(gr*sec*sec);

  static const double alpha = 1/137.0359895;

  static const double e =  4.8032068e-10 * ::sqrt(gr*cm*cm*cm/(sec*sec));

  static const double G_Fermi = 1.4321e-49 * (erg*cm*cm*cm);

  static const double m_e = 9.1093897e-28 * gr;
  static const double m_mu = 1.88356e-25 * gr;//Booklet ?

  static const double vie_mu = 2.197e-6 * sec;

  /////////////////
  // Other units :
  /////////////////
  // How Coulomb is defined ? Coulomb related to c ? Seems to be c/10 !
  static const double Cb = 2.99792458e+9 * ::sqrt(gr*cm*cm*cm/(sec*sec));

  // Rem : Cb/c around 0.1 * sqrt(gr*cm) !!!

  //  e_Cb = e/Cb sans dimension.
  static const double e_Cb  = 1.60217733e-19; //Redundant with e and Cb.
  // 1/e_Cb = 6.24150e+18  (Found in CLHEP/SystemOfUnits.h)

  // 1 Volt * 1 Cb = 1 Newton * 1 metre.
  //static const double Newton = 1 * kgr * m / (sec * sec);
  static const double Newton = 1e+5 * gr * cm / (sec * sec);
  //  1 Newton * 1 * cm = 10e5 * erg.
  static const double Volt = 100 * cm * Newton /Cb;
  //static const double Volt = 1e+7 * erg/Cb;
  //static const double Ampere = 1 * Cb/sec;

  static const double eV = e * Volt; //1.60217733e-12 * erg;
  // eV = e * Volt = Cb * e_Cb * 1e+7 * erg/Cb = e_Cb * 1e7 * erg
  static const double MeV = 1e+6 * eV;
  static const double m_e_MeV = 0.51099906;
  static const double m_mu_MeV = 105.658389;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include "Particles.h"

static void mu_decay() {

  Lib::Particle electron("e-",Lib::m_e);
  Lib::Particle nu_mu("nu_mu",0);
  Lib::Particle anti_nu_e("anti_nu_e",0);

  Lib::Particle muon("mu-",Lib::m_mu);
  Lib::Decay decay;
  decay.push_back(electron);
  decay.push_back(anti_nu_e);
  decay.push_back(nu_mu);
  muon.addDecay(decay);    

  muon.dump(std::cout);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


int main() {

  std::cout << " Booklet : Cb = " << Lib::Cb << " csg" << std::endl;
  std::cout << " Booklet : eV = " << Lib::eV << " erg" << std::endl;
  std::cout << " Booklet : Cb/c (csg) = " << Lib::Cb/Lib::c << " Why ?"<< std::endl;

 {double m_planck = ::sqrt(Lib::hbar * Lib::c / Lib::kgrav);
  std::cout << " Check : sqrt(hbar*c/kgrav) = " << m_planck << " gr" << std::endl;}

  // Check alpha :
 {double alpha = Lib::e * Lib::e / (Lib::hbar * Lib::c);
  std::cout << " Check : e2/hc = " << 1/alpha 
      << " (Booklet : alpha = " << 1/Lib::alpha << ")" << std::endl;}

 {double e_from_alpha =  ::sqrt(Lib::alpha * Lib::hbar * Lib::c);
  std::cout << " Check : sqrt(alpha*hbar*c) = " << e_from_alpha
      << " (Booklet : e = " << Lib::e << " csg)" << std::endl;}

 {double e_Cb = Lib::e / Lib::Cb;
  std::cout << " Check : e / Cb  = " << e_Cb
      << " (Booklet : e = " << Lib::e_Cb << " Cb)"<< std::endl;}

 {double m_e_MeV = (Lib::m_e * Lib::c * Lib::c)/Lib::MeV;
  std::cout << " Check : m_e * c2 / MeV = " << m_e_MeV
      << " (Booklet : m_e = " << Lib::m_e_MeV << " MeV)"<< std::endl;}

 {double m_mu_MeV = (Lib::m_mu * Lib::c * Lib::c)/Lib::MeV;
  std::cout << " Check : m_mu * c2 / MeV = " << m_mu_MeV
      << " (Booklet : m_mu = " << Lib::m_mu_MeV << " MeV)"<< std::endl;}

  // w_mu from the process : mu- -> e- + vebar + vmu :
 {double w_mu = ::pow(Lib::G_Fermi,2)  *  ::pow(Lib::m_mu,5) *
  ::pow(Lib::c,4) / (192 * ::pow(inlib::pi(),3) * ::pow(Lib::hbar,7));
  std::cout << " w_mu = " << w_mu << " / sec" << std::endl;

  std::cout << " t_mu = (1/w_mu) = " << 1/w_mu 
      << " (Booklet : t_mu = " << Lib::vie_mu << " sec)"<< std::endl;}

  mu_decay();

  return 0;
}
