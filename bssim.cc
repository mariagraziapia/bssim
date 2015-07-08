//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

#include "DetectorConstruction.hh"
#include "PrimaryGenerator.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "PhysicsList.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4UIterminal.hh"

//#ifdef G4UI_USE
#if defined(G4UI_USE) && (defined(G4VERSION_96) || defined(G4VERSION_94) || defined(G4VERSION_102))
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

#include <fstream>


int main(int argc,char** argv)
{
  // Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
 
  // Detector construction
  DetectorConstruction* detector = new DetectorConstruction();
  runManager -> SetUserInitialization(detector);  
  
  // Physics list
  PhysicsList* physicsList = new PhysicsList();
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
    
  // Primary generator action
  PrimaryGenerator* primaryGenerator = new PrimaryGenerator();
  runManager->SetUserAction(primaryGenerator);
 
  // Set user action classes
  RunAction* runAction = new RunAction();
  runManager->SetUserAction(runAction);

  EventAction* eventAction = new EventAction();
  runManager->SetUserAction(eventAction);


  // Initialize G4 kernel
  //
  //runManager->Initialize();
  
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1) 
    {
      // batch mode
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }
  else 
    {
      // Interactive mode : define UI session 
      //(different code for Geant4 9.1 and for later versions)
      
#if defined(G4UI_USE) && !defined(G4VERSION_91)
      // Geant4 versions later than 9.1
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
      
#ifdef G4VIS_USE
      //UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
      //UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
      
      ui->SessionStart();
      delete ui;
#endif

#if defined(G4UI_USE) && defined(G4VERSION_91)
      //Geant4 version 9.1
      // Initialize the interactive session 
      G4UIsession* session = 0;
      if (argc == 1)   // Define UI session for interactive mode.
	{
	  session = new G4UIterminal();
	}
      
      // get the pointer to the User Interface manager 
      if (session)   // Define UI session for interactive mode.
	{ 
	  G4cout << " UI session starts ..." << G4endl;
	  //UImanager -> ApplyCommand("/control/execute VisualisationMacro.mac");    
	  session -> SessionStart();
	  delete session;
	}
      else           // Batch mode
	{ 
	  G4String command = "/control/execute ";
	  G4String fileName = argv[1];
	  UImanager -> ApplyCommand(command+fileName);
	}  
#endif
      
    }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

