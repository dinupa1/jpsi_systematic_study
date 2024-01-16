/*
 * dinupa3@gmail.com
 * Calculating smearing effect due to detector resolution
 */

#include <iostream>

#include "chuckcuts.hh"

using namespace std;

using namespace ROOT;


TString LH2_jpsi_data = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LH2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LD2_jpsi_data = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LD2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LH2_psip_data = "/seaquest/users/chleung/pT_ReWeight/mc_psiprime_LH2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LD2_psip_data = "/seaquest/users/chleung/pT_ReWeight/mc_psiprime_LD2_M027_S002_messy_occ_pTxFweight_v2.root";


TString basic_jpsi_cuts = Form("%s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data(), jPsiCut_MC.Data());

TString basic_psip_cuts = Form("%s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data(), psipCut.Data());

RDataFrame df_LH2_jpsi_data("Tree", LH2_jpsi_data.Data());
auto df_LH2_jpsi_with_basic_cuts = df_LH2_jpsi_data.Filter(basic_jpsi_cuts.Data());

RDataFrame df_LD2_jpsi_data("Tree", LD2_jpsi_data.Data());
auto df_LD2_jpsi_with_basic_cuts = df_LD2_jpsi_data.Filter(basic_jpsi_cuts.Data());

RDataFrame df_LH2_psip_data("Tree", LH2_psip_data.Data());
auto df_LH2_psip_with_basic_cuts = df_LH2_psip_data.Filter(basic_psip_cuts.Data());

RDataFrame df_LD2_psip_data("Tree", LD2_psip_data.Data());
auto df_LD2_psip_with_basic_cuts = df_LD2_psip_data.Filter(basic_psip_cuts.Data());