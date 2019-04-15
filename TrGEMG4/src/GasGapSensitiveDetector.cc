#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4HCofThisEvent.hh"
#include "G4HCtable.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

#include "Geant4Sim/TrGEMG4/interface/TrGEMAnalysis.hh"
#include "Geant4Sim/TrGEMG4/interface/GasGapSensitiveDetector.hh"
#include "Geant4Sim/TrGEMG4/interface/GasGapHit.hh"

#include <TVector3.h>

GasGapSensitiveDetector::GasGapSensitiveDetector(G4String SDname)
: G4VSensitiveDetector(SDname),
  charge(0),
  primaryene(0.),
  zinteraction(0),
  contaPrimary(0),
  contaInteraction(0),
  contaSec(0),
  contaSec_B(0),
  contaTrack(0),
  contaGar(0)

{
  G4cout << "*************************************" << G4endl ;
  G4cout << "*** Creating SD with name: "<< SDname << " ***" << G4endl ;  
  G4cout << "*************************************" << G4endl ;

  // Declaration of the hit collection name
  G4String myCollectionName = "GasGapHitCollection" ;
  collectionName.insert( myCollectionName) ;
  
  ttTrack.clear();
  ttTrack_B.clear();
  ttTrack_Gar.clear();
  postTrack.clear();
  
  zinteraction=-5;
}


GasGapSensitiveDetector::~GasGapSensitiveDetector()
{}

G4bool GasGapSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  //This method is called every time a G4Step is performed in the logical volume
  //to which this SD is attached: the GAS GAP.

  //To identify where the step is we use the touchable navigation,
  //Remember we need to use PreStepPoint!

  contaSec=0;
  contaSec_B=0;
  contaTrack=0;
  contaGar=0;

  // G4double COINCIDENCE[9] = {1e10,50,100,500,1e3,1e4,1e5,1e6,1e8}; //50 ns time window fot the coincidence

  G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
  G4int copyNo = touchable->GetVolume(0)->GetCopyNo();
  G4int layerIndex = copyNo;
  G4String volName = touchable->GetVolume(0)->GetName();

  //We get now the energy deposited by this step

  G4double edep = step->GetTotalEnergyDeposit();
  G4Track* track = step->GetTrack();

  G4double energy= step->GetPreStepPoint()->GetKineticEnergy();
  double t= track->GetGlobalTime();
  double x= track->GetPosition().getX();
  double y= track->GetPosition().getY();
  double z= track->GetPosition().getZ();
  TVector3 position(x,y,z);

  double px= track->GetMomentum().getX();
  double py= track->GetMomentum().getY();
  double pz= track->GetMomentum().getZ();
  TVector3 momentum(px, py, pz);


  G4int pdg = track->GetParticleDefinition()->GetPDGEncoding();
  G4int charge = track->GetParticleDefinition()->GetPDGCharge();
  G4StepPoint* point = step->GetPostStepPoint();
  const G4VProcess* proc = point->GetProcessDefinedStep();
  const G4String procname = proc->GetProcessName();
  G4double edepI=edep-step->GetNonIonizingEnergyDeposit();
  G4int trackIndex=track->GetTrackID();
  G4int parentIndex = track->GetParentID();
  double genz= track->GetVertexPosition().getZ(); 
  
  G4String genprocess;
  if(track->GetCreatorProcess()!=0) {
    const G4VProcess * genproc=track->GetCreatorProcess();  
    genprocess = genproc->GetProcessName();
  }
  else  {genprocess="primary";}

  if ((*step->GetSecondary()).size()>0 && trackIndex==1 && contaInteraction == 0){
    zinteraction=z; 
    contaInteraction=1;
  } 

  // we're in one of the gaps

  for(unsigned int T=0;T< ttTrack.size();T++){
    if (ttTrack[T]==trackIndex){
      contaSec=9999;
      break;
    }
  }

  if(contaSec!=9999)  {
    TrGEMAnalysis::GetInstance()->SaveGapTrack(trackIndex, pdg, charge, volName, copyNo, energy ,position, momentum);
    contaSec=trackIndex;
    ttTrack.push_back(trackIndex);
  }  
    

  //check if edep is from primary or secondary:
  G4String isPri = step->GetTrack()->GetTrackID() == 1 ? "Yes" : "No";

  // Tricks to implement hits 
  hitMap_t::iterator it = hitMap.find(layerIndex) ;
  GasGapHit* aHit = 0 ;
  if( it != hitMap.end() ) {
    aHit = it->second ;
  }
  else {
    aHit = new GasGapHit(layerIndex) ;
    hitMap.insert( std::make_pair(layerIndex, aHit) ) ;
    hitCollection->insert(aHit) ;
  }
  aHit->AddEdep(edep) ;

  return true;
}

void GasGapSensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
  hitCollection = new GasGapHitCollection(GetName(), collectionName[0]) ;
  static G4int HCID = -1 ;
  if(HCID<0) HCID = GetCollectionID(0) ; // <<-- this is to get an ID for collectionName[0]
  HCE->AddHitsCollection(HCID, hitCollection) ;

  // Reset map of hits
  hitMap.clear() ;
}

void GasGapSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  ttTrack.clear();
  ttTrack_B.clear();
  ttTrack_Gar.clear();
  postTrack.clear();

  contaPrimary=0;
  contaInteraction=0;
  contaSec=0;
  contaSec_B=0;
  contaTrack=0;
  contaGar=0;
  primaryene=0.;
  zinteraction=-5.;
 
 
}
