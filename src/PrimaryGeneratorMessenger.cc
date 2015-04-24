
/////////////////////////////////////////////////////////////////////////
// G4Tutorial:
//
// PrimaryGeneratorMessenger.cc
// 
// Messenger for changing properties associated with the primary particle/
// vertex generator
//
/////////////////////////////////////////////////////////////////////////


#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGenerator.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGenerator* primGen) :
    primaryGenerator(primGen) {

  sourceDirectory = new G4UIdirectory("/source/");
  sourceDirectory -> SetGuidance("Particle source commands");

  primEnergyCmd = 
             new G4UIcmdWithADoubleAndUnit("/source/primEnergy", this);
  primEnergyCmd -> SetGuidance("Specification of primary electron energy");
  primEnergyCmd -> SetParameterName("primEnergy", true);
  primEnergyCmd -> SetDefaultValue(1.0);
  primEnergyCmd -> SetDefaultUnit("MeV");
  primEnergyCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);  

  primParticleCmd = 
             new G4UIcmdWithAString("/source/primParticle", this);
  primParticleCmd -> SetGuidance("Specification of primary particle type");
  primParticleCmd -> SetParameterName("primParticle", true);
  primParticleCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);  

  sigmaEnergyCmd = 
             new G4UIcmdWithADoubleAndUnit("/source/sigmaEnergy", this);
  sigmaEnergyCmd -> SetGuidance("Specification of sigma (energy distr.)");
  sigmaEnergyCmd -> SetParameterName("sigmaEnergy", true);
  sigmaEnergyCmd -> SetDefaultValue(0.0);
  sigmaEnergyCmd -> SetDefaultUnit("keV");
  sigmaEnergyCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);  

  incidAngleCmd = 
              new G4UIcmdWithADoubleAndUnit("/source/incidentAngle", this);
  incidAngleCmd -> SetGuidance("Specification of incident beam angle");
  incidAngleCmd -> SetParameterName("incidentAngle", true);
  incidAngleCmd -> SetDefaultValue(0.0);
  incidAngleCmd -> SetDefaultUnit("deg");
  incidAngleCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);  

  zVertexCmd = 
              new G4UIcmdWithADoubleAndUnit("/source/zVertex", this);
  zVertexCmd -> SetGuidance("Specification of primary vertex Z coordinate");
  zVertexCmd -> SetParameterName("zVertex", true);
  zVertexCmd -> SetDefaultValue(0.0);
  zVertexCmd -> SetDefaultUnit("mm");
  zVertexCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);  

  referenceCmd = 
             new G4UIcmdWithAString("/source/reference", this);
  referenceCmd -> SetGuidance("Specification of the experimental reference of this simulation");
  referenceCmd -> SetParameterName("reference", true);
  referenceCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);  
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger() {

  delete incidAngleCmd;
  delete sigmaEnergyCmd;
  delete primParticleCmd;
  delete primEnergyCmd;
  delete referenceCmd;
  delete sourceDirectory;
  delete zVertexCmd;
}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* cmd, G4String val) {

  if (cmd == primEnergyCmd) 
     primaryGenerator -> SetPrimaryKineticEnergy(
                            primEnergyCmd -> GetNewDoubleValue(val));
  if (cmd == primParticleCmd) 
     primaryGenerator -> SetParticleType(val);

  if (cmd == sigmaEnergyCmd) 
     primaryGenerator -> SetSigmaKineticEnergy(
                            sigmaEnergyCmd -> GetNewDoubleValue(val));
  if (cmd == incidAngleCmd) 
     primaryGenerator -> SetIncidentAngle(
                            incidAngleCmd -> GetNewDoubleValue(val));
  if (cmd == zVertexCmd) 
     primaryGenerator -> SetVertexZ(
                            zVertexCmd -> GetNewDoubleValue(val));
   if (cmd == referenceCmd) 
     primaryGenerator -> SetReference(val);
}
