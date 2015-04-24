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
//

#ifndef DETECTORMESSENGER_HH
#define DETECTORMESSENGER_HH

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;

class DetectorMessenger: public G4UImessenger
{
public:
  DetectorMessenger(DetectorConstruction* );
  virtual ~DetectorMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:

  void DetectorCommand();
  void CoatingCommand();
  void InsideCommand();
  void TargetCommand();

  G4UIdirectory*             		dirDet;
  G4UIdirectory*             		dirCoating;
  G4UIdirectory*             		dirInside;
  G4UIdirectory*             		dirTarget;

  DetectorConstruction* 	        detector;

  // Detector Parameter
  G4UIcmdWithAString*        		detMaterCmd;
  G4UIcmdWithADoubleAndUnit*		detInnerRadius;
  G4UIcmdWithADoubleAndUnit*		detOuterRadius;

  // Coating Parameter
  G4UIcmdWithAString*        		coatMaterCmd;
  G4UIcmdWithADoubleAndUnit*		coatThickness;

  // Sphere Inside Parameter
  G4UIcmdWithAString*        		inMaterCmd;

  // Target Parameter
  G4UIcmdWithAString*			targetShapeType;
  G4UIcmdWithAString*			targetMaterCmd;
  G4UIcmdWith3VectorAndUnit*		targetXYZ;
  G4UIcmdWithADoubleAndUnit*		targetRadius;
  G4UIcmdWithADoubleAndUnit*		targetThickness;
  G4UIcmdWithADoubleAndUnit*		targetShiftZ;

  // Step limitation in target
  G4UIcmdWithADoubleAndUnit*		stepLimit;

  // Production mode
  G4UIcmdWithABool*			cutsMode;

  // Construct BigMother
  G4UIcmdWithABool*			bigMotherCmd;

  // Construct hierarchy of hemispheres
  G4UIcmdWithABool*			hieraCmd;
};

#endif /* DETECTORMESSENGER_HH_ */
