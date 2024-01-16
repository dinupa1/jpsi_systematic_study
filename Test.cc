/*
 * dinupa3@gmail.com
 * Apply chuckcuts to data and make simple root file
*/

#include<iostream>

// #include "ChamberAcceptance.hh"

using namespace ROOT;


int nbins = 50;

// void PrintDf(ROOT::RDF::RNode df)
// {
//     auto display = df.Display({"x1_st1", "x1_st2", "x1_st3", "pT"});
//     display->Print();
// }


void Test()
{
//     PrintDf(df_LH2_psip_with_basic_cuts);

    double xF_bins[6] = {0.4, 0.6, 0.65, 0.7, 0.77, 0.95};
    double pT_bins[6] = {0, 0.3, 0.45, 0.65, 0.9, 1.5};
    double xmins[12] = {-20., -30., -60., -60., -5., -80., -130., -130., 0., -150., -150., -150.};
    double xmaxs[12] = {15., 30., 60., 60., 80., 5., 130., 130., 150., 0., 150., 150.};
    TString targets[2] = {"targetPos==1", "targetPos==3"};
//     double particles_mc[2] = {jPsiCut_MC, psipCut};
//     double particles[2] = {jPsiCut, psipCut};
//     RDataFrame df_mcs[4] = {df_LH2_jpsi_with_basic_cuts, df_LH2_psip_with_basic_cuts, df_LD2_jpsi_with_basic_cuts, df_LD2_psip_with_basic_cuts};
    TString df_names[4] = {"LH2_jpsi", "LH2_psip", "LD2_jpsi", "LD2_psip"};
    TString var_names[4] = {"x1", "x2", "y1", "y2"};
    TString stations[3] = {"st1", "st2", "st3"};
    TString target_names[2] = {"LH2", "LD2"};
    TString particle_names[2] = {"jpsi", "psip"};

    for(int i = 0; i < 2; i++) //targets
    {
        for(int j = 0; j < 2; j++) // particles
        {
            for(int ii = 0; ii < 4; ii++) // variable
            {
                for(int jj = 0; jj < 3; jj++) //stations
                {
                    for(int kk = 0; kk < 5; kk++) // kinematics
                    {
                        std::cout << "---> " <<  df_names[2*i + j].Data() << std::endl;

                        TString var_name = Form("%s_%s", var_names[ii].Data(), stations[jj].Data());
                        std::cout << xmins[3* ii + jj] << " < " << var_name.Data() << " < " << xmaxs[3* ii + jj] << std::endl;

                        TString xF_hist_name = Form("%s_%s_%s_xF%d", target_names[i].Data(), particle_names[j].Data(), var_name.Data(), kk);
                        std::cout << xF_hist_name.Data() << std::endl;

                        TString xF_cuts = Form("%.2f < xF && xF < %.2f", xF_bins[kk], xF_bins[kk+1]);
                        std::cout << xF_cuts.Data() << std::endl;
                    }
                }
            }
        }
    }
}
