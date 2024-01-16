/*
 * dinupa3@gmail.com
 * Apply chuckcuts to data and make RDataFrame
*/

#include<iostream>

#include "chuckcuts.hh"

using namespace ROOT;

TString real_data = "/seaquest/users/chleung/rootfiles/run56_2111v42_tmp_noPhys_D0.root";
TString mix_data = "/seaquest/users/chleung/rootfiles/mix_FPGA4_run56_2111v42_tmp_noPhys.root";
TString LH2_jpsi_data = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LH2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LD2_jpsi_data = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LD2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LH2_psip_data = "/seaquest/users/chleung/pT_ReWeight/mc_psiprime_LH2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LD2_psip_data = "/seaquest/users/chleung/pT_ReWeight/mc_psiprime_LD2_M027_S002_messy_occ_pTxFweight_v2.root";


void SaveDataFrames()
{

    TString basic_MC_cuts = Form("%s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data());

    TString basic_real_cuts = Form("%s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data());


    TString basic_mix_cuts = Form("%s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), "(targetPos==1 | targetPos==3)");

    RDataFrame df_real("Tree", real_data.Data());

    auto df_real_st2_pT = df_real.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("z0_st1", "620.").Define("z0_st2", "1345.").Define("z0_st3", "1910.").Define("x1_st2", "x1_st3 - (px1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("x2_st2", "x2_st3 - (px2_st3/pz2_st3)* (z0_st3 - z0_st2)").Define("y1_st2", "y1_st3 - (py1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("y2_st2", "y2_st3 - (py2_st3/pz2_st3)* (z0_st3 - z0_st2)");

    auto df_real_with_basic_cuts = df_real_st2_pT.Filter(basic_real_cuts.Data());

    df_real_with_basic_cuts.Snapshot("Tree", "data/e906_data.root");

    RDataFrame df_mix("Tree", mix_data.Data());

    auto df_mix_st2_pT = df_mix.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("z0_st1", "620.").Define("z0_st2", "1345.").Define("z0_st3", "1910.").Define("x1_st2", "x1_st3 - (px1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("x2_st2", "x2_st3 - (px2_st3/pz2_st3)* (z0_st3 - z0_st2)").Define("y1_st2", "y1_st3 - (py1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("y2_st2", "y2_st3 - (py2_st3/pz2_st3)* (z0_st3 - z0_st2)");

    auto df_mix_with_basic_cuts = df_mix_st2_pT.Filter(basic_mix_cuts.Data());

    df_mix_with_basic_cuts.Snapshot("Tree", "data/e906_mix_data.root");

    RDataFrame df_LH2_jpsi("Tree", LH2_jpsi_data.Data());

    auto df_LH2_jpsi_st2_pT = df_LH2_jpsi.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("z0_st1", "620.").Define("z0_st2", "1345.").Define("z0_st3", "1910.").Define("x1_st2", "x1_st3 - (px1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("x2_st2", "x2_st3 - (px2_st3/pz2_st3)* (z0_st3 - z0_st2)").Define("y1_st2", "y1_st3 - (py1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("y2_st2", "y2_st3 - (py2_st3/pz2_st3)* (z0_st3 - z0_st2)");

    auto df_LH2_jpsi_with_basic_cuts = df_LH2_jpsi_st2_pT.Filter(basic_MC_cuts.Data());

    df_LH2_jpsi_with_basic_cuts.Snapshot("Tree", "data/e906_LH2_jpsi_MC.root");


    RDataFrame df_LD2_jpsi("Tree", LD2_jpsi_data.Data());

    auto df_LD2_jpsi_st2_pT = df_LD2_jpsi.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("z0_st1", "620.").Define("z0_st2", "1345.").Define("z0_st3", "1910.").Define("x1_st2", "x1_st3 - (px1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("x2_st2", "x2_st3 - (px2_st3/pz2_st3)* (z0_st3 - z0_st2)").Define("y1_st2", "y1_st3 - (py1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("y2_st2", "y2_st3 - (py2_st3/pz2_st3)* (z0_st3 - z0_st2)");

    auto df_LD2_jpsi_with_basic_cuts = df_LD2_jpsi_st2_pT.Filter(basic_MC_cuts.Data());

    df_LD2_jpsi_with_basic_cuts.Snapshot("Tree", "data/e906_LD2_jpsi_MC.root");

    RDataFrame df_LH2_psip("Tree", LH2_psip_data.Data());

    auto df_LH2_psip_st2_pT = df_LH2_psip.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("z0_st1", "620.").Define("z0_st2", "1345.").Define("z0_st3", "1910.").Define("x1_st2", "x1_st3 - (px1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("x2_st2", "x2_st3 - (px2_st3/pz2_st3)* (z0_st3 - z0_st2)").Define("y1_st2", "y1_st3 - (py1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("y2_st2", "y2_st3 - (py2_st3/pz2_st3)* (z0_st3 - z0_st2)");

    auto df_LH2_psip_with_basic_cuts = df_LH2_psip_st2_pT.Filter(basic_MC_cuts.Data());

    df_LH2_psip_with_basic_cuts.Snapshot("Tree", "data/e906_LH2_psip_MC.root");

    RDataFrame df_LD2_psip("Tree", LD2_psip_data.Data());

    auto df_LD2_psip_st2_pT = df_LD2_psip.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("z0_st1", "620.").Define("z0_st2", "1345.").Define("z0_st3", "1910.").Define("x1_st2", "x1_st3 - (px1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("x2_st2", "x2_st3 - (px2_st3/pz2_st3)* (z0_st3 - z0_st2)").Define("y1_st2", "y1_st3 - (py1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("y2_st2", "y2_st3 - (py2_st3/pz2_st3)* (z0_st3 - z0_st2)");

    auto df_LD2_psip_with_basic_cuts = df_LD2_psip_st2_pT.Filter(basic_MC_cuts.Data());

    df_LD2_psip_with_basic_cuts.Snapshot("Tree", "data/e906_LD2_psip_MC.root");

}
