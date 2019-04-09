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
  for(Long64_t i = 0; i < nEntries; i++)
  {
    tEvent->GetEntry(i);
    hPrimaryEne->Fill(primaryEne);

    for(int iEl = 0; iEl < nElectron; iEl++)
    {
      tElectron->GetEntry(iElectron);
      if (gap == 1 or gap == 0) 
      {
        hEleGap[0]->Fill(primaryEne);
        bool test = false;
        for (unsigned int ie = 1; ie < partId->size(); ie++)
        {
          test = (partId->at(ie) == 11)|test;
        }
        if (!test) hEleEne[0]->Fill(primaryEne,kineticEnergy);
      }
      iElectron++;
    }

    //for(int i = 0; i < nPositron; i++)
    //{
    //  tPositron->GetEntry(i);
    //  hPosGap[gap]->Fill(primaryEne);
    //  iPositron++;
    //}
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
    auto analysis = new GeantAnalysis(temp);
    
    for(Int_t i = 2; i < argc; i++)
    { 
      temp = argv[i];
      analysis->SetFile(temp);
      analysis->Analysis();
    }
    delete analysis;
  }
  return 0;
}
