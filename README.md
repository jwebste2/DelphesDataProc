# DelphesDataProc

***
## Summary
This is a collection of tools used to process and clean Delphes ttH and ttbar events so they are ready to used for MVA studies. This code was used to produce to prepare data for the studies done in [arXiv:1610.03088](https://arxiv.org/abs/1610.03088).

#### Structure
This is a standard C++ package. The source code is in the [src](src/) directory. Header files are in the [DelphesDataProc](DelphesDataProc/) directory. Bash and python scripts are in the [scripts](scripts/) directory. Output from all scripts and executables should be directed to the [output](output/) directory.

#### Installation
Before compiling this code you need to first install Delphes, MadGraph, and TTHbbLeptonic. The following versions/tags have been found to work well:

* [Delphes-3.3.0](https://github.com/delphes/delphes/releases/tag/3.3.0)
* [MG5\_aMC\_v2\_3\_3](https://launchpad.net/mg5amcnlo/2.0/2.3.x/+download/MG5_aMC_v2.3.3.tar.gz)
* [TTHbbLeptonic-02-03-39-02](https://svnweb.cern.ch/cern/wsvn/atlasphys-hsg8/Physics/Higgs/HSG8/AnalysisCode/TTHbbLeptonic/tags/TTHbbLeptonic-02-03-39-02/)

Once these packages are installed, please update the corresponding paths near the top of the [Makefile](Makefile). Also, please be sure that the path to Delphes has been appended to your LD\_LIBRARY\_PATH, e.g.

    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/my/path/to/Delphes-3.3.0

This is nominally done inside the [setup.sh](setup.sh) script, so you will need to edit this script before sourcing it.

Finally, you will also need ROOT to compile but this should be set up automatically when you run setup.sh (provided you are running from a system like lxplus with an afs mount). 

Once all of this is done you should be able to compile any executable in the [src](src/) directory, as follows:

    make [Source file name with .cpp removed]
    # e.g. for a source file "ProcessDataForTthVsTtbar.cpp"
    make ProcessDataForTthVsTtbar

This will create an executable called "ProcessDataForTthVsTtbar" in the [run](run/) directory.

***
## Typical Workflow

#### Step 1: Process MadGraph output with Delphes
The C++ code in this package uses Delphes format ROOT files as input. Before compiling and running the C++ code you may need to process some MadGraph output using Delphes. This can be done using the scripts in [scripts/RunDelphes](scripts/RunDelphes/).

First, be sure to set up your Delphes environment. This is easily done using the script provided by Delphes:

    cd /your/path/to/Delphes-3.3.0
    . DelphesEnv.sh

Next, copy the card file and Delphes run script from this git repository, e.g.:

    cp path/to/DelphesDataProc/scripts/RunDelphes/delphes_card_ATLAS.tcl cards/
    cp path/to/DelphesDataProc/scripts/RunDelphes/RUN_SINGLE_FILE.sh .
    chmod +x RUN_SINGLE_FILE.sh

Edit RUN_SINGLE_FILE.sh to ensure the output dir near the top of the file makes sense. Then you can run over 1 file as follows:

    ./RUN_SINGLE_FILE.sh [Input tag_1_pythia_events.hep.gz path]
    # e.g.
    ./RUN_SINGLE_FILE.sh /cnfs/data1/users/ac.jahreda/ttbar_massdep/ttbar_01p_singlecore_100k_10_mass160_Feb23_111252/ttbar_01p/Events/run_01/tag_1_pythia_events.hep.gz

If you are using the Argonne cluster (specifically atlas1.hep.anl.gov) then you can easily thread jobs to condor. First build a job submission file that looks like this:

    Universe     = vanilla
    Getenv       = True
    Requirements = (Machine != "atlas50.hep.anl.gov")
    Executable   = RUN_ttbarReco.sh
    Log          = [Your log directory]/RUN_SINGLE_FILE.sh.$(Process).log
    Output       = [Your log directory]/RUN_SINGLE_FILE.sh.$(Process).out
    Error        = [Your log directory]/RUN_SINGLE_FILE.sh.$(Process).err
    Arguments  =  /cnfs/data1/users/ac.jahreda/ttbar_massdep/ttbar_01p_singlecore_100k_10_mass160_Feb23_111252/ttbar_01p/Events/run_01/tag_1_pythia_events.hep.gz
    Queue
    Arguments  =  /cnfs/data1/users/ac.jahreda/ttbar_massdep/ttbar_01p_singlecore_100k_10_mass170_Feb23_101906/ttbar_01p/Events/run_01/tag_1_pythia_events.hep.gz
    Queue
    Arguments  =  /cnfs/data1/users/ac.jahreda/ttbar_massdep/ttbar_01p_singlecore_100k_10_mass171_Feb23_102751/ttbar_01p/Events/run_01/tag_1_pythia_events.hep.gz
    Queue
    Arguments  =  /cnfs/data1/users/ac.jahreda/ttbar_massdep/ttbar_01p_singlecore_100k_10_mass172_Feb23_103653/ttbar_01p/Events/run_01/tag_1_pythia_events.hep.gz
    Queue
    Arguments  =  /cnfs/data1/users/ac.jahreda/ttbar_massdep/ttbar_01p_singlecore_100k_10_mass174_Feb23_104549/ttbar_01p/Events/run_01/tag_1_pythia_events.hep.gz
    Queue
    ...

Then launch the jobs, as follows:

    condor_submit [Path to submit file]

The jobs can be monitoring using "condor_q"

#### Step 2: Process Delphes output and apply event selection

Once you have generated Delphes output, then you can process the events and apply a selection using the C++ code. If you want to produce CSV and ROOT input for training an MVA to separate ttH signal from ttbar background, then please use [src/ProcessDataForTthVsTtbar.cpp](src/ProcessDataForTthVsTtbar.cpp), e.g.:

    make ProcessDataForTthVsTtbar
    ./run/ProcessDataForTthVsTtbar ljet 0 0 100 0

The above example will run over 1/100th of the input data (both signal and background). You can get a better understanding of each input argument by running the execuble with no arguments and then looking at the error message.

If you want to produce CSV input for training a ttbar reconstruction MVA, then please use [src/ProcessDataForTtbarReco.cpp](src/ProcessDataForTtbarReco.cpp), e.g.:

    make ProcessDataForTtbarReco
    ./run/ProcessDataForTtbarReco 173 ljet none 4 4 100 0

You can thread jobs to condor by following the "Step 1" instructions.

#### Step 3: Plot MVA performance
I added to this repository the script that I used to generate plots and tables from Marcus, Roberto, and Soo's MVA output. I put this here as a reference so it doesn't get lost. If you need to actually run it then let me know!

***
## Git Refresher (for Terminal Work)

#### Clone the repository on your local machine:
    git clone https://github.com/jwebste2/DelphesDataProc.git DelphesDataProc

#### Basic Pulling:
    git pull

#### Basic Pushing:
    # To include changes to files that are already tracked
    git add -u

    # To add new files that are not yet tracked
    git add filenameA filenameB ...

    # Then commit the changes    
    git commit -m "Add a log message"

    # Then push the commit
    git push

#### Merging your branch into master
    # Pull updates from master to your branch
    git pull

    # Commit and push your branch updates
    git commit -m "log message"
    git push

    # Switch to the master branch and then merge in your changes from your_branch
    git checkout master
    git pull
    git merge your_branch

    # Go back to editing your branch if you want
    git checkout your_branch
    git rebase master


#### Checking Logs:
    # Format ==> git log -NEntriesToPrint, e.g.
    git log -5

