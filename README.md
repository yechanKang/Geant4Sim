# How to compile GEANT4 application with cmssw
```
scram p -n gSim CMSSW CMSSW_10_4_0
cd gSim/src
git clone https://github.com/yeckang/Geant4Sim.git Geant4Sim
scram b -j4
```

If you run above script, you can run Geant4 application with following command.
```
TrGEMG4 <particle name> <number of events>
```
For example, you can run 1000 gamma events like following command.
```
TrGEMG4 gamma 1000
```

If simulations has done, you should run the analyser for it.
The analyser contains the scripts for applying log scaled bins to x-axis.
You can run the analyser with following command.
```
GeantAnalysis <input file name>
```
Then you can get the histograms.
The scripts for the analyser is located at the 'Analyser' directory.
You may need to modify them.

Also the scripts to run simulation and analyser with HTCondor is ready.
You can find them in the 'test/Batch' directories.
It has tested with the server at the University of Seoul.
You may need to change the scripts to run the server you are using.
