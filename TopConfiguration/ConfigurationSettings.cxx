#include "ConfigurationSettings.h"

namespace top {

  ConfigurationSettings* ConfigurationSettings::m_instance = 0;

  ConfigurationSettings* ConfigurationSettings::get() {
    if( !m_instance ) m_instance = new ConfigurationSettings();
    return m_instance;
  }
  
};
