#!/usr/local/bin/python2

import ROOT
import csv
import pandas
import csv
import sys
import os

from math import sqrt

import Tools.Report as report
import Tools.HistTools as htools

from Tools.RootCanvas import RootCanvas

from Tools.MVAResultProcessor import *

c = RootCanvas()

from Tools.MVAPaperStyle import *
setStyle( htools )

##################################################################
###                     BEGIN SOURCE                           ###
##################################################################

"""
Tool for creating NN response distributions and ROC curves using Roberto's Test
Results for MVA ttH paper
"""

rList = []
nPlots = 0

gRocOverlay_SampleSize     = {}
gRocOverlay_SampleSize_inc = [ "1K" , "10K" , "20K" , "100K" , "200K" , "338K" ]
# [ "1K" , "10K" , "20K" , "30K" , "100K" , "200K" , "338K" ]

gRocOverlay_Structures = {}

gRocOverlay_Inputs = {}

gRocOverlay_Classifiers = {}


# back drop for plotting ROC curves
# just use a single one over and over again
hRoc = ROOT.TH1D( "hRoc" , ";t#bar{t} Background Eff.;t#bar{t}H Signal Eff." , 1 , 0 , 1 )
hRoc.SetMaximum( 1.0 )
htools.makePretty( hRoc , color=-1 , linewidth=0 )
lineRoc = ROOT.TLine( 0 , 0 , 1 , 1 )
htools.makePretty( lineRoc , color=-1 , linewidth=2 , linestyle=ROOT.kDashed )

# signal and background weights to scale to 1/fb
w_sig = 463.4 * 0.10350984 # xsec [fb] * cut eff
w_bkg = 313300.0 * 0.01304861 # xsec [fb] * loose cut eff


nbins = 1000 # 50

#################################
# First plots Soo's data
#################################

for dataFileName in [ "nbexpert_Top10.txt" , "nbexpert_Top20.txt" , "nbexpert_Top200.txt" , "nbexpert_BasicFeatures.txt" ]:
    #break
    r = MVAResultProcessor()
    r.classifierName = "NeuroBayes"
    r.featuresTag    = "10" if "10" in dataFileName else \
                       "200" if "200" in dataFileName else \
                       "20" if "20" in dataFileName else \
                       "Basic" if "Basic" in dataFileName else \
                       "UNKNOWN"
    r.featuresTitle  = "Top 10 Features" if "10" in dataFileName else \
                       "Top 200 Features" if "200" in dataFileName else \
                       "Top 20 Features" if "20" in dataFileName else \
                       "Basic Features" if "Basic" in dataFileName else \
                       "UNKNOWN"

    dataFilePath = "Data/"+dataFileName
    
    inputFile = open( dataFilePath , 'r' )

    xmin = -1.0 if "10" in dataFileName else -1.0
    xmax =  1.0 if "10" in dataFileName else  1.0
    
    hResponseSig = ROOT.TH1D( "hResponseSig" , ";%s Output;Normalized Number of Events"%(r.classifierName) , nbins , xmin , xmax )
    hResponseBkg = ROOT.TH1D( "hResponseBkg" , ";%s Output;Normalized Number of Events"%(r.classifierName) , nbins , xmin , xmax )

    # Each line in the file contains a NN output for an event, followed by
    # 0 (for background) or 1 (for signal)
    # First two lines in the file are header that should be skipped

    inputFile.readline()
    inputFile.readline()

    while True:

        line  = inputFile.readline()
        words = line.split()

        if not( len(words) == 2 ):
            # Must be end of file
            break

        response = float(words[0])
        issignal = int(words[1])
        
        # Include overflow and underflow
        if response <= xmin: response = xmin + 0.00001
        if response >= xmax: response = xmax - 0.00001
            
        if issignal==1:
            hResponseSig.Fill( response )
        else:
            hResponseBkg.Fill( response )
                
    inputFile.close()

    htools.norm( hResponseSig )
    htools.norm( hResponseBkg )
    htools.makePretty( hResponseSig , color="sig" , linestyle="sig" )
    htools.makePretty( hResponseBkg , color="bkg" , linestyle="bkg" )
    htools.scaleMax( [hResponseBkg,hResponseSig] , 1.25 )

    # Create a ROC curve and calculate AUC
    gRoc = ROOT.TGraph()
    sigEff = 0.0
    bkgEff = 0.0
    auc = 0.0
    gRoc.SetPoint( gRoc.GetN() , bkgEff , sigEff )
    for ibin in range(hResponseBkg.GetNbinsX(),0,-1):
        thisBinSig = hResponseSig.GetBinContent(ibin)
        thisBinBkg = hResponseBkg.GetBinContent(ibin)
        if thisBinSig==0 and thisBinBkg==0: continue
        auc += ( sigEff + (thisBinSig/2.0) ) * thisBinBkg
        sigEff += thisBinSig
        bkgEff += thisBinBkg
        gRoc.SetPoint( gRoc.GetN() , bkgEff , sigEff )
        if sigEff > 0 and bkgEff > 0:
            SoverB      = (100.0 * sigEff * w_sig) / (100.0 * bkgEff * w_bkg)
            SoverSqrtB  = (100.0 * sigEff * w_sig) / sqrt(100.0 * bkgEff * w_bkg)
            SoverSqrtSB = (100.0 * sigEff * w_sig) / sqrt( (100.0 * bkgEff * w_bkg) + (100.0 * sigEff * w_sig) )
            if SoverB >= r.maxSoverB[0]:
                r.maxSoverB = [ SoverB , hResponseBkg.GetXaxis().GetBinLowEdge(ibin) ]
            if SoverSqrtB >= r.maxSoverSqrtB[0]:
                r.maxSoverSqrtB = [ SoverSqrtB , hResponseBkg.GetXaxis().GetBinLowEdge(ibin) ]
            if SoverSqrtSB >= r.maxSoverSqrtSB[0]:
                r.maxSoverSqrtSB = [ SoverSqrtSB , hResponseBkg.GetXaxis().GetBinLowEdge(ibin) ]

    report.debug( "%s %s s/sqrt(b)=%g cut=%g s/sqrt(s+b)=%g cut=%g" % (r.classifierName,r.featuresTitle,r.maxSoverSqrtB[0],r.maxSoverSqrtB[1],r.maxSoverSqrtSB[0],r.maxSoverSqrtSB[1]) )
                
    r.auc = auc
    r.aucStr = "%0.3f" % (auc)
    
    # Now Plot everything
    
    legend = ROOT.TLegend()
    legend.AddEntry( 0 , "Test Data" , "" )
    legend.AddEntry( hResponseSig , "t#bar{t}H Signal" , "le" )
    legend.AddEntry( hResponseBkg , "t#bar{t} Background" , "le" )
    htools.makePretty( legend , textsize=0.8 , textlocation=htools.LEFT )

    text = ROOT.TPaveText()
    text.AddText( r.classifierName )
    text.AddText( r.featuresTitle )
    text.AddText( "AUC = %0.3f" % (auc) )
    htools.makePretty( text , textsize=0.8 , textlocation=htools.RIGHT )
        
    hResponseBkg.Draw( "hist e" )
    hResponseSig.Draw( "hist e same" )
    legend.Draw()
    text.Draw()
    c.save( "SooTop"+r.featuresTag+"_NNOutput" )
        
    htools.makePretty( gRoc , color="roc" )

    textAuc = ROOT.TPaveText()
    textAuc.AddText( "AUC = %0.3f" % (auc) )
    htools.makePretty( textAuc , textsize=1.0 , textlocation=htools.LEFT )

    textRoc = ROOT.TPaveText()
    textRoc.AddText( r.classifierName )
    textRoc.AddText( r.featuresTitle )
    htools.makePretty( textRoc , textsize=1.0 , textlocation=htools.RIGHT )
    textRoc.SetTextAlign( 22 )
    textRoc.SetX1NDC( textRoc.GetX1NDC()-0.2 )
    textRoc.SetX2NDC( textRoc.GetX2NDC()-0.2 )
    textRoc.SetY2NDC( textRoc.GetY2NDC()-0.45 )
    textRoc.SetY1NDC( textRoc.GetY1NDC()-0.45 )
        
    hRoc.Draw( "hist" )
    gRoc.Draw( "l" )
    lineRoc.Draw()
    textAuc.Draw()
    textRoc.Draw()
    c.save( "SooTop"+r.featuresTag+"_ROC" )

    if r.featuresTag=="200":
        gRocOverlay_Classifiers[r] = gRoc.Clone( "gRocOverlay_Classifiers_"+r.classifierName+r.featuresTag )
    
    del hResponseBkg
    del hResponseSig
    del legend
    del text
    
    del gRoc
    del textAuc
    del textRoc



#################################
# Next plot Roberto's data
#################################

dataDir = "Data/RobertoResults"

# "All Features results" , "Basic Features results" , "Extended Features only results" , "20 Features only results"

for dataSubdir in [ "All Features results" , "Basic Features results" , "Extended Features only results" ]:
    break
    fileList = [ f for f in os.listdir(os.path.join(dataDir,dataSubdir,"output")) if ".csv" in f ]
    for f in fileList:

        # MVAResultProcessor __init__ function will automagically load all important information
        # about this particular run configuration, e.g. nLayers, nNodes, AUC, trainingTime, etc.
        r = MVAResultProcessor( dataDir , dataSubdir , f )

        #if not( r.nTrainingEventsStr=="338K" ): continue
        if r.nLayers==5 and r.nNodes==15: continue
        
        ####################
        # Now the configuration information is all loaded.
        # We are ready to open the event-by-event response file for processing
        ####################

        inputFile = open( r.outputFilePath , 'r' )

        xmin = -0.5
        xmax =  1.5

        # For some special cases we set xmin and xmax to different values...
        if r.featuresTag=="AF" and r.nLayers==2 and r.nNodes==15 and r.nTrainingEventsStr=="338K":
            xmin = -0.1
            xmax = 1.1
        elif r.featuresTag=="AF" and r.nLayers==2 and r.nNodes==15 and r.nTrainingEventsStr=="1K":
            xmin = -1.0
            xmax = 2.0
        #else: continue

        
        hResponseSig = ROOT.TH1D( "hResponseSig" , ";%s Output;Normalized Number of Events"%(r.classifierName) , nbins , xmin , xmax )
        hResponseBkg = ROOT.TH1D( "hResponseBkg" , ";%s Output;Normalized Number of Events"%(r.classifierName) , nbins , xmin , xmax )

        # Each line in the file contains a NN response for a single event
        # First half of the file is background events, second half is signal events
        for i in range(2*21158):

            line  = inputFile.readline()
            words = line.split()
            
            if not( len(words)==1 ):
                report.crash( "Wrong number of words in line:" , words )

            response = float(words[0])

            # Include overflow and underflow
            if response <= xmin: response = xmin + 0.00001
            if response >= xmax: response = xmax - 0.00001
            
            if i >= 21158:
                hResponseSig.Fill( response )
            else:
                hResponseBkg.Fill( response )
                
        inputFile.close()

        n_sig = hResponseSig.Integral()
        n_bkg = hResponseBkg.Integral()
        
        htools.norm( hResponseSig )
        htools.norm( hResponseBkg )
        htools.makePretty( hResponseSig , color="sig" , linestyle="sig" )
        htools.makePretty( hResponseBkg , color="bkg" , linestyle="bkg" )
        htools.scaleMax( [hResponseBkg,hResponseSig] , 1.4 )
        
        legend = ROOT.TLegend()
        legend.AddEntry( 0 , "Test Data" , "" )
        legend.AddEntry( hResponseSig , "t#bar{t}H Signal" , "le" )
        legend.AddEntry( hResponseBkg , "t#bar{t} Background" , "le" )
        htools.makePretty( legend , textsize=0.8 , textlocation=htools.LEFT )

        text = ROOT.TPaveText()
        text.AddText( "%s %i#times%i" % (r.classifierName,r.nLayers,r.nNodes) )
        text.AddText( r.featuresTitle )
        text.AddText( "%s Training Events" % (r.nTrainingEventsStr) )
        text.AddText( "AUC = %s" % (r.aucStr) )
        htools.makePretty( text , textsize=0.8 , textlocation=htools.RIGHT )
        
        hResponseBkg.Draw( "hist e" )
        hResponseSig.Draw( "hist e same" )
        legend.Draw()
        text.Draw()
        c.save( r.configName+"_NNOutput" )

        # Create a ROC curve
        gRoc = ROOT.TGraph()
        sigEff = 0.0
        bkgEff = 0.0
        gRoc.SetPoint( gRoc.GetN() , bkgEff , sigEff )
        for ibin in range(hResponseBkg.GetNbinsX(),0,-1):
            thisBinSig = hResponseSig.GetBinContent(ibin)
            thisBinBkg = hResponseBkg.GetBinContent(ibin)
            if thisBinSig==0 and thisBinBkg==0: continue
            sigEff += thisBinSig
            bkgEff += thisBinBkg
            gRoc.SetPoint( gRoc.GetN() , bkgEff , sigEff )
            if sigEff > 0 and bkgEff > 0:
                SoverB     = (100.0 * sigEff * w_sig) / (100.0 * bkgEff * w_bkg)
                SoverSqrtB = (100.0 * sigEff * w_sig) / sqrt(100.0 * bkgEff * w_bkg)
                SoverSqrtSB = (100.0 * sigEff * w_sig) / sqrt( (100.0 * bkgEff * w_bkg) + (100.0 * sigEff * w_sig) )
                if SoverB >= r.maxSoverB[0]:
                    r.maxSoverB = [ SoverB , hResponseBkg.GetXaxis().GetBinLowEdge(ibin) ]
                if SoverSqrtB >= r.maxSoverSqrtB[0]:
                    r.maxSoverSqrtB = [ SoverSqrtB , hResponseBkg.GetXaxis().GetBinLowEdge(ibin) ]
                if SoverSqrtSB >= r.maxSoverSqrtSB[0]:
                    r.maxSoverSqrtSB = [ SoverSqrtSB , hResponseBkg.GetXaxis().GetBinLowEdge(ibin) ]
        
        htools.makePretty( gRoc , color="roc" )

        textAuc = ROOT.TPaveText()
        textAuc.AddText( "AUC = %s" % (r.aucStr) )
        htools.makePretty( textAuc , textsize=1.0 , textlocation=htools.LEFT )

        textRoc = ROOT.TPaveText()
        textRoc.AddText( "%s %i#times%i" % (r.classifierName,r.nLayers,r.nNodes) )
        textRoc.AddText( r.featuresTitle )
        textRoc.AddText( "%s Training Events" % (r.nTrainingEventsStr) )
        htools.makePretty( textRoc , textsize=1.0 , textlocation=htools.RIGHT )
        textRoc.SetTextAlign( 22 )
        textRoc.SetX1NDC( textRoc.GetX1NDC()-0.2 )
        textRoc.SetX2NDC( textRoc.GetX2NDC()-0.2 )
        textRoc.SetY2NDC( textRoc.GetY2NDC()-0.45 )
        textRoc.SetY1NDC( textRoc.GetY1NDC()-0.45 )
        
        hRoc.Draw( "hist" )
        gRoc.Draw( "l" )
        lineRoc.Draw()
        textAuc.Draw()
        textRoc.Draw()
        c.save( r.configName+"_ROC" )

        if r.featuresTag=="AF" and r.nLayers==2 and r.nNodes==15 and r.nTrainingEventsStr in gRocOverlay_SampleSize_inc:
            gRocOverlay_SampleSize[r] = gRoc.Clone( "gRocOverlay_SampleSize_"+r.nTrainingEventsStr )

        if r.nLayers==11 and r.nNodes==300 and r.nTrainingEventsStr=="338K":
            gRocOverlay_Inputs[r] = gRoc.Clone( "gRocOverlay_Inputs_"+r.featuresTag )

        if r.featuresTag=="AF" and r.nTrainingEventsStr=="338K":
            if (r.nLayers==2 and r.nNodes==15): #  or (r.nLayers==11 and r.nNodes==300):
                gRocOverlay_Classifiers[r] = gRoc.Clone( "gRocOverlay_Classifiers_%s%i"%(r.classifierName,r.nLayers) )

        if r.featuresTag=="AF" and r.nTrainingEventsStr=="338K":
            gRocOverlay_Structures[r] = gRoc.Clone( "gRocOverlay_Structures_%ix%i"%(r.nLayers,r.nNodes) )
            
        del hResponseBkg
        del hResponseSig
        del legend
        del text
        
        del gRoc
        del textAuc
        del textRoc

        rList += [r]
        
        nPlots += 1

        #if nPlots > 5: break


#################################
# Plot marcus' results
#################################

dataDir = "Data/MarcusResults"

for cTag in [ "XGBoost" ]:
    for fTag in [ "AllFeatures" ]:
    
        r = MVAResultProcessor()
        r.classifierName     = cTag
        r.featuresTag        = fTag
        r.featuresTitle      = "All Features"
        r.nTrainingEventsStr = "338K"
        r.auc                = 0.802
        r.aucStr             = "0.802"

        inputFilePath = os.path.join( dataDir , "train.pred.depth1.col_sub025.1000" )
        inputFile     = open( inputFilePath , 'r' )

        xmin = -0.1
        xmax = 1.1

        hResponseSig = ROOT.TH1D( "hResponseSig" , ";%s Output;Normalized Number of Events"%(r.classifierName) , nbins , xmin , xmax )
        hResponseBkg = ROOT.TH1D( "hResponseBkg" , ";%s Output;Normalized Number of Events"%(r.classifierName) , nbins , xmin , xmax )

        for line in inputFile:
            words    = line.split(',')
            response = float(words[1])
            
            # Include overflow and underflow
            if response <= xmin: response = xmin + 0.00001
            if response >= xmax: response = xmax - 0.00001
            
            if words[0] == "1.0":
                hResponseSig.Fill( response )
            else:
                hResponseBkg.Fill( response )
                
        inputFile.close()

        htools.norm( hResponseSig )
        htools.norm( hResponseBkg )
        htools.makePretty( hResponseSig , color="sig" , linestyle="sig" )
        htools.makePretty( hResponseBkg , color="bkg" , linestyle="bkg" )
        htools.scaleMax( [hResponseBkg,hResponseSig] , 1.4 )
        
        legend = ROOT.TLegend()
        legend.AddEntry( 0 , "Test Data" , "" )
        legend.AddEntry( hResponseSig , "t#bar{t}H Signal" , "le" )
        legend.AddEntry( hResponseBkg , "t#bar{t} Background" , "le" )
        htools.makePretty( legend , textsize=0.8 , textlocation=htools.LEFT )

        text = ROOT.TPaveText()
        text.AddText( r.classifierName )
        text.AddText( r.featuresTitle )
        text.AddText( "%s Training Events" % (r.nTrainingEventsStr) )
        text.AddText( "AUC = %s" % (r.aucStr) )
        htools.makePretty( text , textsize=0.8 , textlocation=htools.RIGHT )
        
        hResponseBkg.Draw( "hist e" )
        hResponseSig.Draw( "hist e same" )
        legend.Draw()
        text.Draw()
        c.save( cTag+"_"+fTag+"_Output" )

        # Create a ROC curve
        gRoc = ROOT.TGraph()
        sigEff = 0.0
        bkgEff = 0.0
        gRoc.SetPoint( gRoc.GetN() , bkgEff , sigEff )
        for ibin in range(hResponseBkg.GetNbinsX(),0,-1):
            thisBinSig = hResponseSig.GetBinContent(ibin)
            thisBinBkg = hResponseBkg.GetBinContent(ibin)
            if thisBinSig==0 and thisBinBkg==0: continue
            sigEff += thisBinSig
            bkgEff += thisBinBkg
            gRoc.SetPoint( gRoc.GetN() , bkgEff , sigEff )
            if sigEff > 0 and bkgEff > 0:
                SoverB      = (100.0 * sigEff * w_sig) / (100.0 * bkgEff * w_bkg)
                SoverSqrtB  = (100.0 * sigEff * w_sig) / sqrt(100.0 * bkgEff * w_bkg)
                SoverSqrtSB = (100.0 * sigEff * w_sig) / sqrt( (100.0 * bkgEff * w_bkg) + (100.0 * sigEff * w_sig) )
                precision   = sigEff / (sigEff + bkgEff)
                recall      = sigEff
                f1score     = (2*precision*recall) / (precision+recall)
                tmpcut      = hResponseBkg.GetXaxis().GetBinLowEdge(ibin)
                if SoverB >= r.maxSoverB[0]: r.maxSoverB = [ SoverB , tmpcut ]
                if SoverSqrtB >= r.maxSoverSqrtB[0]: r.maxSoverSqrtB = [ SoverSqrtB , tmpcut ]
                if SoverSqrtSB >= r.maxSoverSqrtSB[0]: r.maxSoverSqrtSB = [ SoverSqrtSB , tmpcut ]
                if precision >= r.maxPrecision[0]: r.maxPrecision = [ precision , tmpcut ]
                if recall >= r.maxRecall[0]: r.maxRecall = [ recall , tmpcut ]
                if f1score >= r.maxF1score[0]: r.maxF1score = [ f1score , tmpcut ]

        report.debug( "%s %s s/sqrt(b)=%g cut=%g" % (r.classifierName,r.featuresTitle,r.maxSoverSqrtB[0],r.maxSoverSqrtB[1]) )
        report.blank( " s/sqrt(s+b)=%g cut=%g" % (r.maxSoverSqrtSB[0],r.maxSoverSqrtSB[1]) )
        report.blank( " recall=[%g,%g] , precision=[%g,%g] , f1score=[%g,%g]" % \
                      (r.maxRecall[0],r.maxRecall[1],r.maxPrecision[0],r.maxPrecision[1],r.maxF1score[0],r.maxF1score[1]) )
        
        htools.makePretty( gRoc , color="roc" )

        textAuc = ROOT.TPaveText()
        textAuc.AddText( "AUC = %s" % (r.aucStr) )
        htools.makePretty( textAuc , textsize=1.0 , textlocation=htools.LEFT )

        textRoc = ROOT.TPaveText()
        textRoc.AddText( r.classifierName )
        textRoc.AddText( r.featuresTitle )
        textRoc.AddText( "%s Training Events" % (r.nTrainingEventsStr) )
        htools.makePretty( textRoc , textsize=1.0 , textlocation=htools.RIGHT )
        textRoc.SetTextAlign( 22 )
        textRoc.SetX1NDC( textRoc.GetX1NDC()-0.2 )
        textRoc.SetX2NDC( textRoc.GetX2NDC()-0.2 )
        textRoc.SetY2NDC( textRoc.GetY2NDC()-0.45 )
        textRoc.SetY1NDC( textRoc.GetY1NDC()-0.45 )
        
        hRoc.Draw( "hist" )
        gRoc.Draw( "l" )
        lineRoc.Draw()
        textAuc.Draw()
        textRoc.Draw()
        c.save( cTag+"_"+fTag+"_ROC" )

        gRocOverlay_Classifiers[r] = gRoc.Clone( "gRocOverlay_Classifiers_"+r.classifierName+r.featuresTag )
        
        del hResponseBkg
        del hResponseSig
        del legend
        del text
        
        del gRoc
        del textAuc
        del textRoc
        
        
#################################
# Next plot Roberto's data for alternate classifiers
#################################

dataDir = "Data/RobertoResults/Weka_Results"

aucTable = { "NearestNeighbor"+"BasicFeatures" : 0.5346441062482277 ,
             "NaiveBayes"+"BasicFeatures" : 0.6638378502888628 ,
             "DecisionTree"+"BasicFeatures" : 0.5783798037391529 ,
             "RandomForest"+"BasicFeatures" : 0.7293512138797967 ,
             "NearestNeighbor"+"AllFeatures" : 0.5993241327157577 ,
             "NaiveBayes"+"AllFeatures" : 0.714647327391271 ,
             "DecisionTree"+"AllFeatures" : 0.622689149639592 ,
             "RandomForest"+"AllFeatures" : 0.7838794494487127 }

for cTag in [ "DecisionTree" , "NaiveBayes" , "NearestNeighbor" , "RandomForest" ]:
    for fTag in [ "AllFeatures" , "BasicFeatures" ]:
        #break
        r = MVAResultProcessor()
        r.classifierName     = "Decision Tree" if cTag=="DecisionTree" else \
                               "Naive Bayes" if cTag=="NaiveBayes" else \
                               "Nearest Neighbor" if cTag=="NearestNeighbor" else \
                               "Random Forest" if cTag=="RandomForest" else \
                               "UNKOWN CLASSIFIER"
        r.featuresTag        = fTag
        r.featuresTitle      = "All Features" if fTag=="AllFeatures" else \
                               "Basic Features" if fTag=="BasicFeatures" else \
                               "UNKOWN FEATURES"
        r.nTrainingEventsStr = "338K"
        r.auc                = aucTable[cTag+fTag]
        r.aucStr             = "%0.3f" % (r.auc)
        
        inputFilePath = os.path.join( dataDir , cTag+"_"+fTag+"_output.txt" )
        inputFile     = open( inputFilePath , 'r' )

        xmin = 0.0
        xmax = 1.0

        hResponseSig = ROOT.TH1D( "hResponseSig" , ";%s Output;Normalized Number of Events"%(r.classifierName) , nbins , xmin , xmax )
        hResponseBkg = ROOT.TH1D( "hResponseBkg" , ";%s Output;Normalized Number of Events"%(r.classifierName) , nbins , xmin , xmax )

        # Each line in the file contains a NN response for a single event
        # First half of the file is background events, second half is signal events
        for i in range(2*21158):

            line  = inputFile.readline()
            words = line.split()
            
            if not( len(words)==10 ):
                report.crash( "Wrong number of words in line:" , words )

            response = float(words[5])

            # Include overflow and underflow
            if response <= xmin: response = xmin + 0.00001
            if response >= xmax: response = xmax - 0.00001
            
            if i >= 21158:
                hResponseSig.Fill( response )
            else:
                hResponseBkg.Fill( response )
                
        inputFile.close()

        htools.norm( hResponseSig )
        htools.norm( hResponseBkg )
        htools.makePretty( hResponseSig , color="sig" , linestyle="sig" )
        htools.makePretty( hResponseBkg , color="bkg" , linestyle="bkg" )
        htools.scaleMax( [hResponseBkg,hResponseSig] , 1.4 )
        
        legend = ROOT.TLegend()
        legend.AddEntry( 0 , "Test Data" , "" )
        legend.AddEntry( hResponseSig , "t#bar{t}H Signal" , "le" )
        legend.AddEntry( hResponseBkg , "t#bar{t} Background" , "le" )
        htools.makePretty( legend , textsize=0.8 , textlocation=htools.LEFT )

        text = ROOT.TPaveText()
        text.AddText( r.classifierName )
        text.AddText( r.featuresTitle )
        text.AddText( "%s Training Events" % (r.nTrainingEventsStr) )
        text.AddText( "AUC = %s" % (r.aucStr) )
        htools.makePretty( text , textsize=0.8 , textlocation=htools.RIGHT )
        
        hResponseBkg.Draw( "hist e" )
        hResponseSig.Draw( "hist e same" )
        legend.Draw()
        text.Draw()
        c.save( "BasicClassifier_"+cTag+"_"+fTag+"_Output" )

        # Create a ROC curve
        gRoc = ROOT.TGraph()
        sigEff = 0.0
        bkgEff = 0.0
        gRoc.SetPoint( gRoc.GetN() , bkgEff , sigEff )
        for ibin in range(hResponseBkg.GetNbinsX(),0,-1):
            thisBinSig = hResponseSig.GetBinContent(ibin)
            thisBinBkg = hResponseBkg.GetBinContent(ibin)
            if thisBinSig==0 and thisBinBkg==0: continue
            sigEff += thisBinSig
            bkgEff += thisBinBkg
            gRoc.SetPoint( gRoc.GetN() , bkgEff , sigEff )
            if sigEff > 0 and bkgEff > 0:
                SoverB      = (100.0 * sigEff * w_sig) / (100.0 * bkgEff * w_bkg)
                SoverSqrtB  = (100.0 * sigEff * w_sig) / sqrt(100.0 * bkgEff * w_bkg)
                SoverSqrtSB = (100.0 * sigEff * w_sig) / sqrt( (100.0 * bkgEff * w_bkg) + (100.0 * sigEff * w_sig) )
                if SoverB >= r.maxSoverB[0]:
                    r.maxSoverB = [ SoverB , hResponseBkg.GetXaxis().GetBinLowEdge(ibin) ]
                if SoverSqrtB >= r.maxSoverSqrtB[0]:
                    r.maxSoverSqrtB = [ SoverSqrtB , hResponseBkg.GetXaxis().GetBinLowEdge(ibin) ]
                if SoverSqrtSB >= r.maxSoverSqrtSB[0]:
                    r.maxSoverSqrtSB = [ SoverSqrtSB , hResponseBkg.GetXaxis().GetBinLowEdge(ibin) ]
                #if cTag=="DecisionTree" and fTag=="BasicFeatures":
                #    report.debug( "bin %-3i : cut = %-20g, S/B = %-20g S/sqrtB = %-20g" % (ibin,hResponseBkg.GetXaxis().GetBinLowEdge(ibin),SoverB,SoverSqrtB) )

        #if cTag=="DecisionTree" and fTag=="BasicFeatures":
        #    report.crash()

        report.debug( "%s %s s/sqrt(b)=%g cut=%g s/sqrt(s+b)=%g cut=%g" % (r.classifierName,r.featuresTitle,r.maxSoverSqrtB[0],r.maxSoverSqrtB[1],r.maxSoverSqrtSB[0],r.maxSoverSqrtSB[1]) )
        
        htools.makePretty( gRoc , color="roc" )


        textAuc = ROOT.TPaveText()
        textAuc.AddText( "AUC = %s" % (r.aucStr) )
        htools.makePretty( textAuc , textsize=1.0 , textlocation=htools.LEFT )

        textRoc = ROOT.TPaveText()
        textRoc.AddText( r.classifierName )
        textRoc.AddText( r.featuresTitle )
        textRoc.AddText( "%s Training Events" % (r.nTrainingEventsStr) )
        htools.makePretty( textRoc , textsize=1.0 , textlocation=htools.RIGHT )
        textRoc.SetTextAlign( 22 )
        textRoc.SetX1NDC( textRoc.GetX1NDC()-0.2 )
        textRoc.SetX2NDC( textRoc.GetX2NDC()-0.2 )
        textRoc.SetY2NDC( textRoc.GetY2NDC()-0.45 )
        textRoc.SetY1NDC( textRoc.GetY1NDC()-0.45 )
        
        hRoc.Draw( "hist" )
        gRoc.Draw( "l" )
        lineRoc.Draw()
        textAuc.Draw()
        textRoc.Draw()
        c.save( "BasicClassifier_"+cTag+"_"+fTag+"_ROC" )

        if r.featuresTag=="AllFeatures":
            gRocOverlay_Classifiers[r] = gRoc.Clone( "gRocOverlay_Classifier_"+cTag )
        
        del hResponseBkg
        del hResponseSig
        del legend
        del text
        
        del gRoc
        del textAuc
        del textRoc


#
# Overlay ROCs for different training sample sizes
#
if len(gRocOverlay_SampleSize) > 0:

    legend = ROOT.TLegend()
    legend.AddEntry( 0 , "Training Events" , "" )

    keys = sorted( gRocOverlay_SampleSize.keys() , key=key_byTrainingEvents )
    
    for ir,r in enumerate(keys):
        htools.makePretty( gRocOverlay_SampleSize[r] , color=ir )
        legend.AddEntry( gRocOverlay_SampleSize[r] , "%s (AUC=%s)"%(r.nTrainingEventsStr,r.aucStr) , "l" )

    htools.makePretty( legend , textsize=0.8 , textlocation=htools.LEFT )
    legend.SetX1NDC( legend.GetX1NDC() + 0.35 )
    legend.SetX2NDC( legend.GetX2NDC() + 0.35 )
    legend.SetY1NDC( legend.GetY1NDC() - 0.4 )
    legend.SetY2NDC( legend.GetY2NDC() - 0.4 )

    text = ROOT.TPaveText()
    text.AddText( "%s %i#times%i"%(keys[0].classifierName,keys[0].nLayers,keys[0].nNodes) )
    text.AddText( keys[0].featuresTitle )
    htools.makePretty( text , textsize=1.0 , textlocation=htools.LEFT )
    
    hRoc.Draw( "hist" )
    for r in keys:
        gRocOverlay_SampleSize[r].Draw( "l" )
    lineRoc.Draw()
    legend.Draw()
    text.Draw()
    c.save( "OverlayROC_SampleSize" )

    for r in keys:
        del gRocOverlay_SampleSize[r]
    del legend
    del text


#
# Overlay ROCs for different input features
#
if len(gRocOverlay_Inputs) > 0:

    legend = ROOT.TLegend()
    legend.AddEntry( 0 , "Training Input" , "" )

    keys = sorted( gRocOverlay_Inputs.keys() , key=key_byAuc , reverse=True )
    
    for ir,r in enumerate(keys):
        htools.makePretty( gRocOverlay_Inputs[r] , color=ir )
        legend.AddEntry( gRocOverlay_Inputs[r] , "%s (AUC=%s)"%(r.featuresTitle,r.aucStr) , "l" )

    htools.makePretty( legend , textsize=0.8 , textlocation=htools.LEFT )
    legend.SetX1NDC( legend.GetX1NDC() + 0.2 )
    legend.SetX2NDC( legend.GetX2NDC() + 0.2 )
    legend.SetY1NDC( legend.GetY1NDC() - 0.5 )
    legend.SetY2NDC( legend.GetY2NDC() - 0.5 )

    text = ROOT.TPaveText()
    text.AddText( "338K Training Events" )
    text.AddText( "%s %i#times%i" % (keys[0].classifierName,keys[0].nLayers,keys[0].nNodes) )
    htools.makePretty( text , textsize=1.0 , textlocation=htools.LEFT )
    
    hRoc.Draw( "hist" )
    for r in keys:
        gRocOverlay_Inputs[r].Draw( "l" )
    lineRoc.Draw()
    legend.Draw()
    text.Draw()
    c.save( "OverlayROC_Inputs" )

    for r in keys:
        del gRocOverlay_Inputs[r]
    del legend
    del text


    
#
# Overlay ROCs for different classifiers
#
if len(gRocOverlay_Classifiers) > 0:

    legend = ROOT.TLegend()
    legend.AddEntry( 0 , "Classifier & Input Features" , "" )

    keys = sorted( gRocOverlay_Classifiers.keys() , key=key_byAuc , reverse=True )
    
    for ir,r in enumerate(keys):
        htools.makePretty( gRocOverlay_Classifiers[r] , color=ir )
        classifierTitle = r.classifierName
        if r.nLayers > 0:
            classifierTitle += " %i#times%i" % (r.nLayers,r.nNodes)
        legend.AddEntry( gRocOverlay_Classifiers[r] , "%s, %s (AUC=%s)"%(classifierTitle,r.featuresTitle,r.aucStr) , "l" )

    htools.makePretty( legend , textsize=0.6 , textlocation=htools.LEFT )
    legend.SetX1NDC( legend.GetX1NDC() + 0.2 )
    legend.SetX2NDC( legend.GetX2NDC() + 0.2 )
    legend.SetY1NDC( legend.GetY1NDC() - 0.5 )
    legend.SetY2NDC( legend.GetY2NDC() - 0.5 )

    text = ROOT.TPaveText()
    text.AddText( "338K Training Events" )
    #text.AddText( "All Features" )
    htools.makePretty( text , textsize=1.0 , textlocation=htools.LEFT )
    
    hRoc.Draw( "hist" )
    for r in keys:
        gRocOverlay_Classifiers[r].Draw( "l" )
    lineRoc.Draw()
    legend.Draw()
    text.Draw()
    c.save( "OverlayROC_Classifiers" )

    for r in keys:
        del gRocOverlay_Classifiers[r]
    del legend
    del text
    

#
# Overlay ROCs for different Layers x Nodes
#
if len(gRocOverlay_Structures) > 0:

    legend = ROOT.TLegend()
    legend.AddEntry( 0 , "NeuroBGD Layers #times Nodes" , "" )

    keys = sorted( gRocOverlay_Structures.keys() , key=key_byComplexity )
    
    for ir,r in enumerate(keys):
        htools.makePretty( gRocOverlay_Structures[r] , color=ir )
        legend.AddEntry( gRocOverlay_Structures[r] , "%i#times%i (AUC=%s)"%(r.nLayers,r.nNodes,r.aucStr) , "l" )

    htools.makePretty( legend , textsize=0.8 , textlocation=htools.LEFT )
    legend.SetX1NDC( legend.GetX1NDC() + 0.25 )
    legend.SetX2NDC( legend.GetX2NDC() + 0.25 )
    legend.SetY1NDC( legend.GetY1NDC() - 0.45 )
    legend.SetY2NDC( legend.GetY2NDC() - 0.45 )

    text = ROOT.TPaveText()
    text.AddText( "%s Training Events" % (keys[0].nTrainingEventsStr) )
    text.AddText( "%s" % (keys[0].featuresTitle) )
    htools.makePretty( text , textsize=1.0 , textlocation=htools.LEFT )
    
    hRoc.Draw( "hist" )
    for r in keys:
        gRocOverlay_Structures[r].Draw( "l" )
    lineRoc.Draw()
    legend.Draw()
    text.Draw()
    c.save( "OverlayROC_Structures" )

    for r in keys:
        del gRocOverlay_Structures[r]
    del legend
    del text

    
del hRoc
del lineRoc




#
# Print rankings
#
        
report.info( "Printing AUC Ranking" )
rList_byAuc = sorted( rList , key=key_byAuc , reverse=True )
for ir,r in enumerate(rList_byAuc):
    report.blank( "Rank = %-3i : %-20s %-20s %-20i AUC = %g" % (ir+1,r.featuresTag,r.configWords[1],r.nTrainingEvents,r.auc) )

report.info( "Printing Training Time Ranking" )
rList_byTrainingTime = sorted( rList , key=key_byTrainingTime , reverse=True )
for ir,r in enumerate(rList_byTrainingTime):
    report.blank( "%-20s Training Time = %gs" % (r.configName,r.trainingTime) )

report.info( "Printing S/B Ranking" )
rList_bySoverB = sorted( rList , key=key_bySoverB , reverse=True )
for ir,r in enumerate(rList_bySoverB):
    report.blank( "%-20s max(S/B) = %-20g cut = %-10g" % (r.configName,r.maxSoverB[0],r.maxSoverB[1]) )
    
report.info( "Printing S/sqrt(B) Ranking" )
rList_bySoverSqrtB = sorted( rList , key=key_bySoverSqrtB , reverse=True )
for ir,r in enumerate(rList_bySoverSqrtB):
    report.blank( "%-20s max(S/sqrt(B)) = %-20g cut = %-10g" % (r.configName,r.maxSoverSqrtB[0],r.maxSoverSqrtB[1]) )

report.info( "Printing S/sqrt(S+B) Ranking" )
rList_bySoverSqrtSB = sorted( rList , key=key_bySoverSqrtSB , reverse=True )
for ir,r in enumerate(rList_bySoverSqrtSB):
    report.blank( "%-20s max(S/sqrt(S+B)) = %-20g cut = %-10g" % (r.configName,r.maxSoverSqrtSB[0],r.maxSoverSqrtSB[1]) )


    
#
# Make bar graphs
#
    
# Make graphs of AUC vs. nLayers x nNodes
for featuresTitle in [ "All Features" , "Basic Features" , "Extended Features" ]:

    binMap   = { "2x15":1 , "2x300":2 , "5x300":3 , "8x300":4 , "11x300":5 }
    legList  = [ "1K" , "10K" , "20K" , "30K" , "100K" , "200K" , "338K" ]
    hAucMap  = { x : ROOT.TH1D("hAuc"+x,";NeuroBGD Layers #times Nodes;AUC",len(binMap),0,len(binMap)) for x in legList }
    hTimeMap = { x : ROOT.TH1D("hTime"+x,";NeuroBGD Layers #times Nodes;Training Time [s]",len(binMap),0,len(binMap)) for x in legList }
    featuresTag = "NULL"
    for r in rList:
        if not( featuresTitle==r.featuresTitle ): continue
        if not( r.nTrainingEventsStr in hAucMap.keys() ): continue
        if not( r.configWords[1] in binMap.keys() ): continue
        featuresTag = r.featuresTag
        ev = r.nTrainingEventsStr
        ib = binMap[r.configWords[1]]
        hAucMap[ev].SetBinContent( ib , r.auc )
        hTimeMap[ev].SetBinContent( ib , r.trainingTime )

    for label in binMap.keys():
        for nn in legList:
            hAucMap[nn].GetXaxis().SetBinLabel( binMap[label] , label.replace("x","#times") )
            hTimeMap[nn].GetXaxis().SetBinLabel( binMap[label] , label.replace("x","#times") )
        
    legend = ROOT.TLegend()
    legend.AddEntry( 0 , "Training Events" , "" )
    barWidth = 1.0 / float(len(hAucMap)+2)
    for ik,k in enumerate(legList):
        htools.makePretty( hAucMap[k] , color=ik , fill=True )
        hAucMap[k].SetBarWidth( barWidth )
        hAucMap[k].SetBarOffset( barWidth * float(ik+1) )
        hAucMap[k].SetLineWidth( 0 )
        hAucMap[k].SetMaximum( 1.35 )
        hAucMap[k].GetXaxis().SetLabelSize( 0.045 )
        htools.makePretty( hTimeMap[k] , color=ik , fill=True )
        hTimeMap[k].SetBarWidth( barWidth )
        hTimeMap[k].SetBarOffset( barWidth * float(ik+1) )
        hTimeMap[k].SetLineWidth( 0 )
        hTimeMap[k].SetMinimum( 0.1 )
        hTimeMap[k].SetMaximum( 3000000.0 )
        hTimeMap[k].GetXaxis().SetLabelSize( 0.045 )
        legend.AddEntry( hAucMap[k] , k , "f" )

    htools.makePretty( legend , textsize=0.7 , textlocation=htools.LEFT )

    text = ROOT.TPaveText()
    text.AddText( featuresTitle )
    htools.makePretty( text , textsize=0.8 , textlocation=htools.RIGHT )

    for ik,k in enumerate(legList):
        if ik > 0:
            hAucMap[k].Draw( "bar0 same" )
        else:
            hAucMap[k].Draw( "bar0" )
    legend.Draw()
    text.Draw()
    #c.save( "Summary_AUCByNNConfig_"+featuresTag )

    for ik,k in enumerate(legList):
        if ik > 0:
            hTimeMap[k].Draw( "bar0 same" )
        else:
            hTimeMap[k].Draw( "bar0" )
    legend.Draw()
    text.Draw()
    #c.saveLogy( "Summary_TimeByNNConfig_"+featuresTag )

    
    for k in legList:
        del hAucMap[k]
        del hTimeMap[k]
    del legend
    del text

    
# Make graphs of AUC vs. nLayers x nNodes
for featuresTitle in [ "All Features" , "Basic Features" , "Extended Features" ]:

    binMap  = { "1K":1 , "10K":2 , "20K":3 , "30K":4 , "100K":5 , "200K":6 , "338K":7 }
    legList = [ "2x15" , "2x300" , "5x300" , "8x300" , "11x300" ]
    hAucMap = { x : ROOT.TH1D("hAuc"+x,";Training Events;AUC",len(binMap),0,len(binMap)) for x in legList }
    hTimeMap = { x : ROOT.TH1D("hTime"+x,";Training Events;Training Time [s]",len(binMap),0,len(binMap)) for x in legList }
    featuresTag = "NULL"
    for r in rList:
        if not( featuresTitle==r.featuresTitle ): continue
        if not( r.nTrainingEventsStr in binMap.keys() ): continue
        if not( r.configWords[1] in legList ): continue
        featuresTag = r.featuresTag
        nn = r.configWords[1]
        ib = binMap[r.nTrainingEventsStr]
        hAucMap[nn].SetBinContent( ib , r.auc )
        hTimeMap[nn].SetBinContent( ib , r.trainingTime )

    for label in binMap.keys():
        for nn in legList:
            hAucMap[nn].GetXaxis().SetBinLabel( binMap[label] , label )
            hTimeMap[nn].GetXaxis().SetBinLabel( binMap[label] , label )

    legend = ROOT.TLegend()
    legend.AddEntry( 0 , "NeuroBGD Layers #times Nodes" , "" )
    barWidth = 1.0 / float(len(hAucMap)+2)
    for ik,k in enumerate(legList):
        htools.makePretty( hAucMap[k] , color=ik , fill=True )
        hAucMap[k].SetBarWidth( barWidth )
        hAucMap[k].SetBarOffset( barWidth * float(ik+1) )
        hAucMap[k].SetLineWidth( 0 )
        hAucMap[k].SetMinimum( 0.5 )
        hAucMap[k].SetMaximum( 0.9 )
        hAucMap[k].GetXaxis().SetLabelSize( 0.045 )
        htools.makePretty( hTimeMap[k] , color=ik , fill=True )
        hTimeMap[k].SetBarWidth( barWidth )
        hTimeMap[k].SetBarOffset( barWidth * float(ik+1) )
        hTimeMap[k].SetLineWidth( 0 )
        hTimeMap[k].SetMinimum( 0.1 )
        hTimeMap[k].SetMaximum( 1000000.0 )
        hTimeMap[k].GetXaxis().SetLabelSize( 0.045 )
        legend.AddEntry( hAucMap[k] , k.replace("x","#times") , "f" )

    htools.makePretty( legend , textsize=0.7 , textlocation=htools.LEFT )

    text = ROOT.TPaveText()
    text.AddText( featuresTitle )
    htools.makePretty( text , textsize=0.8 , textlocation=htools.RIGHT )
    
    for ik,k in enumerate(legList):
        if ik > 0:
            hAucMap[k].Draw( "bar0 same" )
        else:
            hAucMap[k].Draw( "bar0" )
    legend.Draw()
    text.Draw()
    c.save( "Summary_AUCByTrainingEvents_"+featuresTag )

    for ik,k in enumerate(legList):
        if ik > 0:
            hTimeMap[k].Draw( "bar0 same" )
        else:
            hTimeMap[k].Draw( "bar0" )
    legend.Draw()
    text.Draw()
    c.saveLogy( "Summary_TimeByTrainingEvents_"+featuresTag )

    
    for k in legList:
        del hAucMap[k]
        del hTimeMap[k]
    del legend
    del text


# Make horizontal bar graphs
if True:

    binMap         = { "1K":1 , "10K":2 , "20K":3 , "100K":4 , "200K":5 , "338K":6 }
    legList        = [ "2x15" , "2x300" , "5x300" , "8x300" , "11x300" ]
    featuresTitles = [ "Basic Features" , "Extended Features" , "All Features" ]
    hAucMap        = { x : ROOT.TH1D("hAuc"+x,";Training Events;AUC",len(featuresTitles)*len(binMap),0,len(featuresTitles)*len(binMap)) for x in legList }
    hTimeMap       = { x : ROOT.TH1D("hTime"+x,";Training Events;Training Time [s]",len(featuresTitles)*len(binMap),0,len(featuresTitles)*len(binMap)) for x in legList }
    
    for ititle,title in enumerate(featuresTitles):

        for r in rList:
            if not( title==r.featuresTitle ): continue
            if not( r.nTrainingEventsStr in binMap.keys() ): continue
            if not( r.configWords[1] in legList ): continue
            nn = r.configWords[1]
            ib = binMap[r.nTrainingEventsStr] + (ititle*len(binMap))
            hAucMap[nn].SetBinContent( ib , r.auc )
            hTimeMap[nn].SetBinContent( ib , r.trainingTime )

        for label in binMap.keys():
            for nn in legList:
                hAucMap[nn].GetXaxis().SetBinLabel( binMap[label] + (ititle*len(binMap)) , label )
                hTimeMap[nn].GetXaxis().SetBinLabel( binMap[label] + (ititle*len(binMap)) , "" ) # label )

    aucMin  = 0.5
    aucMax  = 0.82
    timeMin = 0.1
    timeMax = 1000000.0

    text4 = ROOT.TPaveText()
    text4.AddText( "NeuroBGD" )
    text4.AddText( "Layers #times Nodes" )
    
    legend = ROOT.TLegend()
    #legend.AddEntry( 0 , "NeuroBGD" , "" )
    #legend.AddEntry( 0 , "Layers #times Nodes" , "" )
    #legend.AddEntry( 0 , "Structure" , "" )
    barWidth = 1.0 / float(len(hAucMap)+2)
    for ik,k in enumerate(legList):
        htools.makePretty( hAucMap[k] , color=ik , fill=True )
        #hAucMap[k].SetFillStyle( 3004 )
        hAucMap[k].SetBarWidth( barWidth )
        hAucMap[k].SetBarOffset( barWidth * float(ik+1) )
        hAucMap[k].SetLineWidth( 0 )
        hAucMap[k].SetMinimum( aucMin )
        hAucMap[k].SetMaximum( aucMax )
        hAucMap[k].GetXaxis().SetLabelSize( 0.042 )
        hAucMap[k].GetXaxis().SetTitleOffset( 1.6 )
        hAucMap[k].GetXaxis().SetTitleSize( 0.035 )
        hAucMap[k].GetYaxis().SetLabelSize( 0.03 )
        hAucMap[k].GetYaxis().SetTitleOffset( 1.2 )
        htools.makePretty( hTimeMap[k] , color=ik , fill=True )
        hTimeMap[k].SetBarWidth( barWidth )
        hTimeMap[k].SetBarOffset( barWidth * float(ik+1) )
        hTimeMap[k].SetLineWidth( 0 )
        hTimeMap[k].SetMinimum( timeMin )
        hTimeMap[k].SetMaximum( timeMax )
        hTimeMap[k].GetXaxis().SetLabelSize( 0.045 )
        hTimeMap[k].GetXaxis().SetTitle( "" )
        hTimeMap[k].GetYaxis().SetTitleOffset( 1.2 )
        legend.AddEntry( hAucMap[k] , k.replace("x","#times") , "f" )

    htools.makePretty( legend , textsize=0.7 , textlocation=htools.LEFT )
    legend.SetX1NDC( legend.GetX1NDC() + 0.65 )
    legend.SetX2NDC( legend.GetX2NDC() + 0.65 )
    legend.SetY1NDC( legend.GetY1NDC() + 0.014 )
    legend.SetY2NDC( legend.GetY2NDC() + 0.014 )

    htools.makePretty( text4 , textsize=0.7 , textlocation=htools.LEFT )
    text4.SetX1NDC( text4.GetX1NDC() + 0.65 )
    text4.SetX2NDC( text4.GetX2NDC() + 0.65 )
    text4.SetY1NDC( text4.GetY1NDC() + 0.07 )
    text4.SetY2NDC( text4.GetY2NDC() + 0.07 )

    line1 = ROOT.TLine( 0.45 , len(binMap) , aucMax , len(binMap) )
    line2 = ROOT.TLine( 0.45 , 2*len(binMap) , aucMax , 2*len(binMap) )
    htools.makePretty( line1 , color=-1 , linewidth=1 , linestyle=ROOT.kDashed )
    htools.makePretty( line2 , color=-1 , linewidth=1 , linestyle=ROOT.kDashed )

    line3 = ROOT.TLine( timeMin , len(binMap) , timeMax , len(binMap) )
    line4 = ROOT.TLine( timeMin , 2*len(binMap) , timeMax , 2*len(binMap) )
    htools.makePretty( line3 , color=-1 , linewidth=1 , linestyle=ROOT.kDashed )
    htools.makePretty( line4 , color=-1 , linewidth=1 , linestyle=ROOT.kDashed )
    
    text1 = ROOT.TPaveText()
    text1.AddText( "All Features" )
    htools.makePretty( text1 , textsize=0.55 , textlocation=htools.RIGHT )
    text1.SetX1NDC( 0.805 )
    text1.SetY1NDC( 0.70 )
    text1.SetX2NDC( text1.GetX1NDC()+0.01 )
    text1.SetY2NDC( text1.GetY1NDC() + 0.55*0.04 )

    text2 = ROOT.TPaveText()
    text2.AddText( "Extended Features" )
    htools.makePretty( text2 , textsize=0.55 , textlocation=htools.RIGHT )
    text2.SetX1NDC( 0.805 )
    text2.SetY1NDC( 0.405 )
    text2.SetX2NDC( text2.GetX1NDC()+0.01 )
    text2.SetY2NDC( text2.GetY1NDC() + 0.55*0.04 )

    text3 = ROOT.TPaveText()
    text3.AddText( "Basic Features" )
    htools.makePretty( text3 , textsize=0.55 , textlocation=htools.RIGHT )
    text3.SetX1NDC( 0.805 )
    text3.SetY1NDC( 0.115 )
    text3.SetX2NDC( text3.GetX1NDC()+0.01 )
    text3.SetY2NDC( text3.GetY1NDC() + 0.55*0.04 )

    c.canvas.SetCanvasSize( 2000 , 1600 )
    c.canvas.Clear()

    pleft  = ROOT.TPad( "pleft" , "" , 0 , 0 , 1 , 1 )
    pleft.SetFillStyle( 4000 )
    pleft.SetFrameFillStyle( 4000 )
    pleft.SetRightMargin( 0.55 )
    pleft.SetLeftMargin( 0.12 )
    pleft.SetTopMargin( 0.025 )
    pleft.SetBottomMargin( 0.1 )
    
    pright = ROOT.TPad( "pright" , "" , 0 , 0 , 1 , 1 )
    pright.SetFillStyle( 4000 )
    pright.SetFrameFillStyle( 4000 )
    pright.SetLeftMargin( 0.49 )
    pright.SetRightMargin( 0.18 )
    pright.SetTopMargin( 0.025 )
    pright.SetBottomMargin( 0.1 )
    pright.SetLogx( 1 )

    plegend = ROOT.TPad( "plegend" , "" , 0 , 0 , 1 , 1 )
    plegend.SetFillStyle( 4000 )
    plegend.SetFrameFillStyle( 4000 )
    plegend.SetRightMargin( 0 )
    plegend.SetLeftMargin( 0.82 )
    plegend.SetTopMargin( 0.025 )
    plegend.SetBottomMargin( 0.1 )

    c.canvas.cd()
    plegend.Draw()
    plegend.cd()
    legend.Draw()
    text4.Draw()
    
    c.canvas.cd()
    pleft.Draw()
    pleft.cd()
    
    for ik,k in enumerate(legList):
        if ik > 0:
            hAucMap[k].Draw( "hbar0 same" )
        else:
            hAucMap[k].Draw( "hbar0" )
    line1.Draw()
    line2.Draw()
    #legend.Draw()
    #text1.Draw()

    c.canvas.cd()
    pright.Draw()
    pright.cd()
    
    for ik,k in enumerate(legList):
        if ik > 0:
            hTimeMap[k].Draw( "hbar0 same" )
        else:
            hTimeMap[k].Draw( "hbar0" )
            hTimeMap[k].GetYaxis().SetLabelSize( 0.03 )
            hTimeMap[k].GetYaxis().SetLabelOffset( -0.003 )
    line3.Draw()
    line4.Draw()
    text1.Draw()
    text2.Draw()
    text3.Draw()
    
    c.save( "SidewaysSummary" )

    c.canvas.cd()
    c.canvas.Clear()

    for k in legList:
        del hAucMap[k]
        del hTimeMap[k]
    del legend
    del text1
    del text2
    del text3
    del text4
    del line1
    del line2
    del line3
    del line4
    del pleft
    del pright
    del plegend
    
report.info( "done." )

# Open up the directory with output plots, just so we can see the results
c.openPlotDir()
