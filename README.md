# How to compile GEANT4 application with cmssw
```
scram p -n gSim CMSSW CMSSW_10_4_0
cd gSim/src
git clone git@gitlab.sscc.uos.ac.kr:gem/geant4_background.git Geant4Sim
scram b -j4
```
