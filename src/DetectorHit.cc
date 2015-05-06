//


#include "DetectorHit.hh"

G4Allocator<DetectorHit> DetectorHitAllocator;

DetectorHit::DetectorHit():  bsEnergy(0.),
			     bsParentID(-1)
{ }

DetectorHit::~DetectorHit()
{;}

DetectorHit::DetectorHit(const DetectorHit& right) 
:G4VHit()
{
  bsEnergy = right.bsEnergy;
  bsParentID = right.bsParentID;
}

const DetectorHit& DetectorHit::operator=(const DetectorHit& right)
{
  bsEnergy = right.bsEnergy;
  bsParentID = right.bsParentID;
  return *this;
}

int DetectorHit::operator==(const DetectorHit& right) const
{
  return((bsEnergy == right.bsEnergy) && (bsParentID == right.bsParentID));
}

void DetectorHit::Print()
{;}











