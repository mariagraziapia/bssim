/////////////////////////////////////////////////////////////////////////
//
// DetectorMessenger.cc
// 
// UI for DetectorConstruction
//
/////////////////////////////////////////////////////////////////////////
//
// Detector Messenger
//

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4Material.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* det)
 :detector(det){
  DetectorCommand();
  CoatingCommand();
  InsideCommand();
  TargetCommand();
}

DetectorMessenger::~DetectorMessenger() {
  delete dirDet;
  delete dirCoating;
  delete dirInside;
  delete dirTarget;

  delete detMaterCmd;
  delete detInnerRadius;
  delete detOuterRadius;

  delete coatMaterCmd;
  delete coatThickness;

  delete inMaterCmd;

  delete targetShapeType;
  delete targetMaterCmd;
  delete targetXYZ;
  delete targetRadius;
  delete targetThickness;
  delete targetShiftZ;

  delete stepLimit;

  delete cutsMode;

  delete bigMotherCmd;
  delete hieraCmd;
}


void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if ( command == detMaterCmd ) 
    {
      detector->SetDetectorMaterial(newValue);
    }

  if ( command == detInnerRadius ) 
    {
      detector->SetDetectorInnerRadius(detInnerRadius->GetNewDoubleValue(newValue));
    }

  if ( command == detOuterRadius ) 
    {
      detector->SetDetectorOuterRadius(detOuterRadius->GetNewDoubleValue(newValue));
    }

  if ( command == detOuterRadius ) 
    {
      detector->SetDetectorOuterRadius(detOuterRadius->GetNewDoubleValue(newValue));
    }

  // Coating commands
  if ( command == coatMaterCmd )
    {
      detector->SetCoatingMaterial(newValue);
    }

  if ( command == coatThickness ) 
    {
      detector->SetCoatingThickness(coatThickness->GetNewDoubleValue(newValue));
    }

  // Inside commands
  if ( command == inMaterCmd ) 
    {
      detector->SetInsideMaterial(newValue);
    }

  // Target commands
  if ( command == targetShapeType)
    {
      detector->SetTargetGeometry(newValue);
    }

  if ( command == targetMaterCmd) 
    {
    detector->SetTargetMaterial(newValue);
    }

  if ( command == targetXYZ) 
    {
      G4ThreeVector size = targetXYZ->GetNew3VectorValue(newValue);
      detector->SetTargetXYZSize(size);
    }

  if ( command == targetRadius) 
    {
      detector->SetTargetRadius(targetRadius->GetNewDoubleValue(newValue));
    }

  if ( command == targetThickness) 
    {
      detector->SetTargetThickness(targetThickness->GetNewDoubleValue(newValue));
    }

  if ( command == targetShiftZ) 
    {
      detector->SetTargetShiftZ(targetShiftZ->GetNewDoubleValue(newValue));
    }
 
  // Step limit command
  if ( command == stepLimit) 
    {
      detector->SetStepLimit(stepLimit->GetNewDoubleValue(newValue));
    }

  // Production cut mode command
  if ( command == cutsMode) 
    {
      detector->LabelProdMode(cutsMode->GetNewBoolValue(newValue));
    }

  // BigMother command
  if ( command == bigMotherCmd) 
    {
      detector->SetBigMotherConstruction(bigMotherCmd->GetNewBoolValue(newValue));
    }

  // Hierarchy command
  if ( command == hieraCmd) 
    {
      detector->SetHierarchy(hieraCmd->GetNewBoolValue(newValue));
    }


}

void DetectorMessenger::DetectorCommand()
{
  dirDet = new G4UIdirectory("/changeDetector/");
  dirDet->SetGuidance("Command to change the Detector material/size");

  detMaterCmd = new G4UIcmdWithAString("/changeDetector/setMat",this);
  detMaterCmd->SetGuidance("Select Material of the Detector.");
  detMaterCmd->SetParameterName("DetecterMaterial",false);
  detMaterCmd->SetDefaultValue("G4_Si");
  detMaterCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  detInnerRadius = new G4UIcmdWithADoubleAndUnit("/changeDetector/setInnerR",this);
  detInnerRadius->SetGuidance("Insert inner radius sizes of the Detector"
				"\n   0 or negative values mean <<Don't change it>>");
  detInnerRadius->SetParameterName("DetectorInnerRadius",false);
  detInnerRadius->SetDefaultUnit("mm");
  detInnerRadius->SetRange("DetectorInnerRadius>0.");
  detInnerRadius->SetUnitCandidates("nm um mm cm");
  detInnerRadius->AvailableForStates(G4State_PreInit, G4State_Idle);

  detOuterRadius = new G4UIcmdWithADoubleAndUnit("/changeDetector/setOuterR",this);
  detOuterRadius->SetGuidance("Insert outer radius sizes of the Detector.");
  detOuterRadius->SetParameterName("DetectorOuterRadius",false);
  detOuterRadius->SetDefaultUnit("mm");
  detOuterRadius->SetRange("DetectorOuterRadius>0.");
  detOuterRadius->SetUnitCandidates("nm um mm cm");
  detOuterRadius->AvailableForStates(G4State_PreInit, G4State_Idle);

  hieraCmd = new G4UIcmdWithABool("/changeDetector/hierarchy",this);
  hieraCmd->SetGuidance("Construct backward hemispheres as a hierarchy of volumes.");
  hieraCmd->SetParameterName("BuildHierarchy",false);
  //hieraCmd->SetCandidates("true false");
  hieraCmd->SetDefaultValue("false");
  hieraCmd->AvailableForStates(G4State_PreInit);

}

void DetectorMessenger::CoatingCommand()
{
  dirCoating = new G4UIdirectory("/changeCoating/");
  dirCoating->SetGuidance("Command to change the Coating material/thickness");

  coatMaterCmd = new G4UIcmdWithAString("/changeCoating/setMat",this);
  coatMaterCmd->SetGuidance("Select Material of the Coating.");
  coatMaterCmd->SetParameterName("CoatingMaterial",false);
  coatMaterCmd->SetDefaultValue("G4_GRAPHITE");
  coatMaterCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  coatThickness = new G4UIcmdWithADoubleAndUnit("/changeCoating/setThickness",this);
  coatThickness->SetGuidance("Insert outer radius sizes of the Coating.");
  coatThickness->SetParameterName("CoatingThickness", false);
  coatThickness->SetDefaultUnit("mm");
  coatThickness->SetRange("CoatingThickness>0.");
  coatThickness->SetUnitCandidates("nm um mm cm");
  coatThickness->AvailableForStates(G4State_PreInit, G4State_Idle);
}

void DetectorMessenger::InsideCommand()
{
  dirInside = new G4UIdirectory("/changeInside/");
  dirInside->SetGuidance("Command to change the Inside material");

  inMaterCmd = new G4UIcmdWithAString("/changeInside/setMat",this);
  inMaterCmd->SetGuidance("Select Material of the Inside space.");
  inMaterCmd->SetParameterName("InsideMaterial",false);
  inMaterCmd->SetDefaultValue("G4_Galactic");
  inMaterCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

void DetectorMessenger::TargetCommand()
{
  dirTarget = new G4UIdirectory("/changeTarget/");
  dirTarget->SetGuidance("Command to change the Target material/type/size");

  targetShapeType = new G4UIcmdWithAString("/changeTarget/setGeomType",this);
  targetShapeType->SetGuidance("Select Geometry type of the Target.");
  targetShapeType->SetParameterName("TargetType",false);
  targetShapeType->SetCandidates("Box Disk");
  targetShapeType->SetDefaultValue("Box");
  targetShapeType->AvailableForStates(G4State_PreInit);

  targetMaterCmd = new G4UIcmdWithAString("/changeTarget/setMat",this);
  targetMaterCmd->SetGuidance("Select Material of the Target.");
  targetMaterCmd->SetParameterName("TargetMaterial",false);
  targetMaterCmd->SetDefaultValue("G4_GRAPHITE");
  targetMaterCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  targetXYZ = new G4UIcmdWith3VectorAndUnit("/changeTarget/setSizeXYZ",this);
  targetXYZ->SetGuidance("Insert X Y and Z sizes of the box Target.");
  targetXYZ->SetParameterName("SizeX",
				"SizeY",
				"SizeZ", false);
  targetXYZ->SetDefaultUnit("mm");
  targetXYZ->SetUnitCandidates("nm um mm cm");
  targetXYZ->AvailableForStates(G4State_PreInit, G4State_Idle);

  targetRadius = new G4UIcmdWithADoubleAndUnit("/changeTarget/setRadius",this);
  targetRadius->SetGuidance("Insert radius sizes of the disk Target.");
  targetRadius->SetParameterName("TargetRadius",false);
  targetRadius->SetDefaultUnit("mm");
  targetRadius->SetRange("TargetRadius>0.");
  targetRadius->SetUnitCandidates("nm um mm cm");
  targetRadius->AvailableForStates(G4State_PreInit);

  targetThickness = new G4UIcmdWithADoubleAndUnit("/changeTarget/setThickness",this);
  targetThickness->SetGuidance("Insert thickness sizes of the disk Target.");
  targetThickness->SetParameterName("TargetThickness",false);
  targetThickness->SetDefaultUnit("mm");
  targetThickness->SetRange("TargetThickness>0.");
  targetThickness->SetUnitCandidates("nm um mm cm");
  targetThickness->AvailableForStates(G4State_PreInit, G4State_Idle);

  targetShiftZ = new G4UIcmdWithADoubleAndUnit("/changeTarget/shiftZ",this);
  targetShiftZ->SetGuidance("Insert amount by which target entrance face is shifted from Z=0.");
  targetShiftZ->SetParameterName("TargetShiftZ",false);
  targetShiftZ->SetDefaultUnit("mm");
  targetShiftZ->SetRange("TargetShiftZ>0.");
  targetShiftZ->SetUnitCandidates("nm um mm cm");
  targetShiftZ->AvailableForStates(G4State_PreInit, G4State_Idle);

  stepLimit = new G4UIcmdWithADoubleAndUnit("/changeTarget/stepLimit",this);
  stepLimit->SetGuidance("Insert step size limitation in target.");
  stepLimit->SetParameterName("StepLimit",false);
  stepLimit->SetDefaultUnit("mm");
  //stepLimit->SetRange("StepLimit>0.");
  stepLimit->SetUnitCandidates("nm um mm cm");
  stepLimit->AvailableForStates(G4State_PreInit);

  cutsMode = new G4UIcmdWithABool("/changeTarget/isProdMode",this);
  cutsMode->SetGuidance("Identify production mode or cuts/stepLimit optimisation mode.");
  cutsMode->SetParameterName("ProductionMode",false);
  //cutsMode->SetCandidates("true false");
  cutsMode->SetDefaultValue("true");
  cutsMode->AvailableForStates(G4State_PreInit);

  bigMotherCmd = new G4UIcmdWithABool("/changeTarget/inBigMother",this);
  bigMotherCmd->SetGuidance("Place target and detection system in big mother volume rather than world.");
  bigMotherCmd->SetParameterName("BuildBigMother",false);
  //bigMotherCmd->SetCandidates("true false");
  bigMotherCmd->SetDefaultValue("false");
  bigMotherCmd->AvailableForStates(G4State_PreInit);


}
