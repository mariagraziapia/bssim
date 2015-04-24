
/////////////////////////////////////////////////////////////////////////
// G4Tutorial:
//
// PhysicsList.hh
// 
// Activating a list of physics constructors, which create physics 
// processes relevant for the simulation, and setting production thresholds.
//
// This class is mandatory for setting up Geant4 simulations.
//
// NOTE: Predefined physics lists exist in Geant4. The physics list class
//       shown here is an example of assembling a modular physics list
//       by your own.
//
/////////////////////////////////////////////////////////////////////////


#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsListMessenger;


// G4VModularPhysicsList is the abstract base class for creating modular 
// physics lists:

class PhysicsList : public G4VModularPhysicsList {

 public:
   PhysicsList();
   virtual ~PhysicsList();

   // G4VModularPhysicsList has one pure virtual method, which must be
   // implemented by the user: SetCuts. In this method the user can set 
   // production cuts e.g. for certain regions and particles.
   void SetCuts();

   // Following method can be called from the messenger to activate physics
   // constructors (by using their name).
   // Physics constructors are user defined subclasses of G4VPhysicsConstructor
   // (e.g. PhysicsEMElectronEEDL, PhysicsEMPhotonEPDL), which contain groups
   // of related processes (e.g. all EM physics for electrons).
   // To activate a physics constructor the RegisterPhysics method of 
   // G4VModularPhysicsList must be used
   void RegisterPhysConstructor(const G4String& constrName);

   // Following method can be called from the messenger to change the 
   // production cut for the detector region
   void SetProdThreshold(double cut);

 private:
   // Messenger for communication with UI
   PhysicsListMessenger* messenger;

   // Name of the detector region (region to which production cuts are applied)
   //G4String detectorRegionName;

   // Physics constructors 
   static G4VPhysicsConstructor* emElectron;
   static G4VPhysicsConstructor* emPositron;
   static G4VPhysicsConstructor* emPhoton;
   static G4VPhysicsConstructor* emMsc;

};

#endif // PHYSICSLIST_HH
