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

void GetAcceptance(TString var_name, double xmin, double xmax, TString hist_name, TString hist_title)
{
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

    TString hReal_name = Form("%s_real", hist_name.Data());
    TString hMix_name = Form("%s_mix", hist_name.Data());
    TString hJpsi_name = Form("%s_mc", hist_name.Data());

    auto hReal = new TH1D(hReal_name.Data(), hist_title.Data(), 50, xmin, xmax);
    auto hMix = new TH1D(hMix_name.Data(), hist_title.Data(), 50, xmin, xmax);
    auto hJpsi = new TH1D(hJpsi_name.Data(), hist_title.Data(), 50, xmin, xmax);

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

    TString save_name = Form("imgs/%s.png", hist_name.Data());

    auto can = new TCanvas();

    hReal->SetMaximum(1.5* hReal->GetMaximum());
    hReal->Draw("E1");
    hJpsi->Draw("SAME E1");

    can->SaveAs(save_name.Data());
}


void ChamberAcceptance()
{
    GetAcceptance("x1_st1", -30., 20., "hist_x1_st1", "; x1_st1 [cm]; normalized to unity [a.u.]");
    GetAcceptance("x2_st1", -30., 30., "hist_x2_st1", "; x2_st1 [cm]; normalized to unity [a.u.]");
    GetAcceptance("y1_st1", -60., 60., "hist_y1_st1", "; y1_st1 [cm]; normalized to unity [a.u.]");
    GetAcceptance("y2_st1", -60., 60., "hist_y2_st1", "; y2_st1 [cm]; normalized to unity [a.u.]");

    GetAcceptance("x1_st3", 0., 150., "hist_x1_st3", "; x1_st3 [cm]; normalized to unity [a.u.]");
    GetAcceptance("x2_st3", -150., 0., "hist_x2_st3", "; x2_st3 [cm]; normalized to unity [a.u.]");
    GetAcceptance("y1_st3", -150., 150., "hist_y1_st3", "; y1_st3 [cm]; normalized to unity [a.u.]");
    GetAcceptance("y2_st3", -150., 150., "hist_y2_st3", "; y2_st3 [cm]; normalized to unity [a.u.]");
}
