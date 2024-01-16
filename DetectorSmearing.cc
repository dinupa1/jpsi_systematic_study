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

    auto matrix = new RooUnfoldResponse(bins, *xmin, *xmax);

//     df_with_kinematics.Foreach([] (RooUnfoldResponse* M, float x, float y, float w){M->Fill(x, y, w);}, {varTrue.Data(), varReco.Data(), "ReWeight"});

    auto sMatrix = df_with_kinematics.Fill(matrix, {varReco.Data(), varTrue.Data(), "ReWeight"});

    auto R = sMatrix.GetPtr()->HresponseNoOverflow();
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
