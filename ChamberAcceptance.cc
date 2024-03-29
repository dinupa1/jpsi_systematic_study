/*
 * dinupa3@gmail.com
 * Apply chuckcuts to data and make simple root file
 */

#include<TString.h>
#include<iostream>

#include "ChamberAcceptance.hh"

using namespace ROOT;

int nbins = 50;

void GetAcceptance(TString var_name, TString kinematics, TString hist_name, TString target, TString particle, TString particle_MC, TString data_MC)
{
    SaveDataFrames();

    gStyle->SetOptStat(0);

    TString mc_cuts = Form("%s && %s", particle_MC.Data(), kinematics.Data());
    TString real_cuts = Form("%s && %s && %s", particle.Data(), target.Data(), kinematics.Data());
    TString mix_cuts = Form("%s && %s", particle.Data(), kinematics.Data());

    RDataFrame df_real_with_basic_cuts("Tree", "data/e906_data.root");
    RDataFrame df_mix_with_basic_cuts("Tree", "data/e906_mix_data.root");
    RDataFrame df_mc_with_basic_cuts("Tree", data_MC.Data());

    auto df_real = df_real_with_basic_cuts.Filter(real_cuts.Data());
    auto df_mix = df_mix_with_basic_cuts.Filter(mix_cuts.Data());
    auto df_mc = df_mc_with_basic_cuts.Filter(mc_cuts.Data());

    auto xmin = df_real.Min(var_name.Data());
    auto xmax = df_real.Max(var_name.Data());


    TString hReal_name = Form("hist_%s_real", hist_name.Data());
    TString hMix_name = Form("hist_%s_mix", hist_name.Data());
    TString hJpsi_name = Form("hist_%s_jpsi", hist_name.Data());

    TString hist_title = Form("; (%s) %s [cm]; normalized to unity [a.u.]", kinematics.Data(), var_name.Data());

    auto hReal = df_real.Histo1D({hReal_name.Data(), hist_title.Data(), nbins, *xmin, *xmax}, var_name.Data());
    auto hMix = df_mix.Histo1D({hMix_name.Data(), hist_title.Data(), nbins, *xmin, *xmax}, var_name.Data());
    auto hMC = df_mc.Histo1D({hMix_name.Data(), hist_title.Data(), nbins, *xmin, *xmax}, var_name.Data(), "ReWeight");


    hReal->Sumw2();
    hMix->Sumw2();
    hMC->Sumw2();

    hReal->Add(hMix.GetPtr(), -1);

    hReal->Scale(1./hReal->Integral());
    hMC->Scale(1./hMC->Integral());

    hReal->SetFillColor(kViolet-9);

    hMC->SetMarkerColor(kSpring);
    hMC->SetMarkerStyle(20);

    double ymax = 1.5* hReal->GetMaximum();
    hReal->SetMaximum(ymax);

    auto can = new TCanvas();

    TString hist_save = Form("imgs/%s.png", hist_name.Data());

    hReal->Draw("HIST");
    hMC->Draw("E1 SAME");
    can->Update();

//     double mean = hMC->GetMean();
//     double fwhm = 2.355*hMC->GetRMS();
//
//     double fiducial_min = mean - fwhm;
//     double fiducial_max = mean + fwhm;
//
//     TString fiducial_cuts = Form("%s for %s --> %.1f < %s < %.1f", kin_cuts.Data(), target.Data(), fiducial_min, var_name.Data(), fiducial_max);

//     TText* t = new TText(.4, .95, fiducial_cuts.Data());
//     t->SetNDC();
//     t->SetTextAlign(22);
//     t->SetTextColor(kRed+2);
//     t->SetTextFont(43);
//     t->SetTextSize(20);
//     t->Draw();
//     can->Update();

//     TLine* l_min = new TLine(fiducial_min, hReal->GetBinContent(hReal->GetMinimumBin()), fiducial_min,  hReal->GetBinContent(hReal->GetMaximumBin()));
//     l_min->SetLineColor(kRed);
//     l_min->Draw();
//     TLine* l_max = new TLine(fiducial_max, hReal->GetBinContent(hReal->GetMinimumBin()), fiducial_max,  hReal->GetBinContent(hReal->GetMaximumBin()));
//     l_max->SetLineColor(kRed);
//     l_max->Draw();
//     can->Update();
    can->SaveAs(hist_save.Data());

//     cout << fiducial_cuts.Data() << endl;


    /*
     * Ratio plots
     */
    TString ratio_name = Form("ratio_%s", hist_name.Data());
    TString ratio_title = Form("Ratio ; (%s) %s [cm]; #frac{MC data}{Real data} [a.u.]", kinematics.Data(), var_name.Data());

    auto ratio_plot = new TH1D(ratio_name.Data(), ratio_title.Data(), nbins, *xmin, *xmax);

    /*
     * Fractional deviation
     */
    TString fractional_name = Form("frac_%s", hist_name.Data());
    TString fractional_title = Form("Fractional deviation; (%s) %s [cm]; #frac{Real data - MC data}{MC data}", kinematics.Data(), var_name.Data());

    auto fractional_plot = new TH1D(fractional_name.Data(), fractional_title.Data(), nbins, *xmin, *xmax);

    for(int i = 0; i < nbins; i++)
    {
        double real_c = hReal->GetBinContent(i+1);
        double real_e = hReal->GetBinError(i+1);

        double mc_c = hMC->GetBinContent(i+1);
        double mc_e = hMC->GetBinError(i+1);

        double ratio_e2 = (1./real_c)* (1./real_c)* mc_e* mc_e + (mc_c/(real_c* real_c))* (mc_c/(real_c* real_c))* real_e* real_e;

        if(sqrt(ratio_e2) < 10.)
        {
            ratio_plot->SetBinContent(i+1, mc_c/real_c);
            ratio_plot->SetBinError(i+1, sqrt(ratio_e2));
        }

        /*
         * Fractional deviation
         */

        double fraction_e2 = (real_c/(mc_c * mc_c))* (real_c/(mc_c * mc_c))* real_e* real_e + (1./real_c)* (1./real_c)* mc_e* mc_e;

        if(sqrt(fraction_e2) < 10.)
        {
            fractional_plot->SetBinContent(i+1, (real_c - mc_c)/mc_c);
            fractional_plot->SetBinError(i+1, sqrt(fraction_e2));
        }

    }

    ratio_plot->SetMarkerColor(kViolet);
    ratio_plot->SetMarkerStyle(20);

    TString ratio_save = Form("imgs/ratio_%s.png", hist_name.Data());

    ratio_plot->Draw("E1");
    can->Update();

//     TLine* l_min1 = new TLine(fiducial_min, ratio_plot->GetBinContent(ratio_plot->GetMinimumBin()), fiducial_min,  ratio_plot->GetBinContent(ratio_plot->GetMaximumBin()));
//     l_min1->SetLineColor(kRed);
//     l_min1->Draw();
//     TLine* l_max1 = new TLine(fiducial_max, ratio_plot->GetBinContent(ratio_plot->GetMinimumBin()), fiducial_max,  ratio_plot->GetBinContent(ratio_plot->GetMaximumBin()));
//     l_max1->SetLineColor(kRed);
//     l_max1->Draw();
//     can->Update();
    can->SaveAs(ratio_save.Data());

    /*
     * Fractional deviation
     */

    fractional_plot->SetMarkerColor(kViolet);
    fractional_plot->SetMarkerStyle(20);

    TString fractional_save = Form("imgs/fractional_%s.png", hist_name.Data());

    fractional_plot->Draw("E1");
    can->Update();

//     TLine* l_min2 = new TLine(fiducial_min, fractional_plot->GetBinContent(fractional_plot->GetMinimumBin()), fiducial_min,  fractional_plot->GetBinContent(fractional_plot->GetMaximumBin()));
//     l_min2->SetLineColor(kRed);
//     l_min2->Draw();
//     TLine* l_max2 = new TLine(fiducial_max, fractional_plot->GetBinContent(fractional_plot->GetMinimumBin()), fiducial_max,  fractional_plot->GetBinContent(fractional_plot->GetMaximumBin()));
//     l_max2->SetLineColor(kRed);
//     l_max2->Draw();
//     can->Update();
    can->SaveAs(fractional_save.Data());
    std::cout << hist_name.Data() << std::endl;
    std::cout << ratio_name.Data() << std::endl;
    std::cout << fractional_name.Data() << std::endl;
}

void ChamberAcceptance()
{
    double xF_bins[6] = {0.4, 0.6, 0.65, 0.7, 0.77, 0.95};
    double pT_bins[6] = {0, 0.3, 0.45, 0.65, 0.9, 1.5};
//     double xmins[12] = {-25., -35., -65., -65., -10., -85., -135., -135., 5., -155., -155., -155.};
//     double xmaxs[12] = {20., 35., 65., 65., 80., 10., 135., 135., 155., 5., 155., 155.};
    TString targets[2] = {"targetPos==1", "targetPos==3"};
    TString particles_mc[2] = {jPsiCut_MC, psipCut};
    TString particles[2] = {jPsiCut, psipCut};
    TString df_mcs[4] = {"data/e906_LH2_jpsi_MC.root", "data/e906_LH2_psip_MC.root", "data/e906_LD2_jpsi_MC.root", "data/e906_LD2_psip_MC.root"};
    TString df_names[4] = {"LH2_jpsi", "LH2_psip", "LD2_jpsi", "LD2_psip"};
    TString var_names[4] = {"x1", "x2", "y1", "y2"};
    TString stations[3] = {"st1", "st2", "st3"};
    TString target_names[2] = {"LH2", "LD2"};
    TString particle_names[2] = {"jpsi", "psip"};

    for(int i = 0; i < 2; i++) //targets
    {
        for(int j = 0; j < 2; j++) // particles
        {
            for(int ii = 0; ii < 3; ii++) // stations
            {
                for(int jj = 0; jj < 4; jj++) //variables
                {
                    for(int kk = 0; kk < 5; kk++) // kinematics
                    {
                        std::cout << "---> " << df_names[2*i + j].Data() << std::endl;

                        TString var_name = Form("%s_%s", var_names[jj].Data(), stations[ii].Data());
                        std::cout << var_name.Data() << std::endl;

                        TString xF_hist_name = Form("%s_%s_%s_xF%d", target_names[i].Data(), particle_names[j].Data(), var_name.Data(), kk);
                        std::cout << xF_hist_name.Data() << std::endl;

                        TString xF_kinematics = Form("%.2f < xF && xF < %.2f", xF_bins[kk], xF_bins[kk+1]);
                        std::cout << xF_kinematics.Data() << std::endl;

                        GetAcceptance(var_name, xF_kinematics, xF_hist_name, targets[i], particles[j], particles_mc[j], df_mcs[2* i + j]);

                        TString pT_hist_name = Form("%s_%s_%s_pT%d", target_names[i].Data(), particle_names[j].Data(), var_name.Data(), kk);
                        std::cout << pT_hist_name.Data() << std::endl;

                        TString pT_kinematics = Form("%.2f < pT && pT < %.2f", pT_bins[kk], pT_bins[kk+1]);
                        std::cout << pT_kinematics.Data() << std::endl;

                        GetAcceptance(var_name, pT_kinematics, pT_hist_name, targets[i], particles[j], particles_mc[j], df_mcs[2* i + j]);
                    }
                }
            }
        }
    }
}
