/////////////////////////////////////////////////////////////////////////
// G4Tutorial:
//
// RunAction.hh
// 
// Defining actions performed at the beginning and/or the end of each run
//
/////////////////////////////////////////////////////////////////////////


#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4Timer.hh"

class DataFrameDetector;

// G4UserRunAction is the base class for defining user actions performed at 
// the beginning and/or the end of each run:

class RunAction : public G4UserRunAction {

public:
  RunAction();

  virtual ~RunAction();

 // G4UserRunAction has two methods, BeginOfRunAction and EndOfRunAction, 
  // which can be overloaded by the user to define specific actions performed
  // the beginning and the end of each run. 
  // Information about the run can be retrieved from the G4Run objects passed 
  // to these functions.
  void BeginOfRunAction(const G4Run* run);
  void EndOfRunAction(const G4Run* run);

private:

  G4Timer timerRun;
};

#endif // RUNACTION_HH
