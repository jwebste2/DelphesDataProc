//
// This is a dummy class that I use so I can compile
// some TTHbbLeptonic code into my analysis scripts
// without the compiler complaining about some missing
// #include files
//

#ifndef _EVENT_H_
#define _EVENT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <assert.h>

#include <TLorentzVector.h>

namespace xAOD {

  class IParticle {
  private:
    TLorentzVector pmom;
    Int_t _flavor;
  public:
    IParticle( const Int_t &f = -1 )
      : _flavor(f)
    {}
    IParticle( const TLorentzVector &v , const Int_t &f = -1 )
      : pmom(v)
      , _flavor(f)
    {}
    ~IParticle() {}
    
    void setPtEtaPhiM( const Double_t &pt , const Double_t &eta , const Double_t &phi , const Double_t &m ) { pmom.SetPtEtaPhiM( pt , eta , phi , m ); }
    void setP4( const Double_t &pt , const Double_t &eta , const Double_t &phi , const Double_t &m ) { pmom.SetPtEtaPhiM( pt , eta , phi , m ); }
    void setP4( const TLorentzVector& v ) { pmom = v; }
    void makePrivateStore() { /* do something here? */ }
    void setAttribute( const std::string &attr , const double &val ) {
      if( attr=="pt" ) pmom.SetPtEtaPhiM( val , pmom.Eta() , pmom.Phi() , pmom.M() );
      else if( attr=="eta" ) pmom.SetPtEtaPhiM( pmom.Pt() , val , pmom.Phi() , pmom.M() );
      else if( attr=="phi" ) pmom.SetPhi( val );
      else if( attr=="m" ) pmom.SetPtEtaPhiM( pmom.Pt() , pmom.Eta() , pmom.Phi() , val );
      else {
	std::cout << std::endl << "Trying to set attribute " << attr << std::endl;
	assert( false );
      }
    }
    Double_t pt() const { return pmom.Pt(); }
    Double_t px() const { return pmom.Px(); }
    Double_t py() const { return pmom.Py(); }
    Double_t pz() const { return pmom.Pz(); }
    Double_t eta() const { return pmom.Eta(); }
    Double_t phi() const { return pmom.Phi(); }
    Double_t m() const { return pmom.M(); }
    Double_t e() const { return pmom.E(); }
    TLorentzVector p4() const { return pmom; }
    Int_t flavor() const { return _flavor; }
  };
  

  class MissingET : public IParticle {
  public:
    MissingET() : IParticle() {}
    MissingET( const TLorentzVector &v ) : IParticle(v) {}
    ~MissingET() {}
    Double_t met() const { return pt(); }
    Double_t mpx() const { return px(); }
    Double_t mpy() const { return py(); }
  };

  
  class Jet : public IParticle {
  private:
    struct BTagging {
      double mv2c20;
      BTagging( const double &_mv2c20 = -10 ) : mv2c20(_mv2c20) {}
      bool MVx_discriminant( const std::string &algo , double &mv2 ) { mv2 = mv2c20; return true; }
    };
    BTagging *btag;
    Int_t bTagLevel;
  public:
    Jet()
      : IParticle()
      , btag(new BTagging())
      , bTagLevel(-1)
    {}
    Jet( const TLorentzVector &v , const double &mv2c20 , const Int_t &taglevel = -1 )
      : IParticle( v )
      , btag( new BTagging(mv2c20) )
      , bTagLevel( taglevel )
    {}
    ~Jet() { delete btag; }
    BTagging* btagging() const { return btag; }
    Int_t getBTagLevel() const { return bTagLevel; }
  };

  
  class Electron : public IParticle {
  public:
    Electron() : IParticle(1) {}
    Electron( const TLorentzVector &v )
      : IParticle(v,1)
    {}
    ~Electron() {}
  };

  
  class Muon : public IParticle {
  public:
    Muon() : IParticle(2) {}
    Muon( const TLorentzVector &v )
      : IParticle(v,2)
    {}
    ~Muon() {}
  };

  
  typedef std::vector<const xAOD::Jet*> JetContainer;
  typedef std::vector<const xAOD::Electron*> ElectronContainer;
  typedef std::vector<const xAOD::Muon*> MuonContainer;
  
};



namespace top {

  class Event {
  public:

    Event()
      : m_met( new xAOD::MissingET() )
    {}

    ~Event() {
      if( m_met ) delete m_met;
      clear();
    }

    xAOD::MissingET *m_met;
    xAOD::JetContainer m_jets;
    xAOD::JetContainer m_trackJets;
    xAOD::ElectronContainer m_electrons;
    xAOD::MuonContainer m_muons;

    void clear() {
      for( std::size_t i = 0 ; i < m_jets.size() ; i++ ) delete m_jets[i];
      for( std::size_t i = 0 ; i < m_electrons.size() ; i++ ) delete m_electrons[i];
      for( std::size_t i = 0 ; i < m_muons.size() ; i++ ) delete m_muons[i];
      m_jets.clear();
      m_electrons.clear();
      m_muons.clear();
    }
    
  };

};



#endif


