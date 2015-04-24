/////////////////////////////////////////////////////////////////////////
//
// DetectorConstruction
// 
// Geometry and material model
//
/////////////////////////////////////////////////////////////////////////


#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4ThreeVector.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RegionStore.hh"
#include "G4ThreeVector.hh"

#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"

DetectorConstruction::DetectorConstruction(): G4VUserDetectorConstruction(),
					      detectorInnerR(8.6*cm),
					      detectorOuterR(18.6*cm),
					      coatingThickness(1. * mm),
					      targetShape("Box"),
					      targetXYZSize(2.5 * cm, 2.5 * cm, 1. * mm),
					      targetRMin(0. * mm),
					      targetRMax(1.5 * cm),
					      targetThickness(1. * mm),
                                              stepLimit(-9. * mm),
					      pSPhi(0.),
					      pDPhi(360. * deg),
					      pSTheta(90. * deg),
					      pDTheta(90. * deg),
					      SDmakingCheck(0),
					      zShift(0.),
                                              constructBigMother(false),
                                              hieraGeom(false)
{
  detectorMessenger = 0;

  worldMat = 0;
  detectorMat = 0;
  coatingMat = 0; 
  targetMat = 0; 
  insideMat = 0;
  bigMotherMat = 0;

  targetAttribute = 0;
  detectorAttribute = 0;
  coatingAttribute = 0;
  insideAttribute = 0;
  bigMotherAttribute = 0;

  logicWorld = 0;
  logicalSphereDetector = 0;
  logicalSphereCoating = 0;
  logicalSphereInside = 0;
  logicalTarget = 0;
  logicalBigMother = 0;

  physWorld = 0;

  solidSphereDetector = 0;
  solidSphereCoating = 0;
  solidSphereInside = 0;
  solidTarget = 0;
  solidBigMother = 0;

  coatingRMin = 0.;
  coatingRMax = 0.;
  insideRMin = 0.;
  insideRMax = 0.;

  // Consistency checks
  if (detectorOuterR <= detectorInnerR) G4Exception("DetectorConstruction::DetectorConstruction",
						    "ebscatter0000001",
						    FatalException,
						    "Detector outer radius <= inner radius");
  if (coatingThickness >= detectorInnerR) G4Exception("DetectorConstruction::DetectorConstruction",
						      "ebscatter0000001",
						      FatalException,
						      "Coating thickness >= Detector inner radius");
  // Define derived parameters
  ParameterSetup();
  // Define default materials
  DefaultMaterial();

  detectorMessenger = new DetectorMessenger(this);
}

void DetectorConstruction::DefaultMaterial()
{
  // NIST material manager
  G4NistManager* nist = G4NistManager::Instance();


  // Instantiate default materials defined by NIST material manager
  // The default world material is vacuum to avoid wasting CPU time in tracking particles 
  // outside the relevant volumes (target + backward hemisphere)
  worldMat = nist->FindOrBuildMaterial("G4_Galactic");
   // By default the coating layer is irrelevant (is filled with galactic vacuum)
  coatingMat = nist->FindOrBuildMaterial("G4_Galactic");
  targetMat = nist->FindOrBuildMaterial("G4_Si");
  detectorMat = nist->FindOrBuildMaterial("G4_Si");
  insideMat = nist->FindOrBuildMaterial("G4_Galactic");
  bigMotherMat = worldMat;


}


DetectorConstruction::~DetectorConstruction()
{
  if (detectorMessenger) delete detectorMessenger;
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Update parameters, in case they may have been changed through the UI before Construct is invoked
  ParameterSetup();
       
  // ---- World ----
 
  G4double world_sizeXYZ = 1.*m;
  
  G4Box* solidWorld = new G4Box("World",               //its name
				0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     //its size
      
  logicWorld = new G4LogicalVolume(solidWorld,         //its solid
				   worldMat,           //its material
				   "World");           //its name
                                   
  physWorld = new G4PVPlacement(0,                     //no rotation
				G4ThreeVector(),       //at (0,0,0)
				logicWorld,            //its logical volume
				"World",               //its name
				0,                     //its mother  volume
				false,                 //no boolean operation
				0);                    //copy number
                          

  // The following is for test purpose
  // Place target and backward volumes in a big enclosing mother volume to check 
  // whether the observed inhibition of backscattering is related to a boundary with the world
  
  if (constructBigMother) 
    {
      // BigMother geometry
      solidBigMother = new G4Box("BigMother",                                                        //its name
				 0.495*world_sizeXYZ, 0.495*world_sizeXYZ, 0.495*world_sizeXYZ);     //its size
      
      logicalBigMother = new G4LogicalVolume(solidBigMother,          //its solid
					     bigMotherMat,            //its material
					     "BigMother");            //its name
      G4VPhysicalVolume* physBigMother = new G4PVPlacement(0,                           // no rotation
							   G4ThreeVector(0.,0.,0.), // centred in (0,0,0)     
							   logicalBigMother,            // its logical volume
							   "BigMother",                 // its name
							   logicWorld,                  // its mother  volume
							   false,                       // no boolean operation
							   0,                           // copy number
							   true);                      // check overlaps 
      //						   false);                       // check overlaps 
      // The following is an artifact to avoid a compilation warning: unused variable ‘physTarget’
      if (physBigMother !=0) std::cout << "BigMother placed in World" << std::endl;
    }
  
  G4LogicalVolume* logicalEncloser = logicWorld;
  if (logicalBigMother != 0) logicalEncloser =  logicalBigMother;
  
  // ---- Target Geometry ----
  
  // The target shape has two options: a box or a cylinder
  if (targetShape == "Box")
    {
      solidTarget = new G4Box("Target",
			      targetXYZSize.x()*0.5,
			      targetXYZSize.y()*0.5,
			      targetXYZSize.z()*0.5);
      targetThickness = targetXYZSize.z();
    }
  
  else if (targetShape == "Disk")
    {
      solidTarget = new G4Tubs("Target",
			       targetRMin,
			       targetRMax,
			       0.5*targetThickness,
			       0,
			       360.*deg);
    }
  
  logicalTarget = new G4LogicalVolume(solidTarget,          //its solid
				      targetMat,            //its material
				      "Target");            //its name
  
  // The maximum step size of particles in the target can be limited. For
  // this purpose a G4UserLimits object must be instantiated and assigned to
  // the logical volume 
  G4UserLimits* targetVolUserLimits = new G4UserLimits(stepLimit);
  if (stepLimit > 0.) logicalTarget -> SetUserLimits(targetVolUserLimits);
  
  // The target is placed in a position such that the entrance surface encountered by the beam is at z=0 
  // (i.e. the center of the target is at z=0.5 * targetThickness)
  double zPosition = 0.5*targetThickness + zShift;
  G4VPhysicalVolume* physTarget = new G4PVPlacement(0,                      //no rotation
						    G4ThreeVector(0., 0., zPosition ),       
						    logicalTarget,          //its logical volume
						    "Target",               //its name
						    logicalEncloser,        //its mother  volume
						    false,                  //no boolean operation
						    0,                      //copy number
						    true);                 //check overlaps  
  //						    false);                  //check overlaps  
  
  
  // The following is an artifact to avoid a compilation warning: unused variable ‘physTarget’
  if (physTarget !=0) std::cout << "Target placed in World" << std::endl;
  
  if (!hieraGeom) 
    {
      // Original detector geometry: two hemispherical layers and a hemisphere 
      // independently placed in the World

      // ---- Detector Geometry ----
      
      // The detector is a  spheric shell centered at 0,0.0, which covers the backward solid angle
      solidSphereDetector = new G4Sphere("Detector",                       //its name
					 detectorInnerR,
					 detectorOuterR,
					 pSPhi,
					 pDPhi,
					 pSTheta,
					 pDTheta);
      
      logicalSphereDetector = new G4LogicalVolume(solidSphereDetector,  //its solid
						  detectorMat,          //its material
						  "Detector");          //its name
      
      G4VPhysicalVolume* physSphereDetector = new G4PVPlacement(0,                      // no rotation
								G4ThreeVector(0.,0.,0.),// at (0,0,0)
								logicalSphereDetector,  // its logical volume
								"Detector",             // its name
								logicalEncloser,        // its mother  volume
								false,                  // no boolean operation
								0,                      //copy number
								true);                 //check overlaps  
      //							    false);                  //check overlaps  
      
      // The following is an artifact to avoid a compilation warning: unused variable ‘physSphereDetector’
      if (physSphereDetector !=0) std::cout << "Detector placed in World" << std::endl;
      
      // ---- Coating ----
      
      // The coating is a spheric layer inside the detector sphere
      solidSphereCoating = new G4Sphere("Coating",                       //its name
					coatingRMin,
					coatingRMax,
					pSPhi,
					pDPhi,
					pSTheta,
					pDTheta);
      
      logicalSphereCoating = new G4LogicalVolume(solidSphereCoating,   //its solid
						 coatingMat,           //its material
						 "Coating");           //its name
      
      G4VPhysicalVolume* physSphereCoating = new G4PVPlacement(0,                      //no rotation
							       G4ThreeVector(0., 0., 0.), //at (0,0,0)
							       logicalSphereCoating,   //its logical volume
							       "Coating",              //its name
							       logicalEncloser,             //its mother  volume
							       false,                  //no boolean operation
							       0,                      //copy number
							       true);                 //check overlaps  
      //							   false);                  //check overlaps  
      
      // The following is an artifact to avoid a compilation warning: unused variable ‘physSphereCoating’
      if (physSphereCoating !=0) std::cout << "Coating placed in World" << std::endl;
      
      // ---- Inside geometry ----
      // This is the spheric shell between target and coating
      solidSphereInside = new G4Sphere("Inside",                       // its name
				       insideRMin,
				       insideRMax,
				       pSPhi,
				       pDPhi,
				       pSTheta,
				       pDTheta);
      
      logicalSphereInside = new G4LogicalVolume(solidSphereInside,   // its solid
						insideMat,           // its material
						"Inside");           // its name
      
      G4VPhysicalVolume* physSphereInside = new G4PVPlacement(0,                       // no rotation
							      G4ThreeVector(0.,0.,0.), // at (0,0,0)
							      logicalSphereInside,     // its logical volume
							      "Inside",                // its name
							      logicalEncloser,         // its mother volume
							      false,                   // no boolean operation
							      0,                       // copy number
							      true);                   // check overlaps  
      //							  false);                  //check overlaps  
      
      // The following is an artifact to avoid a compilation warning: unused variable ‘physSphereInside’
      if (physSphereInside !=0) std::cout << "Empty volume between target and coating placed in World" << std::endl;
      

    }
  else
    {
      // Hyerarchical detector geometry (according to G. Cosmo's suggestion): 
      // three hemispheres inside one another 
      // ---- Detector Geometry ----
      
      // The detector is a  spheric shell centered at 0,0.0, which covers the backward solid angle
      solidSphereDetector = new G4Sphere("Detector",                       //its name
					 0.,
					 detectorOuterR,
					 pSPhi,
					 pDPhi,
					 pSTheta,
					 pDTheta);
      
      logicalSphereDetector = new G4LogicalVolume(solidSphereDetector,  //its solid
						  detectorMat,          //its material
						  "Detector");          //its name
      
      G4VPhysicalVolume* physSphereDetector = new G4PVPlacement(0,                      // no rotation
								G4ThreeVector(0.,0.,0.),// at (0,0,0)
								logicalSphereDetector,  // its logical volume
								"Detector",             // its name
								logicalEncloser,        // its mother volume
								false,                  // no boolean operation
								0,                      // copy number
								true);                  // check overlaps  
      //							    false);                  //check overlaps  
      
      // The following is an artifact to avoid a compilation warning: unused variable ‘physSphereDetector’
      if (physSphereDetector !=0) std::cout << "Detector placed in hierarchy" << std::endl;
      
      // ---- Coating ----

      // Make sure that coatingRMax is the same as detectorRMin
      coatingRMax = detectorInnerR;
      
      // The coating is a spheric layer inside the detector sphere
      solidSphereCoating = new G4Sphere("Coating",                       //its name
					0.,
					coatingRMax,
					pSPhi,
					pDPhi,
					pSTheta,
					pDTheta);
      
      logicalSphereCoating = new G4LogicalVolume(solidSphereCoating,   //its solid
						 coatingMat,           //its material
						 "Coating");           //its name
      
      G4VPhysicalVolume* physSphereCoating = new G4PVPlacement(0,                       // no rotation
							       G4ThreeVector(0.,0.,0.), // at (0,0,0)
							       logicalSphereCoating,    // its logical volume
							       "Coating",               // its name
							       logicalSphereDetector,   // its mother volume
							       false,                   // no boolean operation
							       0,                       // copy number
							       true);                   // check overlaps  
      //							   false);                  //check overlaps  
      
      // The following is an artifact to avoid a compilation warning: unused variable ‘physSphereCoating’
      if (physSphereCoating !=0) std::cout << "Coating placed in hierarchy" << std::endl;
      
      // ---- Inside geometry ----
      // This is the spheric shell between target and coating
      solidSphereInside = new G4Sphere("Inside",                       // its name
				       0.,
				       insideRMax,
				       pSPhi,
				       pDPhi,
				       pSTheta,
				       pDTheta);
      
      logicalSphereInside = new G4LogicalVolume(solidSphereInside,   // its solid
						insideMat,           // its material
						"Inside");           // its name
      
      G4VPhysicalVolume* physSphereInside = new G4PVPlacement(0,                       // no rotation
							      G4ThreeVector(0.,0.,0.), // at (0,0,0)
							      logicalSphereInside,     // its logical volume
							      "Inside",                // its name
							      logicalSphereCoating,    // its mother volume
							      false,                   // no boolean operation
							      0,                       // copy number
							      true);                   // check overlaps  
      //							  false);                  //check overlaps  
      
      // The following is an artifact to avoid a compilation warning: unused variable ‘physSphereInside’
      if (physSphereInside !=0) std::cout << "Empty volume between target and coating placed in hierarchy" << std::endl;
      
      
    }


  // ---- Visualization ----

  targetAttribute = new G4VisAttributes(G4Colour(0.784313725,	0.784313725,	0.784313725));
  targetAttribute->SetForceSolid(TRUE);
  detectorAttribute = new G4VisAttributes(G4Colour(0.850980392,	0.898039216,	1));
  detectorAttribute->SetForceWireframe(TRUE);
  coatingAttribute = new G4VisAttributes(G4Colour(0.831372549,	0.956862745,	0.980392157));
  coatingAttribute->SetForceWireframe(TRUE);
  // insideAttribute = new G4VisAttributes(G4Colour(1.0,1.,1.0));
  // insideAttribute = new G4VisAttributes(G4Colour(1.0,1.,1.0));
  insideAttribute = new G4VisAttributes(G4Colour(1.0,1.,0.0));
  //insideAttribute->SetForceSolid(TRUE);
  insideAttribute->SetVisibility(TRUE);
  logicalTarget->SetVisAttributes(targetAttribute);
  logicalSphereDetector->SetVisAttributes(detectorAttribute);
  logicalSphereCoating->SetVisAttributes(coatingAttribute);
  logicalSphereInside->SetVisAttributes(insideAttribute);

  if (constructBigMother)
    {
      bigMotherAttribute = new G4VisAttributes(G4Colour(0.831372549,	0.956862745,	0.980392157));
      bigMotherAttribute->SetForceWireframe(TRUE);
      logicalBigMother->SetVisAttributes(bigMotherAttribute);
    }

  return physWorld;
}


G4bool DetectorConstruction::SetDetectorMaterial(const G4String& material)
{
  if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
      detectorMat  = pMat;
      if (logicalSphereDetector)
      	{
      	  logicalSphereDetector -> SetMaterial(detectorMat);
      
      	  G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
      	  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
      	  G4cout << "The material of Detector has been changed to " << material << G4endl;
      	}
    }
  else
    {
      G4cout << "WARNING: material \"" << material << "\" does not exist in NIST elements/materials"
	" table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
      G4cout << "Use command \"/parameter/nist\" to see full materials list!" << G4endl;
      return false;
    }

  return true;
}

void DetectorConstruction::SetDetectorInnerRadius(G4double val)
{
  detectorInnerR = val;

  if (solidSphereDetector && solidSphereCoating && solidSphereInside)
    {
#ifdef G4VERSION_91
      solidSphereDetector->SetInsideRadius(detectorInnerR);
      solidSphereCoating->SetInsideRadius(detectorInnerR - coatingThickness);
#endif

#if defined(G4VERSION_94) || defined(G4VERSION_96)
      solidSphereDetector->SetInnerRadius(detectorInnerR);
      solidSphereCoating->SetInnerRadius(detectorInnerR - coatingThickness);
#endif
  
      solidSphereDetector->SetOuterRadius(detectorOuterR);
      solidSphereCoating->SetOuterRadius(detectorInnerR);
      solidSphereInside->SetOuterRadius(detectorInnerR - coatingThickness);

      //UpdateGeometry();
      G4RunManager::GetRunManager() -> GeometryHasBeenModified();
    }
}


void DetectorConstruction::SetDetectorOuterRadius(G4double val)
{
  detectorOuterR = val;
  //G4RunManager::GetRunManager() -> GeometryHasBeenModified();

  // Consistency checks
  if (solidSphereDetector)
    {
      solidSphereDetector->SetOuterRadius(detectorOuterR);
      //  UpdateGeometry();
      G4RunManager::GetRunManager() -> GeometryHasBeenModified();
    }
}


G4bool DetectorConstruction::SetCoatingMaterial(const G4String& material)
{
  if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
      coatingMat  = pMat;
      if (logicalSphereCoating)
      	{
	  logicalSphereCoating-> SetMaterial(coatingMat);
	  
      	  G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
      	  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
      	  G4cout << "The material of Coating has been changed to " << material << G4endl;
      	}
    }
  else
    {
      G4cout << "WARNING: material \"" << material << "\" does not exist in NIST elements/materials"
	" table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
      G4cout << "Use command \"/parameter/nist\" to see full materials list!" << G4endl;
      return false;
    }

  return true;

}


void DetectorConstruction::SetCoatingThickness(G4double val)
{
  coatingThickness = val;

  if (solidSphereCoating && solidSphereInside)
    {
#ifdef G4VERSION_91
      solidSphereCoating->SetInsideRadius(detectorInnerR - coatingThickness);
#endif

#if defined(G4VERSION_94) || defined(G4VERSION_96)
      solidSphereCoating->SetInnerRadius(detectorInnerR - coatingThickness);
#endif

      solidSphereInside->SetOuterRadius(detectorInnerR - coatingThickness);
      G4RunManager::GetRunManager() -> GeometryHasBeenModified();
    }
}

G4bool DetectorConstruction::SetInsideMaterial(const G4String& material)
{
  if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
      insideMat  = pMat;
      if (logicalSphereInside)
      	{
      	  logicalSphereInside-> SetMaterial(insideMat);
	  
      	  G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
      	  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
      	  G4cout << "The material of Inside has been changed to " << material << G4endl;
      	}
    }
  else
    {
      G4cout << "WARNING: material \"" << material << "\" does not exist in NIST elements/materials"
	" table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
      G4cout << "Use command \"/parameter/nist\" to see full materials list!" << G4endl;
      return false;
    }
  
  return true;
}


void DetectorConstruction::SetTargetGeometry (const G4String& shape)
{
  if (shape == "Box" || shape == "Disk" ) targetShape = shape;
  UpdateGeometry();
}

G4bool DetectorConstruction::SetTargetMaterial(const G4String& material)
{
  if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
      targetMat  = pMat;
      G4cout << "The target material is " << material << G4endl;

      if (logicalTarget)
      	{
      	  logicalTarget-> SetMaterial(targetMat);
      
      	  G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
      	  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
      	  G4cout << "The material of Target has been changed to " << material << G4endl;
      	}
    }
  else
    {
      G4cout << "WARNING: material \"" << material << "\" does not exist in NIST elements/materials"
	" table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
      G4cout << "Use command \"/parameter/nist\" to see full materials list!" << G4endl;
      return false;
    }

  return true;
}

void DetectorConstruction::SetTargetXYZSize(const G4ThreeVector& val)
{
  targetXYZSize = val;
  targetThickness = targetXYZSize.z();
 
  if (solidTarget)
    {
      ((G4Box*)solidTarget)->SetXHalfLength(targetXYZSize.x()*0.5);
      ((G4Box*)solidTarget)->SetYHalfLength(targetXYZSize.y()*0.5);
      ((G4Box*)solidTarget)->SetZHalfLength(targetXYZSize.z()*0.5);
      G4RunManager::GetRunManager() -> GeometryHasBeenModified();
    }
}

void DetectorConstruction::SetTargetRadius(G4double val)
{
  targetRMax = val;
  if (solidTarget)
    {
      ((G4Tubs*) solidTarget) -> SetOuterRadius(targetRMax);
      G4RunManager::GetRunManager() -> GeometryHasBeenModified();
    }
}

void DetectorConstruction::SetTargetThickness(G4double val)
{
  targetThickness = val;
  targetXYZSize.setZ(val);

  if (solidTarget)
    {
      ((G4Tubs*) solidTarget) -> SetZHalfLength(targetThickness*0.5);
      G4RunManager::GetRunManager() -> GeometryHasBeenModified();
    }
}

void DetectorConstruction::SetTargetShiftZ(G4double value)
{
  zShift = value;

  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
}

void DetectorConstruction::SetStepLimit(G4double val)
{
  stepLimit = val;
}

void DetectorConstruction::LabelProdMode(G4bool /* val */)
{

}

void DetectorConstruction::UpdateGeometry()
{
  if (!physWorld) return;


  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
  ParameterSetup();
  
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  
  G4RegionStore::GetInstance()->UpdateMaterialList(physWorld);
  
}

void DetectorConstruction::ParameterSetup()
{
  pSPhi = 0. * deg;
  pDPhi = 360. * deg;
  pSTheta = 90. * deg;
  pDTheta = 90. * deg;

  coatingRMax = detectorInnerR;
  coatingRMin = coatingRMax - coatingThickness;

  // Consistency checks
  if (coatingRMin <=0.) G4Exception("DetectorConstruction::ParameterSetup",
				    "ebscatter0000001",
				    FatalException,
				    "Coating inner radius <= 0!");

  insideRMax = coatingRMin;
  insideRMin = 0.0;

  if (detectorInnerR >= detectorOuterR) G4Exception("DetectorConstruction::ParameterSetup",
						    "ebscatter0000001",
						    FatalException,
						    "Detector inner radius >= inner radius");
}

