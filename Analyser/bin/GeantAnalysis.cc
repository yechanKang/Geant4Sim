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
    hPrimaryEne->Fill(primaryEne*1E+6);
    cout << primaryEne*1E+6 << endl;

    for(int iEl = 0; iEl < nElectron; iEl++)
    {
      tElectron->GetEntry(iElectron);
      hElect->Fill(primaryEne*1E+6);
      bool test = false;
      for (unsigned int ie = 1; ie < partId->size(); ie++)
      {
        test = (partId->at(ie) == 11)|test;
      }
      if (!test) 
      {
        hElectronGenProcess->Fill(primaryEne*1E+6,
                                  processNum->at(0));
        hElectronPrimaryProcess->Fill(primaryEne*1E+6,
                                      processNum->at(partId->size()-1));
        hElectronSecondaryProcess->Fill(primaryEne*1E+6,
                                        processNum->at(partId->size()-2));
      }
      iElectron++;
    }
    for(int iG = 0; iG < nGamma; iG++)
    {
      tGamma->GetEntry(iGamma);
      hGamma->Fill(primaryEne*1E+6);
      hGammaGenProcess->Fill(primaryEne*1E+6,
                             processNum->at(0));
      hGammaPrimaryProcess->Fill(primaryEne*1E+6,
                                 processNum->at(partId->size()-1));
      iGamma++;
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
