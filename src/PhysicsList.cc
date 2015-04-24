/////////////////////////////////////////////////////////////////////////
// 
// PhysicsList.cc
// 
// Activating a list of physics constructors, which create physics 
// processes relevant for the simulation, and setting production thresholds
//
/////////////////////////////////////////////////////////////////////////


#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
#include "Particles.hh"

#ifdef G4VERSION_96
#include "G4EmLivermorePhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysics.hh"
#endif

#ifdef G4VERSION_101
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmStandardPhysicsWVI.hh"
#endif

#include "G4VPhysicsConstructor.hh"
#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"

G4VPhysicsConstructor* PhysicsList::emElectron = 0;
G4VPhysicsConstructor* PhysicsList::emPositron = 0;
G4VPhysicsConstructor* PhysicsList::emPhoton = 0;
G4VPhysicsConstructor* PhysicsList::emMsc = 0;


PhysicsList::PhysicsList() : G4VModularPhysicsList()
{                                  
  // Initialization of value for lower production threshold (the data member 
  // defaultCutValue is defined in the parent class) 
  defaultCutValue = 0.001 * CLHEP::mm;   

  // Instantiation of messenger
  messenger = new PhysicsListMessenger(this);

  // Registering particles
  RegisterPhysics(new Particles());
}


PhysicsList::~PhysicsList() {

  // Deleting instance of messenger 
  delete messenger;
}


void PhysicsList::RegisterPhysConstructor(const G4String& constrName) {

  // ***********************************************************************
  // *** EM: pre-packaged EM PhysicsConstructors ***
  // ***********************************************************************

  if (emMsc == 0) 

    { // The condition is only true if no full e/photon physics)
      // constructor was yet instantiated 
    
      // If the physics constructor name is matching one of the strings,
      // the corresponding physics constructor is instantiated and assigned
      // to the data member emElectron

#if defined(G4VERSION_101)

      if (constrName == "EM-Msc-PhysListSS") 
	{
	  if (emElectron == 0 && emPositron == 0 && emPhoton == 0)
	    {
	      emMsc = new G4EmStandardPhysicsSS;
	    }
	  else
	    {
	      std::cout << "INVALID ATTEMPT TO INSTANTIATE G4EmStandardPhysicsSS" << std::endl;
	    }
	}

      if (constrName == "EM-Msc-PhysListWVI") 
	{
	  if (emElectron == 0 && emPositron == 0 && emPhoton == 0)
	    {
	      emMsc = new G4EmStandardPhysicsWVI;
	    }
	  else
	    {
	      std::cout << "INVALID ATTEMPT TO INSTANTIATE G4EmStandardPhysicsWVI" << std::endl;
	    }
	}

#endif

#if defined(G4VERSION_96) || defined(G4VERSION_101) 
      if (constrName == "EM-Msc-PhysListLivermore") 
	{
	  if (emElectron == 0 && emPositron == 0 && emPhoton == 0)
	    {
	      emMsc = new G4EmLivermorePhysics;
	    }
	  else
	    {
	      std::cout << "INVALID ATTEMPT TO INSTANTIATE G4EmLivermorePhysics" << std::endl;
	    }
	}

      if (constrName == "EM-Msc-PhysListOpt1") 
	{
	  if (emElectron == 0 && emPositron == 0 && emPhoton == 0)
	    {
	      emMsc = new G4EmStandardPhysics_option1;
	    }
	  else
	    {
	      std::cout << "INVALID ATTEMPT TO INSTANTIATE G4EmStandardPhysics_option1" << std::endl;
	    }
	}

      if (constrName == "EM-Msc-PhysListOpt2") 
	{
	  if (emElectron == 0 && emPositron == 0 && emPhoton == 0)
	    {
	      emMsc = new G4EmStandardPhysics_option2();
	    }
	  else
	    {
	      std::cout << "INVALID ATTEMPT TO INSTANTIATE G4EmStandardPhysics_option2" << std::endl;
	    }
	}

      if (constrName == "EM-Msc-PhysListOpt3") 
	{
	  if (emElectron == 0 && emPositron == 0 && emPhoton == 0)
	    {
	      emMsc = new G4EmStandardPhysics_option3();
	    }
	  else
	    {
	      std::cout << "INVALID ATTEMPT TO INSTANTIATE G4EmStandardPhysics_option3" << std::endl;
	    }
	}

      if (constrName == "EM-Msc-PhysListOpt4") 
	{
	  if (emElectron == 0 && emPositron == 0 && emPhoton == 0)
	    {
	      emMsc = new G4EmStandardPhysics_option4();
	    }
	  else
	    {
	      std::cout << "INVALID ATTEMPT TO INSTANTIATE G4EmStandardPhysics_option4" << std::endl;
	    }
	}

      if (constrName == "EM-Msc-PhysListStd") 
	{
	  if (emElectron == 0 && emPositron == 0 && emPhoton == 0)
	    {
	      emMsc = new G4EmStandardPhysics();
	    }
	  else
	    {
	      std::cout << "INVALID ATTEMPT TO INSTANTIATE G4EmStandardPhysics" << std::endl;
	    }
	}

      if (constrName == "EM-Msc-PhysListLowEP") 
	{
	  if (emElectron == 0 && emPositron == 0 && emPhoton == 0)
	    {
	      emMsc = new G4EmLowEPPhysics;
	    }
	  else
	    {
	      std::cout << "INVALID ATTEMPT TO INSTANTIATE GEmLowEPPhysics4" << std::endl;
	    }
	}
#endif
      // The physics constructor is finally registered (The "if" statement is
      // required for cases where the constructor name was not matching any
      // option)
      if (emMsc) RegisterPhysics(emMsc);
    }
}


void PhysicsList::SetCuts() {

  // In order to allow the use of production cuts as low as 250 eV (NOTE: 
  // 250 eV is the smallest possible cut for the Livermore library based EM 
  // models for electrons and photons) the limits of the physics table must 
  // be set accordingly (default of the lower value is 990 eV):
  //G4double lowerProdLimit = 250.0 * CLHEP::eV;
  //G4double upperProdLimit = 100.0 * CLHEP::GeV;
  //G4ProductionCutsTable::GetProductionCutsTable()
  //  -> SetEnergyRange(lowerProdLimit, upperProdLimit);

  // The default cut value, given by the data member defaultCutValue, is set 
  // for all particles (electrons, positrons, photons) for the default region, 
  // which is the world region:
  SetCutsWithDefault();

  // To define production cuts for the detector, a G4ProductionCuts object is
  // first instantiated and the lower cut value is set:
  //G4ProductionCuts* prodCutsDetector = new G4ProductionCuts;
  //prodCutsDetector -> SetProductionCut(defaultCutValue);

  // The next step is to retrieve the G4Region instance, which represents the
  // detector, from the region store (by using the detector region name, which
  // was stored in a data member of the current class):
  //G4Region* detectorRegion = 
  //           G4RegionStore::GetInstance() -> GetRegion(detectorRegionName);

  // Then the G4ProductionCuts instance is assigned to the detector region:
  // (check that detectorRegion is a valid pointer before, otherwise the 
  // program will crash)
  //if (detectorRegion)
  //  detectorRegion -> SetProductionCuts(prodCutsDetector);

  // The production cuts are finally printed:
  DumpCutValuesTable();
}


void PhysicsList::SetProdThreshold(double cut) 
{
  // The default cut value is set (NOTE: defaultCutValue is a data member 
  // defined in the parent class)
  if (cut > 0.0) 
    {
      defaultCutValue = cut;
    }
}
