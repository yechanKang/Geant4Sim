//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file radioactivedecay/rdecay01/src/TrGEMPrimaryGeneratorAction.cc
/// \brief Implementation of the TrGEMPrimaryGeneratorAction class
//
//
// $Id$
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "Geant4Sim/TrGEMG4/interface/TrGEMPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include <TMath.h>
#include <fstream>

// G4int lnneutron = 1;
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrGEMPrimaryGeneratorAction::TrGEMPrimaryGeneratorAction(char* partName_) : partName(partName_)
{
  G4int n_particle = 1;
  //  fParticleGun  = new G4ParticleGun(n_particle);
  //  G4ParticleDefinition* particlen = particleTable -> FindParticle("neutron");
  //G4ParticleDefinition* particlen = G4ParticleTable::GetParticleTable() -> FindParticle("neutron");
  G4ParticleDefinition* particlen = G4ParticleTable::GetParticleTable() -> FindParticle(partName);

  std::string temp = partName;
  if (temp == "neutron") eneRange = 12;
  else if (temp == "gamma") eneRange = 5;
  else if (temp == "e-") eneRange = 5;
  else if (temp == "e+") eneRange = 5;
  else if (temp == "alpha") eneRange = 12;
  else if (temp == "mu-") eneRange = 12;
  else throw;

  fParticleGun  = new G4ParticleGun(particlen,n_particle);
  // fParticleGun->SetParticleEnergy(1*GeV);
  
  /* It opens the file with the initialization settings */
  //std::ifstream in;
  //in.open("/home/scratch/Geant4/CMSSW_8_0_20/src/geant4/GEM_Geant4_bkg/UserData/EnergyCDF.txt");

  //if(in.fail())
  //{
  //  G4cerr << "Primary energy data has not imported!" << G4endl;
  //}

  //G4double xtemp,ytemp;

  //while(!in.eof()){
  //  in >> xtemp >> ytemp ;
  //  ene.push_back(xtemp);
  //  cdf.push_back(ytemp);
  //}

  //in.close();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrGEMPrimaryGeneratorAction::~TrGEMPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrGEMPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double xxx,yyy;
  G4double primaryEne;

  // set direction
  // neutron
  //Parallel Direction
  fParticleGun->SetParticleMomentumDirection((G4ThreeVector(0.,0.,1.))); 

  //set position  
  G4double gX = G4UniformRand()*10.-5.;
  G4double gY = G4UniformRand()*10.-5.;
  fParticleGun->SetParticlePosition(G4ThreeVector(gX,gY,-1.));
  
  // set energy
  // neutron
  yyy=G4UniformRand();

  //for(G4int i=1; i<ene.size();i++)
  //{
  //  if( cdf[i] < yyy && yyy < cdf[i+1] )
  //  {
  //    xxx = (ene[i+1] - ene[i])*G4UniformRand();
  //    primaryEne = ene[i]+xxx;
  //    break;
  //  }
  //}
  
  //primaryEne = TMath::Power(10, (yyy-1)*12);
  primaryEne = TMath::Power(10, (yyy-1)*eneRange);

  fParticleGun->SetParticleEnergy(primaryEne*GeV);
    
  //create vertex
  //   
  fParticleGun->GeneratePrimaryVertex(anEvent);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
