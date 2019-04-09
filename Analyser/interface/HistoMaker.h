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

class HistoMaker {
private:
  std::string fInFileName;
  TH1D* entryCount;

  std::vector<std::string> NomeStrati;
  std::vector<std::string> posProcess;

  TH1D* GetHisto1D(std::string);
  TH2D* GetHisto2D(std::string);

  void BinLogX(TH1* h);
  void BinLogX(TH2* h);
  void DivideHist(TH1D*);

public:
  HistoMaker(std::string fileName);
  ~HistoMaker();

  TH1D* PrimaryEnergy();

  TH1D* SensitivityElectron(Int_t gap);
  TH1D* SensitivityCharge(Int_t gap);
  TH2D* ElectronEnergyDist(Int_t gap);
  TH1D* ChargeEnergyDist(Int_t gap);
  
  TH2D* NeutronProcess(Int_t nXbins);
  TH2D* ElectronProcess(Int_t nXbins);
};

HistoMaker::HistoMaker(std::string fileName)
{
  fInFileName = fileName;
  entryCount = (TH1D*) TFile::Open(fileName.c_str(), "READ")->Get("Primary Energy");
  entryCount->SetDirectory(0);

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
}

HistoMaker::~HistoMaker()
{
}

TH1D* HistoMaker::GetHisto1D(std::string histoName)
{
  TFile* inFile = TFile::Open(fInFileName.c_str(), "READ");
  TH1D* histo = (TH1D*) inFile->Get(histoName.c_str());
  histo->SetDirectory(0);
  inFile->Close();
  return histo;
}

TH2D* HistoMaker::GetHisto2D(std::string histoName)
{
  TFile* inFile = TFile::Open(fInFileName.c_str(), "READ");
  TH2D* histo = (TH2D*) inFile->Get(histoName.c_str());
  histo->SetDirectory(0);
  inFile->Close();
  return histo;
}

void HistoMaker::BinLogX(TH1* h)
{
  TAxis* axis = h->GetXaxis();
  Int_t bins = axis->GetNbins();
  axis->SetTitle("Primary Neutron Energy [MeV]");
  axis->SetTitleOffset(1.2);
  h->SetStats(0);

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

void HistoMaker::BinLogX(TH2* h)
{
  TAxis* axis = h->GetXaxis();
  Int_t bins = axis->GetNbins();
  axis->SetTitle("Primary Neutron Energy [MeV]");
  h->GetZaxis()->SetTitle("Fraction of process in the Energy [%]");
  axis->SetTitleOffset(1.2);
  h->SetStats(0);

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

TH1D* HistoMaker::PrimaryEnergy()
{
  TAxis* axis = entryCount->GetXaxis();
  Double_t from = axis->GetXmin();
  Double_t to = axis->GetXmax();
  Int_t fromBin = axis->FindBin(from);
  Int_t toBin = axis->FindBin(to);
  auto nBins = toBin - fromBin + 1;
  
  TH1D* primaryDist = new TH1D("PrimaryDist", "Primary Particle Energy", nBins, -10, +4);
  primaryDist->GetYaxis()->SetTitle("Fraction [%]");
  BinLogX(primaryDist);
  Double_t totEntry = entryCount->Integral(fromBin, toBin);

  for(auto bin = fromBin; bin < toBin; bin++)
  {
    Double_t entry = entryCount->GetBinContent(bin);
    Double_t energy = entryCount->GetBinCenter(bin);
    primaryDist->Fill(energy, entry*100/totEntry);
  }

  return primaryDist;
}

void HistoMaker::DivideHist(TH1D* h)
{
  h->Sumw2();
  h->Divide(entryCount);
  h->SetStats(0);
}

TH1D* HistoMaker::SensitivityElectron(Int_t gap)
{
  TH1D* sensitivityPlot = new TH1D(*GetHisto1D(Form("EleGap %i", gap)));
  sensitivityPlot->GetYaxis()->SetTitle("Sensitivity");
  DivideHist(sensitivityPlot);
  return sensitivityPlot;
}

TH2D* HistoMaker::ElectronEnergyDist(Int_t gap)
{
  auto hist = new TH2D(*GetHisto2D(Form("EleEne %i", gap)));
  hist->GetYaxis()->SetTitle("Sensitivity");
  hist->SetTitle(";Primary Particle Energy[MeV];Secondary Electron Energy[MeV]");
  hist->Sumw2();
  hist->Divide(GetHisto1D(Form("EleGap %i", gap)));
  return hist;
}

TH1D* HistoMaker::SensitivityCharge(Int_t gap)
{
  TH1D* sensitivityPlot = new TH1D(*GetHisto1D(Form("ChargeGap %i", gap)));
  sensitivityPlot->GetYaxis()->SetTitle("Sensitivity");
  DivideHist(sensitivityPlot);
  return sensitivityPlot;
}

TH2D* HistoMaker::NeutronProcess(Int_t nXbins)
{
  TH2D* neutronProcess = GetHisto2D("NeutronProcess");
  std::vector<Int_t> nProcess;
  TAxis* axis = neutronProcess->GetXaxis();
  Double_t from = axis->GetXmin();
  Double_t to = axis->GetXmax();
  Int_t fromBin = axis->FindBin(from);
  Int_t toBin = axis->FindBin(to);
  TAxis* axisY = neutronProcess->GetYaxis();
  for(Int_t i = 0; i < posProcess.size(); i++)
  {
    Int_t theBin = axisY->FindBin(i);
    if(neutronProcess->Integral(fromBin, toBin, theBin, theBin) > 0.5)
    {
      nProcess.push_back(i);
    }
  }
  TH2D* processPlot = new TH2D("Neutron Process", "Neutron's Processes", nXbins, -10, 4, nProcess.size(), 0, nProcess.size());
  BinLogX(processPlot);
  for(Int_t j = 0; j < nProcess.size(); j++)
  {
    processPlot->GetYaxis()->SetBinLabel(j+1,posProcess[nProcess[j]].c_str());
  }

  for(Int_t i = 0; i < nXbins; i++)
  {
    Double_t totEntry = neutronProcess->Integral(i, i, 0, posProcess.size());
    Double_t energy = axis->GetBinCenter(i);
    for(Int_t j = 0; j < posProcess.size(); j++)
    {
      Int_t BinY = axisY->FindBin(j);
      Double_t entry = neutronProcess->GetBinContent(i,BinY);
      if(entry < 0.5) continue;
      for(Int_t k = 0; k < nProcess.size(); k++)
      {
        if(j == nProcess[k])
        {
          processPlot->Fill(energy, k, entry*100/totEntry);
          break;
        }
      }
    }
  }
  return processPlot;
}

TH2D* HistoMaker::ElectronProcess(Int_t nXbins)
{
  TH2D* neutronProcess = GetHisto2D("ElectronGenProcess");
  std::vector<Int_t> nProcess;
  TAxis* axis = neutronProcess->GetXaxis();
  Double_t from = axis->GetXmin();
  Double_t to = axis->GetXmax();
  Int_t fromBin = axis->FindBin(from);
  Int_t toBin = axis->FindBin(to);
  TAxis* axisY = neutronProcess->GetYaxis();
  for(Int_t i = 0; i < posProcess.size(); i++)
  {
    Int_t theBin = axisY->FindBin(i);
    if(neutronProcess->Integral(fromBin, toBin, theBin, theBin) > 0.5)
    {
      nProcess.push_back(i);
    }
  }
  TH2D* processPlot = new TH2D("Electron Process", "Generating Processes of Electrons", nXbins, -10, 4, nProcess.size(), 0, nProcess.size());
  BinLogX(processPlot);
  for(Int_t j = 0; j < nProcess.size(); j++)
  {
    processPlot->GetYaxis()->SetBinLabel(j+1,posProcess[nProcess[j]].c_str());
  }

  for(Int_t i = 0; i < nXbins; i++)
  {
    Double_t totEntry = neutronProcess->Integral(i, i, 0, posProcess.size());
    Double_t energy = axis->GetBinCenter(i);
    for(Int_t j = 0; j < posProcess.size(); j++)
    {
      Int_t BinY = axisY->FindBin(j);
      Double_t entry = neutronProcess->GetBinContent(i,BinY);
      if(entry < 0.5) continue;
      for(Int_t k = 0; k < nProcess.size(); k++)
      {
        if(j == nProcess[k])
        {
          processPlot->Fill(energy, k, entry*100/totEntry);
          break;
        }
      }
    }
  }
  return processPlot;
}

#endif
