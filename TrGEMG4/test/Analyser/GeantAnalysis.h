#ifndef GeantAnalysis_h
#define GeantAnalysis_h

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

  TTree* fTree;

  TH1D* hPrimaryEne;
  TH1D* hEleGap[4];
  TH1D* hPosGap[4];
  TH1D* hChargeGap[4];

  TH1D* hEleEne[4];
  TH1D* hPosEne[4];
  TH1D* hChargeEne[4];

  TH2D* hPrimaryProcess;
  TH2D* hElectronGenProcess;
  // TH2D* hGenerationLow;
  // TH2D* hGenerationHigh;

  // Reader
  TTree* fChain;

private:
  Double_t primaryEne;
  Int_t EleGap[4];
  Int_t PosGap[4];
  Int_t ChargeGap[4];

  vector<Int_t> *gapTrackPart = 0;
  vector<Int_t> *gapTrackCharge = 0;
  vector<Int_t> *gapTrackIntNum = 0;
  vector<Int_t> *gapTrackGenProcessNum = 0;
  vector<Int_t> *gapTrackVolume = 0;
  vector<Double_t> *gapTrackEne = 0;

  vector<string> NomeStrati;
  vector<string> posProcess;

public:
  GeantAnalysis(string temp);
  ~GeantAnalysis();
  void SetFile(string fileName);
  void ResetBranch();
  void Analysis();

  void BinLogX(TH1* h);
  void BinLogX(TH2* h);
  int findVolume(int);
};

#endif

#ifdef GeantAnalysis_cxx
GeantAnalysis::GeantAnalysis(string temp)
{
  NomeStrati = {
    "FakeBottom",                                   //Fake
    "DriftCopper1","DriftBoard","DriftCopper2",     //Drift Board
    "GasGap1",                                      //Drift Gap
    "Gem1Copper1","Gem1","Gem1Copper2",             //gem1
    "GasGap2",                                      //Transfer I Gap
    "Gem2Copper1","Gem2","Gem2Copper2",             //gem2
    "GasGap3",                                      //Transfer II Gap
    "Gem3Copper1","Gem3","Gem3Copper2",             //gem3
    "GasGap4",                                      //Induction Gap
    "ReadCopper1","ReadoutBoard","ReadCopper2",     //Readout Board
    "FakeTop"                                       //Fake
  };
  posProcess = {                                    //It comes from FTFP_BERT_HP
    "Transportation", "msc", "hIoni", "ionIoni", "hBrems", "hPairProd", "CoulombScat",
    "eIoni", "eBrem", "annihil", "phot", "compt", "conv", "muIoni", "muBrems", "muPairProd",
    "photonNuclear", "electronNuclear", "positronNuclear", "muonNuclear", "Decay", "hadElastic",
    "neutronInelastic", "nCapture", "nFission", "protonInelastic", 
    "pi+Inelastic", "pi-Inelastic", "kaon+Inelastic", "kaon-Inelastic", "kaon0LInelastic", "kaon0SInelastic",
    "lambdaInelastic", "anti-lambdaInelastic", "sigma-Inelastic", "anti_sigma-Inelastic", "sigma+Inelastic",
    "anti_sigma+Inelastic", "xi-Inelastic", "anti_xi-Inelastic", "xi0Inelastic", "anti_xi0Inelastic",
    "omega-Inelastic", "anti_omega-Inelastic", "anti_protonInelastic", "anti_neutronInelastic", "anti_deuteronInelastic",
    "anti_tritonInelastic", "anti_He3Inelastic", "anti_alphaInelastic", "hFritiofCaptureAtRest", "hBertiniCaptureAtRest",
    "muMinusCaptureAtRest", "dInelastic", "tInelastic", "He3Inelastic", "alphaInelastic", "ionInelastic ",
  };
  fOutput = TFile::Open(temp.c_str(), "RECREATE");
  fTree = new TTree("Event","Event") ;

  hPrimaryEne = new TH1D("Primary Energy", "Primary Energy", 100, -10, +4);
  for (int i = 0; i < 4; i++) {
    hEleGap   [i] = new TH1D(Form("EleGap %i", i), "EleGap", 100, -10, +4);
    hPosGap   [i] = new TH1D(Form("PosGap %i", i), "PosGap", 100, -10, +4);
    hChargeGap[i] = new TH1D(Form("ChargeGap %i", i), "ChargeGap", 100, -10, +4);
    hEleEne   [i] = new TH1D(Form("EleEne %i", i),"EleEne",100, -10, +4);
    hPosEne   [i] = new TH1D(Form("PosEne %i", i),"PosEne",100, -10, +4);
    hChargeEne[i] = new TH1D(Form("ChargeEne %i", i),"ChargeEne",100, -10, +4);
    BinLogX(hEleGap[i]);
    BinLogX(hPosGap[i]);
    BinLogX(hChargeGap[i]);
    BinLogX(hEleEne[i]);
    BinLogX(hPosEne[i]);
    BinLogX(hChargeEne[i]);
  }

  BinLogX(hPrimaryEne);



  hPrimaryProcess = new TH2D("PrimaryProcess","PrimaryProcess",100, -10, +4, posProcess.size(), 0, posProcess.size());
  hElectronGenProcess = new TH2D("ElectronGenProcess","ElectronGenProcess",100, -10, +4, posProcess.size(), 0, posProcess.size());

  BinLogX(hPrimaryProcess);
  BinLogX(hElectronGenProcess);
  

}

GeantAnalysis::~GeantAnalysis()
{
  fOutput->Write("",TObject::kOverwrite);
  fOutput->Close();
  return;
}

void GeantAnalysis::SetFile(string fileName)
{
  TFile* inFile = TFile::Open(fileName.c_str(), "READ");
  fChain = (TTree*) inFile->Get("Event");

  fChain->SetBranchAddress("primaryEne", &primaryEne);
  fChain->SetBranchAddress("EleGap", EleGap);
  fChain->SetBranchAddress("PosGap", PosGap);
  fChain->SetBranchAddress("ChargeGap", ChargeGap);

  fChain->SetBranchAddress("gapTrackPart", &gapTrackPart);
  fChain->SetBranchAddress("gapTrackCharge", &gapTrackCharge);
  fChain->SetBranchAddress("gapTrackIntNum", &gapTrackIntNum);
  fChain->SetBranchAddress("gapTrackGenProcessNum", &gapTrackGenProcessNum);
  fChain->SetBranchAddress("gapTrackEne", &gapTrackEne);
  fChain->SetBranchAddress("gapTrackVolume", &gapTrackVolume);
}

void GeantAnalysis::ResetBranch()
{
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

int GeantAnalysis::findVolume(int volNum) {
  if (volNum == 4) return 0;
  else if (volNum == 8) return 1;
  else if (volNum == 12) return 2;
  else if (volNum == 16) return 3;
  else return -1;
}

#endif
