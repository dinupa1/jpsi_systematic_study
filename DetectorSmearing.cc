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
    TString smearingTitle = Form("Smearing ; True %s; Reco. %s", varReco.Data(), varReco.Data());

    auto df_with_kinematics = dataFrame.Filter(kinematics.Data());

    auto xmin = df_with_kinematics.Min(varReco.Data());
    auto xmax = df_with_kinematics.Max(varReco.Data());

    RooUnfoldResponse sMatrix(bins, *xmin, *xmax);

    df_with_kinematics.Foreach([&sMatrix] (float reco_x, float true_x, float weight_x){M->Fill(reco_x, true_x, weight_x);}, {varReco.Data(), varTrue.Data(), "ReWeight"});


    auto R = sMatrix.HresponseNoOverflow();
    auto c1 = new TCanvas();
    R->SetStats(0);
    R->SetNameTitle(smearing.Data(), smearingTitle.Data())
    R->Draw("COLZ");
    c1->Draw();
    c1->SaveAs("smearing.png");
}

void DetectorSmearing()
{
    plotSmearing(df_LH2_jpsi_with_basic_cuts, "mMass", "mass", "0.4 < xF && xF < 0.6", "smearing_xF0");
}
