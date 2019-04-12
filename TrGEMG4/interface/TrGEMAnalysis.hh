#ifndef TRGEMANALYSIS_HH
#define TRGEMANALYSIS_HH 1

#include "globals.hh"
#include <vector>
#include <map>

class G4Run;
class G4Event;
class G4ParticleDefinition;
class TFile;
class TH1D;
// class TNtuple;
class TVector3;
class TTree;
class G4Track;

class TrGEMAnalysis {

  public:

    static TrGEMAnalysis* GetInstance();
    ~TrGEMAnalysis();

    void SetFileName(const G4String name);
    void PrepareNewEvent(const G4Event* anEvent);
    void EndOfEvent(const G4Event* anEvent);
    void PrepareNewRun(const G4Run* aRun);
    void EndOfRun(const G4Run* aRun);
    
    void SavePrimary(G4int primaryPart_, G4double primaryEne_);
    void SaveGapTrack(G4int trackId,
                      G4int gapPart, 
                      G4int aCharge,
                      G4String volume,
                      G4int    copyNo,
                      G4double kinene,
                      TVector3 position,
                      TVector3 momentum);
    void SaveGenTrack(G4int partCode,
                      std::string process,
                      std::string volume,
                      G4int copyNo,
                      G4int trackID,
                      G4int parentID);

    G4int FindVolume(std::string volume);
    G4int FindGapTrackProcess(std::string process);
    G4int FindGeneratingProcess(std::string process);
    
  private:

    TrGEMAnalysis();
    static TrGEMAnalysis* singleton;

    void PreSavingTrack(G4int trackID);
    void PostSavingTrack();

    std::vector<std::string> NomeStrati;
    std::vector<std::string> posProcess;

    bool isNewEvent ;
    G4int eventCounter ;
    G4String fileName;

    std::map<G4int, std::array<G4int, 5>> genMap;
    
    // std::vector<G4int>  gammaContainer;        s

    // ROOT objects
    TFile*    m_ROOT_file;
    
    TTree*    tEvent;
    TTree*    tElectron;
    TTree*    tPositron;
    TTree*    tGamma;
    TTree*    tCharged;
    G4double primaryEne;
    G4int    primaryPart;
    G4int nElectron, nPositron, nGamma, nCharged;
    G4int eleGap[4];
    G4int posGap[4];
    G4int chargeGap[4];

    // Particle Info
    G4int gap;
    std::vector<G4int> partId;
    std::vector<G4int> processNum;
    std::vector<G4int> processVol;
    std::vector<G4int> volCopyNo;

    // GARFIELD quantities
    G4double  kineticEnergy ;

    G4double  positionX ;
    G4double  positionY ;
    G4double  positionZ ;
    
    G4double   momentumX ;
    G4double   momentumY ;
    G4double   momentumZ ;
};

#endif /* TRGEMANALYSIS_HH */
