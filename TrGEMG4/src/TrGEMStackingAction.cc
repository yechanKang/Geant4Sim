#include "Geant4Sim/TrGEMG4/interface/TrGEMStackingAction.hh"
#include "Geant4Sim/TrGEMG4/interface/TrGEMAnalysis.hh"

#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"

TrGEMStackingAction::TrGEMStackingAction()
{}

TrGEMStackingAction::~TrGEMStackingAction() 
{}

G4ClassificationOfNewTrack TrGEMStackingAction::ClassifyNewTrack( const G4Track * aTrack ) 
{
  // always "urgent" in current applications
  G4ClassificationOfNewTrack result( fUrgent );
  
  return result;

}

