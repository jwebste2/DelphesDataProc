#ifndef _TTH_H_
#define _TTH_H_

#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <map>

#include "TString.h"

#include "InputConfig.h"
#include "PhysicsProcess.h"


namespace
tth
{

  //
  // Units
  //
  
  const Double_t GeV   = 0.001;
  const Double_t ifb   = 1.e3;
  const Double_t Zmass = 91187.6;


  //
  // Input configurations
  //
  
  std::map<TString,InputConfig> configs;
  InputConfig input;
  
  static void loadInput( TString tag ) {

    //
    // Define the different background contributions and the data
    //

    //
    // Data
    //

    PhysicsProcess proc_data_50ns( "Data_50ns" , "Data (50 ns)" , "Data (50 ns)" );
    proc_data_50ns.addSample( "267073" );
    proc_data_50ns.addSample( "267167" );
    proc_data_50ns.addSample( "267360" );
    proc_data_50ns.addSample( "267367" );
    proc_data_50ns.addSample( "267599" );
    proc_data_50ns.addSample( "267638" );
    proc_data_50ns.addSample( "267639" );
    proc_data_50ns.addSample( "270806" );
    proc_data_50ns.addSample( "270953" );
    proc_data_50ns.addSample( "271048" );
    proc_data_50ns.addSample( "271298" );
    proc_data_50ns.addSample( "271421" );
    proc_data_50ns.addSample( "271516" );
    proc_data_50ns.addSample( "271595" );
    proc_data_50ns.addSample( "271744" );
    
    PhysicsProcess proc_data_25ns( "Data_25ns" , "Data (25 ns)" , "Data (25 ns)" );
    proc_data_25ns.addSample( "276262" );
    proc_data_25ns.addSample( "276329" );
    proc_data_25ns.addSample( "276336" );
    proc_data_25ns.addSample( "276416" );
    proc_data_25ns.addSample( "276511" );
    proc_data_25ns.addSample( "276689" );
    proc_data_25ns.addSample( "276731" );
    proc_data_25ns.addSample( "276778" );
    proc_data_25ns.addSample( "276790" );
    proc_data_25ns.addSample( "276952" );
    proc_data_25ns.addSample( "276954" );
    proc_data_25ns.addSample( "278880" );
    proc_data_25ns.addSample( "278912" );
    proc_data_25ns.addSample( "278968" ); 
    proc_data_25ns.addSample( "279169" );
    proc_data_25ns.addSample( "279259" );
    proc_data_25ns.addSample( "279279" );
    proc_data_25ns.addSample( "279284" );
    proc_data_25ns.addSample( "279345" );
    proc_data_25ns.addSample( "279515" );
    proc_data_25ns.addSample( "279598" );
    proc_data_25ns.addSample( "279685" );
    proc_data_25ns.addSample( "279764" );
    proc_data_25ns.addSample( "279813" );
    proc_data_25ns.addSample( "279867" );
    proc_data_25ns.addSample( "279928" );
    proc_data_25ns.addSample( "279932" );
    proc_data_25ns.addSample( "279984" );
    proc_data_25ns.addSample( "280231" );
    proc_data_25ns.addSample( "280319" );
    proc_data_25ns.addSample( "280368" );
    proc_data_25ns.addSample( "280423" );
    proc_data_25ns.addSample( "280464" );
    proc_data_25ns.addSample( "280500" );
    proc_data_25ns.addSample( "280520" );
    proc_data_25ns.addSample( "280614" );

    PhysicsProcess proc_cutflow_data( "cutflow_data" , "" , "" , 0 );
    proc_cutflow_data.addSample( "FULLDATA" );

    PhysicsProcess proc_cutflow_data_xmas( "cutflow_data" , "" , "" , 0 );
    proc_cutflow_data_xmas.addSample( "276262" );
    proc_cutflow_data_xmas.addSample( "276329" );
    proc_cutflow_data_xmas.addSample( "276336" );
    proc_cutflow_data_xmas.addSample( "276416" );
    proc_cutflow_data_xmas.addSample( "276511" );
    proc_cutflow_data_xmas.addSample( "276689" );
    proc_cutflow_data_xmas.addSample( "276778" );
    proc_cutflow_data_xmas.addSample( "276790" );
    proc_cutflow_data_xmas.addSample( "276952" );
    proc_cutflow_data_xmas.addSample( "276954" );
    proc_cutflow_data_xmas.addSample( "278880" );
    proc_cutflow_data_xmas.addSample( "278912" );
    proc_cutflow_data_xmas.addSample( "278968" );
    proc_cutflow_data_xmas.addSample( "279169" );
    proc_cutflow_data_xmas.addSample( "279259" );
    proc_cutflow_data_xmas.addSample( "279279" );
    proc_cutflow_data_xmas.addSample( "279284" );
    proc_cutflow_data_xmas.addSample( "279345" );
    proc_cutflow_data_xmas.addSample( "279515" );
    proc_cutflow_data_xmas.addSample( "279598" );
    proc_cutflow_data_xmas.addSample( "279685" );
    proc_cutflow_data_xmas.addSample( "279764" );
    proc_cutflow_data_xmas.addSample( "279813" );
    proc_cutflow_data_xmas.addSample( "279867" );
    proc_cutflow_data_xmas.addSample( "279928" );
    proc_cutflow_data_xmas.addSample( "279932" );
    proc_cutflow_data_xmas.addSample( "279984" );
    proc_cutflow_data_xmas.addSample( "280231" );
    proc_cutflow_data_xmas.addSample( "280319" );
    proc_cutflow_data_xmas.addSample( "280368" );
    proc_cutflow_data_xmas.addSample( "280423" );
    proc_cutflow_data_xmas.addSample( "280464" );
    proc_cutflow_data_xmas.addSample( "280500" );
    proc_cutflow_data_xmas.addSample( "280520" );
    proc_cutflow_data_xmas.addSample( "280614" );
    proc_cutflow_data_xmas.addSample( "280673" );
    proc_cutflow_data_xmas.addSample( "280753" );
    proc_cutflow_data_xmas.addSample( "280853" );
    proc_cutflow_data_xmas.addSample( "280862" );
    proc_cutflow_data_xmas.addSample( "280950" );
    proc_cutflow_data_xmas.addSample( "280977" );
    proc_cutflow_data_xmas.addSample( "281070" );
    proc_cutflow_data_xmas.addSample( "281074" );
    proc_cutflow_data_xmas.addSample( "281075" );
    proc_cutflow_data_xmas.addSample( "281317" );
    proc_cutflow_data_xmas.addSample( "281385" );
    proc_cutflow_data_xmas.addSample( "281411" );
    proc_cutflow_data_xmas.addSample( "282625" );
    proc_cutflow_data_xmas.addSample( "282631" );
    proc_cutflow_data_xmas.addSample( "282712" );
    proc_cutflow_data_xmas.addSample( "282784" );
    proc_cutflow_data_xmas.addSample( "282992" );
    proc_cutflow_data_xmas.addSample( "283074" );
    proc_cutflow_data_xmas.addSample( "283155" );
    proc_cutflow_data_xmas.addSample( "283270" );
    proc_cutflow_data_xmas.addSample( "283429" );
    proc_cutflow_data_xmas.addSample( "283608" );
    proc_cutflow_data_xmas.addSample( "283780" );
    proc_cutflow_data_xmas.addSample( "284006" );
    proc_cutflow_data_xmas.addSample( "284154" );
    proc_cutflow_data_xmas.addSample( "284213" );
    proc_cutflow_data_xmas.addSample( "284285" );
    proc_cutflow_data_xmas.addSample( "284420" );
    proc_cutflow_data_xmas.addSample( "284427" );
    proc_cutflow_data_xmas.addSample( "284484" );
    
    
    //
    // Signal
    //

    PhysicsProcess proc_tth_mcanlo_25ns( "tth_mcanlo_25ns" , "t#bar{t}H (MC@NLO, 25ns)" , "$t\\bar{t}H$ (MC@NLO, 25ns)" , kRed+100 );
    proc_tth_mcanlo_25ns.addSampleFindInfo( "341177" );
    proc_tth_mcanlo_25ns.addSampleFindInfo( "341270" );
    proc_tth_mcanlo_25ns.addSampleFindInfo( "341271" );

    PhysicsProcess proc_cutflow_tth( "cutflow_tth" , "" , "" , 0 );
    proc_cutflow_tth.addSampleFindInfo( "341177" );


    //
    // Single Top
    //

    PhysicsProcess proc_singletop_powheg_25ns( "singletop_powheg_25ns" , "single top (Powheg, 25ns)" , "single top (Powheg, 25ns)" , 0 );
    proc_singletop_powheg_25ns.addSampleFindInfo( "410011" );
    proc_singletop_powheg_25ns.addSampleFindInfo( "410012" );
    proc_singletop_powheg_25ns.addSampleFindInfo( "410013" );
    proc_singletop_powheg_25ns.addSampleFindInfo( "410014" );

    PhysicsProcess proc_singletop_cutflow( "singletop_cutflow" , "single top" , "single top" , 0 );
    proc_singletop_cutflow.addSampleFindInfo( "410015" );
    proc_singletop_cutflow.addSampleFindInfo( "410016" );

    PhysicsProcess proc_ttV_cutflow( "ttV_cutflow" , "ttV/tZ" , "ttV/tZ" , 0 );
    proc_ttV_cutflow.addSampleFindInfo( "410066" );
    proc_ttV_cutflow.addSampleFindInfo( "410067" );
    proc_ttV_cutflow.addSampleFindInfo( "410068" );
    proc_ttV_cutflow.addSampleFindInfo( "410073" );
    proc_ttV_cutflow.addSampleFindInfo( "410074" );
    proc_ttV_cutflow.addSampleFindInfo( "410075" );
    proc_ttV_cutflow.addSampleFindInfo( "410111" );
    proc_ttV_cutflow.addSampleFindInfo( "410112" );
    proc_ttV_cutflow.addSampleFindInfo( "410113" );
    proc_ttV_cutflow.addSampleFindInfo( "410114" );
    proc_ttV_cutflow.addSampleFindInfo( "410115" );
    proc_ttV_cutflow.addSampleFindInfo( "410116" );
    proc_ttV_cutflow.addSampleFindInfo( "410050" );
    
    //
    // TTbar
    //

    PhysicsProcess proc_ttbar_powheg_25ns( "ttbar_powheg_25ns" , "t#bar{t} (Powheg, 25ns)" , "$t\\bar{t}$ (Powheg, 25ns)" , 2 );
    proc_ttbar_powheg_25ns.addSampleFindInfo( "410000" );

    PhysicsProcess proc_ttbar_sherpa_25ns( "ttbar_sherpa_25ns" , "t#bar{t} (Sherpa, 25ns)" , "$t\\bar{t}$ (Sherpa, 25ns)" , 2 );
    proc_ttbar_sherpa_25ns.addSampleFindInfo( "410021" );
    proc_ttbar_sherpa_25ns.addSampleFindInfo( "410022" );
    proc_ttbar_sherpa_25ns.addSampleFindInfo( "410023" );
    proc_ttbar_sherpa_25ns.addSampleFindInfo( "410024" );

    PhysicsProcess proc_ttbar_cutflow( "ttbar_cutflow" , "t#bar{t}" , "$t\\bar{t}$" , 2 );
    proc_ttbar_cutflow.addSampleFindInfo( "410000" );
    proc_ttbar_cutflow.addSampleFindInfo( "410120" );

    PhysicsProcess proc_ttbar_cc( "ttbar_cc" , "t#bar{t}+cc" , "$t\\bar{t}+cc$" , 2 );
    proc_ttbar_cc.addSampleFindInfo( "410000.cc" );

    PhysicsProcess proc_ttbar_light( "ttbar_light" , "t#bar{t}+light" , "$t\\bar{t}+$light" , 3 );
    proc_ttbar_light.addSampleFindInfo( "410000.light" );

    PhysicsProcess proc_ttbar_bb( "ttbar_bb" , "t#bar{t}+bb" , "$t\\bar{t}+bb$" , 4 );
    proc_ttbar_bb.addSampleFindInfo( "410000.bb" );
    
    //
    // Z+jets
    //
    
    PhysicsProcess proc_zjets_powheg_25ns( "zjets_powheg_25ns" , "Z+jets (Powheg, 25ns)" , "$Z$+jets (Powheg, 25ns)" , 4 );
    proc_zjets_powheg_25ns.addSampleFindInfo( "361106" );
    proc_zjets_powheg_25ns.addSampleFindInfo( "361107" );
    proc_zjets_powheg_25ns.addSampleFindInfo( "361108" );

    PhysicsProcess proc_zjets_sherpa_25ns( "zjets_sherpa_25ns" , "Z+jets (Sherpa, 25ns)" , "$Z$+jets (Sherpa, 25ns)" , 4 );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361372" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361373" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361374" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361375" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361376" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361377" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361378" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361379" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361380" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361381" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361382" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361383" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361384" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361385" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361386" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361387" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361388" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361389" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361390" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361391" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361392" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361393" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361394" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361395" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361396" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361397" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361398" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361399" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361400" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361401" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361402" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361403" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361404" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361405" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361406" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361407" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361408" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361409" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361410" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361411" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361412" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361413" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361414" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361415" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361416" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361417" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361418" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361419" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361420" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361421" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361422" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361423" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361424" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361425" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361426" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361427" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361428" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361429" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361430" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361431" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361432" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361433" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361434" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361435" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361436" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361437" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361438" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361439" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361440" );
    proc_zjets_sherpa_25ns.addSampleFindInfo( "361441" );

    PhysicsProcess proc_zjets_sherpa_lm_25ns( "zjets_sherpa_lm_25ns" , "Z+jets LM (Sherpa, 25ns)" , "$Z$+jets LM (Sherpa, 25ns)" , 6 );
    proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361468" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361469" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361470" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361471" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361472" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361473" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361474" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361475" );
    proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361476" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361477" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361478" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361479" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361480" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361481" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361482" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361483" );
    proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361484" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361485" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361486" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361487" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361488" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361489" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361490" );
    //proc_zjets_sherpa_lm_25ns.addSampleFindInfo( "361491" );

    
    
    // Sample is still 50ns, hence the current titles
    PhysicsProcess proc_zjets_madgraph_25ns( "zjets_madgraph_25ns" , "Z+jets (MadGraph, 50ns)" , "$Z$+jets (MadGraph, 50ns)" , 4 );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361500" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361501" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361502" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361503" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361504" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361505" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361506" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361507" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361508" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361509" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361510" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361511" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361512" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361513" );
    proc_zjets_madgraph_25ns.addSampleFindInfo( "361514" );

    PhysicsProcess proc_zjets_cutflow( "zjets_cutflow" , "Z+jets" , "$Z$+jets" , 4 );
    proc_zjets_cutflow.addSampleFindInfo( "361372" );
    proc_zjets_cutflow.addSampleFindInfo( "361373" );
    proc_zjets_cutflow.addSampleFindInfo( "361374" );
    proc_zjets_cutflow.addSampleFindInfo( "361375" );
    proc_zjets_cutflow.addSampleFindInfo( "361376" );
    proc_zjets_cutflow.addSampleFindInfo( "361377" );
    proc_zjets_cutflow.addSampleFindInfo( "361378" );
    proc_zjets_cutflow.addSampleFindInfo( "361379" );
    proc_zjets_cutflow.addSampleFindInfo( "361380" );
    proc_zjets_cutflow.addSampleFindInfo( "361381" );
    proc_zjets_cutflow.addSampleFindInfo( "361382" );
    proc_zjets_cutflow.addSampleFindInfo( "361383" );
    proc_zjets_cutflow.addSampleFindInfo( "361384" );
    proc_zjets_cutflow.addSampleFindInfo( "361385" );
    proc_zjets_cutflow.addSampleFindInfo( "361386" );
    proc_zjets_cutflow.addSampleFindInfo( "361387" );
    proc_zjets_cutflow.addSampleFindInfo( "361388" );
    proc_zjets_cutflow.addSampleFindInfo( "361389" );
    proc_zjets_cutflow.addSampleFindInfo( "361390" );
    proc_zjets_cutflow.addSampleFindInfo( "361391" );
    proc_zjets_cutflow.addSampleFindInfo( "361392" );
    proc_zjets_cutflow.addSampleFindInfo( "361393" );
    proc_zjets_cutflow.addSampleFindInfo( "361394" );
    proc_zjets_cutflow.addSampleFindInfo( "361395" );
    proc_zjets_cutflow.addSampleFindInfo( "361396" );
    proc_zjets_cutflow.addSampleFindInfo( "361397" );
    proc_zjets_cutflow.addSampleFindInfo( "361398" );
    proc_zjets_cutflow.addSampleFindInfo( "361399" );
    proc_zjets_cutflow.addSampleFindInfo( "361400" );
    proc_zjets_cutflow.addSampleFindInfo( "361401" );
    proc_zjets_cutflow.addSampleFindInfo( "361402" );
    //proc_zjets_cutflow.addSampleFindInfo( "361402.Sherpa.DAOD_TOPQ1.e3651_s2586_s2174_r6869_r6282_p2454" );
    //proc_zjets_cutflow.addSampleFindInfo( "361402.Sherpa.DAOD_TOPQ1.e3651_s2608_s2183_r6869_r6282_p2454" );
    proc_zjets_cutflow.addSampleFindInfo( "361403" );
    proc_zjets_cutflow.addSampleFindInfo( "361404" );
    proc_zjets_cutflow.addSampleFindInfo( "361405" );
    proc_zjets_cutflow.addSampleFindInfo( "361406" );
    proc_zjets_cutflow.addSampleFindInfo( "361407" );
    proc_zjets_cutflow.addSampleFindInfo( "361408" );
    proc_zjets_cutflow.addSampleFindInfo( "361409" );
    proc_zjets_cutflow.addSampleFindInfo( "361410" );
    proc_zjets_cutflow.addSampleFindInfo( "361411" );
    proc_zjets_cutflow.addSampleFindInfo( "361412" );
    proc_zjets_cutflow.addSampleFindInfo( "361413" );
    proc_zjets_cutflow.addSampleFindInfo( "361414" );
    proc_zjets_cutflow.addSampleFindInfo( "361415" );
    proc_zjets_cutflow.addSampleFindInfo( "361416" );
    proc_zjets_cutflow.addSampleFindInfo( "361417" );
    proc_zjets_cutflow.addSampleFindInfo( "361418" );
    proc_zjets_cutflow.addSampleFindInfo( "361419" );
    proc_zjets_cutflow.addSampleFindInfo( "361420" );
    proc_zjets_cutflow.addSampleFindInfo( "361421" );
    proc_zjets_cutflow.addSampleFindInfo( "361422" );
    proc_zjets_cutflow.addSampleFindInfo( "361423" );
    proc_zjets_cutflow.addSampleFindInfo( "361424" );
    proc_zjets_cutflow.addSampleFindInfo( "361425" );
    proc_zjets_cutflow.addSampleFindInfo( "361426" );
    proc_zjets_cutflow.addSampleFindInfo( "361427" );
    proc_zjets_cutflow.addSampleFindInfo( "361428" );
    proc_zjets_cutflow.addSampleFindInfo( "361429" );
    proc_zjets_cutflow.addSampleFindInfo( "361430" );
    proc_zjets_cutflow.addSampleFindInfo( "361431" );
    proc_zjets_cutflow.addSampleFindInfo( "361432" );
    proc_zjets_cutflow.addSampleFindInfo( "361433" );
    proc_zjets_cutflow.addSampleFindInfo( "361434" );
    proc_zjets_cutflow.addSampleFindInfo( "361435" );
    proc_zjets_cutflow.addSampleFindInfo( "361436" );
    proc_zjets_cutflow.addSampleFindInfo( "361437" );
    proc_zjets_cutflow.addSampleFindInfo( "361438" );
    proc_zjets_cutflow.addSampleFindInfo( "361439" );
    proc_zjets_cutflow.addSampleFindInfo( "361440" );
    proc_zjets_cutflow.addSampleFindInfo( "361441" );
    proc_zjets_cutflow.addSampleFindInfo( "361442" );
    proc_zjets_cutflow.addSampleFindInfo( "361443" );
    proc_zjets_cutflow.addSampleFindInfo( "361468" );
    proc_zjets_cutflow.addSampleFindInfo( "361469" );
    proc_zjets_cutflow.addSampleFindInfo( "361470" );
    proc_zjets_cutflow.addSampleFindInfo( "361471" );
    proc_zjets_cutflow.addSampleFindInfo( "361472" );
    proc_zjets_cutflow.addSampleFindInfo( "361473" );
    proc_zjets_cutflow.addSampleFindInfo( "361474" );
    proc_zjets_cutflow.addSampleFindInfo( "361475" );
    proc_zjets_cutflow.addSampleFindInfo( "361476" );
    proc_zjets_cutflow.addSampleFindInfo( "361477" );
    proc_zjets_cutflow.addSampleFindInfo( "361478" );
    proc_zjets_cutflow.addSampleFindInfo( "361479" );
    proc_zjets_cutflow.addSampleFindInfo( "361480" );
    proc_zjets_cutflow.addSampleFindInfo( "361481" );
    proc_zjets_cutflow.addSampleFindInfo( "361482" );
    proc_zjets_cutflow.addSampleFindInfo( "361483" );
    proc_zjets_cutflow.addSampleFindInfo( "361484" );
    proc_zjets_cutflow.addSampleFindInfo( "361485" );
    proc_zjets_cutflow.addSampleFindInfo( "361486" );
    proc_zjets_cutflow.addSampleFindInfo( "361487" );
    proc_zjets_cutflow.addSampleFindInfo( "361488" );
    proc_zjets_cutflow.addSampleFindInfo( "361489" );
    proc_zjets_cutflow.addSampleFindInfo( "361490" );
    proc_zjets_cutflow.addSampleFindInfo( "361491" );

    
    //
    // W+jets
    //
    
    PhysicsProcess proc_wjets_powheg_25ns( "wjets_powheg_25ns" , "W+jets (Powheg, 25ns)" , "$W$+jets (Powheg, 25ns)" , 5 );
    proc_wjets_powheg_25ns.addSampleFindInfo( "361100" );
    proc_wjets_powheg_25ns.addSampleFindInfo( "361101" );
    proc_wjets_powheg_25ns.addSampleFindInfo( "361102" );
    proc_wjets_powheg_25ns.addSampleFindInfo( "361103" );
    proc_wjets_powheg_25ns.addSampleFindInfo( "361104" );
    proc_wjets_powheg_25ns.addSampleFindInfo( "361105" );

    PhysicsProcess proc_wjets_sherpa_25ns( "wjets_sherpa_25ns" , "W+jets (Sherpa, 25ns)" , "$W$+jets (Sherpa, 25ns)" , 5 );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361300" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361301" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361302" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361303" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361304" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361305" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361306" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361307" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361308" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361309" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361310" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361311" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361312" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361313" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361314" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361315" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361316" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361317" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361318" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361319" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361320" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361321" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361322" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361323" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361324" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361325" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361326" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361327" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361328" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361329" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361330" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361331" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361332" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361333" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361334" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361335" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361336" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361337" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361338" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361339" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361340" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361341" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361342" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361343" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361344" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361345" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361346" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361347" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361348" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361349" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361350" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361351" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361352" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361353" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361354" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361355" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361356" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361357" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361358" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361359" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361360" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361361" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361362" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361363" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361364" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361365" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361366" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361367" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361368" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361369" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361370" );
    proc_wjets_sherpa_25ns.addSampleFindInfo( "361371" );

    
    PhysicsProcess proc_wjets_cutflow( "wjets_cutflow" , "W+jets" , "$W$+jets" , 5 );
    proc_wjets_cutflow.addSampleFindInfo( "361300" );
    proc_wjets_cutflow.addSampleFindInfo( "361301" );
    proc_wjets_cutflow.addSampleFindInfo( "361302" );
    proc_wjets_cutflow.addSampleFindInfo( "361303" );
    proc_wjets_cutflow.addSampleFindInfo( "361304" );
    proc_wjets_cutflow.addSampleFindInfo( "361305" );
    proc_wjets_cutflow.addSampleFindInfo( "361306" );
    proc_wjets_cutflow.addSampleFindInfo( "361307" );
    proc_wjets_cutflow.addSampleFindInfo( "361308" );
    proc_wjets_cutflow.addSampleFindInfo( "361309" );
    proc_wjets_cutflow.addSampleFindInfo( "361310" );
    proc_wjets_cutflow.addSampleFindInfo( "361311" );
    proc_wjets_cutflow.addSampleFindInfo( "361312" );
    proc_wjets_cutflow.addSampleFindInfo( "361313" );
    proc_wjets_cutflow.addSampleFindInfo( "361314" );
    proc_wjets_cutflow.addSampleFindInfo( "361315" );
    proc_wjets_cutflow.addSampleFindInfo( "361316" );
    proc_wjets_cutflow.addSampleFindInfo( "361317" );
    proc_wjets_cutflow.addSampleFindInfo( "361318" );
    proc_wjets_cutflow.addSampleFindInfo( "361319" );
    proc_wjets_cutflow.addSampleFindInfo( "361320" );
    proc_wjets_cutflow.addSampleFindInfo( "361321" );
    proc_wjets_cutflow.addSampleFindInfo( "361322" );
    proc_wjets_cutflow.addSampleFindInfo( "361323" );
    proc_wjets_cutflow.addSampleFindInfo( "361324" );
    proc_wjets_cutflow.addSampleFindInfo( "361325" );
    proc_wjets_cutflow.addSampleFindInfo( "361326" );
    proc_wjets_cutflow.addSampleFindInfo( "361327" );
    proc_wjets_cutflow.addSampleFindInfo( "361328" );
    proc_wjets_cutflow.addSampleFindInfo( "361329" );
    proc_wjets_cutflow.addSampleFindInfo( "361330" );
    proc_wjets_cutflow.addSampleFindInfo( "361331" );
    proc_wjets_cutflow.addSampleFindInfo( "361332" );
    proc_wjets_cutflow.addSampleFindInfo( "361333" );
    proc_wjets_cutflow.addSampleFindInfo( "361334" );
    proc_wjets_cutflow.addSampleFindInfo( "361335" );
    proc_wjets_cutflow.addSampleFindInfo( "361336" );
    proc_wjets_cutflow.addSampleFindInfo( "361337" );
    proc_wjets_cutflow.addSampleFindInfo( "361338" );
    proc_wjets_cutflow.addSampleFindInfo( "361339" );
    proc_wjets_cutflow.addSampleFindInfo( "361340" );
    proc_wjets_cutflow.addSampleFindInfo( "361341" );
    proc_wjets_cutflow.addSampleFindInfo( "361342" );
    proc_wjets_cutflow.addSampleFindInfo( "361343" );
    proc_wjets_cutflow.addSampleFindInfo( "361344" );
    proc_wjets_cutflow.addSampleFindInfo( "361345" );
    proc_wjets_cutflow.addSampleFindInfo( "361346" );
    proc_wjets_cutflow.addSampleFindInfo( "361347" );
    proc_wjets_cutflow.addSampleFindInfo( "361348" );
    proc_wjets_cutflow.addSampleFindInfo( "361349" );
    proc_wjets_cutflow.addSampleFindInfo( "361350" );
    proc_wjets_cutflow.addSampleFindInfo( "361351" );
    proc_wjets_cutflow.addSampleFindInfo( "361352" );
    proc_wjets_cutflow.addSampleFindInfo( "361353" );
    proc_wjets_cutflow.addSampleFindInfo( "361354" );
    proc_wjets_cutflow.addSampleFindInfo( "361355" );
    proc_wjets_cutflow.addSampleFindInfo( "361356" );
    proc_wjets_cutflow.addSampleFindInfo( "361357" );
    proc_wjets_cutflow.addSampleFindInfo( "361358" );
    proc_wjets_cutflow.addSampleFindInfo( "361359" );
    proc_wjets_cutflow.addSampleFindInfo( "361360" );
    proc_wjets_cutflow.addSampleFindInfo( "361361" );
    proc_wjets_cutflow.addSampleFindInfo( "361362" );
    proc_wjets_cutflow.addSampleFindInfo( "361363" );
    proc_wjets_cutflow.addSampleFindInfo( "361364" );
    proc_wjets_cutflow.addSampleFindInfo( "361365" );
    proc_wjets_cutflow.addSampleFindInfo( "361366" );
    proc_wjets_cutflow.addSampleFindInfo( "361367" );
    proc_wjets_cutflow.addSampleFindInfo( "361368" );
    proc_wjets_cutflow.addSampleFindInfo( "361369" );
    proc_wjets_cutflow.addSampleFindInfo( "361370" );
    proc_wjets_cutflow.addSampleFindInfo( "361371" );
    
    
    //
    // Diboson
    //

    PhysicsProcess proc_diboson_sherpa_25ns( "diboson_sherpa_25ns" , "Diboson (Sherpa, 25ns)" , "Diboson (Sherpa, 25ns)" , 5 );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361063" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361064" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361065" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361066" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361067" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361068" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361069" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361070" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361071" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361072" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361073" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361074" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361077" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361078" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361079" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361080" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361081" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361082" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361083" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361084" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361085" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361086" );
    proc_diboson_sherpa_25ns.addSampleFindInfo( "361087" );

    PhysicsProcess proc_diboson_cutflow( "diboson_cutflow" , "Diboson" , "Diboson" , 5 );
    proc_diboson_cutflow.addSampleFindInfo( "361063" );
    proc_diboson_cutflow.addSampleFindInfo( "361064" );
    proc_diboson_cutflow.addSampleFindInfo( "361065" );
    proc_diboson_cutflow.addSampleFindInfo( "361066" );
    proc_diboson_cutflow.addSampleFindInfo( "361067" );
    proc_diboson_cutflow.addSampleFindInfo( "361068" );
    proc_diboson_cutflow.addSampleFindInfo( "361071" );
    proc_diboson_cutflow.addSampleFindInfo( "361072" );
    proc_diboson_cutflow.addSampleFindInfo( "361073" );
    proc_diboson_cutflow.addSampleFindInfo( "361077" );
    proc_diboson_cutflow.addSampleFindInfo( "361084" );
    proc_diboson_cutflow.addSampleFindInfo( "361086" );


    //
    // Now define different configurations for different data/background compositions
    //

    // Configurations for studying the impact from loosening lepton cuts

    configs["quality1"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.30.1-4" );
    configs["quality1"].addDataProcess( proc_data_25ns );    
    configs["quality1"].addBackgroundProcess( proc_ttbar_powheg_25ns );
    configs["quality1"].addBackgroundProcess( proc_zjets_sherpa_25ns );
    configs["quality1"].addBackgroundProcess( proc_diboson_sherpa_25ns );
    configs["quality1"].addBackgroundProcess( proc_singletop_powheg_25ns );
    configs["quality1"].addSignalProcess( proc_tth_mcanlo_25ns );
    configs["quality1"].addToBlacklist( "276689" );
    configs["quality1"].addToBlacklist( "276731" );
    configs["quality1"].addToBlacklist( "279685" );
    configs["quality1"].addToBlacklist( "361433" );
    configs["quality1"].setLuminosityFromDataTags();
    
    configs["quality1_nodata"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.30.1-4" );
    configs["quality1_nodata"].addBackgroundProcess( proc_ttbar_powheg_25ns );
    configs["quality1_nodata"].addBackgroundProcess( proc_zjets_sherpa_25ns );
    configs["quality1_nodata"].addBackgroundProcess( proc_diboson_sherpa_25ns );
    configs["quality1_nodata"].addBackgroundProcess( proc_singletop_powheg_25ns );
    configs["quality1_nodata"].addSignalProcess( proc_tth_mcanlo_25ns );
    configs["quality1_nodata"].addToBlacklist( "276689" );
    configs["quality1_nodata"].addToBlacklist( "276731" );
    configs["quality1_nodata"].addToBlacklist( "279685" );
    configs["quality1_nodata"].addToBlacklist( "361433" );
    configs["quality1_nodata"].setLuminosity( 4.0*ifb );
    
    configs["quality2_nodata"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.30.1-trunk-1" );
    configs["quality2_nodata"].addBackgroundProcess( proc_ttbar_powheg_25ns );
    configs["quality2_nodata"].addBackgroundProcess( proc_wjets_sherpa_25ns );
    configs["quality2_nodata"].addSignalProcess( proc_tth_mcanlo_25ns );
    configs["quality2_nodata"].setLuminosity( 4.0 * ifb );

    configs["quality2_wjets_nodata"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.30.1-trunk-1" );
    configs["quality2_wjets_nodata"].addBackgroundProcess( proc_wjets_sherpa_25ns );
    configs["quality2_wjets_nodata"].setLuminosity( 4.0 * ifb );

    // Configurations for cutflow checks
    
    configs["cutflow_ttbar"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.30.1-cutflow-1" );
    configs["cutflow_ttbar"].addBackgroundProcess( proc_ttbar_powheg_25ns );
    configs["cutflow_ttbar"].setLuminosity( 1.0*ifb );

    configs["cutflow_tth"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.30.1-cutflow-1" );
    configs["cutflow_tth"].addBackgroundProcess( proc_cutflow_tth );
    configs["cutflow_tth"].setLuminosity( 1.0*ifb );

    configs["cutflow_xmas_tth"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.37.2-cutflow-1" );
    configs["cutflow_xmas_tth"].addBackgroundProcess( proc_cutflow_tth );
    configs["cutflow_xmas_tth"].setLuminosity( 3.20905 * ifb );

    configs["cutflow_xmas"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.37.2-cutflow-1" );
    //configs["cutflow_xmas"].addSignalProcess( proc_cutflow_tth );
    configs["cutflow_xmas"].addBackgroundProcess( proc_ttbar_powheg_25ns );
    //configs["cutflow_xmas"].addDataProcess( proc_cutflow_data_xmas );
    configs["cutflow_xmas"].setLuminosity( 3.20905 * ifb );

    configs["cutflow_xmas2"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.37.2-cutflow-2" );
    //configs["cutflow_xmas2"].addSignalProcess( proc_tth_mcanlo_25ns );
    configs["cutflow_xmas2"].addBackgroundProcess( proc_ttbar_cutflow );
    //configs["cutflow_xmas2"].addBackgroundProcess( proc_singletop_cutflow );
    //configs["cutflow_xmas2"].addBackgroundProcess( proc_ttV_cutflow );
    //configs["cutflow_xmas2"].addBackgroundProcess( proc_diboson_cutflow );
    //configs["cutflow_xmas2"].addBackgroundProcess( proc_zjets_cutflow );
    //configs["cutflow_xmas2"].addBackgroundProcess( proc_wjets_cutflow );
    configs["cutflow_xmas2"].setLuminosity( 3.20905 * ifb );
    
    configs["cutflow_data"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.30.1-cutflow-1" );
    configs["cutflow_data"].addDataProcess( proc_cutflow_data );
    configs["cutflow_data"].setLuminosity( 1.0*ifb );

    configs["tylertest"] = InputConfig( "/xdata/tburch/TTHbbLeptonic/data/TylerTestRun2" );
    configs["tylertest"].addBackgroundProcess( proc_ttbar_powheg_25ns );
    configs["tylertest"].addDataProcess( proc_data_25ns );
    // At the moment I just want to load a single dataset to test the configuration.
    // The rest of the datasets are "blacklisted" here so they don't get loaded.
    // When ready to produce plots, you can remove run numbers from the blacklist...
    configs["tylertest"].addToBlacklist( "276262" );
    configs["tylertest"].addToBlacklist( "276329" );
    configs["tylertest"].addToBlacklist( "276336" );
    configs["tylertest"].addToBlacklist( "276416" );
    configs["tylertest"].addToBlacklist( "276511" );
    configs["tylertest"].addToBlacklist( "276689" );
    configs["tylertest"].addToBlacklist( "276731" );
    configs["tylertest"].addToBlacklist( "276778" );
    configs["tylertest"].addToBlacklist( "276790" );
    configs["tylertest"].addToBlacklist( "276952" );
    configs["tylertest"].addToBlacklist( "278880" );
    configs["tylertest"].addToBlacklist( "278912" );
    configs["tylertest"].addToBlacklist( "278968" ); 
    configs["tylertest"].addToBlacklist( "279169" );
    configs["tylertest"].addToBlacklist( "279259" );
    configs["tylertest"].addToBlacklist( "279279" );
    configs["tylertest"].addToBlacklist( "279284" );
    configs["tylertest"].addToBlacklist( "279345" );
    configs["tylertest"].addToBlacklist( "279515" );
    configs["tylertest"].addToBlacklist( "279598" );
    configs["tylertest"].addToBlacklist( "279685" );
    configs["tylertest"].addToBlacklist( "279764" );
    configs["tylertest"].addToBlacklist( "279813" );
    configs["tylertest"].addToBlacklist( "279867" );
    configs["tylertest"].addToBlacklist( "279928" );
    configs["tylertest"].addToBlacklist( "279932" );
    configs["tylertest"].addToBlacklist( "279984" );
    configs["tylertest"].addToBlacklist( "280231" );
    configs["tylertest"].addToBlacklist( "280319" );
    configs["tylertest"].addToBlacklist( "280368" );
    configs["tylertest"].addToBlacklist( "280423" );
    configs["tylertest"].addToBlacklist( "280464" );
    configs["tylertest"].addToBlacklist( "280500" );
    configs["tylertest"].addToBlacklist( "280520" );
    configs["tylertest"].addToBlacklist( "280614" );
    configs["tylertest"].setLuminosityFromDataTags();

    configs["swift0"] = InputConfig( "/afs/cern.ch/user/s/sswift/work/public/data2/PtOpto-DataTrigFix" );
    configs["swift0"].addDataProcess( proc_data_50ns );
    configs["swift0"].addBackgroundProcess( proc_singletop_powheg_25ns );
    configs["swift0"].addBackgroundProcess( proc_ttbar_powheg_25ns );
    configs["swift0"].addBackgroundProcess( proc_zjets_sherpa_25ns );
    configs["swift0"].setLuminosityFromDataTags();

    configs["swift1"] = InputConfig( "/afs/cern.ch/user/s/sswift/work/public/data2/PtOptoBTAG-DataTrigFix" );
    configs["swift1"].addDataProcess( proc_data_25ns );
    configs["swift1"].addBackgroundProcess( proc_singletop_powheg_25ns );
    configs["swift1"].addBackgroundProcess( proc_ttbar_powheg_25ns );
    configs["swift1"].addBackgroundProcess( proc_zjets_sherpa_25ns );
    configs["swift1"].addBackgroundProcess( proc_zjets_sherpa_lm_25ns );
    configs["swift1"].addBackgroundProcess( proc_diboson_sherpa_25ns ); 
    configs["swift1"].addBackgroundProcess( proc_tth_mcanlo_25ns );
    configs["swift1"].addToBlacklist( "361406" );
    configs["swift1"].addToBlacklist( "361409" );
    configs["swift1"].addToBlacklist( "361069" );
    configs["swift1"].addToBlacklist( "361074" );
    configs["swift1"].addToBlacklist( "361078" );
    configs["swift1"].addToBlacklist( "361079" );
    configs["swift1"].addToBlacklist( "361080" );
    configs["swift1"].setLuminosityFromDataTags();
    
    configs["ttbartest"] = InputConfig( "/atlasfs/atlas/local/jwebster/tth/data/2.3.37.2-cutflow-2" );
    configs["ttbartest"].addBackgroundProcess( proc_ttbar_cc );
    configs["ttbartest"].addBackgroundProcess( proc_ttbar_light );
    configs["ttbartest"].addBackgroundProcess( proc_ttbar_bb );
    configs["ttbartest"].setLuminosity( 1.0 * ifb );
    
    // Load the configuration specified by user tag
    // If the tag doesn't exist then list the existing tags and assert(false)
    
    if( configs[tag].getDataBaseDir()==TString("null") ) {
      report::error( "Failed to find the InputConfig with tag %s" , tag.Data() );
      report::blank( "The configurations defined in tth.h are..." );
      for( std::map<TString,InputConfig>::iterator ib = configs.begin() , ie = configs.end() ; ib != ie ; ++ib ) {
	if( ib->first==tag ) continue;
	report::blank( "   %s" , ib->first.Data() );
      }
      report::blank( "... but feel free to add more!" );
      assert( false );
    }
    
    input = configs[tag];
    input.load();

  }


};

#endif


