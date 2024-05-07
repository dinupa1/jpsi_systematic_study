#ifndef _HODO_UTILS__H_
#define _HODO_UTILS__H_

#include <ROOT/RDataFrame.hxx>
#include <TH1D.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>

#include "chuckcuts.hh"

TString jpsi_mc_cuts = Form("%s && %s && %s && %s && %s && %s",
	chuckCutsPositive_2111v42_tmp.Data(),
	chuckCutsNegative_2111v42_tmp.Data(),
	chuckCutsDimuon_2111v42.Data(),
	physicsCuts_noMassCut_2111v42_tmp.Data(),
	occCuts_2111v42_Run56.Data(),
	jPsiCut_MC.Data()); 

TString psip_mc_cuts = Form("%s && %s && %s && %s && %s && %s",
	chuckCutsPositive_2111v42_tmp.Data(),
	chuckCutsNegative_2111v42_tmp.Data(),
	chuckCutsDimuon_2111v42.Data(),
	physicsCuts_noMassCut_2111v42_tmp.Data(),
	occCuts_2111v42_Run56.Data(),
	psipCut.Data());

TString jpsi_LH2_unmix_cuts = Form("%s && %s && %s && %s && %s && %s && %s",
	chuckCutsPositive_2111v42_tmp.Data(),
	chuckCutsNegative_2111v42_tmp.Data(),
	chuckCutsDimuon_2111v42.Data(),
	physicsCuts_noMassCut_2111v42_tmp.Data(),
	occCuts_2111v42_Run56.Data(),
	jPsiCut.Data(), "targetPos==1");

TString jpsi_LD2_unmix_cuts = Form("%s && %s && %s && %s && %s && %s && %s",
	chuckCutsPositive_2111v42_tmp.Data(),
	chuckCutsNegative_2111v42_tmp.Data(),
	chuckCutsDimuon_2111v42.Data(),
	physicsCuts_noMassCut_2111v42_tmp.Data(),
	occCuts_2111v42_Run56.Data(),
	jPsiCut.Data(), "targetPos==3");

TString psip_LH2_unmix_cuts = Form("%s && %s && %s && %s && %s && %s && %s",
	chuckCutsPositive_2111v42_tmp.Data(),
	chuckCutsNegative_2111v42_tmp.Data(),
	chuckCutsDimuon_2111v42.Data(),
	physicsCuts_noMassCut_2111v42_tmp.Data(),
	occCuts_2111v42_Run56.Data(),
	psipCut.Data(), "targetPos==1");

TString psip_LD2_unmix_cuts = Form("%s && %s && %s && %s && %s && %s && %s",
	chuckCutsPositive_2111v42_tmp.Data(),
	chuckCutsNegative_2111v42_tmp.Data(),
	chuckCutsDimuon_2111v42.Data(),
	physicsCuts_noMassCut_2111v42_tmp.Data(),
	occCuts_2111v42_Run56.Data(),
	psipCut.Data(), "targetPos==3");

TString jpsi_mix_cuts = Form("%s && %s && %s && %s && %s && %s",
	chuckCutsPositive_2111v42_tmp.Data(),
	chuckCutsNegative_2111v42_tmp.Data(),
	chuckCutsDimuon_2111v42.Data(),
	physicsCuts_noMassCut_2111v42_tmp.Data(),
	"(targetPos==1 || targetPos==3)",
	jPsiCut.Data()); // occupancy cut is not applied

TString psip_mix_cuts = Form("%s && %s && %s && %s && %s && %s",
	chuckCutsPositive_2111v42_tmp.Data(),
	chuckCutsNegative_2111v42_tmp.Data(),
	chuckCutsDimuon_2111v42.Data(),
	physicsCuts_noMassCut_2111v42_tmp.Data(),
	"(targetPos==1 || targetPos==3)",
	psipCut.Data());


void SaveTree(TString nameIn, TString nameOut, TString cuts)
{
	ROOT::RDataFrame df0("Tree", nameIn.Data());
	auto df1 = df0.Filter(cuts.Data());
	auto df2 = df1.Define("pT", "sqrt(dpx* dpx + dpy* dpy)")
	.Define("z_st1", "600.")
	.Define("z_st3", "1910.")
	.Define("z_H1X", "669.23200")
	.Define("z_H2X", "1421.1100")
	.Define("z_H3X", "1958.6200")
	.Define("z_H4X", "2243.0000")
	.Define("x1_H1X", "x1_st1 + (px1_st1/pz1_st1)* (z_H1X - z_st1)")
	.Define("x2_H1X", "x2_st1 + (px2_st1/pz2_st1)* (z_H1X - z_st1)")
	.Define("x1_H2X", "x1_st3 + (px1_st3/pz1_st3)* (z_H2X - z_st3)")
	.Define("x2_H2X", "x2_st3 + (px2_st3/pz2_st3)* (z_H2X - z_st3)")
	.Define("x1_H3X", "x1_st3 + (px1_st3/pz1_st3)* (z_H3X - z_st3)")
	.Define("x2_H3X", "x2_st3 + (px2_st3/pz2_st3)* (z_H3X - z_st3)")
	.Define("x1_H4X", "x1_st3 + (px1_st3/pz1_st3)* (z_H4X - z_st3)")
	.Define("x2_H4X", "x2_st3 + (px2_st3/pz2_st3)* (z_H4X - z_st3)");
	df2.Snapshot("tree", nameOut.Data());
}

void SaveHist(TString fmc, TString funmix, TString fmix, TString hname, TString kinematics, TString bname)
{
	TString nmc = Form("%s_mc", hname.Data());
	TString nunmix = Form("%s_unmix", hname.Data());
	TString nmix = Form("%s_mix", hname.Data());

	int nbins = 30;

	TString title = Form("%s; %s; normalized to unity", kinematics.Data(), bname.Data());

	ROOT::RDataFrame df_mc("tree", fmc.Data());
	ROOT::RDataFrame df_unmix("tree", funmix.Data());
	ROOT::RDataFrame df_mix("tree", fmix.Data());

	auto df_mc1 = df_mc.Filter(kinematics.Data());
	auto df_unmix1 = df_unmix.Filter(kinematics.Data());
	auto df_mix1 = df_mix.Filter(kinematics.Data());

	auto xmin = df_unmix1.Min(bname.Data());
	auto xmax = df_unmix1.Max(bname.Data());

	auto h_mc = df_mc1.Histo1D({nmc.Data(), title.Data(), nbins, *xmin, *xmax}, bname.Data(), "ReWeight");
	auto h_unmix = df_unmix1.Histo1D({nunmix.Data(), title.Data(), nbins, *xmin, *xmax}, bname.Data());
	auto h_mix = df_mix1.Histo1D({nmix.Data(), title.Data(), nbins, *xmin, *xmax}, bname.Data());

	h_mc->Sumw2();
    h_unmix->Sumw2();
    h_mix->Sumw2();

	TString nsub = Form("%s_unmix_sub_mix", hname.Data());

	auto h_sub = (TH1D*)h_unmix.GetPtr()->Clone(nsub.Data());

	h_sub->Add(h_mix.GetPtr(), -1);

	h_sub->Scale(1./h_sub->Integral());
    h_mc->Scale(1./h_mc->Integral());

    h_sub->SetFillColor(kAzure+1);

    h_mc->SetLineStyle(2);
    h_mc->SetLineColor(kMagenta+1);
    h_mc->SetLineWidth(2);

    double ymax = 1.5* h_sub->GetMaximum();
    h_sub->SetMaximum(ymax);

    auto can = new TCanvas(hname.Data(), hname.Data(), 800, 800);

    h_sub->Draw("HIST");
    h_mc->Draw("SAME HIST");

    can->Update();

    TString pname = Form("imgs/hodo/%s.png", hname.Data());

    can->SaveAs(pname.Data());

    h_sub->Write();
    h_mc->Write();


    TString nratio = Form("%s_ratio", hname.Data());

	auto h_ratio = (TH1D*)h_sub->Clone(nratio.Data());

	// h_ratio->Add(h_mix.GetPtr(), -1);

	// h_ratio->Scale(1./h_ratio->Integral());

	h_ratio->Divide(h_mc.GetPtr());

	TString title2 = Form("%s; %s ; (unmix - mix)/messy MC", kinematics.Data(), bname.Data());

	h_ratio->SetTitle(title2.Data());

	h_ratio->SetMarkerStyle(21);
	h_ratio->SetMarkerColor(kMagenta+1);

	h_ratio->SetMaximum(6.);
	h_ratio->SetMinimum(-1.);

	h_ratio->Draw("E1");

	TString pname2 = Form("imgs/hodo/%s.png", nratio.Data());
    can->SaveAs(pname2.Data());

    h_ratio->Write();


    // delete h_mc;
    // delete h_unmix;
    // delete h_mix;
    delete h_sub;
    delete h_ratio;
    delete can;
}

#endif /* _HODO_UTILS__H_ */