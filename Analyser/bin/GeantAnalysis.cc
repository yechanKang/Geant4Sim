#define GeantAnalysis_cxx

#include "Geant4Sim/Analyser/interface/GeantAnalysis.h"
#include <iostream>
#include <fstream>

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
    if (chargeGap[0] != 0 or chargeGap[1] != 0) hSens->Fill(primaryEne);

    //for(int iEl = 0; iEl < nElectron; iEl++)
    //{
    //  tElectron->GetEntry(iElectron);
    //  hElect->Fill(primaryEne);
    //  bool test = false;
    //  for (unsigned int ie = 1; ie < partId->size(); ie++)
    //  {
    //    test = (partId->at(ie) == 11)|test;
    //  }
    //  if (!test) 
    //  {
    //    hElectronGenProcess->Fill(primaryEne,
    //                              processNum->at(0));
    //    hElectronPrimaryProcess->Fill(primaryEne,
    //                                  processNum->at(partId->size()-1));
    //    hElectronSecondaryProcess->Fill(primaryEne,
    //                                    processNum->at(partId->size()-2));
    //  }
    //  iElectron++;
    //}
    //for(int iG = 0; iG < nGamma; iG++)
    //{
    //  tGamma->GetEntry(iGamma);
    //  hGamma->Fill(primaryEne*1E+6);
    //  hGammaGenProcess->Fill(primaryEne*1E+6,
    //                         processNum->at(0));
    //  hGammaPrimaryProcess->Fill(primaryEne*1E+6,
    //                             processNum->at(partId->size()-1));
    //  iGamma++;
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
    auto analysis = new GeantAnalysis("step2.root", true);
    
    string temp = argv[1];
    std::ifstream fileList(temp);
    while (!fileList.eof()) {
      fileList >> temp;
      cout << temp << " has opend" << endl;
      analysis->SetFile(temp);
      analysis->Analysis();
    }
    delete analysis;
  }
  return 0;
}
