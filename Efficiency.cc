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

TString CcutsTemp = chuckCutsPositive_2111v42_tmp && chuckCutsNegative_2111v42_tmp && chuckCutsDimuon_2111v42 && physicsCuts_noMassCut_2111v42_tmp && jPsiCut_MC;

void DrawEfficiency(TString input_file, TString var_cut, TString can_name)
{
    auto inFile = TFile::Open(input_file.Data(), "READ");
    auto tree = (TTree*)inFile->Get("Tree");

    auto hAcc = new TH1D("hAcc", "hAcc", 16, 0.0, 400.0);
    auto hReco = new TH1D("hReco", "hAcc", 16, 0.0, 400.0);

    tree->Project("hAcc", "D2", "ReWeight"*(CcutsTemp + var_cut));
    tree->Project("hReco", "D2", "ReWeight"*(CcutsTemp + var_cut +"&& mass > 0.0"));

    auto effi = new TEfficiency(*hReco, *hAcc);
    effi->SetMarkerColor(kAzure);
    effi->SetMarkerStyle(20);
    effi->SetMarkerSize(1);
    effi->SetName("effi");
    effi->SetTitle("effi; D1 [a.u.]; Efficiency [a.u.]");

    auto can = new TCanvas();
    effi->Draw("APE1");
    can->Update();
    auto graph = effi->GetPaintedGraph();
    graph->SetMinimum(0.);
    graph->SetMaximum(1.2);
    can->Update();
    can->SaveAs(can_name.Data());

    delete effi;
    delete can;
}

void Efficiency()
{
    TString LH2_jpsi_file = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LH2_M027_S002_clean_occ_pTxFweight_v2.root";
    TString LD2_jpsi_file = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LD2_M027_S002_clean_occ_pTxFweight_v2.root";
    TString LH2_jpsiprime_file = "/seaquest/users/chleung/pT_ReWeight/mc_psiprime_LH2_M027_S002_clean_occ_pTxFweight_v2.root";
    TString LD2_jpsiprime_file = "/seaquest/users/chleung/pT_ReWeight/mc_psiprime_LD2_M027_S002_clean_occ_pTxFweight_v2.root";

    TString var_cuts[5] = {"&& 0.4 < xF && xF < 0.6", "&& 0.6 < xF && xF < 0.65", "&& 0.65 < xF && xF < 0.7", "&& 0.7 < xF && xF < 0.77", "&& 0.77 < xF && xF < 0.95"}

    for(int i = 0; i < 5; i++)
    {
        /*
         * jpsi efficiency for LH2 data
         */
        TString can_name = Form("imgs/LH2_jpsi_xF%d_effficiency.png", i+1);
        DrawEfficiency(LH2_jpsi_file, var_cuts[i], can_name);

        /*
         * jpsi efficiency for LD2 data
         */
        TString can_name = Form("imgs/LD2_jpsi_xF%d_effficiency.png", i+1);
        DrawEfficiency(LD2_jpsi_file, var_cuts[i], can_name);

        /*
         * jpsiprime efficiency for LH2 data
         */
        TString can_name = Form("imgs/LH2_jpsiprime_xF%d_effficiency.png", i+1);
        DrawEfficiency(LH2_jpsiprime_file, var_cuts[i], can_name);

        /*
         * jpsiprime efficiency for LH2 data
         */
        TString can_name = Form("imgs/LD2_jpsiprime_xF%d_effficiency.png", i+1);
        DrawEfficiency(LD2_jpsiprime_file, var_cuts[i], can_name);
    }
}
