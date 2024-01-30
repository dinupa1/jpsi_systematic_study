/*
 * dinupa3@gmail.com
 * Calculating smearing effect due to detector resolution
 */

R__LOAD_LIBRARY(../apps/RooUnfold/build/libRooUnfold)

#include "../apps/RooUnfold/src/RooUnfoldResponse.h"
#include "../apps/RooUnfold/src/RooUnfoldBayes.h"
#include "DetectorSmearing.hh"

using ROOT::RDataFrame;

using namespace std;

int bins = 20;


void plot_smearing(ROOT::RDF::RNode df_MC, ROOT::RDF::RNode df_real, ROOT::RDF::RNode df_mix, TString varTrue, TString varReco, TString kinematics, TString smearing)
{
    TString smearingTitle = Form("Smearing of %s for %s ; Reco. %s; True %s", varReco.Data(), kinematics.Data(), varReco.Data(), varReco.Data());

    auto df_with_kinematics = df_MC.Filter(kinematics.Data());

    auto xmin = df_with_kinematics.Min(varReco.Data());
    auto xmax = df_with_kinematics.Max(varReco.Data());
    auto ymin = df_with_kinematics.Min(varTrue.Data());
    auto ymax = df_with_kinematics.Max(varTrue.Data());

//     TString outputs = Form("%.2f < %s < %.2f and %.2f < %s < %.2f", *xmin, varReco.Data(), *xmax, *ymin, varTrue.Data(), *ymax);

    cout << kinematics.Data() << endl;

    RooUnfoldResponse sMatrix(bins, *xmin, *xmax, bins, *xmin, *xmax);

    df_with_kinematics.Foreach([&sMatrix] (float reco_x, float true_x, float weight_x){sMatrix.Fill(reco_x, true_x, weight_x);}, {varReco.Data(), varTrue.Data(), "ReWeight"});

    auto R = sMatrix.HresponseNoOverflow();

    auto c1 = new TCanvas("c1", "c1", 800, 800);

    R->SetStats(0);
    R->SetNameTitle(smearing.Data(), smearingTitle.Data());
    R->Draw("COLZ");

    TString save = Form("imgs/smearing/%s.png", smearing.Data());

    c1->SaveAs(save.Data());

    TString hist_title = Form("; %s; Yield [a.u.]", varReco.Data());

    auto df_real_kinematics = df_real.Filter(kinematics.Data());
    auto hist_real = df_real_kinematics.Histo1D({"hist_real", hist_title.Data(), bins, *xmin, *xmax}, varReco.Data());

    auto df_mix_kinematics = df_mix.Filter(kinematics.Data());
    auto hist_mix = df_mix_kinematics.Histo1D({"hist_mix", hist_title.Data(), bins, *xmin, *xmax}, varReco.Data());

    hist_real->Add(hist_mix.GetPtr(), -1);

    RooUnfoldBayes unfold_bayes(&sMatrix, hist_real.GetPtr(), 4);
    auto* hUnfo = unfold_bayes.Hreco();

    hist_real->SetFillColorAlpha(kAzure+6, 0.35);

    auto c2 = new TCanvas("c2", "c2", 800, 800);
    c2->Divide(1, 2);
    c2->cd(1);

    hUnfo->SetMarkerColor(kRed);
    hUnfo->SetMarkerStyle(20);

    hist_real->SetStats(0);
    hist_real->Draw("HIST");
    hUnfo->Draw("SAME E1");

    c2->cd(2);

    auto hUnfo0 = (TH1D*)hUnfo->Clone();
    hUnfo0->Add(hist_real.GetPtr(), -1);

    TString unfo_title = Form("; %s; unfold - real", varReco.Data());

    TString save2 = Form("imgs/smearing/unfo_%s.png", smearing.Data());

    hUnfo0->SetNameTitle("hUnfo", unfo_title.Data());
    hUnfo0->SetFillColorAlpha(kSpring+6, 0.35);

    hUnfo0->SetStats(0);
    hUnfo0->Draw("HIST");

    c2->Update();
    c2->SaveAs(save2.Data());
}


void DetectorSmearing()
{
    gSystem->mkdir("imgs/smearing", 1);

    for(int i = 0; i < 2; i++) // variables
    {
        for(int j = 0; j < 5; j++) // kinematics
        {
            TString xF_cuts = Form("%.2f < xF && xF < %.2f", xFbins[j], xFbins[j+1]);
            TString pT_cuts = Form("%.2f < pT && pT < %.2f", pTbins[j], pTbins[j+1]);

            //
            // LH2 jpsi
            //
            TString LH2_jpsi_xF = Form("smearing_LH2_jpsi_%s_xF%d", varRecos[i].Data(), j);
            plot_smearing(df_LH2_jpsi_with_basic_cuts, df_LH2_jpsi_real_data, df_LH2_jpsi_mix_data, varTrues[i], varRecos[i], xF_cuts, LH2_jpsi_xF);


            TString LH2_jpsi_pT = Form("smearing_LH2_jpsi_%s_pT%d", varRecos[i].Data(), j);
            plot_smearing(df_LH2_jpsi_with_basic_cuts, df_LH2_jpsi_real_data, df_LH2_jpsi_mix_data, varTrues[i], varRecos[i], pT_cuts, LH2_jpsi_pT);

            //
            // LD2 jpsi
            //
            TString LD2_jpsi_xF = Form("smearing_LD2_jpsi_%s_xF%d", varRecos[i].Data(), j);
            plot_smearing(df_LD2_jpsi_with_basic_cuts, df_LD2_jpsi_real_data, df_LD2_jpsi_mix_data, varTrues[i], varRecos[i], xF_cuts, LD2_jpsi_xF);


            TString LD2_jpsi_pT = Form("smearing_LD2_jpsi_%s_pT%d", varRecos[i].Data(), j);
            plot_smearing(df_LD2_jpsi_with_basic_cuts, df_LD2_jpsi_real_data, df_LD2_jpsi_mix_data, varTrues[i], varRecos[i], pT_cuts, LD2_jpsi_pT);


            //
            // LH2 psip
            //
            TString LH2_psip_xF = Form("smearing_LH2_psip_%s_xF%d", varRecos[i].Data(), j);
            plot_smearing(df_LH2_psip_with_basic_cuts, df_LH2_psip_real_data, df_LH2_psip_mix_data, varTrues[i], varRecos[i], xF_cuts, LH2_psip_xF);

            TString LH2_psip_pT = Form("smearing_LH2_psip_%s_pT%d", varRecos[i].Data(), j);
            plot_smearing(df_LH2_psip_with_basic_cuts, df_LH2_psip_real_data, df_LH2_psip_mix_data, varTrues[i], varRecos[i], pT_cuts, LH2_psip_pT);

            //
            // LD2 psip
            //
            TString LD2_psip_xF = Form("smearing_LD2_psip_%s_xF%d", varRecos[i].Data(), j);
            plot_smearing(df_LD2_psip_with_basic_cuts, df_LD2_psip_real_data, df_LD2_psip_mix_data, varTrues[i], varRecos[i], xF_cuts, LD2_psip_xF);


            TString LD2_psip_pT = Form("smearing_LD2_psip_%s_pT%d", varRecos[i].Data(), j);
            plot_smearing(df_LD2_psip_with_basic_cuts, df_LD2_psip_real_data, df_LD2_psip_mix_data, varTrues[i], varRecos[i], pT_cuts, LD2_psip_pT);
        }
    }
}
