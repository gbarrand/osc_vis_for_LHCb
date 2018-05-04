/**
@page rio_introduction Not ROOT

  Rio (for ROOT IO) is a REWRITING of the file IO system of ROOT.
 Rio is intended for people that looks for a free 
 stand alone file IO system, but do not want to enter in the whole 
 ROOT system (or in the whole logic of the core of a framework).

  Potential clients are people that develop their own framework
 and seek for a well defined IO package which is not another framework.

  Previous attempts (Rio-v1r*, Rio-v2*, RioGrande-v3*) were a repacking 
 of the "ROOT core" library. But it appeared that the ROOT core is in fact 
 the core of a framework and then more than an IO package. Usage of 
 these previous Rio versions within the OpenScientist/Lab package showed 
 that it was still necessary to embarque around 200 000 lines of code 
 to store / retrieve an histogram in a file ! This involves the code 
 of the IO machinery but also the code of the CINT interpreter used mainly 
 at this point to have an automatic handling of the dictionary of a couple 
 of classes. It involves also to embarque some code to handle networking, 
 drawing, GUI, etc... that is in fact irrelevant to the problem of storing
 data in a file.

  Some non-discussions with the ROOT conceptors clearly showed that
 these people will not do any attempt in order to have a more modular
 repacking of the ROOT core.

  Moreover the author (G.Barrand) was interest in studying the question of the 
 relationship of the CINT interpreter with the IO machinery. 
 Is CINT really needed ? Isn't it possible to have the IO machinery 
 repacked in order to be able to use an abstract dictionary so that 
 someone can provide the dictionary info by hand, or in an automatic way
 with other interpreters or languages that have introspection ? 
  The mastering of the dictionary 
 production would permit in particular to be able to reuse the dictionary
 machinery of some languages like Python and java to store 
 objects of these language with a minimum of code between these
 languages and the IO machinery. Be able to write a dictionary by hand 
 can permit some software not interpretable by CINT 
 (Geant4, heavily templated code, etc...) to have access to a storage system.
 Some frameworks, like Gaudi, have introduced a data dictionary using XML.
 It could be fine to be able to use this dictionary in direct connection 
 to the IO package 

  It was clear also that some code in the ROOT core was here
 due to a wish of not using existing libraries like STL and to a tendency
 of the authors to rewrite/reinvente most of things. Isn't it possible
 to have something lighter by using for example STL ?

  The author was interested also to study the usage of "pure
 abstract interfaces" in order to have a nice decoupling
 of things. In particular to be able to decouple the 
 dictionary from the IO machinery. With Rio-v3r0 this goal
 had been achieved ; the IO machinery sees only some pure abstract
 classes like IDictionary, IClass, IObject.
 What is astounding (comparing with ROOT) is that these classes have 
 really few methods...

  The logic of the IO itself had been respected as much as possible
 (at least up to the understanding that the author have of the 
 IO logic of ROOT). In particular the streamers of the basic classes
 like TFile, TDirectory, TTree had been respected so that a file
 produced by Rio may be understood by ROOT itself. For example
 a file containing a TTree filled with basic data types like int and floats
 is readable by ROOT.

@section rio_introduction_results First results

  First results are VERY encouraging. In particular now with OpenScientist/Lab
 package, the author is able to store/retrieve histograms and tuples with 
 around 11 000 lines of code only. 

  Files containing TTree with simple data types (int, float) are not only 
 readable by ROOT itself but also with the java implementation of the 
 ROOT IO done by T.Johnson at SLAC.

  In September 2002, we have been able to read LHCb data produced at the
 ROOT format during summer 2002. These data were produced within the
 Gaudi framework, by using the GaudiRootDb service
 with ROOT-3-01-06 behind.  The data have been read by using
 a modified version of GaudiRootDb in order to use Rio-v3r0.
 The main message here, is that now people using the Gaudi framework have 
 a very light IO package at hand to handle file storage.

@section rio_introduction_creadits Credits

  Obviously, to Rene Brun and Fons Rademakers that had designed the ROOT
 IO format.

@section rio_introduction_history History

  The idea of doing Rio was born in November 1998 after the CHEP conference
 of Chicago.
  The versions v1r* to v2r* was a repacking of the ROOT core code.
 The version v3r0 (September 2002) is a REWRITING of the ROOT IO logic.
   
@section rio_introduction_root ROOT itself
 The web site for ROOT is http://root.cern.ch ( but, dixit the authors,
 it is not a CERN product !!!) (What I have to do to have a rio.cern.ch
 URL ?)

 */
