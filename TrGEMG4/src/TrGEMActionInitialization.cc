#include "Geant4Sim/TrGEMG4/interface/TrGEMActionInitialization.hh"
#include "Geant4Sim/TrGEMG4/interface/TrGEMPrimaryGeneratorAction.hh"
#include "Geant4Sim/TrGEMG4/interface/TrGEMRunAction.hh"
#include "Geant4Sim/TrGEMG4/interface/TrGEMEventAction.hh"
#include "Geant4Sim/TrGEMG4/interface/TrGEMSteppingAction.hh"
#include "Geant4Sim/TrGEMG4/interface/TrGEMStackingAction.hh"
#include "Geant4Sim/TrGEMG4/interface/TrGEMTrackingAction.hh"

#include "G4String.hh"

TrGEMActionInitialization::TrGEMActionInitialization(char* partName_)
: G4VUserActionInitialization(), partName(partName_)
{
}

TrGEMActionInitialization::~TrGEMActionInitialization()
{
}

//0000000000000000000000000000000000000000000000000000000000000000000000000000

void TrGEMActionInitialization::BuildForMaster() const {
  TrGEMRunAction* runAction = new TrGEMRunAction;
	SetUserAction(runAction);
}

void TrGEMActionInitialization::Build() const {
  SetUserAction(new TrGEMPrimaryGeneratorAction(partName));
	
  TrGEMRunAction* runAction = new TrGEMRunAction;
  SetUserAction(runAction);

  TrGEMEventAction* eventAction = new TrGEMEventAction();
  SetUserAction(eventAction);

  TrGEMSteppingAction* steppingAction = new TrGEMSteppingAction();
  SetUserAction(steppingAction);

  TrGEMStackingAction* stackingAction = new TrGEMStackingAction();
  SetUserAction(stackingAction);
  
  TrGEMTrackingAction* trackingAction = new TrGEMTrackingAction();
  SetUserAction(trackingAction);
}
