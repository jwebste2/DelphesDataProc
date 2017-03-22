#ifndef _DELPHESROOTTRUTHSELECTOR_H_
#define _DELPHESROOTTRUTHSELECTOR_H_

#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdlib.h>
#include <vector>

#include <TTree.h>
#include <TMath.h>
#include <TLorentzVector.h>

#include "Report.h"
#include "Particle.h"
#include "TopDecay.h"

class DelphesRootTruthSelector
{

private:
  // variables
  
  // vectors for keeping track of classified truth particles
  std::vector<TruthParticle*> v_all;
  std::vector<TruthParticle*> v_jets;
  std::vector<TruthParticle*> v_bjets;
  std::vector<TruthParticle*> v_el;
  std::vector<TruthParticle*> v_mu;

  // Readers for the Delphes trees
  TTree *tree;
  int truth_n;
  std::vector<int> *truth_status;
  std::vector<int> *truth_pdgId;
  std::vector<double> *truth_px;
  std::vector<double> *truth_py;
  std::vector<double> *truth_eta;
  std::vector<double> *truth_phi;
  std::vector<int> *truth_m1;
  std::vector<int> *truth_m2;
  

  // Variables to keep track of the most recent event looks in
  // terms of the decay chain
  int decayt;
  int decaytbar;
  int decayWp;
  int decayWm;
  
private:
  // functions
  
  void cleanup() {

    // Remove all truth particle currently in the record
    for( std::size_t i = 0 ; i < v_all.size() ; i++ ) delete v_all[i];
    v_all.clear();
    v_jets.clear();
    v_bjets.clear();
    v_el.clear();
    v_mu.clear();

    // Also reset the decay chain indicators
    decayt    = topdecay::UNDEFINED;
    decaytbar = topdecay::UNDEFINED;
    decayWp   = topdecay::UNDEFINED;
    decayWm   = topdecay::UNDEFINED;

  }

public:

  DelphesRootTruthSelector( TTree* _tree )
    : tree( _tree )
  {
    // Set up branches
    truth_status = new std::vector<int>();
    truth_pdgId	 = new std::vector<int>();
    truth_px	 = new std::vector<double>();
    truth_py	 = new std::vector<double>();
    truth_eta	 = new std::vector<double>();
    truth_phi	 = new std::vector<double>();
    truth_m1	 = new std::vector<int>();
    truth_m2	 = new std::vector<int>();
    tree->SetBranchAddress( "truth_n" , &truth_n );
    tree->SetBranchAddress( "truth_status" , &truth_status );
    tree->SetBranchAddress( "truth_pdgId" , &truth_pdgId );
    tree->SetBranchAddress( "truth_px" , &truth_px );
    tree->SetBranchAddress( "truth_py" , &truth_py );
    tree->SetBranchAddress( "truth_eta" , &truth_eta );
    tree->SetBranchAddress( "truth_phi" , &truth_phi );
    tree->SetBranchAddress( "truth_m1" , &truth_m1 );
    tree->SetBranchAddress( "truth_m2" , &truth_m2 );
  }

  ~DelphesRootTruthSelector() { cleanup(); }

  
  //
  // Basic return functions
  //
  int getDecayT() { return decayt; }
  int getDecayTbar() { return decaytbar; }
  int getDecayWp() { return decayWp; }
  int getDecayWm() { return decayWm; }

  std::size_t getN() { return v_all.size(); }
  std::size_t getNjets() { return v_jets.size(); }
  std::size_t getNbjets() { return v_bjets.size(); }
  std::size_t getNel() { return v_el.size(); }
  std::size_t getNmu() { return v_mu.size(); }
  std::size_t getNlep() { return v_el.size() + v_mu.size(); }
  
  std::vector<TruthParticle*> getAll() { return v_all; }
  std::vector<TruthParticle*> getJets() { return v_jets; }
  std::vector<TruthParticle*> getBJets() { return v_bjets; }
  std::vector<TruthParticle*> getMu() { return v_mu; }
  std::vector<TruthParticle*> getEl() { return v_el; }
  
  //
  // Real nuts & bolts function used to process each event and
  // create a truth record that we can use to match reconstructed
  // objects
  //
  Bool_t processTruthRecord() {

    // ex->ReadEntry(iev) is already called in DelphesTtbar.cpp
    
    // Make sure we clear vectors and reset indicators from previous event
    cleanup();

    // Loop over particles in the truth record that is in the input tree
    for( std::size_t ipart = 0 ; ipart < truth_n ; ipart++ ) {

      // Only look at particles with status indicating that they are prior to parton shower / hadronization
      if( truth_status->at(ipart) != 3 ) break;

      // Only look at single mother particles, i.e. the top and W decay products
      if( truth_m1->at(ipart) <= 0 ) continue;
      if( truth_m2->at(ipart) >  0 ) continue;

      // Variables for some useful information in the record
      Double_t px  = truth_px->at(ipart);
      Double_t py  = truth_py->at(ipart);
      Double_t pt  = TMath::Sqrt( (px*px) + (py*py) );
      Double_t eta = truth_eta->at(ipart);
      Double_t phi = truth_phi->at(ipart);
      int pdg	   = truth_pdgId->at(ipart);
      int m1	   = truth_m1->at(ipart) - 1;
      int m1m1     = truth_m1->at(m1) - 1;
      int m1m2     = truth_m2->at(m1) - 1;
      int pdgm1	   = truth_pdgId->at(m1);

      // Check if this is the...
      // prompt q from the t decay
      if( (pdg==5 || pdg==3 || pdg==1) && pdgm1==6 ) {
	assert( decayt == topdecay::UNDEFINED );
	v_all.push_back( new TruthParticle( pt , eta , phi , 0.0 , pdg , pdgm1 ) );
	v_jets.push_back( v_all.back() );
	if( pdg==5 ) v_bjets.push_back( v_all.back() );
	decayt = pdg==5 ? topdecay::WB : topdecay::WLIGHT;
      }
      
      // Check if this is the...
      // prompt qbar from the tbar decay
      else if( (pdg==-5 || pdg==-3 || pdg==-1) && pdgm1==-6 ) {
	assert( decaytbar == topdecay::UNDEFINED );
	v_all.push_back( new TruthParticle( pt , eta , phi , 0.0 , pdg , pdgm1 ) );
	v_jets.push_back( v_all.back() );
	if( pdg==-5 ) v_bjets.push_back( v_all.back() );
	decaytbar = pdg==-5 ? topdecay::WB : topdecay::WLIGHT;
      }

      // Check if this is the...
      // lepton or jet coming from t-->W+
      else if( pdgm1 == 24 && m1m1 >= 0 && m1m2 < 0 ) {
	if( truth_pdgId->at(m1m1) == 6 ) {
	  if( TMath::Abs(pdg) <= 6 ) decayWp = topdecay::JETS;
	  // don't bother saving neutrinos
	  else if( TMath::Abs(pdg) % 2 == 0 ) continue;
	  else if( TMath::Abs(pdg) == 11 ) decayWp = topdecay::ELNU;
	  else if( TMath::Abs(pdg) == 13 ) decayWp = topdecay::MUNU;
	  else if( TMath::Abs(pdg) == 15 ) decayWp = topdecay::TAUNU;
	  else assert( false );
	  v_all.push_back( new TruthParticle( pt , eta , phi , 0.0 , pdg , pdgm1 ) );
	  if( decayWp == topdecay::JETS ) v_jets.push_back( v_all.back() );
	  else if( decayWp == topdecay::ELNU ) v_el.push_back( v_all.back() );
	  else if( decayWp == topdecay::MUNU ) v_mu.push_back( v_all.back() );
	}
      }

      // Check if this is the....
      // lepton or jet coming from tbar-->W-
      else if( pdgm1 == -24 && m1m1 >= 0 && m1m2 < 0 ) {
	if( truth_pdgId->at(m1m1) == -6 ) {
	  if( TMath::Abs(pdg) <= 6 ) decayWm = topdecay::JETS;
	  // don't bother saving neutrinos
	  else if( TMath::Abs(pdg) % 2 == 0 ) continue;
	  else if( TMath::Abs(pdg) == 11 ) decayWm = topdecay::ELNU;
	  else if( TMath::Abs(pdg) == 13 ) decayWm = topdecay::MUNU;
	  else if( TMath::Abs(pdg) == 15 ) decayWm = topdecay::TAUNU;
	  else assert( false );
	  v_all.push_back( new TruthParticle( pt , eta , phi , 0.0 , pdg , pdgm1 ) );
	  if( decayWm == topdecay::JETS ) v_jets.push_back( v_all.back() );
	  else if( decayWm == topdecay::ELNU ) v_el.push_back( v_all.back() );
	  else if( decayWm == topdecay::MUNU ) v_mu.push_back( v_all.back() );
	}
      }

      // Otherwise ignore the particle
      
    }

    //
    // Some sanity checks to make sure we didn't do
    // something stupid :)
    //
    if( decayt == topdecay::UNDEFINED ) { report::error( "Missing q from t decay" ); }
    if( decaytbar == topdecay::UNDEFINED ) { report::error( "Missing qbar from tbar decay" ); }
    assert( decayWp != topdecay::UNDEFINED );
    assert( decayWm != topdecay::UNDEFINED );
    assert( v_el.size() <= 2 );
    assert( v_mu.size() <= 2 );
    assert( v_all.size() <= 6 );

    // Success!
    return kTRUE;
    
  }

  
  // return true if the truth record passes user-defined event selection
  Bool_t passesSelection( TString sel ) {
    if( sel==TString("ljet") ) {
      if( decayWp!=topdecay::ELNU && decayWp!=topdecay::MUNU && decayWm!=topdecay::ELNU && decayWm!=topdecay::MUNU ) return kFALSE;
      if( decayWp!=topdecay::JETS && decayWm!=topdecay::JETS ) return kFALSE;
    } else if( sel==TString("jets") ) {
      if( decayWp != topdecay::JETS || decayWm != topdecay::JETS ) return kFALSE;
    } else if( sel==TString("dil") ) {
      if( decayWp==topdecay::JETS || decayWp==topdecay::TAUNU || decayWm==topdecay::JETS || decayWm==topdecay::TAUNU ) return kFALSE;
    }
    return kTRUE;
  }
  
};

#endif
