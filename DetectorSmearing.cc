/*
 * dinupa3@gmail.com
 * Calculating smearing effect due to detector resolution
 */
R__LOAD_LIBRARY(../apps/RooUnfold/build/libRooUnfold)

// #include "../apps/RooUnfold/src/RooUnfoldResponse.h"

#include "DetectorSmearing.hh"

using namespace ROOT;

using namespace std;

int bins = 20;


void plotSmearing(ROOT::RDF::RNode dataFrame, TString varTrue, TString varReco, TString kinematics, TString smearing)
{
    TString smearingTitle = Form("Smearing of %s for %s ; Reco. %s; True %s", varReco.Data(), kinematics.Data(), varReco.Data(), varReco.Data());

    auto df_with_kinematics = dataFrame.Filter(kinematics.Data());

    auto xmin = df_with_kinematics.Min(varReco.Data());
    auto xmax = df_with_kinematics.Max(varReco.Data());
    auto ymin = df_with_kinematics.Min(varTrue.Data());
    auto ymax = df_with_kinematics.Max(varTrue.Data());

    TString outputs = Form("%.2f < %s < %.2f and %.2f < %s < %.2f", *xmin - 0.1, varReco.Data(), *xmax + 0.1, *ymin - 0.1, varTrue.Data(), *ymax + 0.1);

    cout << outputs.Data() << endl;

    RooUnfoldResponse sMatrix(bins, *xmin - 0.1, *xmax + 0.1, bins, *ymin - 0.1, *ymax + 0.1);

    df_with_kinematics.Foreach([&sMatrix] (float reco_x, float true_x, float weight_x){sMatrix.Fill(reco_x, true_x, weight_x);}, {varReco.Data(), varTrue.Data(), "ReWeight"});


    auto R = sMatrix.HresponseNoOverflow();
    auto c1 = new TCanvas();
    R->SetStats(0);
    R->SetNameTitle(smearing.Data(), smearingTitle.Data());
    R->Draw("COLZ");
    c1->Draw();

    TString save = Form("imgs/smearing/%s.png", smearing.Data());

    c1->SaveAs(save.Data());
}

void DetectorSmearing()
{
    for(int i = 0; i < 3; i++) // variables
    {
        for(int j = 0; j < 5; j++) // kinematics
        {
            TString xF_cuts = Form("%.2f < xF && xF < %.2f", xFbins[j], xFbins[j+1]);
            TString pT_cuts = Form("%.2f < pT && pT < %.2f", pTbins[j], pTbins[j+1]);

            //
            // LH2 jpsi
            //
            TString LH2_jpsi_xF = Form("smearing_LH2_jpsi_%s_xF%d", varRecos[i], j);
            plotSmearing(df_LH2_jpsi_with_basic_cuts, varTrues[i], varRecos[i], xF_cuts, LH2_jpsi_xF);

            TString LH2_jpsi_pT = Form("smearing_LH2_jpsi_%s_pT%d", varRecos[i], j);
            plotSmearing(df_LH2_jpsi_with_basic_cuts, varTrues[i], varRecos[i], pT_cuts, LH2_jpsi_pT);

            //
            // LD2 jpsi
            //
            TString LD2_jpsi_xF = Form("smearing_LD2_jpsi_%s_xF%d", varRecos[i], j);
            plotSmearing(df_LD2_jpsi_with_basic_cuts, varTrues[i], varRecos[i], xF_cuts, LD2_jpsi_xF);

            TString LD2_jpsi_pT = Form("smearing_LD2_jpsi_%s_pT%d", varRecos[i], j);
            plotSmearing(df_LD2_jpsi_with_basic_cuts, varTrues[i], varRecos[i], pT_cuts, LD2_jpsi_pT);

            //
            // LH2 psip
            //
            TString LH2_psip_xF = Form("smearing_LH2_psip_%s_xF%d", varRecos[i], j);
            plotSmearing(df_LH2_psip_with_basic_cuts, varTrues[i], varRecos[i], xF_cuts, LH2_psip_xF);

            TString LH2_psip_pT = Form("smearing_LH2_psip_%s_pT%d", varRecos[i], j);
            plotSmearing(df_LH2_psip_with_basic_cuts, varTrues[i], varRecos[i], pT_cuts, LH2_psip_pT);

            //
            // LD2 psip
            //
            TString LD2_psip_xF = Form("smearing_LD2_psip_%s_xF%d", varRecos[i], j);
            plotSmearing(df_LH2_psip_with_basic_cuts, varTrues[i], varRecos[i], xF_cuts, LD2_psip_xF);

            TString LD2_psip_pT = Form("smearing_LD2_psip_%s_pT%d", varRecos[i], j);
            plotSmearing(df_LD2_psip_with_basic_cuts, varTrues[i], varRecos[i], pT_cuts, LD2_psip_pT);
        }
    }
}
