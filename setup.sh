source /e906/app/software/osg/software/e1039/this-e1039.sh
source /seaquest/users/dinupa/seaquest/apps/RooUnfold/build/setup.sh


function update-this {
    echo "---> Updating the branch"
    git pull
}


function chamber-acceptance {
    echo "---> Calculate the chamber acceptance"
    root -b -q ChamberAcceptance.cc
    echo "---> Plots are saved in imgs"
}


function detector-smearing {
    echo "---> Calculate the detector smearing using RooUnfold"
    root -b -q DetectorSmearing.cc
}
