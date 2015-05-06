//

#include "SDDetector.hh"
#include "DetectorHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
//#include "G4ios.hh"
#include "G4StepPoint.hh"
#include "G4Electron.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"

SDDetector::SDDetector(const G4String& name, 
		       const G4String& hitsCollectionName) : G4VSensitiveDetector(name),
							     detectorHitsCollection(0),
							     hitsCollectionID(-1)
{
  collectionName.insert(hitsCollectionName);
}

SDDetector::~SDDetector()
{ 
}

void SDDetector::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  detectorHitsCollection = new DetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection to hce
  if (hitsCollectionID < 0) hitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hitsCollectionID, detectorHitsCollection ); 

}
		       
G4bool SDDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{  
  G4bool result = false;
 
  G4Track* track = step->GetTrack();

  // Identify particles entering Detector
  // Is this an electron?
  const G4ParticleDefinition* particleDefinition = track->GetDefinition();
  if (particleDefinition == G4Electron::ElectronDefinition())
    {
      G4StepPoint* point1 = step->GetPreStepPoint();
      
      if (point1->GetStepStatus() == fGeomBoundary) 
	{
	  // This is a backscattered electron that enters the detector: record its parameters into a hit	  
	  result = true;
	  //std::cout << "SDDetector: backscattered " << std::endl;
	  DetectorHit* hit= new DetectorHit(); 
	  // Parent  
	  int parentID = track->GetParentID();
	  hit->SetParentID(parentID);
	  // Energy
	  //G4double kineticEnergy= track->GetKineticEnergy();  
	  G4double kineticEnergy= point1->GetKineticEnergy();  
	  hit->SetBackscatteredEnergy(kineticEnergy);

	  // Add the hit to the hits collection of Detector
	  detectorHitsCollection->insert(hit);
	}
    }

  return result;
}


void SDDetector::EndOfEvent(G4HCofThisEvent*)
{
  
}

