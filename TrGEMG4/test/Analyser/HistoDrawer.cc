#include "HistoMaker.h"
#include <iostream>

using namespace std;

int HistoDrawer()
{
  HistoMaker histDrawer("neutron.root");
  auto c1 = new TCanvas("c1","c1",600,500);
  //TH1D* h = histDrawer.PrimaryEnergy();
  auto legend = new TLegend(0.1,0.8,0.48,0.9);
  for (int i = 0; i < 1; i++) {
    TH1D* h = histDrawer.SensitivityElectron(i);
    h->SetMarkerColor(i+1);
    h->SetLineColor(i+1);
    h->GetXaxis()->SetRangeUser(1E-9, 1E+3);
    h->SetTitle(";Primary Particle Energy [MeV]");
    h->Draw("E1 same");
    legend->AddEntry(h, Form("Sensitivity"));
    h->Draw("L hist same");
  }

  //TH2D* h = histDrawer.ElectronProcess(100);
  //TH2D* h = histDrawer.NeutronProcess(100);
  //h->Draw("colz");
  
  c1->SetLogx();
  c1->SetLogy();
  legend->Draw();
  //c1->SetGrid();

  c1->Modified();
  c1->Update();
  c1->SaveAs("testMu.pdf");
  return 0;
}
