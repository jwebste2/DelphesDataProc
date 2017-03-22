#!/bin/bash

inputFile=${1}

# Used when running over Jahred's initial MG output that only had a single mass point
#outputDir=/cnfs/data1/users/jwebster/ttbar_01p/massdep/`echo ${inputFile} | grep -o -P '(?<=ttbar_01p\/.\/).*(?=\/ttbar_01p)'`

# Used when running over Jahred's second wave MG output with multiple samples covering different m(top) mass points
outputDir=/cnfs/data1/users/jwebster/ttbar_01p/massdep/`echo ${inputFile} | grep -o -P '(?<=ttbar_massdep\/).*(?=\/ttbar_01p)'`

echo "BASH : making output dir ${outputDir}"
mkdir -p $outputDir

echo "BASH : copying over STDHEP input and unzipping"
echo "BASH : input = ${inputFile}"
cp $inputFile $outputDir
gunzip ${outputDir}/*.hep.gz

ll ${outputDir}
unzippedInputFile=`ls ${outputDir}/*.hep`
outputFile=${outputDir}/delphes_output.root

echo "BASH : Running ${unzippedInputFile} --> ${outputFile}"

./DelphesSTDHEP cards/delphes_card_ATLAS.tcl ${outputFile} ${unzippedInputFile}

echo "BASH : Removing input file"
rm ${unzippedInputFile}

echo "BASH : done."
ls -ltrh ${outputDir}

