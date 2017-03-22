#include <iostream>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TList.h>
#include <TKey.h>

#include "Report.h"
#include "ArgParser.h"
#include "HistTools.h"
#include "Plotter.h"
#include "tth.h"
#include "TreeReader.h"
#include "DelphesReader.h"
#include "StackPlotter.h"

using namespace std;



int
main( int argc , char* argv[] )
{

  ArgParser ap( "DelphesPlots" , "Delphes plots for MVA project" );
  ap.addArg( "selectionTag" , "Lepton selection" , "ee, e, uu, u, eu, ljet, dil" );
  ap.addArg( "minNjets" , "Njet cut" , "0, 1, 2, ..." );
  ap.addArg( "minNbtags" , "N-btag cut" , "0, 1, 2, ..." );
  ap.addArg( "totalSplits" , "Number of splits for dividing job" , "1, 2, ..." );
  ap.addArg( "splitId" , "The split to run in this job" , "0, ..., splits-1" );
  ap.parse( argc , argv );

  Plotter p( ap );
  p.openPs();
  p.openDir();
  p.setCanvas1D();

  TString datadir ( "/atlasfs/atlas/local/jwebster/hepsim/data/rfast004" );
  gSystem->ExpandPathName( datadir );

  vector<TString> vec_ttbar_files;
  vector<TString> vec_ttbar123_files;
  vector<TString> vec_tth_files;
  vector<TString> vec_tth_old_files;
  {
    void *dirp = gSystem->OpenDirectory(datadir);
    const char *entry;
    Int_t n = 0;
    TString tmpfile;
    while( (entry = (char*)gSystem->GetDirEntry(dirp)) ) {
      tmpfile = entry;
      if( tmpfile.BeginsWith("tev14_mg5_ttbar_nj_") ) vec_ttbar_files.push_back( datadir + "/" + tmpfile );
      else if( tmpfile.BeginsWith("tev14_mg5_ttbar_n2j_") ) vec_ttbar123_files.push_back( datadir + "/" + tmpfile );
      else if( tmpfile.BeginsWith("tev14_mg5_Httbar_") ) vec_tth_old_files.push_back( datadir + "/" + tmpfile );
      else if( tmpfile.BeginsWith("tev13_mg5_ttH_") ) vec_tth_files.push_back( datadir + "/" + tmpfile );
    }
    delete entry;
  }
  report::info( "Found %i ttbar files" , Int_t(vec_ttbar_files.size()) );
  report::info( "Found %i ttbar123 files" , Int_t(vec_ttbar123_files.size()) );
  report::info( "Found %i tth files" , Int_t(vec_tth_files.size()) );

  Double_t totalSplits = ap.getAtof("totalSplits");
  Double_t splitId     = ap.getAtof("splitId");

  Int_t i , f;
  Double_t xsec , nev;

  // http://atlaswww.hep.anl.gov/hepsim/info.php?item=200
  PhysicsProcess proc_ttbar123( "ttbar123" , "t#bar{t} Np#neq0" , "$t\\bar{t} Np\\neq=0$" , 100+kGreen , "Delphes" );
  i    = TMath::FloorNint( splitId * Double_t(vec_ttbar123_files.size()) / totalSplits );
  f    = TMath::FloorNint( (splitId+1.0) * Double_t(vec_ttbar123_files.size()) / totalSplits );
  xsec = 336.354802117; // 13 TeV
  nev  = 22300922.0;
  report::info( "Loading ttbar123 files with IDs in range [ %i , %i )" , i , f );
  for( ; i < f ; ++i )
    proc_ttbar123.addSample( vec_ttbar123_files[i] , xsec , 1.0 , nev );

  // http://atlaswww.hep.anl.gov/hepsim/info.php?item=203
  PhysicsProcess proc_tth( "tth" , "t#bar{t}H" , "$t\\bar{t}H$" , 100+kRed , "Delphes" );
  i    = TMath::FloorNint( splitId * Double_t(vec_tth_files.size()) / totalSplits );
  f    = TMath::FloorNint( (splitId+1.0) * Double_t(vec_tth_files.size()) / totalSplits );
  xsec = 1.0 / 2.69745; // 13 TeV, extrapolating from the int lumi reported on hepsim page
  nev  = 10000000.0;
  report::info( "Loading tth files with IDs in range [ %i , %i )" , i , f );
  for( ; i < f ; ++i )
    proc_tth.addSample( vec_tth_files[i] , xsec , 1.0 , nev );
  
  // http://atlaswww.hep.anl.gov/hepsim/info.php?item=141
  /*
  PhysicsProcess proc_tth( "tth" , "t#bar{t}H" , "$t\\bar{t}H$" , 100+kRed , "Delphes" );
  i    = TMath::FloorNint( splitId * Double_t(vec_tth_files.size()) / totalSplits );
  f    = TMath::FloorNint( (splitId+1.0) * Double_t(vec_tth_files.size()) / totalSplits );
  xsec = 0.55913934067; // 14 TeV
  nev  = 1000000.0;
  report::info( "Loading tth files with IDs in range [ %i , %i )" , i , f );
  for( ; i < f ; ++i )
    proc_tth.addSample( vec_tth_files[i] , xsec , 1.0 , nev );
  */
  
  // http://atlaswww.hep.anl.gov/hepsim/info.php?item=181
  /*
  PhysicsProcess proc_ttbar( "ttbar" , "t#bar{t}" , "$t\\bar{t}$" , 100+kBlue , "Delphes" );
  i    = TMath::FloorNint( splitId * Double_t(vec_ttbar_files.size()) / totalSplits );
  f    = TMath::FloorNint( (splitId+1.0) * Double_t(vec_ttbar_files.size()) / totalSplits );
  xsec = 503.9000; // 14 TeV
  nev  = 2700.0 * Double_t(f-i);
  report::info( "Loading ttbar files with IDs in range [ %i , %i )" , i , f );
  for( ; i < f ; ++i )
    proc_ttbar.addSample( vec_ttbar_files[i] , xsec , 1.0 , nev );
  

    xsec = 1.0; // 313.3000;
  nev  = 1000.0; // 2043.0;
  //PhysicsProcess proc_ttbar_test_default( "ttbar_test_default" , "default" , "default" , 101 , "Delphes" );
  //proc_ttbar_test_default.addSample( "/atlasfs/atlas/local/jahred/delphestest/tev14_mg5_ttbar_n2j_0001_default.root" , xsec , 1.0 , nev );
  PhysicsProcess proc_ttbar_test_leptons( "ttbar_test_leptons" , "leptons" , "leptons" , 102 , "Delphes" );
  proc_ttbar_test_leptons.addSample( "/atlasfs/atlas/local/jahred/delphestest/tev14_mg5_ttbar_n2j_0001_leptons.root" , xsec , 1.0 , nev );
  PhysicsProcess proc_ttbar_test_photons( "ttbar_test_photons" , "photons" , "photons" , 103 , "Delphes" );
  proc_ttbar_test_photons.addSample( "/atlasfs/atlas/local/jahred/delphestest/tev14_mg5_ttbar_n2j_0001_photons.root" , xsec , 1.0 , nev );
  PhysicsProcess proc_ttbar_test_updated( "ttbar_test_updated" , "combined" , "combined" , 104 , "Delphes" );
  proc_ttbar_test_updated.addSample( "/atlasfs/atlas/local/jahred/delphestest/tev14_mg5_ttbar_n2j_0001_updated.root" , xsec , 1.0 , nev );

  xsec = 1.0;
  nev  = 1000.0;
  PhysicsProcess proc_atlas( "atlas" , "ATLAS" , "ATLAS" , 102 , "Delphes" );
  PhysicsProcess proc_cms( "cms" , "CMS" , "CMS" , 103 , "Delphes" );
  PhysicsProcess proc_snowmass( "snowmass" , "Snowmass" , "Snowmass" , 104 , "Delphes" );
  for( int i = 1 ; i < 10 ; i++ ) {
    proc_atlas.addSample( TString::Format("/users/ac.jahreda/jahred/delphestest/tev14_mg5_ttbar_n2j_000%i_atlas.root",i) , xsec , 1.0 , nev );
    proc_cms.addSample( TString::Format("/users/ac.jahreda/jahred/delphestest/tev14_mg5_ttbar_n2j_000%i_cms.root",i) , xsec , 1.0 , nev );
    proc_snowmass.addSample( TString::Format("/users/ac.jahreda/jahred/delphestest/tev14_mg5_ttbar_n2j_000%i_snowmass.root",i) , xsec , 1.0 , nev );
  }
  */
  
  
  InputConfig input( "Delphes" );
  if( proc_ttbar123.numSamples() > 0 ) input.addBackgroundProcess( proc_ttbar123 );
  if( proc_tth.numSamples() > 0 ) input.addSignalProcess( proc_tth );
  //if( proc_ttbar.numSamples() > 0 ) input.addBackgroundProcess( proc_ttbar );
  ////input.addBackgroundProcess( proc_ttbar_test_default );
  //input.addBackgroundProcess( proc_ttbar_test_leptons );
  //input.addBackgroundProcess( proc_ttbar_test_photons );
  //input.addBackgroundProcess( proc_ttbar_test_updated );
  //input.addBackgroundProcess( proc_atlas );
  //input.addBackgroundProcess( proc_cms );
  //input.addBackgroundProcess( proc_snowmass );
  input.setLuminosity( 1000.0 );
  input.loadRootFiles();

  
  DelphesReader tr;
  tr.setSelectionTag( ap["selectionTag"] );
  tr.setMinNjets( ap.getAtoi("minNjets") );
  tr.setMinNbtags( ap.getAtoi("minNbtags") );
  tr.setTotalSplits( ap.getAtoi("totalSplits") );
  tr.setSplitId( ap.getAtoi("splitId") );

  p.openRoot();

  TH1D *h_lumi = new TH1D( "h_lumi" , "h_lumi" , 1 , 0 , 1 );
  h_lumi->SetBinContent( 1 , tth::input.getLuminosity() );
  p.write( h_lumi , "h_lumi" );

  StackPlotter sp( &input , &p , &tr );
  //sp.addToQueue( "isTagged_ALL" , "isTagged" , 15 , -5 , 10 );
  //sp.addToQueue( "good_jet_flavor_ALL" , "Jet Flavor" , 20 , -10 , 10 );
  //sp.addToQueue( "good_njets" , "nJets" , 20 , 0 , 20 );
  //sp.addToQueue( "good_nbtags" , "nBtags" , 20 , 0 , 20 );
  //sp.addToQueue( "good_el_pt_SIZE" , "nElectrons" , 10 , 0 , 10 );
  //sp.addToQueue( "good_mu_pt_SIZE" , "nMuons" , 10 , 0 , 10 );

  //sp.addToQueue( "isTagged_ALL" , "Jet b-tag level (Jahred's algo, higher means tighter)" , 8 , -2 , 6 );

  // Common
  sp.addToQueue( "nJets" , "nJets" , 20 , 0 , 20 );
  sp.addToQueue( "nLeptons" , "nLeptons" , 10 , 0 , 10 );
  sp.addToQueue( "good_nbtags_1" , "nBTags (Highest Eff WP)" , 10 , 0 , 10 );
  sp.addToQueue( "HT_all" , "H_{T}^{all} [GeV]" , 30 , 0 , 1500 , tth::GeV );
  sp.addToQueue( "HT_had" , "H_{T}^{had} [GeV]" , 30 , 0 , 1500 , tth::GeV );
  sp.addToQueue( "pT_jet0" , "p_{T} (first jet) [GeV]" , 25 , 0 , 500 , tth::GeV );
  sp.addToQueue( "pT_jet1" , "p_{T} (second jet) [GeV]" , 25 , 0 , 500 , tth::GeV );
  sp.addToQueue( "pT_jet2" , "p_{T} (third jet) [GeV]" , 25 , 0 , 500 , tth::GeV );
  sp.addToQueue( "pT_jet3" , "p_{T} (fourth jet) [GeV]" , 25 , 0 , 500 , tth::GeV );

  /*
  sp.addToQueue( "Aplanarity_ujets" , "Aplanarity_ujets" , 100 , -1 , 2 );
  sp.addToQueue( "Aplanority_ujets" , "Aplanority_ujets" , 100 , -1 , 2 );
  sp.addToQueue( "Sphericity_ujets" , "Sphericity_ujets" , 100 , -1 , 2 );
  sp.addToQueue( "Spherocity_ujets" , "Spherocity_ujets" , 100 , -1 , 2 );
  sp.addToQueue( "SphericityT_ujets" , "SphericityT_ujets" , 100 , -1 , 2 );
  sp.addToQueue( "Planarity_ujets" , "Planarity_ujets" , 100 , -1 , 2 );
  sp.addToQueue( "Variable_C_ujets" , "Variable_C_ujets" , 100 , -1 , 2 );
  sp.addToQueue( "Variable_D_ujets" , "Variable_D_ujets" , 100 , -1 , 2 );
  sp.addToQueue( "Circularity_ujets" , "Circularity_ujets" , 100 , -1 , 2 );
  sp.addToQueue( "PlanarFlow_ujets" , "PlanarFlow_ujets" , 100 , -1 , 2 );
  */
  
  /*
  sp.addToQueue( "good_nbtags_1" , "nBTags(1)" , 20 , 0 , 20 );
  sp.addToQueue( "good_nbtags_2" , "nBTags(2)" , 20 , 0 , 20 );
  sp.addToQueue( "good_nbtags_3" , "nBTags(3)" , 20 , 0 , 20 );
  sp.addToQueue( "good_nbtags_4" , "nBTags(4)" , 20 , 0 , 20 );
  sp.addToQueue( "good_nbtags_5" , "nBTags(5)" , 20 , 0 , 20 );
  */
  
  /*
  sp.addToQueue( "nBTags" , "nBTags" , 20 , 0 , 20 );
  sp.addToQueue( "Centrality" , "Centrality" , 80 , -2 , 2 );
  sp.addToQueue( "Mbb_MindR" , "Mbb_MindR" , 50 , -10 , 600 , tth::GeV );
  sp.addToQueue( "dRbb_MaxPt" , "dRbb_MaxPt" , 50 , -2 , 10 );

  // L+jets
  sp.addToQueue( "pT_jet5" , "pT_jet5" , 50 , -2 , 500 , tth::GeV );
  sp.addToQueue( "H1_all" , "H1_all" , 50 , -2 , 2 );
  sp.addToQueue( "dRbb_avg" , "dRbb_avg" , 50 , -2 , 10 );
  sp.addToQueue( "Mbj_MaxPt" , "Mbj_MaxPt" , 50 , -2 , 600 , tth::GeV );
  sp.addToQueue( "dRlepbb_MindR" , "dRlepbb_MindR" , 50 , -2 , 10 );
  sp.addToQueue( "Muu_MindR" , "Muu_MindR" , 50 , -2 , 600 , tth::GeV );
  sp.addToQueue( "Aplan_b" , "Aplan_b" , 50 , -2 , 2 );
  sp.addToQueue( "Njet_pT40" , "Njet_pT40" , 20 , 0 , 20 );
  sp.addToQueue( "Mbj_MindR" , "Mbj_MindR" , 50 , -2 , 600 , tth::GeV );
  sp.addToQueue( "HT_had" , "HT_had" , 50 , -2 , 1000 , tth::GeV  );
  sp.addToQueue( "Mjj_MindR" , "Mjj_MindR" , 50 , -2 , 600 , tth::GeV );
  sp.addToQueue( "Mbb_MaxPt" , "Mbb_MaxPt" , 50 , -2 , 600 , tth::GeV );
  sp.addToQueue( "pTuu_MindR" , "pTuu_MindR" , 50 , -2 , 800 , tth::GeV );
  sp.addToQueue( "Mbb_MaxM" , "Mbb_MaxM" , 50 , -2 , 600 , tth::GeV );
  sp.addToQueue( "dRuu_MindR" , "dRuu_MindR" , 50 , -2 , 10 );
  sp.addToQueue( "Mjjj_MaxPt" , "Mjjj_MaxPt" , 50 , -2 , 600 , tth::GeV );
  
  // DIL
  sp.addToQueue( "Mjj_MaxPt" , "Mjj_MaxPt" , 50 , -10 , 600 , tth::GeV );
  sp.addToQueue( "dEtajj_MaxdEta" , "dEtajj_MaxdEta" , 24 , -2 , 10 );
  sp.addToQueue( "MHiggs" , "MHiggs" , 50 , -10 , 600 , tth::GeV );
  sp.addToQueue( "dRHl_MindR" , "dRHl_MindR" , 24 , -2 , 10 );
  sp.addToQueue( "NHiggs_30" , "NHiggs_30" , 12 , -2 , 10 );
  sp.addToQueue( "Aplan_had" , "Aplan_had" , 50 , -2 , 2 );
  sp.addToQueue( "Mjj_MinM" , "Mjj_MinM" , 50 , -10 , 600 , tth::GeV );
  sp.addToQueue( "dRHl_MaxdR" , "dRHl_MaxdR" , 24 , -2 , 10 );
  sp.addToQueue( "Mjj_HiggsMass" , "Mjj_HiggsMass" , 50 , -10 , 600 , tth::GeV );
  sp.addToQueue( "HT_all" , "HT_all" , 50 , -10 , 1000 , tth::GeV );
  sp.addToQueue( "dRbb_MaxM" , "dRbb_MaxM" , 24 , -2 , 10 );
  sp.addToQueue( "dRlj_MindR" , "dRlj_MindR" , 24 , -2 , 10 );
  sp.addToQueue( "H4_all" , "H4_all" , 50 , -2 , 2 );
  sp.addToQueue( "pT_jet3" , "pT_jet3" , 50 , -10 , 500 , tth::GeV );
  */
  
  sp.overlayMC( kFALSE );
  tr.saveOutputTrees( ap.getTag() );
  
  p.closeRoot();
  p.closePs();
  report::info( "done." );
  return 0;

}
