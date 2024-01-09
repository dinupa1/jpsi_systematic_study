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

TString input_file = "/seaquest/users/chleung/pT_ReWeight/mc_jpsi_LH2_M027_S002_clean_occ_pTxFweight_v2.root";
TString output_name = "imgs/LH2_efficiency.png";

void Efficiency()
{
    TString CcutsTemp = chuckCutsPositive_2111v42_tmp && chuckCutsNegative_2111v42_tmp && chuckCutsDimuon_2111v42 && physicsCuts_noMassCut_2111v42_tmp && jPsiCut_MC;

    auto inFile = TFile::Open(input_file.Data(), "READ");
    auto tree = (TTree*)inFile->Get("Tree");

    auto hAcc = new TH1D("hAcc", "hAcc", 16, 0.0, 400.0);
    auto hReco = new TH1D("hReco", "hAcc", 16, 0.0, 400.0);

    tree->Project("hAcc", "D2", "ReWeight"*CcutsTemp);
    tree->Project("hReco", "D2", "ReWeight"*(CcutsTemp + "&& mass > 0.0"));

    auto effi = new TEfficiency(*hReco, *hAcc);
    effi->SetMarkerColor(kAzure);
    effi->SetMarkerStyle(20);
    effi->SetMarkerSize(1);
    effi->SetName("effi");
    effi->SetTitle("effi; D1 [a.u.]; Efficiency [a.u.]");

    auto can = new TCanvas();
    effi->Draw("APE1");
    can->SaveAs(output_name.Data());

    delete effi;
    delete can;
}
