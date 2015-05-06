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
