#define GeantAnalysis_cxx

#include "GeantAnalysis.h"
#include <iostream>

using namespace std;

void GeantAnalysis::Analysis()
{
  Long64_t nEntries = fChain->GetEntries();
  ResetBranch();
  for(Long64_t i = 0; i < nEntries; i++)
  {
    bool eleGap = true;
    bool posGap = true;
    bool chargeGap = true;
    ResetBranch();
    fChain->GetEntry(i);
    hPrimaryEne->Fill(primaryEne);
    for (int i = 0; i < 4; i++) {
      if(EleGap[i] == 1 && eleGap)       hEleGap[i]->Fill(primaryEne), eleGap = false;
      if(PosGap[i] == 1 && posGap)       hPosGap[i]->Fill(primaryEne), posGap = false;
      if(ChargeGap[i] == 1 && chargeGap) hChargeGap[i]->Fill(primaryEne), chargeGap = false;
    }

    for(UInt_t i = 0 ; i < gapTrackPart->size(); i++)
    {
      int num = findVolume((*gapTrackVolume)[i]);
      if((*gapTrackPart)[i] == 11) 
      {
        hEleEne[num]->Fill((*gapTrackEne)[i]);
        hElectronGenProcess->Fill(primaryEne, (*gapTrackGenProcessNum)[i]);
      }
      if((*gapTrackPart)[i] == -11) hPosEne[num]->Fill((*gapTrackEne)[i]);
      if((*gapTrackCharge)[i] != 0) hChargeEne[num]->Fill((*gapTrackEne)[i]);
      if((*gapTrackIntNum)[i] == 2) hPrimaryProcess->Fill(primaryEne,(*gapTrackGenProcessNum)[i]);
    }
  }
}

int main(Int_t argc, Char_t** argv)
{
  if(argc == 1)
  {
    cout << "Error : no arguments\n";
    return 0;
  }
  else
  {
    string temp = argv[1];
    GeantAnalysis analysis(temp);
    
    for(Int_t i = 2; i < argc; i++)
    { 
      temp = argv[i];
      analysis.SetFile(temp);
      analysis.Analysis();
    }
    return 0;
  }
}
