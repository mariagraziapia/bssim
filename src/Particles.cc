/////////////////////////////////////////////////////////////////////////
//
// Particles.cc
// 
// Constructing particles used in the simulation
//
/////////////////////////////////////////////////////////////////////////

#include "Particles.hh"
#include "G4ParticleTable.hh"
#include "G4LeptonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4MesonConstructor.hh"

Particles::Particles(const G4String& name) : G4VPhysicsConstructor(name) 
{ 

}

Particles::~Particles() 
{

}

void Particles::ConstructParticle() 
{ 
  // Particles are constructed:
  G4LeptonConstructor lepton;
  lepton.ConstructParticle();
 
  G4BosonConstructor boson;
  boson.ConstructParticle();

  // Needed for WentzelVI multiple scattering model
  G4BaryonConstructor baryon;
  baryon.ConstructParticle();

  // Needed for EmLivermorePhysicsList
  G4MesonConstructor meson;
  meson.ConstructParticle();

  // Needed for Coulomb scattering model
  G4IonConstructor ion;
  ion.ConstructParticle();
}
