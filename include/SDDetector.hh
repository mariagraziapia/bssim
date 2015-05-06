//
// SDDetector - Sensitive Detector
//

#ifndef SDDETECTOR_HH
#define SDDETECTOR_HH 1

#include "G4VSensitiveDetector.hh"
#include "DetectorHitsCollection.hh"
#include "globals.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

// Class for SensitiveDetector associated with backscattering Detector

class SDDetector : public G4VSensitiveDetector
{
public:

  SDDetector(const G4String& name, const G4String& hitsCollectionName);
  virtual ~SDDetector();
  
  // Methods from base class
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

private:

  // Non-owned pointer
  DetectorHitsCollection* detectorHitsCollection;
  G4int hitsCollectionID;
};


#endif

