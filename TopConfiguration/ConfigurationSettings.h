//
// This is a dummy class that I use so I can compile
// some TTHbbLeptonic code into my analysis scripts
// without the compiler complaining about some missing
// #include files
//

#ifndef _CONFIGURATIONSETTINGS_H_
#define _CONFIGURATIONSETTINGS_H_

#include <vector>
#include <map>
#include <string>
#include <assert.h>

#include <TString.h>

namespace top {

  class ConfigurationSettings {

  private:
    static ConfigurationSettings *m_instance;

  public:
    ConfigurationSettings() {}
    ~ConfigurationSettings() {}
    //if( m_instance ) delete m_instance;

    const std::string value( const TString &s ) const {
      if( s==TString("ttH_btagAlgo") ) return "MV2c20";
      if( s==TString("ttH_btagCut") ) return "-0.4434";
      assert( false );
    }
    static ConfigurationSettings* get();
    //{
    //  if( !m_instance ) m_instance = new ConfigurationSettings();
    //  return m_instance;
    //}
  };
 
};


#endif
