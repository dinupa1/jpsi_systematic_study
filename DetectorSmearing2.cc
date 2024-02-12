/*
 * dinupa3@gmail.com
 * Calculating smearing effect due to detector resolution
 */
#include "../apps/RooUnfold/src/RooUnfoldResponse.h"
#include "../apps/RooUnfold/src/RooUnfoldBayes.h"
#include "DetectorSmearing.hh"

using ROOT::RDataFrame;

using namespace std;


void plot_smearing(ROOT::RDF::RNode df_MC, ROOT::RDF::RNode df_real, ROOT::RDF::RNode df_mix, TString smearing)
{
    /*
     * pT bins
     */
    auto hRecopT = df_MC.Histo1D({"hRecopT", "hRecopT", 5, pTbins}, "pT", "ReWeight");
    auto hTruepT = df_MC.Histo1D({"hTruepT", "hTruepT", 5, pTbins}, "mpT", "ReWeight");
    auto hRespT = df_MC.Histo2D({"hRespT", "hRespT", 5, pTbins, 5, pTbins}, "pT", "mpT", "ReWeight");

    RooUnfoldResponse respT(hRecopT.GetPtr(), hTruepT.GetPtr(), hRespT.GetPtr());

    /*TH1D* hRecopT = new TH1D("hRecopT", "hRecopT", 5, pTbins);
    TH1D* hTruepT = new TH1D("hTruepT", "hTruepT", 5, pTbins);
    TH2D* hRespT = new TH2D("hRespT", "hRespT", 5, pTbins, 5, pTbins);

    RooUnfoldResponse respT(hRecopT, hTruepT, hRespT);

    df_MC.Foreach([&respT] (float reco_x, float true_x, float weight_x){respT.Fill(reco_x, true_x, weight_x);}, {"pT", "mpT", "ReWeight"});*/

    auto RpT = respT.HresponseNoOverflow();

    TString save0 = Form("imgs/smearing/%s_pT_smearing.png", smearing.Data());

    TCanvas* c = new TCanvas("c", "c", 800, 800);
    hRespT->SetStats(0);

    TString title1 = Form("%s_pT_smearing", smearing.Data());
    hRespT->SetNameTitle(title1.Data(), "; reco. pT [GeV]; true pT [GeV]");
    hRespT->Draw("COLZ");
    c->SaveAs(save0.Data());

    auto hDatapT = df_real.Histo1D({"hDatapT", "hDatapT", 5, pTbins}, "pT");
    auto hMixpT = df_mix.Histo1D({"hMixpT", "hMixpT", 5, pTbins}, "pT");

    hDatapT->Add(hMixpT.GetPtr(), -1);

    double ymax_real = 1.5* hDatapT->GetMaximum();
    double ymin_real = 0.1* hDatapT->GetMinimum();
    hDatapT->SetMaximum(ymax_real);
    hDatapT->SetMinimum(ymin_real);

    RooUnfoldBayes unfold_bayes(&respT, hDatapT.GetPtr(), 4);
    auto* hUnfopT = unfold_bayes.Hreco();

    hDatapT->SetFillColorAlpha(kAzure+6, 0.35);

    hUnfopT->SetMarkerColor(kRed);
    hUnfopT->SetMarkerStyle(20);

    hDatapT->SetStats(0);
    hDatapT->Draw("HIST");
    hUnfopT->Draw("SAME E1");

    TString save1 = Form("imgs/smearing/%s_pT_unfo.png", smearing.Data());
    c->SaveAs(save1.Data());

    TH1D* DiffpT = new TH1D("DiffpT", "; pT [GeV]; fractional difference", 5, pTbins);

    for(int ii = 0; ii < 5; ii++)
    {
        double a = hUnfopT->GetBinContent(ii+1);
        double b = hDatapT->GetBinContent(ii+1);
        double s_a = hUnfopT->GetBinError(ii+1);
        double s_b = hDatapT->GetBinError(ii+1);
        double value = abs(a - b)/b;
        double error2 = (1./b)* (1./b)* s_a* s_a + (a/(b* b))* (a/(b* b))* s_b* s_b;

        DiffpT->SetBinContent(ii+1, value);
        DiffpT->SetBinError(ii+1, sqrt(error2));
    }

    DiffpT->SetMarkerColor(kRed);
    DiffpT->SetMarkerStyle(20);
    DiffpT->Draw("E1");

    TString save2 = Form("imgs/smearing/%s_pT_frac_diff.png", smearing.Data());
    c->SaveAs(save2.Data());




    /*
     * xF bins
     */
    auto hRecoxF = df_MC.Histo1D({"hRecoxF", "hRecoxF", 5, xFbins}, "xF", "ReWeight");
    auto hTruexF = df_MC.Histo1D({"hTruexF", "hTruexF", 5, xFbins}, "mxF", "ReWeight");
    auto hResxF = df_MC.Histo2D({"hResxF", "hResxF", 5, xFbins, 5, xFbins}, "xF", "mxF", "ReWeight");

    RooUnfoldResponse resxF(hRecoxF.GetPtr(), hTruexF.GetPtr(), hResxF.GetPtr());

    /*TH1D* hRecoxF = new TH1D("hRecoxF", "hRecoxF", 5, xFbins);
    TH1D* hTruexF = new TH1D("hTruexF", "hTruexF", 5, xFbins);
    TH2D* hResxF = new TH2D("hResxF", "hResxF", 5, xFbins, 5, xFbins);

    RooUnfoldResponse resxF(hRecoxF, hTruexF, hResxF);

    df_MC.Foreach([&resxF] (float reco_x, float true_x, float weight_x){resxF.Fill(reco_x, true_x, weight_x);}, {"xF", "mxF", "ReWeight"});*/

    auto RxF = resxF.HresponseNoOverflow();

    TString save3 = Form("imgs/smearing/%s_xF_smearing.png", smearing.Data());

    hResxF->SetStats(0);

    TString title2 = Form("%s_xF_smearing", smearing.Data());
    hResxF->SetNameTitle(title2.Data(), "; reco. xF; true xF");
    hResxF->Draw("COLZ");
    c->SaveAs(save3.Data());

    auto hDataxF = df_real.Histo1D({"hDataxF", "hDataxF", 5, xFbins}, "xF");
    auto hMixxF = df_mix.Histo1D({"hMixxF", "hMixxF", 5, xFbins}, "xF");

    hDataxF->Add(hMixxF.GetPtr(), -1);

    double ymax_real1 = 1.5* hDataxF->GetMaximum();
    double ymin_real1 = 0.1* hDataxF->GetMinimum();
    hDataxF->SetMaximum(ymax_real1);
    hDataxF->SetMinimum(ymin_real1);

    RooUnfoldBayes unfold_bayesxF(&resxF, hDataxF.GetPtr(), 4);
    auto* hUnfoxF = unfold_bayesxF.Hreco();

    hDataxF->SetFillColorAlpha(kAzure+6, 0.35);

    hUnfoxF->SetMarkerColor(kRed);
    hUnfoxF->SetMarkerStyle(20);

    hDataxF->SetStats(0);
    hDataxF->Draw("HIST");
    hUnfoxF->Draw("SAME E1");

    TString save4 = Form("imgs/smearing/%s_xF_unfo.png", smearing.Data());
    c->SaveAs(save4.Data());


    TH1D* DiffxF = new TH1D("DiffxF", "; xF; fractional difference", 5, pTbins);

    for(int ii = 0; ii < 5; ii++)
    {
        double a = hUnfoxF->GetBinContent(ii+1);
        double b = hDataxF->GetBinContent(ii+1);
        double s_a = hUnfoxF->GetBinError(ii+1);
        double s_b = hDataxF->GetBinError(ii+1);
        double value = abs(a - b)/b;
        double error2 = (1./b)* (1./b)* s_a* s_a + (a/(b* b))* (a/(b* b))* s_b* s_b;

        DiffxF->SetBinContent(ii+1, value);
        DiffxF->SetBinError(ii+1, sqrt(error2));
    }

    DiffxF->SetMarkerColor(kRed);
    DiffxF->SetMarkerStyle(20);
    DiffxF->Draw("E1");

    TString save5 = Form("imgs/smearing/%s_xF_frac_diff.png", smearing.Data());
    c->SaveAs(save5.Data());


    delete DiffpT;
    delete DiffxF;
//     delete hTruepT;
//     delete hRecopT;
//     delete hRespT;
//     delete hTruexF;
//     delete hRecoxF;
//     delete hResxF;
}


void DetectorSmearing2()
{
    gSystem->Load("/seaquest/users/dinupa/seaquest/apps/RooUnfold/build/libRooUnfold.so");
    gSystem->mkdir("imgs/smearing", 1);

    TH1::SetDefaultSumw2();

    plot_smearing(df_LH2_jpsi_with_basic_cuts, df_LH2_jpsi_real_data, df_LH2_jpsi_mix_data, "LH2_jpsi");

    plot_smearing(df_LH2_psip_with_basic_cuts, df_LH2_psip_real_data, df_LH2_psip_mix_data, "LH2_psip");

    plot_smearing(df_LD2_jpsi_with_basic_cuts, df_LD2_jpsi_real_data, df_LD2_jpsi_mix_data, "LD2_jpsi");

    plot_smearing(df_LD2_psip_with_basic_cuts, df_LD2_psip_real_data, df_LD2_psip_mix_data, "LD2_psip");

}
