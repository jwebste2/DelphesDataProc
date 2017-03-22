#DelphesDataProc

***
##Summary
This is a collection of tools used to process and clean Delphes ttH and ttbar events so they are ready to used for MVA studies. This code was used to produce to prepare data for the studies done in https://arxiv.org/abs/1610.03088.

####Structure
FIXME...

***

## Processing Delphes
FIXME...

***
##Git Refresher (for Terminal Work)

####Clone the repository on your local machine:
    git clone https://github.com/jwebste2/TrainingSandbox.git TrainingSandbox

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
