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
    TString hTrue = Form("hist_m%s_%s", variable.Data(), smearing.Data());
    TString hReco = Form("hist_%s_%s", variable.Data(), smearing.Data());
    TString tTitle = Form("; True %s; counts", variable.Data());
    TString rTitle = Form("; Reco. %s; counts", variable.Data());
    TString variableTrue = Form("m%s", variable.Data());
    TString smearingTitle = Form("Smearing ; True %s; Reco. %s", variable.Data(), variable.Data());

    auto df_with_kinematics = dataFrame.Filter(kinematics.Data());

    auto xmin = df_with_kinematics.Min(variable.Data());
    auto xmax = df_with_kinematics.Max(variable.Data());

    auto hist_true = df_with_kinematics.Histo1D({hTrue.Data(), tTitle.Data(), bins, *xmin, *xmax}, variableTrue.Data(), "ReWeight");
    auto hist_reco = df_with_kinematics.Histo1D({hReco.Data(), rTitle.Data(), bins, *xmin, *xmax}, variable.Data(), "ReWeight");

    auto hist_smearing = new TH2D(smearing.Data(), smearingTitle.Data(), bins, *xmin, *xmax, bins, *xmin, *xmax);

    RooUnfoldResponse smearing_matrix(hist_reco.GetPtr(), hist_true.GetPtr(), smearing.Data(), smearingTitle.Data());

    auto R = smearing_matrix.HresponseNoOverflow();
    auto c1 = new TCanvas();
    R->SetStats(0);
    R->Draw("COLZ");
    c1->Draw();
    c1->SaveAs("smearing.png");
}

void DetectorSmearing()
{
    plotSmearing(df_LH2_jpsi_with_basic_cuts, "Mass", "0.4 < xF < 0.6", "smearing_xF0");
}
