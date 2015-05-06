//
/////////////////////////////////////////////////////////////////////////
//


#ifndef EVENTACTION_HH
#define EVENTACTION_HH 

#include "G4UserEventAction.hh"
#include "globals.hh"
//#include <fstream>

/// Event action class


class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();
    
  // static access method

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

     
private:
  //SDHitsCollection* GetHitsCollection(const G4String& hcName, const G4Event* event) const;

  G4int     fPrintModulo;


}; 


#endif

    
