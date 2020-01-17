#ifndef GeantAnalysis_h
#define GeantAnalysis_h

#include "Geant4Sim/TrGEMG4/data/Geometry.h"
#include "Geant4Sim/TrGEMG4/data/FTFP_BERT.h"

#include <TROOT.h>
#include <TMath.h>
#include <TFile.h>

#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <iostream>
#include <vector>

using namespace std;

class GeantAnalysis {
private:
  // Writer
  TFile* fOutput;

  TH1D* hPrimaryEne;
  TH1D* hSens;

  TH1D* hElect;
  TH1D* hGamma;

  TH2D* hGammaPrimaryProcess;
  TH2D* hGammaGenProcess;
  TH2D* hElectronPrimaryProcess;
  TH2D* hElectronSecondaryProcess;
  TH2D* hElectronGenProcess;

private:
  double primaryEne;

  double lowBound, highBound;

  TTree* tEvent;

  int eleGap[4];
  int posGap[4];
  int chargeGap[4];
  double edepGap[4];

  int sensChar;
  int nElectron;
  int nPositron;
  int nGamma;
  int nCharged;
  
  TTree* tElectron;
  TTree* tPositron;
  TTree* tGamma;
  TTree* tCharged;

  int            gap;
  int            gapPos;
  int            gapCharge;
  vector<int>    *partId        = 0;
  vector<int>    *processNum    = 0;
  vector<int>    *processVol    = 0;
  double kineticEnergy;

  double positionX, positionY, positionZ;
  double momentumX, momentumY, momentumZ;

  vector<string> layerName;
  vector<string> posProcess;

public:
  GeantAnalysis(string temp, bool t);
  ~GeantAnalysis();
  void SetFile(string fileName);
  void ResetBranch();
  void Analysis();

  void BinLogX(TH1* h);
  void BinLogX(TH2* h);
  void BinLogY(TH2* h);
  int findVolume(int);
};

#endif

#ifdef GeantAnalysis_cxx
GeantAnalysis::GeantAnalysis(string temp, bool t)
{
  if (t) {
    lowBound = -12;
    highBound = 3;
  } else {
    lowBound = -12;
    highBound = 3;
  }
  int nBins = 1000;
  layerName = Geometry::layerName;
  posProcess = FTFP_BERT::processes;
  fOutput = TFile::Open(temp.c_str(), "RECREATE");

  hPrimaryEne = new TH1D("Primary Energy", "Primary Energy", nBins, lowBound, highBound);
  hSens = new TH1D("Sensitivity", "Sensitivity", nBins, lowBound, highBound);

  hElect = new TH1D("Electron", "Electron", nBins, lowBound, highBound);
  hGamma = new TH1D("Gamma", "Gamma", nBins, lowBound, highBound);
  BinLogX(hPrimaryEne);
  BinLogX(hSens);
  BinLogX(hElect);
  BinLogX(hGamma);

  hElectronPrimaryProcess = new TH2D("ElectronPrimaryProcess","ElectronPrimaryProcess",nBins, lowBound, highBound, posProcess.size(), 0, posProcess.size());
  hElectronSecondaryProcess = new TH2D("ElectronSecondaryProcess","ElectronSecondaryProcess",nBins, lowBound, highBound, posProcess.size(), 0, posProcess.size());
  hElectronGenProcess = new TH2D("ElectronGenProcess","ElectronGenProcess",nBins, lowBound, highBound, posProcess.size(), 0, posProcess.size());
  hGammaPrimaryProcess = new TH2D("GammaPrimaryProcess","GammaPrimaryProcess",nBins, lowBound, highBound, posProcess.size(), 0, posProcess.size());
  hGammaGenProcess = new TH2D("GammaGenProcess","GammaGenProcess",nBins, lowBound, highBound, posProcess.size(), 0, posProcess.size());

  BinLogX(hElectronPrimaryProcess);
  BinLogX(hElectronSecondaryProcess);
  BinLogX(hElectronGenProcess);
  BinLogX(hGammaPrimaryProcess);
  BinLogX(hGammaGenProcess);
}

GeantAnalysis::~GeantAnalysis()
{
  fOutput->Write();
  fOutput->Close();
  return;
}

void GeantAnalysis::SetFile(string fileName)
{
  TFile* inFile = TFile::Open(fileName.c_str(), "READ");
  tEvent = (TTree*) inFile->Get("Event");

  tEvent->SetBranchAddress("primaryEne", &primaryEne);
  tEvent->SetBranchAddress("sensChar", &sensChar);
  tEvent->SetBranchAddress("EleGap", eleGap);
  tEvent->SetBranchAddress("PosGap", posGap);
  tEvent->SetBranchAddress("ChargeGap", chargeGap);
  tEvent->SetBranchAddress("nElectron", &nElectron);
  tEvent->SetBranchAddress("nPositron", &nPositron);
  tEvent->SetBranchAddress("nGamma",    &nGamma);
  tEvent->SetBranchAddress("nCharged",  &nCharged);
  tEvent->SetBranchAddress("edepGap", edepGap);

  tElectron = (TTree*) inFile->Get("Electron");
  tElectron->SetBranchAddress("gap",      &gap);
  tElectron->SetBranchAddress("partId",   &partId);
  tElectron->SetBranchAddress("processVol", &processVol);
  tElectron->SetBranchAddress("processNum", &processNum);
  tElectron->SetBranchAddress("kineticEnergy", &kineticEnergy);
  tElectron->SetBranchAddress("positionX", &positionX);
  tElectron->SetBranchAddress("positionY", &positionY);
  tElectron->SetBranchAddress("positionZ", &positionZ);
  tElectron->SetBranchAddress("momentumX", &momentumX);
  tElectron->SetBranchAddress("momentumY", &momentumY);
  tElectron->SetBranchAddress("momentumZ", &momentumZ);

  tPositron = (TTree*) inFile->Get("Positron");
  tPositron->SetBranchAddress("gap",      &gapPos);
  tPositron->SetBranchAddress("partId",   &partId);
  tPositron->SetBranchAddress("processVol", &processVol);
  tPositron->SetBranchAddress("processNum", &processNum);
  tPositron->SetBranchAddress("kineticEnergy", &kineticEnergy);
  tPositron->SetBranchAddress("positionX", &positionX);
  tPositron->SetBranchAddress("positionY", &positionY);
  tPositron->SetBranchAddress("positionZ", &positionZ);
  tPositron->SetBranchAddress("momentumX", &momentumX);
  tPositron->SetBranchAddress("momentumY", &momentumY);
  tPositron->SetBranchAddress("momentumZ", &momentumZ);

  tGamma = (TTree*) inFile->Get("Gamma");
  tGamma->SetBranchAddress("gap",      &gapCharge);
  tGamma->SetBranchAddress("partId",   &partId);
  tGamma->SetBranchAddress("processVol", &processVol);
  tGamma->SetBranchAddress("processNum", &processNum);
  tGamma->SetBranchAddress("kineticEnergy", &kineticEnergy);
  tGamma->SetBranchAddress("positionX", &positionX);
  tGamma->SetBranchAddress("positionY", &positionY);
  tGamma->SetBranchAddress("positionZ", &positionZ);
  tGamma->SetBranchAddress("momentumX", &momentumX);
  tGamma->SetBranchAddress("momentumY", &momentumY);
  tGamma->SetBranchAddress("momentumZ", &momentumZ);

  tCharged = (TTree*) inFile->Get("Charged");
  tCharged->SetBranchAddress("partId",   &partId);
  tCharged->SetBranchAddress("processVol", &processVol);
  tCharged->SetBranchAddress("processNum", &processNum);
}

void GeantAnalysis::BinLogX(TH1* h)
{
  TAxis* axis = h->GetXaxis();
  Int_t bins = axis->GetNbins();

  Double_t from = axis->GetXmin();
  Double_t to = axis->GetXmax();
  Double_t width = (to-from) / bins;
  Double_t* new_bins = new Double_t[bins + 1];

  for (Int_t i = 0; i <= bins; i++)
  {
    new_bins[i] = TMath::Power(10, from + i * width);
  }
  axis->Set(bins, new_bins);
  delete new_bins;
}

void GeantAnalysis::BinLogX(TH2* h)
{
  TAxis* axis = h->GetXaxis();
  Int_t bins = axis->GetNbins();

  Double_t from = axis->GetXmin();
  Double_t to = axis->GetXmax();
  Double_t width = (to-from) / bins;
  Double_t* new_bins = new Double_t[bins + 1];

  for (Int_t i = 0; i <= bins; i++)
  {
    new_bins[i] = TMath::Power(10, from + i * width);
  }
  axis->Set(bins, new_bins);
  delete new_bins;
}

void GeantAnalysis::BinLogY(TH2* h)
{
  TAxis* axis = h->GetYaxis();
  Int_t bins = axis->GetNbins();

  Double_t from = axis->GetXmin();
  Double_t to = axis->GetXmax();
  Double_t width = (to-from) / bins;
  Double_t* new_bins = new Double_t[bins + 1];

  for (Int_t i = 0; i <= bins; i++)
  {
    new_bins[i] = TMath::Power(10, from + i * width);
  }
  axis->Set(bins, new_bins);
  delete new_bins;
}

int GeantAnalysis::findVolume(int volNum) {
  if (volNum == 4) return 0;
  else if (volNum == 8) return 1;
  else if (volNum == 12) return 2;
  else if (volNum == 16) return 3;
  else return -1;
}

#endif
