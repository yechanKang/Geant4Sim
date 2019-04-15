#include "../interface/HistoMaker.h"
#include <iostream>

using namespace std;

int HistoDrawer()
{
  //HistoMaker histDrawer("neutron.root");
  HistoMaker histDrawer("gamma.root");
  auto c1 = new TCanvas("c1","c1",600,500);
  //TH1D* h = histDrawer.PrimaryEnergy();
  auto legend = new TLegend(0.1,0.8,0.48,0.9);
  for (int i = 0; i < 1; i++) {
    auto h = histDrawer.SensitivityElectron(i);
    h->Draw("E1 same");
    h->Draw("L1 hist same");
    //h->Draw("colz");
  }
  //auto h = histDrawer.MakeSelectedBin2D("PrimaryProcess");
  //h->GetXaxis()->SetRangeUser(1E-2, 1E+3);
  //h->Draw("colz");

  //TH2D* h = histDrawer.ElectronProcess(100);
  //TH2D* h = histDrawer.NeutronProcess(100);
  //h->Draw("colz");
  
  c1->SetLogx();
  c1->SetLogy();
  c1->SetGridx();
  c1->SetGridy();
  c1->SetTickx();
  c1->SetTicky();
  //legend->Draw();
  //c1->SetGrid();

  c1->Modified();
  c1->Update();
  c1->SaveAs("testMu.pdf");
  return 0;
}
