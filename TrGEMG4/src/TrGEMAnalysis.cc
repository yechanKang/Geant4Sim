#include "Geant4Sim/TrGEMG4/interface/TrGEMAnalysis.hh"
#include "Geant4Sim/TrGEMG4/data/FTFP_BERT.h"

#include "G4UnitsTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4String.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>

#include <TVector3.h>

TrGEMAnalysis* TrGEMAnalysis::singleton = 0 ;

TrGEMAnalysis* TrGEMAnalysis::GetInstance() {
  if ( singleton == 0 ) {
    static TrGEMAnalysis analysis;
    singleton = &analysis;
  }
  return singleton;
}

TrGEMAnalysis::~TrGEMAnalysis() 
{}

TrGEMAnalysis::TrGEMAnalysis() 
{
  m_ROOT_file = 0;
  NomeStrati= 
  {
    "FakeBottom",                                   //Fake             --- 0
    "DriftCopper1","DriftBoard","DriftCopper2",     //Drift Board      --- 1,2,3
    "GasGap1",                                      //Drift Gap        --- 4
    "Gem1Copper1","Gem1","Gem1Copper2",             //gem1             --- 5,6,7
    "GasGap2",                                      //Transfer I Gap   --- 8
    "Gem2Copper1","Gem2","Gem2Copper2",             //gem2             --- 9,10,11
    "GasGap3",                                      //Transfer II Gap  --- 12
    "Gem3Copper1","Gem3","Gem3Copper2",             //gem3             --- 13,14,15
    "GasGap4",                                      //Induction Gap    --- 16
    "ReadCopper1","ReadoutBoard","ReadCopper2",     //Readout Board    --- 17,18,19
    "FakeTop"                                       //Fake             --- 20
  };
  posProcess=FTFP_BERT::processes;                  //It comes from FTFP_BERT_HP
}

void TrGEMAnalysis::SetFileName(const G4String name)
{
  fileName = name;
}


void TrGEMAnalysis::PrepareNewRun(const G4Run* /*aRun*/)
{

  eventCounter = 0 ;
  //Reset variables relative to the run
  
  // create ROOT file
  //fileName = "root://cms-xrdr.sdfarm.kr:1094///xrd/store/user/yekang/TrGEMG4/" + fileName + ".root";
  fileName = fileName + ".root";
  m_ROOT_file = TFile::Open(fileName.c_str(),"RECREATE");
  if(m_ROOT_file) 
    G4cout << "ROOT file " << fileName << " is created " << G4endl;
  else {
      //G4Exception("ROOT file task3.root has not been created!");
    G4cerr << "ROOT file task3.root has not been created!" << G4endl;
  }

  // create tree and branch(es)
  tEvent = new TTree("Event","Run with awesome data") ;

  tEvent->Branch("primaryEne",  &primaryEne,  "primaryEne/D");
  tEvent->Branch("primaryPart", &primaryPart, "primaryPart/I");

  tEvent->Branch("EleGap",    &eleGap,    "EleGap[4]/I");
  tEvent->Branch("PosGap",    &posGap,    "PosGap[4]/I");
  tEvent->Branch("ChargeGap", &chargeGap, "ChargeGap[4]/I");

  tEvent->Branch("nElectron", &nElectron, "nElectron/I");
  tEvent->Branch("nPositron", &nPositron, "nPositron/I");
  tEvent->Branch("nGamma",    &nGamma,    "nGamma/I"   );
  tEvent->Branch("nCharged",  &nCharged,  "nCharged/I" );

  tElectron = new TTree("Electron", "Electron Info");
  tElectron->Branch("gap",        &gap, "gap/I");
  tElectron->Branch("partId",     &partId);
  tElectron->Branch("processVol", &processVol);
  tElectron->Branch("volCopyNo",  &volCopyNo);
  tElectron->Branch("processNum", &processNum);
  tElectron->Branch("kineticEnergy", &kineticEnergy);
  tElectron->Branch("positionX", &positionX, "positionX/D");
  tElectron->Branch("positionY", &positionY, "positionY/D");
  tElectron->Branch("positionZ", &positionZ, "positionZ/D");
  tElectron->Branch("momentumX", &momentumX, "momentumX/D");
  tElectron->Branch("momentumY", &momentumY, "momentumY/D");
  tElectron->Branch("momentumZ", &momentumZ, "momentumZ/D");
  
  tPositron = new TTree("Positron", "Positron Info");
  tPositron->Branch("gap",        &gap, "gap/I");
  tPositron->Branch("partId",   &partId);
  tPositron->Branch("processVol", &processVol);
  tPositron->Branch("volCopyNo",  &volCopyNo);
  tPositron->Branch("processNum", &processNum);
  tPositron->Branch("kineticEnergy", &kineticEnergy);
  tPositron->Branch("positionX", &positionX, "positionX/D");
  tPositron->Branch("positionY", &positionY, "positionY/D");
  tPositron->Branch("positionZ", &positionZ, "positionZ/D");
  tPositron->Branch("momentumX", &momentumX, "momentumX/D");
  tPositron->Branch("momentumY", &momentumY, "momentumY/D");
  tPositron->Branch("momentumZ", &momentumZ, "momentumZ/D");
  
  tGamma = new TTree("Gamma", "Gamma Info");
  tGamma->Branch("gap",        &gap, "gap/I");
  tGamma->Branch("partId",   &partId);
  tGamma->Branch("processVol", &processVol);
  tGamma->Branch("volCopyNo",  &volCopyNo);
  tGamma->Branch("processNum", &processNum);
  tGamma->Branch("kineticEnergy", &kineticEnergy);
  tGamma->Branch("positionX", &positionX, "positionX/D");
  tGamma->Branch("positionY", &positionY, "positionY/D");
  tGamma->Branch("positionZ", &positionZ, "positionZ/D");
  tGamma->Branch("momentumX", &momentumX, "momentumX/D");
  tGamma->Branch("momentumY", &momentumY, "momentumY/D");
  tGamma->Branch("momentumZ", &momentumZ, "momentumZ/D");
  
  tCharged = new TTree("Charged", "other charged particle info");
  tCharged->Branch("gap",      &gap, "gap/I");
  tCharged->Branch("partId",   &partId);
  tCharged->Branch("processVol", &processVol);
  tCharged->Branch("volCopyNo",  &volCopyNo);
  tCharged->Branch("processNum", &processNum);
}

void TrGEMAnalysis::PrepareNewEvent(const G4Event* /*anEvent*/)
{
  //Reset variables relative to this event
  isNewEvent = true ;

  primaryEne=0.;
  
  nElectron = 0;
  nPositron = 0;
  nGamma    = 0;
  nCharged  = 0;

  for ( int i = 0; i < 4; i++) {
    eleGap[i]    = 0;
    posGap[i]    = 0;
    chargeGap[i] = 0;
  }

  genMap.clear();
}

void TrGEMAnalysis::EndOfEvent(const G4Event* /*anEvent*/)
{
  tEvent->Fill() ; 
}

void TrGEMAnalysis::EndOfRun(const G4Run* aRun)
{

  //Some print outs
  G4int numEvents = aRun->GetNumberOfEvent();
  if(numEvents == 0) { return; }

  // Writing and closing the ROOT file
  G4cout << "ROOT: files writing..." << G4endl;
  m_ROOT_file->Write();
  G4cout << "ROOT: files closing..." << G4endl;
  m_ROOT_file->Close();
  delete m_ROOT_file;

}

void TrGEMAnalysis::SavePrimary(G4int primaryPart_, G4double primaryEne_){
  primaryPart = primaryPart_;
  primaryEne = primaryEne_;
}

void TrGEMAnalysis::SaveGapTrack(
  G4int trackId,
  G4int gapPart, 
  G4int gapCharge,
  G4String volume,
  G4int    copyNo,
  G4double kinene, 
  TVector3 position, 
  TVector3 momentum ) 
{
  if (trackId == 1) return;
  G4int volNum = FindVolume(volume);
  gap = volNum / 4 - 1;
  kineticEnergy = kinene;
  positionX = position.x();
  positionY = position.y();
  positionZ = position.z();
  momentumX = momentum.x();
  momentumY = momentum.y();
  momentumZ = momentum.z();
  PreSavingTrack(trackId);
  if (gapPart == 11)
  {
    nElectron++;
    eleGap[gap]++;
    tElectron->Fill();
  } else if (gapPart == -11)
  {
    nPositron++;
    posGap[gap]++;
    tPositron->Fill();
  } else if (gapPart == 22)
  {
    nGamma++;
    tGamma->Fill();
  } else if (gapCharge != 0) 
  {
    nCharged++;
    chargeGap[gap]++;
    tCharged->Fill();
  }
  PostSavingTrack();
}

void TrGEMAnalysis::SaveGenTrack(
  G4int partCode, 
  std::string process, 
  std::string volume, 
  G4int copyNo,
  G4int trackID, 
  G4int parentID)
{
  if(process == "primary") return;
  G4int volNum = FindVolume(volume);
  G4int processNum = FindGeneratingProcess(process);
  G4int id = trackID;
  genMap[id][0] = parentID;
  genMap[id][1] = partCode;
  genMap[id][2] = processNum;
  genMap[id][3] = volNum;
  genMap[id][4] = copyNo;
}

G4int TrGEMAnalysis::FindVolume(std::string volume)
{
  for(G4int i = 0; i < 21; i++)
  {
    if(volume == (NomeStrati[i]+"Log")) return i;
    else if(volume == NomeStrati[i]) return i;
  }
  return -1;
}

G4int TrGEMAnalysis::FindGapTrackProcess(std::string process)
{
  for(unsigned int i = 0; i < posProcess.size(); i++)
  {
    if(posProcess[i] == process) return i;
  }
  G4cout << process << G4endl;
  return -1;
}

G4int TrGEMAnalysis::FindGeneratingProcess(std::string process)
{
  for(unsigned int i = 0; i < posProcess.size(); i++)
  {
    if(posProcess[i] == process) return i;
  }
  G4cout << process << G4endl;
  return -1;
}

void TrGEMAnalysis::PreSavingTrack(G4int trackID)
{
  G4int id = genMap[trackID][0];
  partId.push_back(genMap[trackID][1]);
  processNum.push_back(genMap[trackID][2]);
  processVol.push_back(genMap[trackID][3]);
  volCopyNo.push_back(genMap[trackID][4]);
  while(id != 1) {
    partId.push_back(genMap[id][1]);
    processNum.push_back(genMap[id][2]);
    processVol.push_back(genMap[id][3]);
    volCopyNo.push_back(genMap[id][4]);
    id = genMap[id][0];
  }
}

void TrGEMAnalysis::PostSavingTrack()
{
  partId.clear();
  processNum.clear();
  processVol.clear();
  volCopyNo.clear();

  kineticEnergy = 0;
  positionX = 0;
  positionY = 0;
  positionZ = 0;
  momentumX = 0;
  momentumY = 0;
  momentumZ = 0;
}
