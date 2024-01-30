/*
 * dinupa3@gmail.com
 * Calculating smearing effect due to detector resolution
 */
#ifndef _DETECTOR_SMEARING__HH_
#define _DETECTOR_SMEARING__HH_

#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <TString.h>
#include <iostream>

#include "chuckcuts.hh"

using namespace std;

using ROOT::RDataFrame;

// MC data
TString LH2_jpsi_data = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LH2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LD2_jpsi_data = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LD2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LH2_psip_data = "/seaquest/users/chleung/pT_ReWeight/mc_psiprime_LH2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LD2_psip_data = "/seaquest/users/chleung/pT_ReWeight/mc_psiprime_LD2_M027_S002_messy_occ_pTxFweight_v2.root";

// real and mix data
TString real_data = "/seaquest/users/chleung/rootfiles/run56_2111v42_tmp_noPhys_D0.root";
TString mix_data = "/seaquest/users/chleung/rootfiles/mix_FPGA4_run56_2111v42_tmp_noPhys.root";


// MC data frames
TString basic_jpsi_cuts = Form("%s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data(), jPsiCut_MC.Data());

TString basic_psip_cuts = Form("%s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data(), psipCut.Data());

RDataFrame df_LH2_jpsi_data("Tree", LH2_jpsi_data.Data());
auto df_LH2_jpsi_with_basic_cuts0 = df_LH2_jpsi_data.Filter(basic_jpsi_cuts.Data());
auto df_LH2_jpsi_with_basic_cuts = df_LH2_jpsi_with_basic_cuts0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("mpT", "sqrt(mdpx* mdpx + mdpy* mdpy)");

RDataFrame df_LD2_jpsi_data("Tree", LD2_jpsi_data.Data());
auto df_LD2_jpsi_with_basic_cuts0 = df_LD2_jpsi_data.Filter(basic_jpsi_cuts.Data());
auto df_LD2_jpsi_with_basic_cuts = df_LD2_jpsi_with_basic_cuts0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("mpT", "sqrt(mdpx* mdpx + mdpy* mdpy)");

RDataFrame df_LH2_psip_data("Tree", LH2_psip_data.Data());
auto df_LH2_psip_with_basic_cuts0 = df_LH2_psip_data.Filter(basic_psip_cuts.Data());
auto df_LH2_psip_with_basic_cuts = df_LH2_psip_with_basic_cuts0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("mpT", "sqrt(mdpx* mdpx + mdpy* mdpy)");

RDataFrame df_LD2_psip_data("Tree", LD2_psip_data.Data());
auto df_LD2_psip_with_basic_cuts0 = df_LD2_psip_data.Filter(basic_psip_cuts.Data());
auto df_LD2_psip_with_basic_cuts = df_LD2_psip_with_basic_cuts0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("mpT", "sqrt(mdpx* mdpx + mdpy* mdpy)");


// real and mix data frames
TString basic_real_cuts = Form("%s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data());

TString basic_mix_cuts = Form("%s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), "(targetPos==1 || targetPos==3)");

//
//
RDataFrame df_real_data("Tree", real_data.Data());

TString LH2_jpsi_real_cuts = Form("%s && %s && %s", basic_real_cuts.Data(), "targetPos==1", jPsiCut.Data());
auto df_LH2_jpsi_real_data0 = df_real_data.Filter(LH2_jpsi_real_cuts.Data());
auto df_LH2_jpsi_real_data = df_LH2_jpsi_real_data0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)");

TString LD2_jpsi_real_cuts = Form("%s && %s && %s", basic_real_cuts.Data(), "targetPos==3", jPsiCut.Data());
auto df_LD2_jpsi_real_data0 = df_real_data.Filter(LD2_jpsi_real_cuts.Data());
auto df_LD2_jpsi_real_data = df_LD2_jpsi_real_data0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)");


//
TString LH2_psip_real_cuts = Form("%s && %s && %s", basic_real_cuts.Data(), "targetPos==1", psipCut.Data());
auto df_LH2_psip_real_data0 = df_real_data.Filter(LH2_psip_real_cuts.Data());
auto df_LH2_psip_real_data = df_LH2_psip_real_data0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)");

TString LD2_psip_real_cuts = Form("%s && %s && %s", basic_real_cuts.Data(), "targetPos==3", psipCut.Data());
auto df_LD2_psip_real_data0 = df_real_data.Filter(LD2_psip_real_cuts.Data());
auto df_LD2_psip_real_data = df_LD2_psip_real_data0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)");

//
//
RDataFrame df_mix_data("Tree", mix_data.Data());

TString LH2_jpsi_mix_cuts = Form("%s && %s", basic_mix_cuts.Data(), jPsiCut.Data());
auto df_LH2_jpsi_mix_data0 = df_mix_data.Filter(LH2_jpsi_mix_cuts.Data());
auto df_LH2_jpsi_mix_data = df_LH2_jpsi_mix_data0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)");

TString LD2_jpsi_mix_cuts = Form("%s && %s", basic_mix_cuts.Data(), jPsiCut.Data());
auto df_LD2_jpsi_mix_data0 = df_mix_data.Filter(LD2_jpsi_mix_cuts.Data());
auto df_LD2_jpsi_mix_data = df_LD2_jpsi_mix_data0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)");


//
TString LH2_psip_mix_cuts = Form("%s && %s", basic_mix_cuts.Data(), psipCut.Data());
auto df_LH2_psip_mix_data0 = df_mix_data.Filter(LH2_psip_mix_cuts.Data());
auto df_LH2_psip_mix_data = df_LH2_psip_mix_data0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)");

TString LD2_psip_mix_cuts = Form("%s && %s", basic_mix_cuts.Data(), psipCut.Data());
auto df_LD2_psip_mix_data0 = df_mix_data.Filter(LD2_psip_mix_cuts.Data());
auto df_LD2_psip_mix_data = df_LD2_psip_mix_data0.Define("pT", "sqrt(dpx* dpx + dpy* dpy)");

TString varRecos[3] = {"pT", "xF"};
TString varTrues[3] = {"mpT", "mxF"};
double xFbins[6] = {0.4, 0.6, 0.65, 0.7, 0.77, 0.95};
double pTbins[6] = {0., 0.3, 0.45, 0.65, 0.9, 1.5};

#endif /*_DETECTOR_SMEARING__HH_*/
