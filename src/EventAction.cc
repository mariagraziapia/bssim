//

#include "EventAction.hh"
#include "DetectorHitsCollection.hh"
#include "DetectorHit.hh"
#include "Summary.hh"

#include "G4SDManager.hh"
#include "G4Event.hh"


EventAction::EventAction() : G4UserEventAction(),
			     fPrintModulo(1000)
{						

}


EventAction::~EventAction()
{ 

}


void EventAction::BeginOfEventAction(const G4Event* event)
{  
  G4int eventNb = event->GetEventID();
  if (eventNb%fPrintModulo == 0) { 
    G4cout << "\n---> Begin of event: " << eventNb << G4endl;
  }
 
}


void EventAction::EndOfEventAction(const G4Event* event )
{
   // Hard-coded minimum energy for detection
  G4double eThreshold = 50. * CLHEP::eV;

  // The following line verifies that a valid hits colletion has been found
  int hDetCollectionIndex = G4SDManager::GetSDMpointer()->GetCollectionID("DetectorHitsCollection");
  //  if (hDetCollectionIndex < 0) return;
      
  //Now, get the HCofThisEvent: it contains all the hits collections
  //that have been defined (one hit collection may be associated to 
  //each detector).
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
 
  //Among all the HCs defined for the events, retrieve the one having 
  //ID=hDetCollectionIndex. All of them can be retrieved in the same way.
  DetectorHitsCollection* hDetCollection = 0;
  if (HCE) hDetCollection = (DetectorHitsCollection*)(HCE -> GetHC(hDetCollectionIndex));

  //Ok, now we have the hit collection at hand. If it is not a NULL pointer,
  //we can have a look at it, and read the information we need.

  if (hDetCollection) 
    {
      // Read the number of hits contained in the collection
      int numberHDet = hDetCollection->entries();
      // std::cout << "---------- nHits: " << numberHDet << std::endl;

      // Number of detected backscattered electrons
      G4int nBsDet = 0;
      // Number of detected backscattered electrons which are primary electrons
      G4int nBsPrimary = 0;
      // Number of detected backscattered electrons which are delta rays (no primary electron is backscattered)
      G4int nBsDelta = 0;
      // Number of backscattered electrons above the detection energy threshold
      G4int nBsAboveThreshold = 0;
      // We can loop and get each single hit
      for (int i = 0; i < numberHDet ; i++) 
	{
	  // Retrieve the i-th hit from the collection.
	  DetectorHit* hit = (*hDetCollection)[i];
	  double e = hit->GetBackscatteredEnergy();
	  int parent = hit->GetParentID();
          if (e > 0.)
	    {
	      // This is a backscattered electron entering the detector
	      nBsDet++;
	      if (parent != 0) 
		{		
		  nBsDelta++;
		}
	      else
		{
		  nBsPrimary++;
		}
	      if (e > eThreshold) 
		{
		  // Counted
		  //std:: cout << "EventAction: COUNTED" << std::endl;
		  nBsAboveThreshold++;
		}
	    }	
	}

      // Accumulate in backscattering counter
      Summary* summary = Summary::Instance();
      if (nBsAboveThreshold > 0) summary->AddBsOverThreshold();
    }

}



