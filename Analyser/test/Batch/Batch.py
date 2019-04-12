#!/usr/bin/env python 

import os, json, sys
import numpy as np
from math import ceil

application = 'GeantAnalysis'
jobName = sys.argv[1]

fileList = '%s_dataset.txt'%(jobName)

cmsswBase = os.environ['CMSSW_BASE']
username = "yekang"
dirName = jobName
maxFiles = 10
    
Dirname = "%s/src/Geant4Sim/Analyser/test/Batch/%s"%(cmsswBase,jobName)
if os.path.isdir(Dirname):
    print "ERROR: output directory already existing."
    sys.exit()
else: os.makedirs(Dirname)

files = np.array([])
for f in open(fileList).readlines():
    f = f.strip()
    f = f.strip('\',"')
    if len(f) < 5: continue
    if '#' == f[0] or '.root' != f[-5:]: continue
    files = np.append(files,[f])
nFiles = len(files) 
nSection = int(ceil(1.0*nFiles/maxFiles))

for section in range(nSection):
    begin = section*maxFiles
    end = min(begin + maxFiles, nFiles)
    FileNames = files[begin:end]
    FileNamesStr = " ".join(str(i) for i in FileNames)
    distFileName = "root://uosaf0007.sscc.uos.ac.kr:1094//xrootd/store/user/yekang/TrGEMG4/GE11/postProc/%s/%s_%i.root"%(jobName, jobName, section)

    print "@@ Writing run script..."
    jds = "%s/submit.jds" %Dirname 
    fout = open(jds, "w")
    print>>fout, "# Job description file for condor job"
    print>>fout, """executable = {0}/bin/slc6_amd64_gcc700/GeantAnalysis
universe   = vanilla

log = condor.log

Accounting_Group = group_cms
getenv     = True
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
output = {1}/job_{2}.log
error = {1}/job_{2}.err
queue""" .format(cmsswBase, Dirname, section)
    fout.close()

    subBatch = "condor_submit -batch-name {} -append 'arguments={} {}' {}".format(jobName, distFileName, FileNamesStr, jds)
    os.system(subBatch)
