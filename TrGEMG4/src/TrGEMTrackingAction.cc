#include "Geant4Sim/TrGEMG4/interface/TrGEMTrackingAction.hh"
#include "Geant4Sim/TrGEMG4/interface/TrGEMAnalysis.hh"

#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"

TrGEMTrackingAction::TrGEMTrackingAction()
{}

TrGEMTrackingAction::~TrGEMTrackingAction()
{}

void TrGEMTrackingAction::PreUserTrackingAction( const G4Track* aTrack )
{
  G4int partCode = aTrack->GetParticleDefinition()->GetPDGEncoding();
  G4double energy = aTrack->GetKineticEnergy();
  G4String process;
  if(aTrack->GetCreatorProcess()!=0) {
    const G4VProcess * genproc = aTrack->GetCreatorProcess();  
    process = genproc->GetProcessName();
  }
  else  {
    process="primary";
    TrGEMAnalysis::GetInstance()->SavePrimary(partCode, energy);
  }
  G4String volume = aTrack->GetVolume()->GetName();
  //G4int copyNo = aTrack->GetVolume()->GetCopyNo();
  G4int copyNo = 0;
  G4int trackID = aTrack->GetTrackID();
  G4int parentID = aTrack->GetParentID();
  
  TrGEMAnalysis::GetInstance()->SaveGenTrack(partCode, process, volume, copyNo, trackID, parentID);
}

void TrGEMTrackingAction::PostUserTrackingAction( const G4Track* aTrack )
{}
