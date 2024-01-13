/*
 * dinupa3@gmail.com
 * Apply chuckcuts to data and make simple root file
 */

#include<TString.h>
#include<iostream>

#include "chuckcuts.h"

using namespace ROOT;

TString real_data = "/seaquest/users/chleung/rootfiles/run56_2111v42_tmp_noPhys_D0.root";
TString mix_data = "/seaquest/users/chleung/rootfiles/mix_FPGA4_run56_2111v42_tmp_noPhys.root";
TString LH2_jpsi_data = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LH2_M027_S002_clean_occ_pTxFweight_v2.root";

int nbins = 50;


void GetAcceptance(TString var_name, double xmin, double xmax, TString kin_cuts, TString hist_name)
{
    TString MC_cuts = Form("%s && %s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data(), jPsiCut_MC.Data(), kin_cuts.Data());

    TString Real_cuts = Form("%s && %s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data(), jPsiCut.Data(), kin_cuts.Data());

    TString Mix_cuts = Form("%s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), jPsiCut.Data(), kin_cuts.Data());

    RDataFrame df_real("Tree", real_data.Data());
    auto df_real1 = df_real.Filter(Real_cuts.Data());

    RDataFrame df_mix("Tree", mix_data.Data());
    auto df_mix1 = df_mix.Filter(Mix_cuts.Data());

    RDataFrame df_mix("Tree", LH2_jpsi_data.Data());
    auto df_mc1 = df_mc.Filter(MC_cuts.Data());

    TString hReal_name = Form("hist_%s_real", hist_name.Data());
    TString hMix_name = Form("hist_%s_mix", hist_name.Data());
    TString hJpsi_name = Form("hist_%s_jpsi", hist_name.Data());

    TString hist_title = Form("; %s [cm]; normalized to unity [a.u.]", var_name.Data());

    auto hReal = df_real1.Histo1D({hReal_name.Data(), hist_title.Data(), nbins, xmin, xmax}, var_name.Data());

    auto can = new TCanvas();

    TString hist_save = Form("%s.png", hist_name.Data());

    hReal->Draw("E1");
    can->SaveAs(hist_save.Data());
}

void ApplyCuts()
{
    GetAcceptance("x1_st1", -30., 20., "0.4 < xF && xF < 0.6", "x1_st1_xF0");
}
