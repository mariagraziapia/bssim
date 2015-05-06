//


#ifndef DETECTORHIT_HH
#define DETECTORHIT_HH 1

#include "globals.hh"
#include "G4VHit.hh"
//#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class DetectorHit : public G4VHit
{
public:

  DetectorHit();
  DetectorHit(const DetectorHit&);
  virtual ~DetectorHit();

  // Operators
  const DetectorHit& operator=(const DetectorHit&);
  G4int operator==(const DetectorHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  // Methods from base class
  virtual void Draw() {}
  virtual void Print();

  // Specific operations

  void SetParentID(int id)                   { bsParentID = id; }
  void SetBackscatteredEnergy(double e)      { bsEnergy = e; }
 
  int GetParentID()                          { return bsParentID; }
  double GetBackscatteredEnergy()            { return bsEnergy; }

private:

  double bsEnergy;
  int bsParentID;
};

extern G4Allocator<DetectorHit> DetectorHitAllocator;

inline void* DetectorHit::operator new(size_t)
{
  void *hit;
  hit = (void *) DetectorHitAllocator.MallocSingle();
  return hit;
}

inline void DetectorHit::operator delete(void *hit)
{
  DetectorHitAllocator.FreeSingle((DetectorHit*) hit);
}

#endif
