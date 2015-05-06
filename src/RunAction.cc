/////////////////////////////////////////////////////////////////////////
// G4Tutorial:
//
// RunAction.cc
// 
// Defining actions performed at the beginning and/or the end of each run
//
/////////////////////////////////////////////////////////////////////////


#include "RunAction.hh"
#include "Summary.hh"

#include "G4Run.hh"

#include <sstream>

RunAction::RunAction()
{

}

RunAction::~RunAction()
{
}

 

void RunAction::BeginOfRunAction(const G4Run* run) 
{
  // Start counting time
  // timerRun.Start();	

  // The run ID is printed at the beginning of each run
  G4cout << "INFORMATION: Run No " 
	 << run -> GetRunID() 
         << " starts " 
	 << G4endl;

  // Initialize the Summary for the current run
  Summary* summary =  Summary::Instance();
  summary->ResetData();
}

void RunAction::EndOfRunAction(const G4Run* run) 
{
  // Stop counting time
  // timerRun.Stop();

  G4int nEvents =  run -> GetNumberOfEvent();


  // The run ID and the number of processed events are printed at the end of 
  // each run
  G4cout << "INFORMATION: Run No " 
	 << run -> GetRunID() 
         << " ends - Number of events = " 
	 << nEvents
    //         << run -> GetNumberOfEvent() 
	 << G4endl;

  // Write the summary to a file
  Summary* summary =  Summary::Instance();
  summary->SetEvents(nEvents);
  summary->Dump();


}
