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

// jPsiCut_MC

int nbins = 50;


void GetAcceptance(TString var_name, double xmin, double xmax, TString kin_cuts, TString hist_name, TString target, TString particle, TString particle_MC)
{
    TString MC_cuts = Form("%s && %s && %s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data(), particle_MC.Data(), kin_cuts.Data(), target.Data());

    TString Real_cuts = Form("%s && %s && %s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data(), particle.Data(), kin_cuts.Data(), target.Data());

    TString Mix_cuts = Form("%s && %s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), particle.Data(), kin_cuts.Data(), target.Data());

    RDataFrame df_real("Tree", real_data.Data());
    auto df_real1 = df_real.Filter(Real_cuts.Data());

    RDataFrame df_mix("Tree", mix_data.Data());
    auto df_mix1 = df_mix.Filter(Mix_cuts.Data());

    RDataFrame df_mc("Tree", LH2_jpsi_data.Data());
    auto df_mc1 = df_mc.Filter(MC_cuts.Data());

    TString hReal_name = Form("hist_%s_real", hist_name.Data());
    TString hMix_name = Form("hist_%s_mix", hist_name.Data());
    TString hJpsi_name = Form("hist_%s_jpsi", hist_name.Data());

    TString hist_title = Form("; %s [cm]; normalized to unity [a.u.]", var_name.Data());

    auto hReal = df_real1.Histo1D({hReal_name.Data(), hist_title.Data(), nbins, xmin, xmax}, var_name.Data());
    auto hMix = df_mix1.Histo1D({hMix_name.Data(), hist_title.Data(), nbins, xmin, xmax}, var_name.Data());
    auto hMC = df_mc1.Histo1D({hMix_name.Data(), hist_title.Data(), nbins, xmin, xmax}, var_name.Data(), "ReWeight");


    hReal->Sumw2();
    hMix->Sumw2();
    hJMC->Sumw2();

    hReal->Add(hMix.GetPtr(), -1);

    hReal->Scale(1./hReal->Integral());
    hMC->Scale(1./hMC->Integral());

    hReal->SetFillColor(kViolet-2);

    hMC->SetMarkerColor(kSpring);
    hMC->SetMarkerStyle(20);

     double ymax = 1.5* hReal->GetMaximum();
     hReal->SetMaximum(ymax);

    auto can = new TCanvas();

    TString hist_save = Form("imgs/%s.png", hist_name.Data());

    hReal->Draw("HIST");
    hMC->Draw("HIST SAME");
    can->Update();

    double mean = hMC->GetMean();
    double fwhm = 2.355*hMC->GetRMS();

    TString fiducial_cuts = Form("Mean = %.1f ,FWHM = %.1f", mean, fwhm);

    TText* t = new TText(.2, .95, fiducial_cuts.Data());
    t->SetNDC();
    t->SetTextAlign(22);
    t->SetTextColor(kRed+2);
    t->SetTextFont(43);
    t->SetTextSize(20);
    t->Draw();
    can->Update();

    TLine* l_min = new TLine(mean - 0.75* fwhm, 0., mean - 0.75* fwhm, ymax);
    l_min->SetLineColor(kRed);
    l_min->Draw();
    TLine* l_max = new TLine(mean + 0.75* fwhm, 0., mean + 0.75* fwhm, ymax);
    l_max->SetLineColor(kRed);
    l_max->Draw();
    can->Update();
    can->SaveAs(hist_save.Data());

    TString outputs = Form("--> Fiducial cuts = %.1f < %s < %.1f", mean - 0.75* fwhm, var_name.Data(), mean + 0.75* fwhm);
    cout << outputs.Data() << endl;
}

void ApplyCuts()
{
    GetAcceptance("x1_st1", -30., 20., "0.4 < xF && xF < 0.6", "LH2_jpsi_x1_st1_xF0", "targetPos==1", jPsiCut, jPsiCut_MC);
}
