import sys, os

outputName = 'step1.root'
binName = sys.argv[1] #Args For Bin
jobName = sys.argv[2]
partName = sys.argv[3]
nJobs = sys.argv[4]
maxEvent = sys.argv[5]
destDir = sys.argv[6]
shCommand = '{0} {1} {2}'.format(binName, partName, maxEvent)
jobDir = os.path.abspath('') + '/' + jobName
envName = os.getenv("CMSSW_BASE").split("/")[-1]
if os.path.isdir(jobDir) :
  print("Job Direcotry Already Exist")
  sys.exit()
os.system("mkdir -p %s"%jobDir)
os.system("mkdir -p %s"%destDir)

#Create Submit.jds
fout = open("%s/submit.jds"%jobDir, "w")
print>>fout,"# Job description file for condor job %s"%jobName
print>>fout,"""executable = run_{0}.sh
universe   = vanilla
arguments  = $(Process)
JobBatchName = {0}

log = condor.log

getenv     = True
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
output = job_$(Process).log
error = job_$(Process).err
request_memory = 2000MB
queue {1}""".format(jobName, nJobs)
fout.close()

#Create Submit.sh
fout = open('%s/submit.sh'%jobDir, 'w')
print>>fout,'#!/bin/bash'
print>>fout,'condor_submit submit.jds'
fout.close()

os.chmod('%s/submit.sh'%jobDir, 0755)

#Create run_*.sh
fout = open('{0}/run_{1}.sh'.format(jobDir, jobName), 'w')

#Set up at the node that jobs run
print>>fout,"""#!/bin/bash
if [ $# != 1 ]; then
    echo "JOB SECTION NUMBER IS MISSING!!!"
    exit 1
fi
SECTION=${@: -1}
FSECTION=`printf %03d $SECTION`

if [ _$CMS_PATH == _ ]; then
  export CMS_PATH=/cvmfs/cms.cern.ch
fi
source $CMS_PATH/cmsset_default.sh
"""

print>>fout,"""hostname
pwd
whoami
if [ -f $CMSSW_BASE/proxy.x509 ]; then
    export X509_USER_PROXY=$CMSSW_BASE/proxy.x509
fi""".format(envName)

print>>fout,"""
echo BEGIN `date` {0} #>> {1}/submit.log
echo {0}
touch ___started___job___
time {0}
EXITCODE=$?
if [ $EXITCODE == 0 ]; then
    echo ENDED `date` {0} #>> {1}/submit.log


else
    rm -f core.*
    echo TERMINATED_$EXITCODE `date` {0} #>> {1}/submit.log
    exit 1
fi""".format(shCommand, jobDir)

print>>fout,"""
for FILE in %s; do
    EXT=${FILE##*.}
    PREFIX=${FILE%%.${EXT}}
    TRANSFERCMD=(mv)
    DESTDIR=%s
    ${TRANSFERCMD[@]} ${FILE} ${DESTDIR}/${PREFIX}_${FSECTION}.${EXT}
    if [ $? -ne 0 ]; then
        echo "Failed to copy file.. retry in 5 seconds"
        sleep 5
        echo "trying: " ${TRANSFERCMD[@]} ${FILE} ${DESTDIR}/${PREFIX}_${FSECTION}.${EXT}
        ${TRANSFERCMD[@]} ${FILE} ${DESTDIR}/${PREFIX}_${FSECTION}.${EXT}
    fi
    if [ $? -ne 0 ]; then
        echo "Failed to copy file.. (trial2) retry in 30 seconds"
        sleep 30
        ${TRANSFERCMD[@]} ${FILE} ${DESTDIR}/${PREFIX}_${FSECTION}.${EXT}
    fi
done
echo FINISHED `date` # >> %s/submit.log
"""%(outputName, destDir, jobDir)
fout.close()
os.chmod('{0}/run_{1}.sh'.format(jobDir, jobName), 0755)


fout = open("%s/submit.log" %jobDir, "w")
print>>fout, "SUBMIT STARTED", jobName
fout = None
os.chmod("%s/submit.log"%jobDir, 0666)

os.system("cd %s;./submit.sh;cd -"%jobDir)
