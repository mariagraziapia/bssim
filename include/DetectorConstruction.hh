//


#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH 1

#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"
#include "G4ThreeVector.hh"

class G4VSolid;
class G4Tubs;
class G4Sphere;
class G4Material;
class G4VPhysicalVolume;
class G4LogicalVolume;
class DetectorMessenger;
class G4VisAttributes;

//class G4ThreeVector;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

  G4bool SetDetectorMaterial(const G4String& material);
  void SetDetectorInnerRadius(G4double val);
  void SetDetectorOuterRadius(G4double val);

  G4bool SetCoatingMaterial(const G4String& material);
  void SetCoatingThickness(G4double val);

  G4bool SetInsideMaterial(const G4String& material);

  void SetTargetGeometry (const G4String& shape);
  G4bool SetTargetMaterial(const G4String& material);
  void SetTargetXYZSize(const G4ThreeVector& val);
  void SetTargetRadius(G4double val);
  void SetTargetThickness(G4double val);

  void SetTargetShiftZ(G4double value);

  void SetStepLimit(G4double val);
  void LabelProdMode(G4bool val);
  void SetHierarchy(G4bool value) { hieraGeom = value; }

  virtual G4VPhysicalVolume* Construct();

  void SetBigMotherConstruction(G4bool value) { constructBigMother = value; }

private:

  void ParameterSetup();
  void DefaultMaterial();
  void UpdateGeometry();

  DetectorMessenger* detectorMessenger;

  G4Material* worldMat;
  G4Material* detectorMat;
  G4Material* coatingMat; 
  G4Material* targetMat; 
  G4Material* insideMat;
  G4Material* bigMotherMat;

  G4VisAttributes* targetAttribute;
  G4VisAttributes* detectorAttribute;
  G4VisAttributes* coatingAttribute;
  G4VisAttributes* insideAttribute;
  G4VisAttributes* bigMotherAttribute;

  //SensitiveDetector* detectorSD;
  //SensitiveDetector* coatingSD; 
  //SensitiveDetector* targetSD;

  //G4SDManager* SDManager;

  G4LogicalVolume* logicWorld;
  G4LogicalVolume* logicalSphereDetector;
  G4LogicalVolume* logicalSphereCoating;
  G4LogicalVolume* logicalSphereInside;
  G4LogicalVolume* logicalTarget;
  G4LogicalVolume* logicalBigMother;

  G4VPhysicalVolume* physWorld;

  G4Sphere* solidSphereDetector;
  G4Sphere* solidSphereCoating;
  G4Sphere* solidSphereInside;
  G4VSolid* solidTarget;
  G4VSolid* solidBigMother;

  G4double detectorInnerR;
  G4double detectorOuterR;
 
  G4double coatingThickness;
  G4double coatingRMin;
  G4double coatingRMax;

  G4double insideRMin;
  G4double insideRMax;

  G4String targetShape;

  // Target geometry parameters for box target
  G4ThreeVector targetXYZSize;

  // Target geometry parameters for disk target
  G4double targetRMin; 
  G4double targetRMax;
  G4double targetThickness;

  // Step limit in target
  G4double stepLimit;

  // Geometry angle parameter
  G4double pSPhi;
  G4double pDPhi; 
  G4double pSTheta;
  G4double pDTheta;

  G4bool SDmakingCheck;

  // Shift of Z start of the target w.r.t. Z=0
  G4double zShift;

  // Construct BigMother volume enclosing target and detector system
  G4bool constructBigMother;

  // Construct original geometry (otherwise hierarchical geometry)
  G4bool hieraGeom;

};


#endif

