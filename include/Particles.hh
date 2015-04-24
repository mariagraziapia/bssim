
/////////////////////////////////////////////////////////////////////////
// G4Tutorial:
//
// Particles.hh
// 
// Constructing particles used in the simulation.
//
/////////////////////////////////////////////////////////////////////////


#ifndef PARTICLES_HH
#define PARTICLES_HH

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"


// G4VPhysicsConstructor is the abstract base class for physics constructors:

class Particles : public G4VPhysicsConstructor {

 public: 
   Particles(const G4String& name = "Particles");
   virtual ~Particles();
 
  

   // G4VPhysicsConstructor has two pure virtual methods which must be 
   // implemented by the user: ConstructParticle and ConstructProcess.
   // Since only particles are constructed here, ConstructProcess is 
   // irrelevant and performs no action. ConstructParticle is used to
   // instantiate all particle definition classes relevant for the 
   // simulation.
   void ConstructParticle();
   void ConstructProcess() { }; 
};

#endif // PARTICLES_HH








