#define GeantAnalysis_cxx

#include "Geant4Sim/Analyser/interface/GeantAnalysis.h"
#include <iostream>

using namespace std;

void GeantAnalysis::Analysis()
{
  Long64_t nEntries = tEvent->GetEntries();
  int iElectron = 0;
  int iPositron = 0;
  int iGamma    = 0;
  int iCharged  = 0;
  for(Long64_t i = 0; i < nEntries; i++)
  {
    tEvent->GetEntry(i);
    hPrimaryEne->Fill(primaryEne);

    for(int iEl = 0; iEl < nElectron; iEl++)
    {
      tElectron->GetEntry(iElectron);
      if (gap == 0 or gap == 1) 
      {
        hEleGap[0]->Fill(primaryEne);
        bool test = false;
        for (unsigned int ie = 1; ie < partId->size(); ie++)
        {
          test = (partId->at(ie) == 11)|test;
        }
        if (!test) 
        {
          hEleEne[0]->Fill(primaryEne,kineticEnergy);
          hElectronGenProcess->Fill(primaryEne,processNum->at(0));
          hPrimaryProcess->Fill(primaryEne,
                                processNum->at(partId->size()-1));
        }
      }
      iElectron++;
    }
    for(int iEl = 0; iEl < nPositron; iEl++)
    {
      tPositron->GetEntry(iPositron);
      if (gapPos == 0 or gapPos == 1) 
      {
        hEleGap[0]->Fill(primaryEne);
      }
      iPositron++;
    }
    for(int iEl = 0; iEl < nCharged; iEl++)
    {
      tCharged->GetEntry(iCharged);
      if (gapCharge == 0 or gapCharge == 1) 
      {
        hEleGap[0]->Fill(primaryEne);
      }
      iCharged++;
    }
  }
}

int main(Int_t argc, Char_t** argv)
{
  if(argc == 1)
  {
    cout << "Error : no arguments\n";
  }
  else
  {
    string temp = argv[1];
    auto analysis = new GeantAnalysis(temp, true);
    
    for(Int_t i = 2; i < argc; i++)
    { 
      temp = argv[i];
      cout << temp << " has opend" << endl;
      analysis->SetFile(temp);
      analysis->Analysis();
    }
    delete analysis;
  }
  return 0;
}
