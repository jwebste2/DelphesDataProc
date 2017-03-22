#ifndef _INPUTCONFIG_H_
#define _INPUTCONFIG_H_

#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <map>

#include "TString.h"
#include "TSystem.h"
#include "TLegend.h"

#include "Report.h"
#include "HistTools.h"
#include "PhysicsProcess.h"
#include "Config.h"


typedef std::map<TString,Double_t>::iterator it_lumiblocks;

class
InputConfig
{

private:

  TString databasedir;
  Double_t lumi;

  Bool_t sort_backgrounds;
  
  std::vector<PhysicsProcess> signals;
  std::vector<PhysicsProcess> backgrounds;
  std::vector<PhysicsProcess> data;

  std::vector<TString> blacklist;
  
  std::map<TString,Double_t> lumiblocks; // lumis in pb here taken from lumi calculator

public:

  InputConfig( TString _databasedir = "null" )
    : databasedir( _databasedir )
    , lumi( 0.0 )
    , sort_backgrounds( kTRUE )
  {

    // expand the base directory for input data
    gSystem->ExpandPathName(databasedir);

    // FIXME if you need to specify luminosity from lumi blocks
    lumiblocks["RunNumber"] = -666.0;
    
  }
  ~InputConfig() {}

  TString getDataBaseDir() { return databasedir; }
  std::size_t numSignals() { return signals.size(); }
  std::size_t numBackgrounds() { return backgrounds.size(); }
  std::size_t numData() { return data.size(); }
  std::size_t numProcesses() { return signals.size() + backgrounds.size() + data.size(); }
  PhysicsProcess getSignal( std::size_t i ) const { return signals[i]; }
  PhysicsProcess getBackground( std::size_t i ) const { return backgrounds[i]; }
  PhysicsProcess getData( std::size_t i = 0 ) const { return data[i]; }
  PhysicsProcess getProcess( std::size_t i = 0 ) const {
    if( i < signals.size() ) return signals[i];
    if( i < signals.size() + backgrounds.size() ) return backgrounds[i-signals.size()];
    return data[i-signals.size()-backgrounds.size()];
  }

  Bool_t getSortBackgrounds() { return sort_backgrounds; }

  void setSortBackgrounds( Bool_t v ) { sort_backgrounds = v; }
  void addSignalProcess( PhysicsProcess p ) { signals.push_back(p); }
  void addBackgroundProcess( PhysicsProcess p ) { backgrounds.push_back(p); }
   void addDataProcess( PhysicsProcess p ) { data.push_back(p); }

  void addSignalProcess( TString _name , TString _roottitle , TString _latextitle , Int_t _color ) { signals.push_back( PhysicsProcess(_name,_roottitle,_latextitle,_color) ); }
  void addBackgroundProcess( TString _name , TString _roottitle , TString _latextitle , Int_t _color ) { backgrounds.push_back( PhysicsProcess(_name,_roottitle,_latextitle,_color) ); }
  void addDataProcess( TString _name , TString _roottitle , TString _latextitle ) { data.push_back( PhysicsProcess(_name,_roottitle,_latextitle) ); }

  void addSignalSample( TString _tag , Double_t _xsec , Double_t _kfactor ) { signals.back().addSample(_tag,_xsec,_kfactor); }
  void addBackgroundSample( TString _tag , Double_t _xsec , Double_t _kfactor ) { backgrounds.back().addSample(_tag,_xsec,_kfactor); }
  void addDataSample( TString _tag ) { data.back().addSample(_tag); }

  void addToBlacklist( TString _tag ) { blacklist.push_back(_tag); }
  void setBlacklist( std::vector<TString> v ) { blacklist = v; }
  
  void setLuminosity( Double_t _lumi ) { lumi = _lumi; }
  void setLuminosityFromDataTags() {
    assert( data.size() == 1 );
    lumi = 0.0;
    for( std::size_t i = 0 ; i < data[0].numSamples() ; i++ ) {
      // skip the blacklisted datasets
      if( std::find(blacklist.begin(),blacklist.end(),data[0].getSampleTag(i)) != blacklist.end() ) {
	//report::warn( "skipping blacklisted %s in lumi calculation" , data[0].getSampleTag(i).Data() );
	continue;
      }
      // loop over luminosity blocks until we find the one that matches this sample tag
      for( it_lumiblocks j = lumiblocks.begin() ; j != lumiblocks.end() ; j++ ) {
	if( data[0].getSampleTag(i).Contains(j->first) ) {
	  lumi += j->second;
	  break;
	}
      }
      //lumi += lumiblocks[data[0].getSampleTag(i)] / 1000.0;
    }
    report::info( "calculated Lumi = %g for InputConfig %s" , lumi , databasedir.Data() );
  }


  Double_t getLuminosity() { return lumi; }

  
  //
  // Fancy load function for tth analysis
  // Looks for directories with the correct tag containing "output.root" files
  //
  void load() {

    // get list of data directories in this base path
    std::vector<TString> datadirs;
    {
      void* dirp = gSystem->OpenDirectory(databasedir);
      
      const char* entry;
      Int_t n = 0;
      TString tmpdir;
      
      while( (entry = (char*)gSystem->GetDirEntry(dirp)) ) {
	tmpdir = entry;
	if( tmpdir.Contains("user.") ) datadirs.push_back( tmpdir );
      }

      delete entry;
    }

    for( std::size_t i = 0 ; i < signals.size() ; i++ ) { signals[i].cleanBlacklisted(blacklist); signals[i].loadAsSignal(databasedir,datadirs,lumi); }
    for( std::size_t i = 0 ; i < backgrounds.size() ; i++ ) { backgrounds[i].cleanBlacklisted(blacklist); backgrounds[i].loadAsBackground(databasedir,datadirs,lumi); }
    for( std::size_t i = 0 ; i < data.size() ; i++ ) { data[i].cleanBlacklisted(blacklist); data[i].loadAsData(databasedir,datadirs); }

  }

 

  //
  // Simple load function that just loads using filenames
  //
  void loadRootFiles() {
    for( std::size_t i = 0 ; i < signals.size() ; i++ ) signals[i].loadAsSignal(lumi);
    for( std::size_t i = 0 ; i < backgrounds.size() ; i++ ) backgrounds[i].loadAsBackground(lumi);
    for( std::size_t i = 0 ; i < data.size() ; i++ ) data[i].loadAsData();
  }

  
  void dumpSamples() {
    for( PhysicsProcess proc : signals ) proc.dumpSamples( databasedir );
    for( PhysicsProcess proc : backgrounds ) proc.dumpSamples( databasedir );
    for( PhysicsProcess proc : data ) proc.dumpSamples( databasedir , kFALSE );
  }

};

#endif

