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

int bins = 10;


void plot_smearing(ROOT::RDF::RNode df_MC, ROOT::RDF::RNode df_real, ROOT::RDF::RNode df_mix, TString varTrue, TString varReco, TString kinematics, TString smearing, double &x_mean, double &x_error)
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

    double ymax_real = 1.5* hist_real->GetMaximum();
    double ymin_real = 0.1* hist_real->GetMinimum();
    hist_real->SetMaximum(ymax_real);
    hist_real->SetMinimum(ymin_real);


    RooUnfoldBayes unfold_bayes(&sMatrix, hist_real.GetPtr(), 4);
    auto* hUnfo = unfold_bayes.Hreco();

    hist_real->SetFillColorAlpha(kAzure+6, 0.35);

    auto c2 = new TCanvas("c2", "c2", 800, 800);
    c2->Divide(1, 3);
    c2->cd(1);

    hUnfo->SetMarkerColor(kRed);
    hUnfo->SetMarkerStyle(20);

    hist_real->SetStats(0);
    hist_real->Draw("HIST");
    hUnfo->Draw("SAME E1");

    c2->cd(2);

    auto hUnfo0 = new TH1D("hUnfo0", "hUnfo0", bins, *xmin, *xmax);

    auto unfo_sumw2 = hUnfo->GetSumw2();
    auto real_sumw2 = hist_real->GetSumw2();

    for(int ii = 0; ii < bins; ii++)
    {
        double a = hUnfo->GetBinContent(ii+1);
        double b = hist_real->GetBinContent(ii+1);
        double s_a = hUnfo->GetBinError(ii+1);
        double s_b = hist_real->GetBinError(ii+1);
        double value = abs(a - b)/b;
        double error2 = (1./b)* (1./b)* s_a* s_a + (a/(b* b))* (a/(b* b))* s_b* s_b;

        hUnfo0->SetBinContent(ii+1, value);
        hUnfo0->SetBinError(ii+1, sqrt(error2));
    }

    // get the integral
    double error1, error2;
    double area1 = hUnfo->IntegralAndError(1, 10, error1);
    double area2 = hist_real->IntegralAndError(1, 10, error2);
    x_mean = abs(area1 - area2)/area2;
    double x_error2 = (1./area2)* (1./area2)* error1* error1 + (area1/(area2* area2))* (area1/(area2* area2))* error2* error2;
    x_error = sqrt(x_error2);

//     cout << "---> x mean " << x_mean << " error " << x_error << endl;


//     hUnfo0->Add(hist_real.GetPtr(), -1);

    TString unfo_title = Form("; %s; abs(unfold - raw)/raw", varReco.Data());

//     TString save2 = Form("imgs/smearing/unfo_%s.png", smearing.Data());

    hUnfo0->SetNameTitle("hUnfo", unfo_title.Data());
    hUnfo0->SetFillColorAlpha(kAzure+6, 0.35);

    hUnfo0->SetStats(0);
    hUnfo0->Draw("E4");

    c2->cd(3);

    auto hUnfo1 = new TH1D("hUnfo1", "hUnfo1", bins, *xmin, *xmax);

    for(int ii = 0; ii < bins; ii++)
    {
        double a = hUnfo->GetBinContent(ii+1);
        double b = hist_real->GetBinContent(ii+1);
        double s_a = hUnfo->GetBinError(ii+1);
        double s_b = hist_real->GetBinError(ii+1);
        double value = (a - b)/b;
        double error2 = (1./b)* (1./b)* s_a* s_a + (a/(b* b))* (a/(b* b))* s_b* s_b;

        hUnfo1->SetBinContent(ii+1, value);
        hUnfo1->SetBinError(ii+1, sqrt(error2));
    }


//     hUnfo0->Add(hist_real.GetPtr(), -1);

    TString unfo_title1 = Form("; %s; unfold - raw/raw", varReco.Data());

    TString save2 = Form("imgs/smearing/unfo_%s.png", smearing.Data());

    hUnfo1->SetNameTitle("hUnfo", unfo_title1.Data());
    hUnfo1->SetMarkerColor(kRed);
    hUnfo1->SetMarkerStyle(21);

    hUnfo1->SetStats(0);
    hUnfo1->Draw("E1");

    c2->Update();
    c2->SaveAs(save2.Data());


    delete hUnfo0;
    delete hUnfo1;
}


void DetectorSmearing()
{
    gSystem->mkdir("imgs/smearing", 1);

    TH1::SetDefaultSumw2();

    double LH2_jpsi_pT_yeild[5];
    double LH2_jpsi_pT_yeildE[5];
    double LH2_jpsi_xF_yeild[5];
    double LH2_jpsi_xF_yeildE[5];

    double LH2_psip_pT_yeild[5];
    double LH2_psip_pT_yeildE[5];
    double LH2_psip_xF_yeild[5];
    double LH2_psip_xF_yeildE[5];

    double LD2_jpsi_pT_yeild[5];
    double LD2_jpsi_pT_yeildE[5];
    double LD2_jpsi_xF_yeild[5];
    double LD2_jpsi_xF_yeildE[5];

    double LD2_psip_pT_yeild[5];
    double LD2_psip_pT_yeildE[5];
    double LD2_psip_xF_yeild[5];
    double LD2_psip_xF_yeildE[5];

    for(int j = 0; j < 5; j++) // kinematics
    {
        TString xF_cuts = Form("%.2f < xF && xF < %.2f", xFbins[j], xFbins[j+1]);
        TString pT_cuts = Form("%.2f < pT && pT < %.2f", pTbins[j], pTbins[j+1]);

        //
        // LH2 jpsi
        //
        TString LH2_jpsi_xF = Form("smearing_LH2_jpsi_%s_xF%d", "pT", j);
        plot_smearing(df_LH2_jpsi_with_basic_cuts, df_LH2_jpsi_real_data, df_LH2_jpsi_mix_data, varTrues[0], varRecos[0], xF_cuts, LH2_jpsi_xF, LH2_jpsi_xF_yeild[j], LH2_jpsi_xF_yeildE[j]);


        TString LH2_jpsi_pT = Form("smearing_LH2_jpsi_%s_pT%d", "xF", j);
        plot_smearing(df_LH2_jpsi_with_basic_cuts, df_LH2_jpsi_real_data, df_LH2_jpsi_mix_data, varTrues[1], varRecos[1], pT_cuts, LH2_jpsi_pT, LH2_jpsi_pT_yeild[j], LH2_jpsi_pT_yeildE[j]);

        //
        // LD2 jpsi
        //
        TString LD2_jpsi_xF = Form("smearing_LD2_jpsi_%s_xF%d", "pT", j);
        plot_smearing(df_LD2_jpsi_with_basic_cuts, df_LD2_jpsi_real_data, df_LD2_jpsi_mix_data, varTrues[0], varRecos[0], xF_cuts, LD2_jpsi_xF, LD2_jpsi_xF_yeild[j], LD2_jpsi_xF_yeildE[j]);

        TString LD2_jpsi_pT = Form("smearing_LD2_jpsi_%s_pT%d", "xF", j);
        plot_smearing(df_LD2_jpsi_with_basic_cuts, df_LD2_jpsi_real_data, df_LD2_jpsi_mix_data, varTrues[1], varRecos[1], pT_cuts, LD2_jpsi_pT, LD2_jpsi_pT_yeild[j], LD2_jpsi_pT_yeildE[j]);


        //
        // LH2 psip
        //
        TString LH2_psip_xF = Form("smearing_LH2_psip_%s_xF%d", "pT", j);
        plot_smearing(df_LH2_psip_with_basic_cuts, df_LH2_psip_real_data, df_LH2_psip_mix_data, varTrues[0], varRecos[0], xF_cuts, LH2_psip_xF, LH2_psip_xF_yeild[j], LH2_psip_xF_yeildE[j]);

        TString LH2_psip_pT = Form("smearing_LH2_psip_%s_pT%d", "xF", j);
        plot_smearing(df_LH2_psip_with_basic_cuts, df_LH2_psip_real_data, df_LH2_psip_mix_data, varTrues[1], varRecos[1], pT_cuts, LH2_psip_pT, LH2_psip_pT_yeild[j], LH2_psip_pT_yeildE[j]);

        //
        // LD2 psip
        //
        TString LD2_psip_xF = Form("smearing_LD2_psip_%s_xF%d", "pT", j);
        plot_smearing(df_LD2_psip_with_basic_cuts, df_LD2_psip_real_data, df_LD2_psip_mix_data, varTrues[0], varRecos[0], xF_cuts, LD2_psip_xF, LD2_psip_xF_yeild[j], LD2_psip_xF_yeildE[j]);


        TString LD2_psip_pT = Form("smearing_LD2_psip_%s_pT%d", "xF", j);
        plot_smearing(df_LD2_psip_with_basic_cuts, df_LD2_psip_real_data, df_LD2_psip_mix_data, varTrues[1], varRecos[1], pT_cuts, LD2_psip_pT, LD2_psip_pT_yeild[j], LD2_psip_pT_yeildE[j]);

    }


    auto c3 = new TCanvas("c3", "c3", 800, 800);

    auto hLH2_jpsi_pT_yield = new TH1D("hLH2_jpsi_pT_yield", "; pT [GeV]; abs. yield dif. ratio [a.u.]", 5, pTbins);
    auto hLH2_jpsi_xF_yield = new TH1D("hLH2_jpsi_xF_yield", "; xF [a.u.]; abs. yield dif. ratio [a.u.]", 5, xFbins);
    auto hLH2_psip_pT_yield = new TH1D("hLH2_psip_pT_yield", "; pT [GeV]; abs. yield dif. ratio [a.u.]", 5, pTbins);
    auto hLH2_psip_xF_yield = new TH1D("hLH2_psip_xF_yield", "; xF [a.u.]; abs. yield dif. ratio [a.u.]", 5, xFbins);

    auto hLD2_jpsi_pT_yield = new TH1D("hLD2_jpsi_pT_yield", "; pT [GeV]; abs. yield dif. ratio [a.u.]", 5, pTbins);
    auto hLD2_jpsi_xF_yield = new TH1D("hLD2_jpsi_xF_yield", "; xF [a.u.]; abs. yield dif. ratio [a.u.]", 5, xFbins);
    auto hLD2_psip_pT_yield = new TH1D("hLD2_psip_pT_yield", "; pT [GeV]; abs. yield dif. ratio [a.u.]", 5, pTbins);
    auto hLD2_psip_xF_yield = new TH1D("hLD2_psip_xF_yield", "; xF [a.u.]; abs. yield dif. ratio [a.u.]", 5, xFbins);

    for(int ii = 0; ii < 5; ii++)
    {
        hLH2_jpsi_pT_yield->SetBinContent(ii+1, LH2_jpsi_pT_yeild[ii]);
        hLH2_jpsi_pT_yield->SetBinError(ii+1, LH2_jpsi_pT_yeildE[ii]);
        hLH2_jpsi_xF_yield->SetBinContent(ii+1, LH2_jpsi_xF_yeild[ii]);
        hLH2_jpsi_xF_yield->SetBinError(ii+1, LH2_jpsi_xF_yeildE[ii]);

        hLH2_psip_pT_yield->SetBinContent(ii+1, LH2_psip_pT_yeild[ii]);
        hLH2_psip_pT_yield->SetBinError(ii+1, LH2_psip_pT_yeildE[ii]);
        hLH2_psip_xF_yield->SetBinContent(ii+1, LH2_psip_xF_yeild[ii]);
        hLH2_psip_xF_yield->SetBinError(ii+1, LH2_psip_xF_yeildE[ii]);

        hLD2_jpsi_pT_yield->SetBinContent(ii+1, LD2_jpsi_pT_yeild[ii]);
        hLD2_jpsi_pT_yield->SetBinError(ii+1, LD2_jpsi_pT_yeildE[ii]);
        hLD2_jpsi_xF_yield->SetBinContent(ii+1, LD2_jpsi_xF_yeild[ii]);
        hLD2_jpsi_xF_yield->SetBinError(ii+1, LD2_jpsi_xF_yeildE[ii]);

        hLD2_psip_pT_yield->SetBinContent(ii+1, LD2_psip_pT_yeild[ii]);
        hLD2_psip_pT_yield->SetBinError(ii+1, LD2_psip_pT_yeildE[ii]);
        hLD2_psip_xF_yield->SetBinContent(ii+1, LD2_psip_xF_yeild[ii]);
        hLD2_psip_xF_yield->SetBinError(ii+1, LD2_psip_xF_yeildE[ii]);

//         cout << LH2_jpsi_pT_yeild[ii] << endl;
    }


    TH1D* hYield[8] = {hLH2_jpsi_pT_yield, hLH2_jpsi_xF_yield, hLH2_psip_pT_yield, hLH2_psip_xF_yield, hLD2_jpsi_pT_yield, hLD2_jpsi_xF_yield, hLD2_psip_pT_yield, hLD2_psip_xF_yield};

    for(int ii = 0; ii < 8; ii++)
    {
        hYield[ii]->SetMarkerColor(kRed);
        hYield[ii]->SetMarkerStyle(21);
        hYield[ii]->SetStats(0);

        TString save = Form("imgs/smearing/%s.png", hYield[ii]->GetName());
        hYield[ii]->Draw("E1");

        c3->Update();
        c3->SaveAs(save.Data());
    }
}
