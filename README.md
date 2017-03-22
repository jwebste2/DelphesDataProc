#DelphesDataProc

***
##Summary
This is a collection of tools used to process and clean Delphes ttH and ttbar events so they are ready to used for MVA studies. This code was used to produce to prepare data for the studies done in [arXiv:1610.03088](https://arxiv.org/abs/1610.03088).

####Structure
This is a standard C++ package. The source code is in the [src](src/) directory. Header files are in the [DelphesDataProc](DelphesDataProc/) directory. Bash and python scripts are in the [scripts](scripts/) directory. Output from all scripts and executables should be directed to the [output](output/) directory.

####Installation
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
    # e.g. for a source file "ProcessData.cpp"
    make ProcessData

This will create an executable called "ProcessData" in the [run](run/) directory.

***
##Typical Workflow

####Step 1: Process MG output with Delphes

####Step 2: Process Delphes output and apply event selection

***
##Git Refresher (for Terminal Work)

####Clone the repository on your local machine:
    git clone https://github.com/jwebste2/DelphesDataProc.git DelphesDataProc

####Basic Pulling:
    git pull

####Basic Pushing:
    # To include changes to files that are already tracked
    git add -u

    # To add new files that are not yet tracked
    git add filenameA filenameB ...

    # Then commit the changes    
    git commit -m "Add a log message"

    # Then push the commit
    git push

####Merging your branch into master
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


####Checking Logs:
    # Format ==> git log -NEntriesToPrint, e.g.
    git log -5

