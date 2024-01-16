/*
 * dinupa3@gmail.com
 * Calculating smearing effect due to detector resolution
 */

R_LOAD_LIBRARY(../apps/RooUnfold/build/libRooUnfold)

#include "../apps/RooUnfold/src/RooUnfoldResponse.h"

#include "DetectorSmearing.hh"

using namespace ROOT;

using namespace std;

int bins = 20;


void plotSmearing(ROOT::RDF::RNode dataFrame, TString variable, TString kinematics, TString smearing)
{
    TString variableTrue = Form("m%s", variable.Data());
    TString smearingTitle = Form("Smearing ; True %s; Reco. %s", variable.Data(), variable.Data());

    auto df_with_kinematics = dataFrame.Filter(kinematics.Data());

    auto xmin = df_with_kinematics.Min(variable.Data());
    auto xmax = df_with_kinematics.Max(variable.Data());

    RooUnfoldResponse matrix(bins, *xmin, *xmax);

    auto sMatrix = df_with_kinematics.Fill(matrix, {variable.Data(), variableTrue.Data(), "ReWeight"});

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
    plotSmearing(df_LH2_jpsi_with_basic_cuts, "Mass", "0.4 < xF < 0.6", "smearing_xF0");
}
