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
TString LH2_jpsi_data = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LH2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LD2_jpsi_data = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LD2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LH2_psip_data = "/seaquest/users/chleung/pT_ReWeight/mc_psiprime_LH2_M027_S002_messy_occ_pTxFweight_v2.root";
TString LD2_psip_data = "/seaquest/users/chleung/pT_ReWeight/mc_psiprime_LD2_M027_S002_messy_occ_pTxFweight_v2.root";

// jPsiCut_MC

int nbins = 50;

void GetAcceptance(TString var_name, double xmin, double xmax, TString kin_cuts, TString hist_name, TString target, TString particle, TString particle_MC, TString MC_data)
{
    gStyle->SetOptStat(0);

    TString MC_cuts = Form("%s && %s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data(), particle_MC.Data(), kin_cuts.Data());

    TString Real_cuts = Form("%s && %s && %s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), occCuts_2111v42_Run56.Data(), particle.Data(), kin_cuts.Data(), target.Data());

    TString Mix_cuts = Form("%s && %s && %s && %s && %s && %s && %s", chuckCutsPositive_2111v42_tmp.Data(), chuckCutsNegative_2111v42_tmp.Data(), chuckCutsDimuon_2111v42.Data(), physicsCuts_noMassCut_2111v42_tmp.Data(), particle.Data(), kin_cuts.Data(), "(targetPos==1 | targetPos==3)");

    RDataFrame df_real("Tree", real_data.Data());
    auto df_real0 = df_real.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("z0_st1", "620.").Define("z0_st2", "1345.").Define("z0_st3", "1920.").Define("x1_st2", "x1_st3 - (px1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("x2_st2", "x2_st3 - (px2_st3/pz2_st3)* (z0_st3 - z0_st2)").Define("y1_st2", "y1_st3 - (py1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("y2_st2", "y2_st3 - (py2_st3/pz2_st3)* (z0_st3 - z0_st2)");
    auto df_real1 = df_real0.Filter(Real_cuts.Data());

    RDataFrame df_mix("Tree", mix_data.Data());
    auto df_mix0 = df_mix.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("z0_st1", "620.").Define("z0_st2", "1345.").Define("z0_st3", "1920.").Define("x1_st2", "x1_st3 - (px1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("x2_st2", "x2_st3 - (px2_st3/pz2_st3)* (z0_st3 - z0_st2)").Define("y1_st2", "y1_st3 - (py1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("y2_st2", "y2_st3 - (py2_st3/pz2_st3)* (z0_st3 - z0_st2)");
    auto df_mix1 = df_mix0.Filter(Mix_cuts.Data());

    RDataFrame df_mc("Tree", MC_data.Data());
    auto df_mc0 = df_mc.Define("pT", "sqrt(dpx* dpx + dpy* dpy)").Define("z0_st1", "620.").Define("z0_st2", "1345.").Define("z0_st3", "1920.").Define("x1_st2", "x1_st3 - (px1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("x2_st2", "x2_st3 - (px2_st3/pz2_st3)* (z0_st3 - z0_st2)").Define("y1_st2", "y1_st3 - (py1_st3/pz1_st3)* (z0_st3 - z0_st2)").Define("y2_st2", "y2_st3 - (py2_st3/pz2_st3)* (z0_st3 - z0_st2)");
    auto df_mc1 = df_mc0.Filter(MC_cuts.Data());

    TString hReal_name = Form("hist_%s_real", hist_name.Data());
    TString hMix_name = Form("hist_%s_mix", hist_name.Data());
    TString hJpsi_name = Form("hist_%s_jpsi", hist_name.Data());

    TString hist_title = Form("; (%s) %s [cm]; normalized to unity [a.u.]", kin_cuts.Data(), var_name.Data());

    auto hReal = df_real1.Histo1D({hReal_name.Data(), hist_title.Data(), nbins, xmin, xmax}, var_name.Data());
    auto hMix = df_mix1.Histo1D({hMix_name.Data(), hist_title.Data(), nbins, xmin, xmax}, var_name.Data());
    auto hMC = df_mc1.Histo1D({hMix_name.Data(), hist_title.Data(), nbins, xmin, xmax}, var_name.Data(), "ReWeight");


    hReal->Sumw2();
    hMix->Sumw2();
    hMC->Sumw2();

    hReal->Add(hMix.GetPtr(), -1);

    hReal->Scale(1./hReal->Integral());
    hMC->Scale(1./hMC->Integral());

    hReal->SetFillColor(kViolet-9);

    hMC->SetMarkerColor(kSpring);
    hMC->SetMarkerStyle(20);

    double ymax = 1.5* hReal->GetMaximum();
    hReal->SetMaximum(ymax);

    auto can = new TCanvas();

    TString hist_save = Form("imgs/%s.png", hist_name.Data());

    hReal->Draw("HIST");
    hMC->Draw("E1 SAME");
    can->Update();

    double mean = hMC->GetMean();
    double fwhm = 2.355*hMC->GetRMS();

    double fiducial_min = mean - fwhm;
    double fiducial_max = mean + fwhm;

    TString fiducial_cuts = Form("%s for %s --> %.1f < %s < %.1f", kin_cuts.Data(), target.Data(), fiducial_min, var_name.Data(), fiducial_max);

//     TText* t = new TText(.4, .95, fiducial_cuts.Data());
//     t->SetNDC();
//     t->SetTextAlign(22);
//     t->SetTextColor(kRed+2);
//     t->SetTextFont(43);
//     t->SetTextSize(20);
//     t->Draw();
//     can->Update();

//     TLine* l_min = new TLine(fiducial_min, hReal->GetBinContent(hReal->GetMinimumBin()), fiducial_min,  hReal->GetBinContent(hReal->GetMaximumBin()));
//     l_min->SetLineColor(kRed);
//     l_min->Draw();
//     TLine* l_max = new TLine(fiducial_max, hReal->GetBinContent(hReal->GetMinimumBin()), fiducial_max,  hReal->GetBinContent(hReal->GetMaximumBin()));
//     l_max->SetLineColor(kRed);
//     l_max->Draw();
//     can->Update();
    can->SaveAs(hist_save.Data());

    cout << fiducial_cuts.Data() << endl;


    /*
     * Ratio plots
     */
    TString ratio_name = Form("ratio_%s", hist_name.Data());
    TString ratio_title = Form("Ratio ; (%s) %s [cm]; #frac{MC data}{Real data} [a.u.]", kin_cuts.Data(), var_name.Data());

    auto ratio_plot = new TH1D(ratio_name.Data(), ratio_title.Data(), nbins, xmin, xmax);

    /*
     * Fractional deviation
     */
    TString fractional_name = Form("frac_%s", hist_name.Data());
    TString fractional_title = Form("Fractional deviation; (%s) %s [cm]; #frac{Real data - MC data}{MC data}", kin_cuts.Data(), var_name.Data());

    auto fractional_plot = new TH1D(fractional_name.Data(), fractional_title.Data(), nbins, xmin, xmax);

    for(int i = 0; i < nbins; i++)
    {
        double real_c = hReal->GetBinContent(i+1);
        double real_e = hReal->GetBinError(i+1);

        double mc_c = hMC->GetBinContent(i+1);
        double mc_e = hMC->GetBinError(i+1);

        double ratio_e2 = (1./real_c)* (1./real_c)* mc_e* mc_e + (mc_c/(real_c* real_c))* (mc_c/(real_c* real_c))* real_e* real_e;

        if(sqrt(ratio_e2) < 10.)
        {
            ratio_plot->SetBinContent(i+1, mc_c/real_c);
            ratio_plot->SetBinError(i+1, sqrt(ratio_e2));
        }

        /*
         * Fractional deviation
         */

        double fraction_e2 = (real_c/(mc_c * mc_c))* (real_c/(mc_c * mc_c))* real_e* real_e + (1./real_c)* (1./real_c)* mc_e* mc_e;

        if(sqrt(fraction_e2) < 10.)
        {
            fractional_plot->SetBinContent(i+1, (real_c - mc_c)/mc_c);
            fractional_plot->SetBinError(i+1, sqrt(fraction_e2));
        }

    }

    ratio_plot->SetMarkerColor(kViolet);
    ratio_plot->SetMarkerStyle(20);

    TString ratio_save = Form("imgs/ratio_%s.png", hist_name.Data());

    ratio_plot->Draw("E1");
    can->Update();

//     TLine* l_min1 = new TLine(fiducial_min, ratio_plot->GetBinContent(ratio_plot->GetMinimumBin()), fiducial_min,  ratio_plot->GetBinContent(ratio_plot->GetMaximumBin()));
//     l_min1->SetLineColor(kRed);
//     l_min1->Draw();
//     TLine* l_max1 = new TLine(fiducial_max, ratio_plot->GetBinContent(ratio_plot->GetMinimumBin()), fiducial_max,  ratio_plot->GetBinContent(ratio_plot->GetMaximumBin()));
//     l_max1->SetLineColor(kRed);
//     l_max1->Draw();
//     can->Update();
    can->SaveAs(ratio_save.Data());

    /*
     * Fractional deviation
     */

    fractional_plot->SetMarkerColor(kViolet);
    fractional_plot->SetMarkerStyle(20);

    TString fractional_save = Form("imgs/fractional_%s.png", hist_name.Data());

    fractional_plot->Draw("E1");
    can->Update();

//     TLine* l_min2 = new TLine(fiducial_min, fractional_plot->GetBinContent(fractional_plot->GetMinimumBin()), fiducial_min,  fractional_plot->GetBinContent(fractional_plot->GetMaximumBin()));
//     l_min2->SetLineColor(kRed);
//     l_min2->Draw();
//     TLine* l_max2 = new TLine(fiducial_max, fractional_plot->GetBinContent(fractional_plot->GetMinimumBin()), fiducial_max,  fractional_plot->GetBinContent(fractional_plot->GetMaximumBin()));
//     l_max2->SetLineColor(kRed);
//     l_max2->Draw();
//     can->Update();
    can->SaveAs(fractional_save.Data());
}

void ChamberAcceptance()
{
    double xF_bins[6] = {0.4, 0.6, 0.65, 0.7, 0.77, 0.95};
    double pT_bins[6] = {0, 0.3, 0.45, 0.65, 0.9, 1.5};

    /*
     * LH2 Jpsi plots
     */
    for(int i = 0; i < 5; i++)
    {
        /*
         * xF bins
         */
        TString xF_cut = Form("%.2f < xF && xF < %.2f", xF_bins[i], xF_bins[i+1]);

        TString xF_hist1 = Form("LH2_jpsi_x1_st1_xF%d", i);
        GetAcceptance("x1_st1", -20., 15., xF_cut, xF_hist1, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist2 = Form("LH2_jpsi_x2_st1_xF%d", i);
        GetAcceptance("x2_st1", -30., 30., xF_cut, xF_hist2, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist3 = Form("LH2_jpsi_y1_st1_xF%d", i);
        GetAcceptance("y1_st1", -60., 60., xF_cut, xF_hist3, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist4 = Form("LH2_jpsi_y2_st1_xF%d", i);
        GetAcceptance("y2_st1", -60., 60., xF_cut, xF_hist4, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist5 = Form("LH2_jpsi_x1_st3_xF%d", i);
        GetAcceptance("x1_st3", 0., 150., xF_cut, xF_hist5, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist6 = Form("LH2_jpsi_x2_st3_xF%d", i);
        GetAcceptance("x2_st3", -150., 0., xF_cut, xF_hist6, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist7 = Form("LH2_jpsi_y1_st3_xF%d", i);
        GetAcceptance("y1_st3", -150., 150., xF_cut, xF_hist7, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist8 = Form("LH2_jpsi_y2_st3_xF%d", i);
        GetAcceptance("y2_st3", -150., 150., xF_cut, xF_hist8, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist9 = Form("LH2_jpsi_x1_st2_xF%d", i);
        GetAcceptance("x1_st2", -20., 15., xF_cut, xF_hist9, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist10 = Form("LH2_jpsi_x2_st2_xF%d", i);
        GetAcceptance("x2_st2", -30., 30., xF_cut, xF_hist10, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist11 = Form("LH2_jpsi_y1_st2_xF%d", i);
        GetAcceptance("y1_st2", -60., 60., xF_cut, xF_hist11, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString xF_hist12 = Form("LH2_jpsi_y2_st2_xF%d", i);
        GetAcceptance("y2_st2", -60., 60., xF_cut, xF_hist12, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        /*
         * pT bins
         */
        TString pT_cut = Form("%.2f < pT && pT < %.2f", pT_bins[i], pT_bins[i+1]);

        TString pT_hist1 = Form("LH2_jpsi_x1_st1_pT%d", i);
        GetAcceptance("x1_st1", -20., 15., pT_cut, pT_hist1, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist2 = Form("LH2_jpsi_x2_st1_pT%d", i);
        GetAcceptance("x2_st1", -30., 30., pT_cut, pT_hist2, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist3 = Form("LH2_jpsi_y1_st1_pT%d", i);
        GetAcceptance("y1_st1", -60., 60., pT_cut, pT_hist3, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist4 = Form("LH2_jpsi_y2_st1_pT%d", i);
        GetAcceptance("y2_st1", -60., 60., pT_cut, pT_hist4, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist5 = Form("LH2_jpsi_x1_st3_pT%d", i);
        GetAcceptance("x1_st3", 0., 150., pT_cut, pT_hist5, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist6 = Form("LH2_jpsi_x2_st3_pT%d", i);
        GetAcceptance("x2_st3", -150., 0., pT_cut, pT_hist6, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist7 = Form("LH2_jpsi_y1_st3_pT%d", i);
        GetAcceptance("y1_st3", -150., 150., pT_cut, pT_hist7, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist8 = Form("LH2_jpsi_y2_st3_pT%d", i);
        GetAcceptance("y2_st3", -150., 150., pT_cut, pT_hist8, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist9 = Form("LH2_jpsi_x1_st2_pT%d", i);
        GetAcceptance("x1_st2", -20., 15., pT_cut, pT_hist9, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist10 = Form("LH2_jpsi_x2_st2_pT%d", i);
        GetAcceptance("x2_st2", -30., 30., pT_cut, pT_hist10, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist11 = Form("LH2_jpsi_y1_st2_pT%d", i);
        GetAcceptance("y1_st2", -60., 60., pT_cut, pT_hist11, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);

        TString pT_hist12 = Form("LH2_jpsi_y2_st2_pT%d", i);
        GetAcceptance("y2_st2", -60., 60., pT_cut, pT_hist12, "targetPos==1", jPsiCut, jPsiCut_MC, LH2_jpsi_data);
    }

/*

//      LD2 jpsi plots

    for(int i = 0; i < 5; i++)
    {

//           xF bins

        TString xF_cut = Form("%.2f < xF && xF < %.2f", xF_bins[i], xF_bins[i+1]);

        TString xF_hist1 = Form("LD2_jpsi_x1_st1_xF%d", i);
        GetAcceptance("x1_st1", -20., 15., xF_cut, xF_hist1, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString xF_hist2 = Form("LD2_jpsi_x2_st1_xF%d", i);
        GetAcceptance("x2_st1", -30., 30., xF_cut, xF_hist2, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString xF_hist3 = Form("LD2_jpsi_y1_st1_xF%d", i);
        GetAcceptance("y1_st1", -60., 60., xF_cut, xF_hist3, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString xF_hist4 = Form("LD2_jpsi_y2_st1_xF%d", i);
        GetAcceptance("y2_st1", -60., 60., xF_cut, xF_hist4, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString xF_hist5 = Form("LD2_jpsi_x1_st3_xF%d", i);
        GetAcceptance("x1_st3", 0., 150., xF_cut, xF_hist5, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString xF_hist6 = Form("LD2_jpsi_x2_st3_xF%d", i);
        GetAcceptance("x2_st3", -150., 0., xF_cut, xF_hist6, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString xF_hist7 = Form("LD2_jpsi_y1_st3_xF%d", i);
        GetAcceptance("y1_st3", -150., 150., xF_cut, xF_hist7, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString xF_hist8 = Form("LD2_jpsi_y2_st3_xF%d", i);
        GetAcceptance("y2_st3", -150., 150., xF_cut, xF_hist8, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);


//           pT bins

        TString pT_cut = Form("%.2f < pT && pT < %.2f", pT_bins[i], pT_bins[i+1]);

        TString pT_hist1 = Form("LD2_jpsi_x1_st1_pT%d", i);
        GetAcceptance("x1_st1", -20., 15., pT_cut, pT_hist1, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString pT_hist2 = Form("LD2_jpsi_x2_st1_pT%d", i);
        GetAcceptance("x2_st1", -30., 30., pT_cut, pT_hist2, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString pT_hist3 = Form("LD2_jpsi_y1_st1_pT%d", i);
        GetAcceptance("y1_st1", -60., 60., pT_cut, pT_hist3, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString pT_hist4 = Form("LD2_jpsi_y2_st1_pT%d", i);
        GetAcceptance("y2_st1", -60., 60., pT_cut, pT_hist4, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString pT_hist5 = Form("LD2_jpsi_x1_st3_pT%d", i);
        GetAcceptance("x1_st3", 0., 150., pT_cut, pT_hist5, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString pT_hist6 = Form("LD2_jpsi_x2_st3_pT%d", i);
        GetAcceptance("x2_st3", -150., 0., pT_cut, pT_hist6, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString pT_hist7 = Form("LD2_jpsi_y1_st3_pT%d", i);
        GetAcceptance("y1_st3", -150., 150., pT_cut, pT_hist7, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);

        TString pT_hist8 = Form("LD2_jpsi_y2_st3_pT%d", i);
        GetAcceptance("y2_st3", -150., 150., pT_cut, pT_hist8, "targetPos==3", jPsiCut, jPsiCut_MC, LD2_jpsi_data);
    }


//      LH2 psip plots

    for(int i = 0; i < 5; i++)
    {

//          xF bins

        TString xF_cut = Form("%.2f < xF && xF < %.2f", xF_bins[i], xF_bins[i+1]);

        TString xF_hist1 = Form("LH2_psip_x1_st1_xF%d", i);
        GetAcceptance("x1_st1", -20., 15., xF_cut, xF_hist1, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString xF_hist2 = Form("LH2_psip_x2_st1_xF%d", i);
        GetAcceptance("x2_st1", -30., 30., xF_cut, xF_hist2, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString xF_hist3 = Form("LH2_psip_y1_st1_xF%d", i);
        GetAcceptance("y1_st1", -60., 60., xF_cut, xF_hist3, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString xF_hist4 = Form("LH2_psip_y2_st1_xF%d", i);
        GetAcceptance("y2_st1", -60., 60., xF_cut, xF_hist4, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString xF_hist5 = Form("LH2_psip_x1_st3_xF%d", i);
        GetAcceptance("x1_st3", 0., 150., xF_cut, xF_hist5, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString xF_hist6 = Form("LH2_psip_x2_st3_xF%d", i);
        GetAcceptance("x2_st3", -150., 0., xF_cut, xF_hist6, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString xF_hist7 = Form("LH2_psip_y1_st3_xF%d", i);
        GetAcceptance("y1_st3", -150., 150., xF_cut, xF_hist7, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString xF_hist8 = Form("LH2_psip_y2_st3_xF%d", i);
        GetAcceptance("y2_st3", -150., 150., xF_cut, xF_hist8, "targetPos==1", psipCut, psipCut, LH2_psip_data);


//          pT bins

        TString pT_cut = Form("%.2f < pT && pT < %.2f", pT_bins[i], pT_bins[i+1]);

        TString pT_hist1 = Form("LH2_psip_x1_st1_pT%d", i);
        GetAcceptance("x1_st1", -20., 15., pT_cut, pT_hist1, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString pT_hist2 = Form("LH2_psip_x2_st1_pT%d", i);
        GetAcceptance("x2_st1", -30., 30., pT_cut, pT_hist2, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString pT_hist3 = Form("LH2_psip_y1_st1_pT%d", i);
        GetAcceptance("y1_st1", -60., 60., pT_cut, pT_hist3, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString pT_hist4 = Form("LH2_psip_y2_st1_pT%d", i);
        GetAcceptance("y2_st1", -60., 60., pT_cut, pT_hist4, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString pT_hist5 = Form("LH2_psip_x1_st3_pT%d", i);
        GetAcceptance("x1_st3", 0., 150., pT_cut, pT_hist5, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString pT_hist6 = Form("LH2_psip_x2_st3_pT%d", i);
        GetAcceptance("x2_st3", -150., 0., pT_cut, pT_hist6, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString pT_hist7 = Form("LH2_psip_y1_st3_pT%d", i);
        GetAcceptance("y1_st3", -150., 150., pT_cut, pT_hist7, "targetPos==1", psipCut, psipCut, LH2_psip_data);

        TString pT_hist8 = Form("LH2_psip_y2_st3_pT%d", i);
        GetAcceptance("y2_st3", -150., 150., pT_cut, pT_hist8, "targetPos==1", psipCut, psipCut, LH2_psip_data);
    }


//      LD2 psip plots

    for(int i = 0; i < 5; i++)
    {

//          xF bins

        TString xF_cut = Form("%.2f < xF && xF < %.2f", xF_bins[i], xF_bins[i+1]);

        TString xF_hist1 = Form("LD2_psip_x1_st1_xF%d", i);
        GetAcceptance("x1_st1", -20., 15., xF_cut, xF_hist1, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString xF_hist2 = Form("LD2_psip_x2_st1_xF%d", i);
        GetAcceptance("x2_st1", -30., 30., xF_cut, xF_hist2, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString xF_hist3 = Form("LD2_psip_y1_st1_xF%d", i);
        GetAcceptance("y1_st1", -60., 60., xF_cut, xF_hist3, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString xF_hist4 = Form("LD2_psip_y2_st1_xF%d", i);
        GetAcceptance("y2_st1", -60., 60., xF_cut, xF_hist4, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString xF_hist5 = Form("LD2_psip_x1_st3_xF%d", i);
        GetAcceptance("x1_st3", 0., 150., xF_cut, xF_hist5, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString xF_hist6 = Form("LD2_psip_x2_st3_xF%d", i);
        GetAcceptance("x2_st3", -150., 0., xF_cut, xF_hist6, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString xF_hist7 = Form("LD2_psip_y1_st3_xF%d", i);
        GetAcceptance("y1_st3", -150., 150., xF_cut, xF_hist7, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString xF_hist8 = Form("LD2_psip_y2_st3_xF%d", i);
        GetAcceptance("y2_st3", -150., 150., xF_cut, xF_hist8, "targetPos==3", psipCut, psipCut, LD2_psip_data);


//          pT bins

        TString pT_cut = Form("%.2f < pT && pT < %.2f", pT_bins[i], pT_bins[i+1]);

        TString pT_hist1 = Form("LD2_psip_x1_st1_pT%d", i);
        GetAcceptance("x1_st1", -20., 15., pT_cut, pT_hist1, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString pT_hist2 = Form("LD2_psip_x2_st1_pT%d", i);
        GetAcceptance("x2_st1", -30., 30., pT_cut, pT_hist2, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString pT_hist3 = Form("LD2_psip_y1_st1_pT%d", i);
        GetAcceptance("y1_st1", -60., 60., pT_cut, pT_hist3, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString pT_hist4 = Form("LD2_psip_y2_st1_pT%d", i);
        GetAcceptance("y2_st1", -60., 60., pT_cut, pT_hist4, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString pT_hist5 = Form("LD2_psip_x1_st3_pT%d", i);
        GetAcceptance("x1_st3", 0., 150., pT_cut, pT_hist5, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString pT_hist6 = Form("LD2_psip_x2_st3_pT%d", i);
        GetAcceptance("x2_st3", -150., 0., pT_cut, pT_hist6, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString pT_hist7 = Form("LD2_psip_y1_st3_pT%d", i);
        GetAcceptance("y1_st3", -150., 150., pT_cut, pT_hist7, "targetPos==3", psipCut, psipCut, LD2_psip_data);

        TString pT_hist8 = Form("LD2_psip_y2_st3_pT%d", i);
        GetAcceptance("y2_st3", -150., 150., pT_cut, pT_hist8, "targetPos==3", psipCut, psipCut, LD2_psip_data);
    }
*/


}
