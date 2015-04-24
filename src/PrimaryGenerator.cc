/////////////////////////////////////////////////////////////////////////
//
// PrimaryGenerator.cc
// 
// Definition of primary particles and their properties (spectrum
// of energies, initial vertices, ...)
//
/////////////////////////////////////////////////////////////////////////

#include "PrimaryGenerator.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4Electron.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "Randomize.hh"


PrimaryGenerator::PrimaryGenerator() {

  // Initialization of particle source properties
  primaryKineticEnergy = 100. * CLHEP::keV; 
  sigmaKineticEnergy = 0. * CLHEP::keV;
  incidentAngle = 0.0 * CLHEP::deg;
  zVtx = 0.;

  // Instantiation of the particle gun. As default arguments primaries
  // are assumed to be electrons, and the number of particles per invokation
  // of the GeneratePrimaryVertex method is set to 1:
  particleGun = new G4ParticleGun(G4Electron::Electron(), 1);  

  // Instantiation of messenger
  messenger = new PrimaryGeneratorMessenger(this);
}


PrimaryGenerator::~PrimaryGenerator() {

  // The instances of particle gun and messenger are deleted:
  delete particleGun;
  delete messenger;
}


void PrimaryGenerator::GeneratePrimaries(G4Event* event) {

  // This implementation of the GeneratePrimaries method creates primary
  // particles, where
  //  *) the particle energy is randomly sampled from a Gaussian distribution
  //     (the primary energy and sigma are defined by the data members
  //     primaryKineticEnergy and sigmaKineticEnergy)
  //  *) the beam axis lies in the z-y-plane (with z < 0) and is assumed to 
  //     cross the detector surface in the origin of the coordinate system 
  //     (the detector surface lies in the x-y-plane).  
  //     The beam axis can be rotated, where the x-axis acts as rotation axis. 
  //     The rotation angle is defined as the angle between the negative z-axis
  //     and the beam axis (the rotation angle is given by the data member 
  //     incidentAngle). 

  // The G4RandGauss::shoot(mean, sigma) function is used for sampling randomly
  // the kinetic energy if the user has specified a sigma > 0.0:
  G4double kineticEnergy = primaryKineticEnergy;
  if(sigmaKineticEnergy > 0.0 * CLHEP::keV)
    kineticEnergy = G4RandGauss::shoot(primaryKineticEnergy, 
				       sigmaKineticEnergy);

  // The particle gun (which is a concrete implementation of 
  // G4VPrimaryGenerator) is used to create the primary particle/vertex.
  // Particle properties can be set by using dedicated member functions of 
  // the particle gun implementation:
  particleGun -> SetParticleEnergy(kineticEnergy);

  //If incidentAngle=0, the initial position is on the negative half of 
  //the z-axis. When an incidentAngle is set, this position is rotated 
  //with respect to the x axis, so coordinates will be:
  // (0,10*mm*cos(incidentAngle),-10*mm*sin(incidentAngle)). 
  //The rotation is performed automatically by rotateX()
  //particleGun -> SetParticlePosition(G4ThreeVector(0.0, 0.0,-10.0 * CLHEP::mm).rotateX(incidentAngle));
  //particleGun -> SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
  // double zVtx = 0.001 * CLHEP::nm;
  particleGun -> SetParticlePosition(G4ThreeVector(0.0, 0.0, zVtx));

  //Set particle direction so that the impinging point of the primary 
  //is always (0,0,0)
  //particleGun -> SetParticleMomentumDirection(G4ThreeVector(0.0, 0.0, 1.0).rotateX(incidentAngle));
  //particleGun -> SetParticleMomentumDirection(G4ThreeVector(0.0, 0.0, 1.0).rotateZ(incidentAngle));
  G4double phi = CLHEP::pi * G4UniformRand(); 
  particleGun -> SetParticleMomentumDirection(G4ThreeVector(std::sin(incidentAngle)*std::cos(phi),
							    std::sin(incidentAngle)*std::sin(phi), 
							    std::cos(incidentAngle)));
  //std::cout << incidentAngle/CLHEP::deg << " ==== cos(theta) = " << std::cos(incidentAngle) << std::endl;

  // Creating the primary vertex: The current event must be passed as argument.
  particleGun -> GeneratePrimaryVertex(event);
}


void PrimaryGenerator::SetPrimaryKineticEnergy(G4double kinEnergy) 
{
  
  if (kinEnergy > 0.0 * CLHEP::MeV) 
    {
      primaryKineticEnergy = kinEnergy;
    }
}


void PrimaryGenerator::SetSigmaKineticEnergy(G4double sigma) 
{  
  if (sigma >= 0.0 * CLHEP::MeV) {
    sigmaKineticEnergy = sigma;
  }
}


void PrimaryGenerator::SetIncidentAngle(G4double angle) 
{
  if (angle >= 0.0 && angle < 90.0 * CLHEP::deg) 
    {
      incidentAngle = angle;
    }
}


void PrimaryGenerator::SetParticleType(const G4String& name) 
{
  // The particle iterator is used to search the particle table for a 
  // certain particle 
  G4ParticleTable::G4PTblDicIterator* particleIterator = G4ParticleTable::GetParticleTable() -> GetIterator();
  particleIterator -> reset();
  
  while( (*particleIterator)() ) 
    {
      G4ParticleDefinition* particle = particleIterator -> value();
      G4String particleName = particle -> GetParticleName();
      
      // Only if the particle was found, the new primary particle type is 
      // forwarded to the particle gun
      if (particleName == name) 
	{
	  particleGun -> SetParticleDefinition(particle);
	  break;
	}
    }
}

void PrimaryGenerator::SetReference(const G4String& /* name */) 
{
  // Identify the experimental configuration to be simulated
}

void PrimaryGenerator::SetVertexZ(G4double value) 
{
  zVtx = value;
}

