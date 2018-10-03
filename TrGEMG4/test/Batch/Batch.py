#!/usr/bin/env python 

import os, json, sys
import numpy as np
from math import ceil

application = 'TrGEMG4'
jobName = sys.argv[1]
nJobs = int(sys.argv[2])
nEvent = int(sys.argv[3])

cmsswBase = os.environ['CMSSW_BASE']
username = os.environ['USER']
dirName = "{}/src/Geant4Sim/TrGEMG4/test/Result_Batch/{}".format(cmsswBase, jobName)

for index in range(nJobs):
    FileNamesStr = "{}/{}_{}".format(dirName, jobName, index)
    
    Dirname = "%s/src/Geant4Sim/TrGEMG4/test/Batch/%s"%(cmsswBase,jobName)
    if os.path.isdir(Dirname):
        print "ERROR: output directory already existing."
        sys.exit()
    else: os.makedirs(Dirname)

    print "@@ Writing run script..."
    jds = "%s/submit.jds" %Dirname 
    fout = open(jds, "w")
    print>>fout, "# Job description file for condor job"
    print>>fout, """executable = {0}/bin/slc6_amd64_gcc700/{1}
universe   = vanilla

log = condor.log

getenv     = True
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
output = {2}/job_{3}.log
error = {2}/job_{3}.err
queue""" .format(cmsswBase, application, Dirname, index)
    fout.close()

    subBatch = "condor_submit -batch-name {} -append 'arguments={} {}' {}".format(jobName, FileNamesStr, nEvent, jds)
    os.system(subBatch)
