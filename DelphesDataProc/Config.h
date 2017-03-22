#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <map>

#include "TString.h"

namespace
config
{

  const TString XsecFilePath( "input/XSection-MC15-13TeV-fromSusyGrp.data" ); // Updated 2016.03.22

  // Pileup weights currently destroy statistics because the <mu> distribution in MC is not a
  // good representation of the data (... so there are many events with PU weight = 0). For studies
  // that require good statistics we can turn them off here:
  const Bool_t UsePileupWeights ( kFALSE );

  // Currently only impact the weighting for PhysicsChain objects
  const Bool_t UseZjetsReweighting ( kTRUE );

  // Path to directory where we want to save output root files that will be loaded by TtHFitter
  const TString FitterInputDir( "/atlasfs/atlas/local/jwebster/tth/reprocessed/20170127/FitterInput" );

};

#endif
