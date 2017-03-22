#ifndef _DELPHESREADER_H_
#define _DELPHESREADER_H_

#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <map>
#include <vector>

#include "TString.h"
#include "TTree.h"
#include "TDataType.h"
#include "TMath.h"
#include "TLeaf.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TRandom3.h"

#include "Report.h"
#include "tth.h"

#include "TTHbbLeptonic/MVAVariables.h"
#include "TTHbbLeptonic/PairedSystem.h"
#include "TopEvent/Event.h"

// Delphes Includes
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"

using namespace std;
using namespace tth;

class
DelphesReader : public TreeReader
{

private:

  const Double_t MeV = 1000.0;
  const Double_t GeV = 1.0;
  
  ExRootTreeReader *ex;
  TClonesArray *br_el;
  TClonesArray *br_mu;
  TClonesArray *br_jet;
  TClonesArray *br_met;
  TClonesArray *br_part;

  std::vector<TLorentzVector> good_jets;
  std::vector<TLorentzVector> good_el;
  std::vector<TLorentzVector> good_mu;
  std::vector<std::size_t> good_jets_ids;
  std::vector<std::size_t> good_el_id;
  std::vector<std::size_t> good_mu_id;
  
  TString selectionTag;
  UInt_t minNjets;
  UInt_t minNbtags;
  UInt_t totalSplits;
  UInt_t splitId;

  TF1 *func0B;
  TF1 *func1B;
  TF1 *func2B;
  TF1 *func3B;
  TF1 *func4B;

  TF1 *func0C;
  TF1 *func1C;
  TF1 *func2C;
  TF1 *func3C;
  TF1 *func4C;

  TF1 *func0L;
  TF1 *func1L;
  TF1 *func2L;
  TF1 *func3L;
  TF1 *func4L;

  TRandom3 r;

  bool isTagged[5];
  
  Int_t getTagLevel( const float &pt , const int &flavor ) {

    Int_t tagLevel = 0;
    float efficiency[5];
    //P(tight | loose) = P(loose | tight) * P(tight)/ P (loose) = P(tight)/ P (loose)

    if (flavor == 5) {
      efficiency[0] = func0B->Eval(pt);//loosest
      efficiency[1] = func1B->Eval(pt);
      efficiency[2] = func2B->Eval(pt);
      efficiency[3] = func3B->Eval(pt);
      efficiency[4] = func4B->Eval(pt);//tightest
    }
    else if (flavor == 4) {
      efficiency[0] = func0C->Eval(pt);//loosest
      efficiency[1] = func1C->Eval(pt);
      efficiency[2] = func2C->Eval(pt);
      efficiency[3] = func3C->Eval(pt);
      efficiency[4] = func4C->Eval(pt);//tightest
    }
    else {
      efficiency[0] = func0L->Eval(pt);//loosest
      efficiency[1] = func1L->Eval(pt);
      efficiency[2] = func2L->Eval(pt);
      efficiency[3] = func3L->Eval(pt);
      efficiency[4] = func4L->Eval(pt);//tightest
    }
    float uniform = r.Uniform(1.0);

    if( uniform < efficiency[0] ) {
      tagLevel = 1;
      float looseGivenTight = efficiency[1] / efficiency[0];
      uniform = r.Uniform(1.0);
      if( uniform < looseGivenTight ) {
	tagLevel = 2;
        looseGivenTight = efficiency[2] / efficiency[1];
        uniform = r.Uniform(1.0);
        if( uniform < looseGivenTight ) {
	  tagLevel = 3;
          looseGivenTight = efficiency[3] / efficiency[2];
          uniform = r.Uniform(1.0);
          if( uniform < looseGivenTight ) {
	    tagLevel = 4;
            looseGivenTight = efficiency[4] / efficiency[3];
            uniform = r.Uniform(1.0);
            if( uniform < looseGivenTight ) {
	      tagLevel = 5;
            }
	  }
	}
      }
    }

    return tagLevel;
  }

  std::ofstream *outputCsv;
  Double_t weight;
  

public:

  DelphesReader()
    : TreeReader()
    , selectionTag( "None" )
    , minNjets( 0 )
    , minNbtags( 0 )
    , totalSplits( 0 )
    , splitId( 0 )
    , r(8675309)
    , func0B( new TF1("func0B","0.85*TMath::TanH(0.0026*x[0])*(30.0/(1+0.063*x[0]))",0,500) )
    , func1B( new TF1("func1B","0.84*TMath::TanH(0.0025*x[0])*(28.0/(1+0.068*x[0]))",0,500) )
    , func2B( new TF1("func2B","0.82*TMath::TanH(0.0024*x[0])*(27.0/(1+0.07*x[0]))",0,500) )
    , func3B( new TF1("func3B","0.75*TMath::TanH(0.0023*x[0])*(25.0/(1+0.072*x[0]))",0,500) )
    , func4B( new TF1("func4B","0.7*TMath::TanH(0.0022*x[0])*(25.0/(1+0.077*x[0]))",0,500) )
    , func0C( new TF1("func0C","0.25*TMath::TanH(0.018*x[0])*(1/(1+0.0013*x[0]))",0,500) )
    , func1C( new TF1("func1C","0.24*TMath::TanH(0.016*x[0])*(1/(1+0.0012*x[0]))",0,500) )
    , func2C( new TF1("func2C","0.23*TMath::TanH(0.014*x[0])*(1/(1+0.0011*x[0]))",0,500) )
    , func3C( new TF1("func3C","0.22*TMath::TanH(0.011*x[0])*(1/(1+0.0010*x[0]))",0,500) )
    , func4C( new TF1("func4C","0.20*TMath::TanH(0.008*x[0])*(1/(1+0.0009*x[0]))",0,500) )
    , func0L( new TF1("func0L","0.01*0.00038*x[0]",0,500) )
    , func1L( new TF1("func1L","0.008*0.00036*x[0]",0,500) )
    , func2L( new TF1("func2L","0.006*0.0003*x[0]",0,500) )
    , func3L( new TF1("func3L","0.003*0.00025*x[0]",0,500) )
    , func4L( new TF1("func4L","0.001*0.0001*x[0]",0,500) )
    , outputCsv( 0 )
  {}

  ~DelphesReader() {
    delete ex;
    //delete br_el;
    //delete br_mu;
    //delete br_jet;
    //delete br_met;
    delete func0B;
    delete func1B;
    delete func2B;
    delete func3B;
    delete func4B;
    delete func0C;
    delete func1C;
    delete func2C;
    delete func3C;
    delete func4C;
    delete func0L;
    delete func1L;
    delete func2L;
    delete func3L;
    delete func4L;
  }

  void setSelectionTag( TString v ) { selectionTag = v; }
  void setMinNjets( UInt_t v ) { minNjets = v; }
  void setMinNbtags( UInt_t v ) { minNbtags = v; }
  void setTotalSplits( UInt_t v ) { totalSplits = v; }
  void setSplitId( UInt_t v ) { splitId = v; }

  void setupOutputTree( const TString &tname ) {

    // Initilialize output TTree
    TTree *tmp_tree = new TTree( tname , tname );
    tmp_tree->SetDirectory( 0 );
    tmp_tree->Branch( "weight" , &weight );
    tmp_tree->Branch( "ee" , &map_char["ee"] );
    tmp_tree->Branch( "uu" , &map_char["uu"] );
    tmp_tree->Branch( "eu" , &map_char["eu"] );
    tmp_tree->Branch( "e" , &map_char["e"] );
    tmp_tree->Branch( "u" , &map_char["u"] );

    tmp_tree->Branch( "nJets" , &map_int["nJets"] );
    tmp_tree->Branch( "good_nbtags_1" , &map_uint["good_nbtags_1"] );
    tmp_tree->Branch( "good_nbtags_2" , &map_uint["good_nbtags_2"] );
    tmp_tree->Branch( "good_nbtags_3" , &map_uint["good_nbtags_3"] );
    tmp_tree->Branch( "good_nbtags_4" , &map_uint["good_nbtags_4"] );
    tmp_tree->Branch( "good_nbtags_5" , &map_uint["good_nbtags_5"] );

    tmp_tree->Branch( "pT_met" , &map_float["pT_met"] );
    tmp_tree->Branch( "eta_met" , &map_float["eta_met"] );
    tmp_tree->Branch( "phi_met" , &map_float["phi_met"] );

    tmp_tree->Branch( "pT_lepton0" , &map_float["pT_lepton0"] );
    tmp_tree->Branch( "eta_lepton0" , &map_float["eta_lepton0"] );
    tmp_tree->Branch( "phi_lepton0" , &map_float["phi_lepton0"] );
    tmp_tree->Branch( "flavor_lepton0" , &map_int["flavor_lepton0"] );
    tmp_tree->Branch( "pT_lepton1" , &map_float["pT_lepton1"] );
    tmp_tree->Branch( "eta_lepton1" , &map_float["eta_lepton1"] );
    tmp_tree->Branch( "phi_lepton1" , &map_float["phi_lepton1"] );
    tmp_tree->Branch( "flavor_lepton1" , &map_int["flavor_lepton1"] );
    
    for( Int_t ij = 0 ; ij < 10 ; ij++ ) {
      TString itag = TString::Format( "jet%i" , ij );
      tmp_tree->Branch( "pT_"+itag , &map_float["pT_"+itag] );
      tmp_tree->Branch( "eta_"+itag , &map_float["eta_"+itag] );
      tmp_tree->Branch( "phi_"+itag , &map_float["phi_"+itag] );
      tmp_tree->Branch( "M_"+itag , &map_float["M_"+itag] );
      tmp_tree->Branch( "bTagLevel_"+itag , &map_int["bTagLevel_"+itag] );
    }

    // Begin high-level observables
    
    tmp_tree->Branch( "nJetsAbovePt25" , &map_int["nJetsAbovePt25"] );
    tmp_tree->Branch( "nJetsAbovePt30" , &map_int["nJetsAbovePt30"] );
    tmp_tree->Branch( "nJetsAbovePt35" , &map_int["nJetsAbovePt35"] );
    tmp_tree->Branch( "nJetsAbovePt40" , &map_int["nJetsAbovePt40"] );

    for( Int_t ij = 0 ; ij < 5 ; ij++ ) {
      TString itag = TString::Format( "jet%i" , ij );
      tmp_tree->Branch( "pT_b"+itag , &map_float["pT_b"+itag] );
      tmp_tree->Branch( "eta_b"+itag , &map_float["eta_b"+itag] );
      tmp_tree->Branch( "phi_b"+itag , &map_float["phi_b"+itag] );
      tmp_tree->Branch( "M_b"+itag , &map_float["M_b"+itag] );
    }

    tmp_tree->Branch( "HT_all" , &map_float["HT_all"] );
    tmp_tree->Branch( "HT_had" , &map_float["HT_had"] );
    tmp_tree->Branch( "Centrality" , &map_float["Centrality"] );
    tmp_tree->Branch( "NHiggs_30" , &map_float["NHiggs_30"] );
    tmp_tree->Branch( "MHiggs" , &map_float["MHiggs"] );

    tmp_tree->Branch( "H1_all" , &map_float["H1_all"] );
    tmp_tree->Branch( "H2_all" , &map_float["H2_all"] );
    tmp_tree->Branch( "H3_all" , &map_float["H3_all"] );
    tmp_tree->Branch( "H4_all" , &map_float["H4_all"] );
    tmp_tree->Branch( "H5_all" , &map_float["H5_all"] );
    tmp_tree->Branch( "H1transverse_all" , &map_float["H1transverse_all"] );
    tmp_tree->Branch( "H2transverse_all" , &map_float["H2transverse_all"] );
    tmp_tree->Branch( "H3transverse_all" , &map_float["H3transverse_all"] );
    tmp_tree->Branch( "H4transverse_all" , &map_float["H4transverse_all"] );
    tmp_tree->Branch( "H5transverse_all" , &map_float["H5transverse_all"] );

    tmp_tree->Branch( "Thrust_all" , &map_float["Thrust_all"] );
    tmp_tree->Branch( "ThrustAxisX_all" , &map_float["ThrustAxisX_all"] );
    tmp_tree->Branch( "ThrustAxisY_all" , &map_float["ThrustAxisY_all"] );
    tmp_tree->Branch( "ThrustAxisZ_all" , &map_float["ThrustAxisZ_all"] );
    
    for( variableIter iv = mva_variable_names.begin() , fv = mva_variable_names.end() ; iv != fv ; ++iv ) {
      for( pairingIter ip = mva_pairing_names.begin() , fp = mva_pairing_names.end() ; ip != fp ; ++ip ) {
	TString itag = ip->second + "_" + iv->second;
	tmp_tree->Branch( "M"+itag , &map_float["M"+itag] );
	tmp_tree->Branch( "Pt"+itag , &map_float["Pt"+itag] );
	tmp_tree->Branch( "PtSum"+itag , &map_float["PtSum"+itag] );
	tmp_tree->Branch( "dR"+itag , &map_float["dR"+itag] );
	tmp_tree->Branch( "dPhi"+itag , &map_float["dPhi"+itag] );
	tmp_tree->Branch( "dEta"+itag , &map_float["dEta"+itag] );
      }
      tmp_tree->Branch( "Mjjj_"+iv->second , &map_float["Mjjj_"+iv->second] );
      tmp_tree->Branch( "Ptjjj_"+iv->second , &map_float["Ptjjj_"+iv->second] );
    }
    
    tmp_tree->Branch( "DileptonMass" , &map_float["DileptonMass"] );
    tmp_tree->Branch( "DileptonPt" , &map_float["DileptonPt"] );
    tmp_tree->Branch( "DileptonSumPt" , &map_float["DileptonSumPt"] );
    tmp_tree->Branch( "DileptondR" , &map_float["DileptondR"] );
    tmp_tree->Branch( "DileptondPhi" , &map_float["DileptondPhi"] );
    tmp_tree->Branch( "DileptondEta" , &map_float["DileptondEta"] );
    
    for( collectionIter ic = mva_collection_names.begin() , fc = mva_collection_names.end() ; ic != fc ; ++ic ) {
      tmp_tree->Branch( "Aplanarity_"+ic->second , &map_float["Aplanarity_"+ic->second] );
      tmp_tree->Branch( "Aplanority_"+ic->second , &map_float["Aplanority_"+ic->second] );
      tmp_tree->Branch( "Sphericity_"+ic->second , &map_float["Sphericity_"+ic->second] );
      tmp_tree->Branch( "Spherocity_"+ic->second , &map_float["Spherocity_"+ic->second] );
      tmp_tree->Branch( "SphericityT_"+ic->second , &map_float["SphericityT_"+ic->second] );
      tmp_tree->Branch( "Planarity_"+ic->second , &map_float["Planarity_"+ic->second] );
      tmp_tree->Branch( "Variable_C_"+ic->second , &map_float["Variable_C_"+ic->second] );
      tmp_tree->Branch( "Variable_D_"+ic->second , &map_float["Variable_D_"+ic->second] );
      tmp_tree->Branch( "Circularity_"+ic->second , &map_float["Circularity_"+ic->second] );
      tmp_tree->Branch( "PlanarFlow_"+ic->second , &map_float["PlanarFlow_"+ic->second] );
    }
    
    tmp_tree->Branch( "dRlepbb_MindR" , &map_float["dRlepbb_MindR"] );
    
    // Close existing csv file and initialize a new one to contain same output as tree
    if( outputCsv ) outputCsv->close();
    outputCsv = new std::ofstream( TString::Format("output/DelphesReader_%s_%u_%u_%u_%u_%s.csv",selectionTag.Data(),minNjets,minNbtags,totalSplits,splitId,tname.Data()).Data() );
    (*outputCsv) << "Event";
    TIter next = tmp_tree->GetListOfLeaves();
    TLeaf *leaf = 0;
    while( (leaf = (TLeaf*)next()) ) {
      TString leaftype = leaf->GetTypeName();
      if( leaftype.BeginsWith("vector") ) continue;
      //TString leafname ( leaf->GetName() );
      (*outputCsv) << "," << leaf->GetName();
    }
    (*outputCsv) << std::endl;
    
    outputTrees.push_back( tmp_tree );
  }

  void fillOutputTree( const std::size_t &ifile , const Long64_t &ievent , const Double_t &w ) {

    weight = w;
    outputTrees.back()->Fill();

    (*outputCsv) << (Long64_t(ifile) * Long64_t(27000)) + ievent;
    TIter next = outputTrees.back()->GetListOfLeaves();
    TLeaf *leaf = 0;
    while( (leaf = (TLeaf*)next()) ) {
      TString leaftype = leaf->GetTypeName();
      if( leaftype.BeginsWith("vector") ) continue;
      //std::cout << leaf->GetName() << " = " << leaf->GetValue() << std::endl;
      (*outputCsv) << "," << leaf->GetValue();
    }
    (*outputCsv) << std::endl;

    // FIXME
    //doTruthMatching();
    
  }

  void doTruthMatching() {

    std::map<UInt_t,GenParticle*> mp;

    report::debug( "nParticles = %i" , br_part->GetEntriesFast() );
    
    for( std::size_t ipart = 0 ; ipart < br_part->GetEntriesFast() ; ++ipart ) {
      GenParticle *p = (GenParticle*) br_part->At(ipart);
      report::debug( "Particle%-3i UniqueID = %-3i , Status = %-3i , PID = %-3i , M1 = %-3i , M2 = %-3i" , int(ipart) , int(p->GetUniqueID()) , p->Status , p->PID , p->M1 , p->M2 );
      mp[ipart] = p;
    }
    

    assert( false );
    return;
    
    for( std::size_t ijet = 0 ; ijet < good_jets_ids.size() ; ijet++ ) {

      Jet *jet = (Jet*) br_jet->At(good_jets_ids[ijet]);
      report::debug( "Jet[%i] NParticles=%i NConstituents=%i" , int(ijet) , jet->Particles.GetSize() , jet->Constituents.GetSize() );

      /*
      for( Int_t ipart = 0 ; ipart < jet->Particles.GetSize() ; ipart++ ) {
	GenParticle *p = (GenParticle*) jet->Particles.At(ipart);
	std::cout << "  Particle = " << p << std::endl;
	// FAILS because not all particles are saved...
	report::blank( "   GenParticle[%i] Status=%i PID=%i" , ipart , p->Status , p->PID );
      }
      */
      
    }

    
  }
  
  void saveOutputTrees( const TString &fname ) {

    for( std::size_t i = 0 ; i < outputTrees.size() ; i++ ) {
      TString fullfname = TString::Format("output/%s_%s.root",fname.Data(),outputTrees[i]->GetName());
      report::debug( "Saving output tree %s to file %s" , outputTrees[i]->GetName() , fullfname.Data() );
      TFile *fout = new TFile( fullfname , "recreate" );
      outputTrees[i]->SetDirectory( fout );
      outputTrees[i]->Write( "tthAnaOutput" );
      delete outputTrees[i];
      fout->Close();
      delete fout;
    }

    outputCsv->close();
    
    /* Old code to produce a single file with a different tree name for each process
    TFile *fout = new TFile( fname , "recreate" );
    for( std::size_t i = 0 ; i < outputTrees.size() ; i++ ) {
      outputTrees[i]->SetDirectory( fout );
      outputTrees[i]->Write( outputTrees[i]->GetName() );
      delete outputTrees[i];
    }
    fout->Close();
    delete fout;
    */

  }
  
  void setTree( TTree *_tree ) {

    UInt_t fUniqueID[50];
    _tree->SetBranchStatus( "Particle.fUniqueID" , 1 );
    _tree->SetBranchAddress( "Particle.fUniqueID" , &fUniqueID );

    _tree->GetEntry( 0 );
    //std::cout << "fUniqueID[0]  = " << fUniqueID[0] << std::endl;
    //std::cout << "fUniqueID[10] = " << fUniqueID[10] << std::endl;

    /*
    TIter next = _tree->GetListOfLeaves();
    TLeaf *leaf = 0;
    while( (leaf = (TLeaf*)next()) ) {
      TString leafname = leaf->GetName();
      TString leaftitle = leaf->GetTitle();
      TString leaftype = leaf->GetTypeName();
      report::debug( "name=%s title=%s type=%s" , leafname.Data() , leaftitle.Data() , leaftype.Data() );
    }
    assert( false );
    */
    

    ex = new ExRootTreeReader( _tree );

    br_el   = ex->UseBranch( "Electron" );
    br_mu   = ex->UseBranch( "Muon" );
    br_jet  = ex->UseBranch( "Jet" );
    br_met  = ex->UseBranch( "MissingET" );
    br_part = ex->UseBranch( "Particle" );

    // Observables for objects passing the object selection criteria
    setBranch_uint( "good_nbtags_1" , kFALSE );
    setBranch_uint( "good_nbtags_2" , kFALSE );
    setBranch_uint( "good_nbtags_3" , kFALSE );
    setBranch_uint( "good_nbtags_4" , kFALSE );
    setBranch_uint( "good_nbtags_5" , kFALSE );
    setBranch_vector_float( "good_jet_pt" , kFALSE );
    setBranch_vector_float( "good_jet_eta" , kFALSE );
    setBranch_vector_float( "good_jet_phi" , kFALSE );
    setBranch_vector_float( "good_jet_mass" , kFALSE );
    setBranch_vector_int( "good_jet_btag" , kFALSE );
    setBranch_vector_int( "good_jet_flavor" , kFALSE );
    setBranch_vector_float( "good_el_pt" , kFALSE );
    setBranch_vector_float( "good_el_eta" , kFALSE );
    setBranch_vector_float( "good_el_phi" , kFALSE );
    setBranch_vector_float( "good_mu_pt" , kFALSE );
    setBranch_vector_float( "good_mu_eta" , kFALSE );
    setBranch_vector_float( "good_mu_phi" , kFALSE );

    // Variables from MVAVariables tool
    // Common
    setBranch_int( "nJets" , kFALSE );
    setBranch_int( "nLeptons" , kFALSE );
    setBranch_int( "nJetsAbovePt25" , kFALSE );
    setBranch_int( "nJetsAbovePt30" , kFALSE );
    setBranch_int( "nJetsAbovePt35" , kFALSE );
    setBranch_int( "nJetsAbovePt40" , kFALSE );
    setBranch_float( "HT_all" , kFALSE );
    setBranch_float( "HT_had" , kFALSE );
    setBranch_float( "Centrality" , kFALSE );
    setBranch_float( "MHiggs" , kFALSE );
    setBranch_float( "NHiggs_30" , kFALSE );

    // fox wolfram moments
    setBranch_float( "H1_all" , kFALSE );
    setBranch_float( "H2_all" , kFALSE );
    setBranch_float( "H3_all" , kFALSE );
    setBranch_float( "H4_all" , kFALSE );
    setBranch_float( "H5_all" , kFALSE );
    setBranch_float( "H1transverse_all" , kFALSE );
    setBranch_float( "H2transverse_all" , kFALSE );
    setBranch_float( "H3transverse_all" , kFALSE );
    setBranch_float( "H4transverse_all" , kFALSE );
    setBranch_float( "H5transverse_all" , kFALSE );

    // Thrust
    setBranch_float( "Thrust_all" , kFALSE );
    setBranch_float( "ThrustAxisX_all" , kFALSE );
    setBranch_float( "ThrustAxisY_all" , kFALSE );
    setBranch_float( "ThrustAxisZ_all" , kFALSE );

    // MET
    setBranch_float( "pT_met" , kFALSE );
    setBranch_float( "eta_met" , kFALSE );
    setBranch_float( "phi_met" , kFALSE );
    
    // Jet kinematics
    for( Int_t ij = 0 ; ij < 10 ; ij++ ) {
      TString itag = TString::Format( "jet%i" , ij );
      setBranch_float( "pT_"+itag , kFALSE );
      setBranch_float( "eta_"+itag , kFALSE );
      setBranch_float( "phi_"+itag , kFALSE );
      setBranch_float( "M_"+itag , kFALSE );
      setBranch_int( "bTagLevel_"+itag , kFALSE );
    }

    for( Int_t ij = 0 ; ij < 5 ; ij++ ) {
      TString itag = TString::Format( "jet%i" , ij );
      setBranch_float( "pT_b"+itag , kFALSE );
      setBranch_float( "eta_b"+itag , kFALSE );
      setBranch_float( "phi_b"+itag , kFALSE );
      setBranch_float( "M_b"+itag , kFALSE );
    }
    
    // Lepton kinematics
    setBranch_float( "pT_lepton0" , kFALSE );
    setBranch_float( "eta_lepton0" , kFALSE );
    setBranch_float( "phi_lepton0" , kFALSE );
    setBranch_int( "flavor_lepton0" , kFALSE );
    setBranch_float( "pT_lepton1" , kFALSE );
    setBranch_float( "eta_lepton1" , kFALSE );
    setBranch_float( "phi_lepton1" , kFALSE );
    setBranch_int( "flavor_lepton1" , kFALSE );

    // Two-object systems
    for( variableIter iv = mva_variable_names.begin() , fv = mva_variable_names.end() ; iv != fv ; ++iv ) {
      for( pairingIter ip = mva_pairing_names.begin() , fp = mva_pairing_names.end() ; ip != fp ; ++ip ) {
	TString itag = ip->second + "_" + iv->second;
	setBranch_float( "M"+itag , kFALSE );
	setBranch_float( "Pt"+itag , kFALSE );
	setBranch_float( "PtSum"+itag , kFALSE );
	setBranch_float( "dR"+itag , kFALSE );
	setBranch_float( "dPhi"+itag , kFALSE );
	setBranch_float( "dEta"+itag , kFALSE );
      }
      setBranch_float( "Mjjj_"+iv->second , kFALSE );
      setBranch_float( "Ptjjj_"+iv->second , kFALSE );
    }

    // DIL
    setBranch_float( "DileptonMass" , kFALSE );
    setBranch_float( "DileptonPt" , kFALSE );
    setBranch_float( "DileptonSumPt" , kFALSE );
    setBranch_float( "DileptondR" , kFALSE );
    setBranch_float( "DileptondPhi" , kFALSE );
    setBranch_float( "DileptondEta" , kFALSE );

    // Collection features
    for( collectionIter ic = mva_collection_names.begin() , fc = mva_collection_names.end() ; ic != fc ; ++ic ) {
      setBranch_float( "Aplanarity_"+ic->second , kFALSE );
      setBranch_float( "Aplanority_"+ic->second , kFALSE );
      setBranch_float( "Sphericity_"+ic->second , kFALSE );
      setBranch_float( "Spherocity_"+ic->second , kFALSE );
      setBranch_float( "SphericityT_"+ic->second , kFALSE );
      setBranch_float( "Planarity_"+ic->second , kFALSE );
      setBranch_float( "Variable_C_"+ic->second , kFALSE );
      setBranch_float( "Variable_D_"+ic->second , kFALSE );
      setBranch_float( "Circularity_"+ic->second , kFALSE );
      setBranch_float( "PlanarFlow_"+ic->second , kFALSE );
    }

    // L+jets
    setBranch_float( "dRlepbb_MindR" , kFALSE );

    // Lepton channels
    setBranch_char( "noSel" , kFALSE );
    setBranch_char( "ee" , kFALSE );
    setBranch_char( "uu" , kFALSE );
    setBranch_char( "eu" , kFALSE );
    setBranch_char( "ll" , kFALSE );
    setBranch_char( "e" , kFALSE );
    setBranch_char( "u" , kFALSE );
    setBranch_char( "l" , kFALSE );
    setBranch_char( "combinedSel" , kFALSE );

  }


  
  Bool_t getEntry( const Long64_t &ientry ) {

    ex->ReadEntry( ientry );

    //report::debug( "ientry = %i" , ientry );
    //if( ientry > 100 ) assert( false );
    //return kFALSE;

    map_uint["good_nbtags_1"] = 0;
    map_uint["good_nbtags_2"] = 0;
    map_uint["good_nbtags_3"] = 0;
    map_uint["good_nbtags_4"] = 0;
    map_uint["good_nbtags_5"] = 0;

    m_event.clear();
    good_jets.clear();
    good_jets_ids.clear();
    good_el.clear();
    good_mu.clear();
    good_el_id.clear();
    good_mu_id.clear();
    map_vector_float["good_jet_pt"]->clear();
    map_vector_float["good_jet_eta"]->clear();
    map_vector_float["good_jet_phi"]->clear();
    map_vector_float["good_jet_mass"]->clear();
    map_vector_int["good_jet_flavor"]->clear();
    map_vector_int["good_jet_btag"]->clear();
    map_vector_float["good_el_pt"]->clear();
    map_vector_float["good_el_eta"]->clear();
    map_vector_float["good_el_phi"]->clear();
    map_vector_float["good_mu_pt"]->clear();
    map_vector_float["good_mu_eta"]->clear();
    map_vector_float["good_mu_phi"]->clear();


    //
    // Jet selection
    //
    for( std::size_t i = 0 ; i < br_jet->GetEntriesFast() ; ++i ) {

      Jet *jet = (Jet*) br_jet->At(i);

      if( (jet->PT * GeV) < 20 ) continue;
      if( TMath::Abs(jet->Eta) > 2.5 ) continue;

      TLorentzVector vjet;
      vjet.SetPtEtaPhiM( jet->PT * MeV , jet->Eta , jet->Phi , jet->Mass * MeV );

      map_vector_float["good_jet_pt"]->push_back( jet->PT * MeV );
      map_vector_float["good_jet_eta"]->push_back( jet->Eta );
      map_vector_float["good_jet_phi"]->push_back( jet->Phi );
      map_vector_float["good_jet_mass"]->push_back( jet->Mass * MeV );
      map_vector_int["good_jet_flavor"]->push_back( jet->Flavor );

      Int_t tagLevel = getTagLevel( jet->PT*GeV , jet->Flavor );
      map_vector_int["good_jet_btag"]->push_back( tagLevel );

      if( tagLevel > 0 ) {
	map_uint["good_nbtags_1"]++;
	if( tagLevel > 1 ) {
	  map_uint["good_nbtags_2"]++;
	  if( tagLevel > 2 ) {
	    map_uint["good_nbtags_3"]++;
	    if( tagLevel > 3 ) {
	      map_uint["good_nbtags_4"]++;
	      if( tagLevel > 4 ) {
		map_uint["good_nbtags_5"]++;
	      }
	    }
	  }
	}
      }
      
      good_jets.push_back( vjet );
      good_jets_ids.push_back( i );

      /*jet->BTag*/
      m_event.m_jets.push_back( new xAOD::Jet(vjet,tagLevel>0?10:-10,tagLevel) );

    }

    if( good_jets.size() < minNjets ) return kFALSE;
    if( map_uint["good_nbtags_1"] < minNbtags ) return kFALSE;

    //
    // Electron selection
    //
    for( std::size_t i = 0 ; i < br_el->GetEntriesFast() ; ++i ) {

      Electron *el = (Electron*) br_el->At(i);

      if( (el->PT * GeV) < 20 ) continue;
      if( TMath::Abs(el->Eta) > 2.5 ) continue;

      TLorentzVector vel;
      vel.SetPtEtaPhiM( el->PT * MeV , el->Eta , el->Phi , 0.0 );

      map_vector_float["good_el_pt"]->push_back( el->PT * MeV );
      map_vector_float["good_el_eta"]->push_back( el->Eta );
      map_vector_float["good_el_phi"]->push_back( el->Phi );
      good_el.push_back( vel );
      good_el_id.push_back( i );

      m_event.m_electrons.push_back( new xAOD::Electron(vel) );

    }

    //
    // Muon selection
    //
    for( std::size_t i = 0 ; i < br_mu->GetEntriesFast() ; ++i ) {

      Muon *mu = (Muon*) br_mu->At(i);

      if( (mu->PT * GeV) < 20 ) continue;
      if( TMath::Abs(mu->Eta) > 2.5 ) continue;

      TLorentzVector vmu;
      vmu.SetPtEtaPhiM( mu->PT * MeV , mu->Eta , mu->Phi , 0.0 );

      map_vector_float["good_mu_pt"]->push_back( mu->PT * MeV );
      map_vector_float["good_mu_eta"]->push_back( mu->Eta );
      map_vector_float["good_mu_phi"]->push_back( mu->Phi );
      good_mu.push_back( vmu );
      good_mu_id.push_back( i );

      m_event.m_muons.push_back( new xAOD::Muon(vmu) );

    }

    // Channels
    map_char["noSel"]	    = 1;
    map_char["ee"]	    = good_el.size()==2 && good_mu.size()==0 ? 1 : 0;
    map_char["uu"]	    = good_el.size()==0 && good_mu.size()==2 ? 1 : 0;
    map_char["eu"]	    = good_el.size()==1 && good_mu.size()==1 ? 1 : 0;
    map_char["ll"]	    = (good_el.size()+good_mu.size())==2 ? 1 : 0;
    map_char["e"]	    = good_el.size()==1 && good_mu.size()==0 ? 1 : 0;
    map_char["u"]	    = good_el.size()==0 && good_mu.size()==1 ? 1 : 0;
    map_char["l"]	    = (good_el.size()+good_mu.size())==1 ? 1 : 0;
    map_char["anyl"]        = (good_el.size()+good_mu.size())>0 ? 1 : 0;
    map_char["dil"]         = map_char["ll"] && good_jets.size() >= 4 && map_uint["good_nbtags_1"] >= 3;
    map_char["ljet"]        = map_char["l"] && good_jets.size() >= 6 && map_uint["good_nbtags_1"] >= 3;
    map_char["combinedSel"] = map_char["dil"] || map_char["ljet"];
    if( !map_char[selectionTag] ) return kFALSE;
    
    MissingET *met = (MissingET*) br_met->At(0);
    m_event.m_met->setP4( met->MET * MeV , met->Eta , met->Phi , 0 );
    map_float["pT_met"]	 = met->MET * MeV;
    map_float["eta_met"] = met->Eta;
    map_float["phi_met"] = met->Phi;

    std::size_t nlep = good_el.size() + good_mu.size();
    std::size_t njet = good_jets.size();

    map_int["nLeptons"] = nlep;
    
    //
    // Use MVAVariables to generate more training observables
    //
    
    MVAVariables *m_mva = new MVAVariables();
    m_mva->initialise( m_event );

    TLorentzVector vleadingLep;
    const xAOD::IParticle *leadingLep = m_mva->getLeadingPtLepton();
    vleadingLep.SetPtEtaPhiE( leadingLep->pt() , leadingLep->eta() , leadingLep->phi() , leadingLep->e() );
    PairedSystem *ps_lepbb_MindR = new PairedSystem( m_mva->getEntry(pairing::bb,variable::MindR) , vleadingLep );
    
    // Common
    map_int["nJets"]	      = m_mva->nJets();

    map_int["nJetsAbovePt25"] = m_mva->nJetsAbovePt(25);
    map_int["nJetsAbovePt30"] = m_mva->nJetsAbovePt(30);
    map_int["nJetsAbovePt35"] = m_mva->nJetsAbovePt(35);
    map_int["nJetsAbovePt40"] = m_mva->nJetsAbovePt(40);
    map_int["nBTags"]	      = m_mva->nbTag();
    map_float["HT_all"]	      = m_mva->HT(collection::all);
    map_float["HT_had"]	      = m_mva->HT(collection::jets);
    map_float["Centrality"]   = m_mva->Centrality(collection::all);
    map_float["MHiggs"]	      = m_mva->higgsCandidateMass();
    map_float["NHiggs_30"]    = m_mva->nHiggsCandidatesMassWindow(pairing::bb,30);
    

    // Fox Wolfram Moments
    map_float["H1_all"] = m_mva->FirstFoxWolframMoment(collection::all);
    map_float["H2_all"] = m_mva->SecondFoxWolframMoment(collection::all);
    map_float["H3_all"] = m_mva->ThirdFoxWolframMoment(collection::all);
    map_float["H4_all"] = m_mva->FourthFoxWolframMoment(collection::all);
    map_float["H5_all"] = m_mva->FifthFoxWolframMoment(collection::all);
    map_float["H1transverse_all"] = m_mva->FirstFoxWolframTransverseMoment(collection::all);
    map_float["H2transverse_all"] = m_mva->SecondFoxWolframTransverseMoment(collection::all);
    map_float["H3transverse_all"] = m_mva->ThirdFoxWolframTransverseMoment(collection::all);
    map_float["H4transverse_all"] = m_mva->FourthFoxWolframTransverseMoment(collection::all);
    map_float["H5transverse_all"] = m_mva->FifthFoxWolframTransverseMoment(collection::all);

    // Thrust
    map_float["Thrust_all"]	 = njet + nlep > 0 ? m_mva->getThrust(collection::all) : 0;
    map_float["ThrustAxisX_all"] = njet + nlep > 0 ? m_mva->getThrustAxis(collection::all).X() : 0;
    map_float["ThrustAxisY_all"] = njet + nlep > 0 ? m_mva->getThrustAxis(collection::all).Y() : 0;
    map_float["ThrustAxisZ_all"] = njet + nlep > 0 ? m_mva->getThrustAxis(collection::all).Z() : 0;

    // Jet kinematics
    for( Int_t ij = 0 ; ij < 10 ; ij++ ) {
      TString itag = TString::Format( "jet%i" , ij );
      if( njet > ij ) {
	map_float["pT_"+itag]	   = m_mva->getPtOrderedJet(ij)->pt();
	map_float["eta_"+itag]	   = m_mva->getPtOrderedJet(ij)->eta();
	map_float["phi_"+itag]	   = m_mva->getPtOrderedJet(ij)->phi();
	map_float["M_"+itag]	   = m_mva->getPtOrderedJet(ij)->m();
	map_int["bTagLevel_"+itag] = m_mva->getPtOrderedJet(ij)->getBTagLevel();
      } else {
	map_float["pT_"+itag]	   = -1;
	map_float["eta_"+itag]	   = -10;
	map_float["phi_"+itag]	   = -5;
	map_float["M_"+itag]	   = -1;
	map_int["bTagLevel_"+itag] = -1;
      }
    }

    for( Int_t ij = 0 ; ij < 5 ; ij++ ) {
      TString itag = TString::Format( "jet%i" , ij );
      if( map_uint["good_nbtags_1"] > ij ) {
	map_float["pT_b"+itag]  = m_mva->getPtOrdered_bJet(ij)->pt();
	map_float["eta_b"+itag] = m_mva->getPtOrdered_bJet(ij)->eta();
	map_float["phi_b"+itag] = m_mva->getPtOrdered_bJet(ij)->phi();
	map_float["M_b"+itag]   = m_mva->getPtOrdered_bJet(ij)->m();
      } else {
	map_float["pT_b"+itag]  = -1;
	map_float["eta_b"+itag] = -10;
	map_float["phi_b"+itag] = -5;
	map_float["M_b"+itag]   = -1;
      }
    }

    // Lepton kinematics
    if( nlep > 0 ) {
      map_float["pT_lepton0"]   = m_mva->getLeadingPtLepton()->pt();
      map_float["eta_lepton0"]  = m_mva->getLeadingPtLepton()->eta();
      map_float["phi_lepton0"]  = m_mva->getLeadingPtLepton()->phi();
      map_int["flavor_lepton0"] = m_mva->getLeadingPtLepton()->flavor();
    } else {
      map_float["pT_lepton0"]   = -1;
      map_float["eta_lepton0"]  = -10;
      map_float["phi_lepton0"]  = -5;
      map_int["flavor_lepton0"] = -1;
    }

    if( nlep > 1 ) {
      map_float["pT_lepton1"]   = m_mva->getSubleadingPtLepton()->pt();
      map_float["eta_lepton1"]  = m_mva->getSubleadingPtLepton()->eta();
      map_float["phi_lepton1"]  = m_mva->getSubleadingPtLepton()->phi();
      map_int["flavor_lepton1"] = m_mva->getSubleadingPtLepton()->flavor();
    } else {
      map_float["pT_lepton1"]   = -1;
      map_float["eta_lepton1"]  = -10;
      map_float["phi_lepton1"]  = -5;
      map_int["flavor_lepton1"] = -1;
    }
    
    // Composite objects
    for( variableIter iv = mva_variable_names.begin() , fv = mva_variable_names.end() ; iv != fv ; ++iv ) {
      for( pairingIter ip = mva_pairing_names.begin() , fp = mva_pairing_names.end() ; ip != fp ; ++ip ) {
	TString itag = ip->second + "_" + iv->second;
	map_float["M"+itag] = m_mva->MassofPair( ip->first , iv->first );
	map_float["Pt"+itag] = m_mva->PtofPair( ip->first , iv->first );
	map_float["PtSum"+itag] = m_mva->PtSumofPair( ip->first , iv->first );
	map_float["dR"+itag] = m_mva->deltaRofPair( ip->first , iv->first );
	map_float["dPhi"+itag] = m_mva->deltaPhiofPair( ip->first , iv->first );
	map_float["dEta"+itag] = m_mva->deltaEtaofPair( ip->first , iv->first );
      }
      map_float["Mjjj_"+iv->second] = m_mva->MassofJetTriplet( iv->first );
      map_float["Ptjjj_"+iv->second] = m_mva->PtofJetTriplet( iv->first );
    }

    // DIL
    if( nlep > 1 ) {
      map_float["DileptonMass"]	 = m_mva->DileptonMass();
      map_float["DileptonPt"]	 = m_mva->DileptonPt();
      map_float["DileptonSumPt"] = m_mva->DileptonSumPt();
      map_float["DileptondR"]	 = m_mva->DileptondR();
      map_float["DileptondPhi"]	 = m_mva->DileptondPhi();
      map_float["DileptondEta"]	 = m_mva->DileptondEta();
    } else {
      map_float["DileptonMass"]	 = -1;
      map_float["DileptonPt"]	 = -1;
      map_float["DileptonSumPt"] = -1;
      map_float["DileptondR"]	 = -1;
      map_float["DileptondPhi"]	 = -5;
      map_float["DileptondEta"]	 = -10;
    }
    
    // Collection features
    for( collectionIter ic = mva_collection_names.begin() , fc = mva_collection_names.end() ; ic != fc ; ++ic ) {
      map_float["Aplanarity_"+ic->second] = m_mva->Aplanarity( ic->first );
      map_float["Aplanority_"+ic->second] = m_mva->Aplanority( ic->first );
      map_float["Sphericity_"+ic->second] = m_mva->Sphericity( ic->first );
      map_float["Spherocity_"+ic->second] = m_mva->Spherocity( ic->first );
      map_float["SphericityT_"+ic->second] = m_mva->SphericityT( ic->first );
      map_float["Planarity_"+ic->second] = m_mva->Planarity( ic->first );
      map_float["Variable_C_"+ic->second] = m_mva->Variable_C( ic->first );
      map_float["Variable_D_"+ic->second] = m_mva->Variable_D( ic->first );
      map_float["Circularity_"+ic->second] = m_mva->Circularity( ic->first );
      map_float["PlanarFlow_"+ic->second] = m_mva->PlanarFlow( ic->first );
    }

    // L+jets
    map_float["dRlepbb_MindR"] = ps_lepbb_MindR->DeltaR();
    
    delete ps_lepbb_MindR;
    delete m_mva;

    return kTRUE;
  }


  Bool_t passesSelection() { return map_char[selectionTag]; }


  TString getSelectionTitle() {
    TString jetseltag = TString::Format( ", #geq%ij, #geq%ib" , minNjets , minNbtags );
    if( selectionTag==TString("ljet") ) return "Single Lepton, #geq6j, #geq3b";
    else if( selectionTag==TString("dil") ) return "DIL, #geq4j, #geq3b";
    else if( selectionTag==TString("noSel") ) return "No lepton selection" + jetseltag;
    else if( selectionTag==TString("combinedSel") ) return "Combined signal regions";
    return selectionTag + jetseltag;
  }



  Double_t leadingLeptonPt() {
    if( good_el.size()==0 && good_mu.size()==0 ) return -666;
    if( good_el.size()==0 ) return good_mu[0].Pt();
    if( good_mu.size()==0 ) return good_el[0].Pt();
    return good_el[0].Pt() > good_mu[0].Pt() ? good_el[0].Pt() : good_mu[0].Pt();
  }

};

#endif
