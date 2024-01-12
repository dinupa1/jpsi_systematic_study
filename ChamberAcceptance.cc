//
// dinupa3@gmail.com
//
#include<TFile.h>
#include<TTree.h>
#include<TH1D.h>
#include<TCanvas.h>
#include<TString.h>
#include<TMath.h>
#include<TEfficiency.h>
#include<iostream>

#include"/seaquest/users/chleung/include/cuts/chuckcuts.h"

using namespace std;

void GetAcceptance(TString var_name, double xmin, double xmax)
{
    int nbins = 50;

    auto can = new TCanvas();

    TString MC_cuts = chuckCutsPositive_2111v42_tmp && chuckCutsNegative_2111v42_tmp && chuckCutsDimuon_2111v42 && physicsCuts_noMassCut_2111v42_tmp && occCuts_2111v42_Run56 && jPsiCut_MC;

    TString Real_cuts = chuckCutsPositive_2111v42_tmp && chuckCutsNegative_2111v42_tmp && chuckCutsDimuon_2111v42 && physicsCuts_noMassCut_2111v42_tmp && occCuts_2111v42_Run56 && jPsiCut;

    TString Mix_cuts = chuckCutsPositive_2111v42_tmp && chuckCutsNegative_2111v42_tmp && chuckCutsDimuon_2111v42 && physicsCuts_noMassCut_2111v42_tmp && jPsiCut;


    TString real_data = "/seaquest/users/chleung/rootfiles/run56_2111v42_tmp_noPhys_D0.root";
    TString mix_data = "/seaquest/users/chleung/rootfiles/mix_FPGA4_run56_2111v42_tmp_noPhys.root";
    TString LH2_jpsi_data = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LH2_M027_S002_clean_occ_pTxFweight_v2.root";


    auto file_real = TFile::Open(real_data.Data(), "READ");
    auto tree_real = (TTree*)file_real->Get("Tree");

    auto file_mc = TFile::Open(LH2_jpsi_data.Data(), "READ");
    auto tree_mc = (TTree*)file_mc->Get("Tree");

    auto file_mix = TFile::Open(mix_data.Data(), "READ");
    auto tree_mix = (TTree*)file_mix->Get("Tree");

    TString hReal_name = Form("hist_%s_real", var_name.Data());
    TString hMix_name = Form("hist_%s_mix", var_name.Data());
    TString hJpsi_name = Form("hist_%s_mc", var_name.Data());

    TString hist_title = Form("; %s [cm]; normalized to unity [a.u.]", var_name.Data());

    /*
     * Plots
     */

    auto hReal = new TH1D(hReal_name.Data(), hist_title.Data(), nbins, xmin, xmax);
    auto hMix = new TH1D(hMix_name.Data(), hist_title.Data(), nbins, xmin, xmax);
    auto hJpsi = new TH1D(hJpsi_name.Data(), hist_title.Data(), nbins, xmin, xmax);

    hReal->Sumw2();
    hMix->Sumw2();
    hJpsi->Sumw2();

    tree_real->Project(hReal_name.Data(), var_name.Data(), Real_cuts);
    tree_mix->Project(hMix_name.Data(), var_name.Data(), Mix_cuts);
    tree_mc->Project(hJpsi_name.Data(), var_name.Data(), "ReWeight"*MC_cuts);

    hReal->Add(hMix, -1);

    hReal->Scale(1./hReal->Integral());
    hJpsi->Scale(1./hJpsi->Integral());

    hReal->SetMarkerColor(kViolet);
    hReal->SetMarkerStyle(20);

    hJpsi->SetMarkerColor(kSpring);
    hJpsi->SetMarkerStyle(20);

    TString save_name = Form("imgs/hist_%s.png", var_name.Data());

    hReal->SetMaximum(1.5* hReal->GetMaximum());
    hReal->Draw("E1");
    hJpsi->Draw("SAME E1");

    can->SaveAs(save_name.Data());


    /*
     * Ratio plots
     */
    TString ratio_name = Form("ratio_%s", var_name.Data());
    TString ratio_title = Form("Ratio ; %s [cm]; #frac{MC data}{Real data} [a.u.]", var_name.Data());

    auto ratio_plot = new TH1D(ratio_name.Data(), ratio_title.Data(), nbins, xmin, xmax);

    /*
     * Fractional deviation
     */
    TString fractional_name = Form("frac_%s", var_name.Data());
    TString fractional_title = Form("Fractional deviation; %s [cm]; #frac{Real data - MC data}{MC data}", var_name.Data());

    auto fractional_plot = new TH1D(fractional_name.Data(), fractional_title.Data(), nbins, xmin, xmax);

    for(int i = 0; i < nbins; i++)
    {
        double real_c = hReal->GetBinContent(i+1);
        double real_e = hReal->GetBinError(i+1);

        double mc_c = hJpsi->GetBinContent(i+1);
        double mc_e = hJpsi->GetBinError(i+1);

        double ratio_e2 = (1./real_c)* (1./real_c)* mc_e* mc_e + (mc_c/(real_c* real_c))* (mc_c/(real_c* real_c))* real_e* real_e;

        ratio_plot->GetBinContent(i+1, mc_c/real_c);
        ratio_plot->GetBinError(i+1, sqrt(ratio_e2));

        /*
         * Fractional deviation
         */

        double fraction_e2 = (real_c/(mc_c * mc_c))* (real_c/(mc_c * mc_c))* real_e* real_e + (1./real_c)* (1./real_c)* mc_e* mc_e;

        fractional_plot->GetBinContent(i+1, (real_c - mc_c)/mc_c);
        fractional_plot->GetBinError(i+1, sqrt(fraction_e2));

    }

    ratio_plot->SetMarkerColor(kViolet);
    ratio_plot->SetMarkerStyle(20);

    TString ratio_save = Form("imgs/ratio_%s.png", var_name.Data());

    ratio_plot->Draw("E1");
    can->SaveAs(ratio_save.Data());

    /*
     * Fractional deviation
     */

    fractional_plot->SetMarkerColor(kViolet);
    fractional_plot->SetMarkerStyle(20);

    TString fractional_save = Form("imgs/fractional_%s.png", var_name.Data());

    fractional_plot->Draw("E1");
    can->SaveAs(fractional_save.Data());

}


void ChamberAcceptance()
{
    GetAcceptance("x1_st1", -30., 20.);
    GetAcceptance("x2_st1", -30., 30.);
    GetAcceptance("y1_st1", -60., 60.);
    GetAcceptance("y2_st1", -60., 60.);

    GetAcceptance("x1_st3", 0., 150.);
    GetAcceptance("x2_st3", -150., 0.);
    GetAcceptance("y1_st3", -150., 150.);
    GetAcceptance("y2_st3", -150., 150.);
}
