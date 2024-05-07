#include <TString.h>
#include <TSystem.h>
#include "HodoUtils.hh"



void HodoAna()
{

    double bin_edges[2][6] = {{0.4, 0.6, 0.65, 0.7, 0.77, 0.95}, {0, 0.3, 0.45, 0.65, 0.9, 1.5}};
/*
    TString mc_files[4] = {"mc_jpsi_LH2_M027_S002_messy_occ_pTxFweight_v2.root",
                            "mc_jpsi_LD2_M027_S002_messy_occ_pTxFweight_v2.root",
                            "mc_psiprime_LH2_M027_S002_messy_occ_pTxFweight_v2.root",
                            "mc_psiprime_LD2_M027_S002_messy_occ_pTxFweight_v2.root"};

    TString unmix_file = "run56_2111v42_tmp_noPhys_D0.root";
    TString mix_file = "mix_FPGA4_run56_2111v42_tmp_noPhys.root";

    TString mc_out_files[4] = {"jpsi_LH2_mc.root", "jpsi_LD2_mc.root", "psip_LH2_mc.root", "psip_LD2_mc.root"};
    TString unmix_out_files[4] = {"jpsi_LH2_unmix.root", "jpsi_LD2_unmix.root", "psip_LH2_unmix.root", "psip_LD2_unmix.root"};
    TString mix_out_files[4] = {"jpsi_LH2_mix.root", "jpsi_LD2_mix.root", "psip_LH2_mix.root", "psip_LD2_mix.root"};

    TString mc_cuts[4] = {jpsi_mc_cuts.Data(), jpsi_mc_cuts.Data(), psip_mc_cuts.Data(), psip_mc_cuts.Data()};
    TString unmix_cuts[4] = {jpsi_LH2_unmix_cuts.Data(), jpsi_LD2_unmix_cuts.Data(), psip_LH2_unmix_cuts.Data(), psip_LD2_unmix_cuts.Data()};
    TString mix_cuts[4] = {jpsi_mix_cuts.Data(), jpsi_mix_cuts.Data(), psip_mix_cuts.Data(), psip_mix_cuts.Data()};

    for(int ii = 0; ii < 4; ii++)
    {
        SaveTree(mc_files[ii].Data(), mc_out_files[ii].Data(), mc_cuts[ii]);
        SaveTree(unmix_file.Data(), unmix_out_files[ii].Data(), unmix_cuts[ii]);
        SaveTree(mix_file.Data(), mix_out_files[ii].Data(), mix_cuts[ii]);
    }
*/

    // SaveHist(TString fmc, TString funmix, TString fmix, TString hname, TString kinematics, TString bname)

    TString hnames[4] = {"H1X", "H2X", "H3X", "H4X"};
    TString knames[2] = {"xF", "pT"};
    TString bnames[2] = {"x1", "x2"};
    TString pnames[2] = {"jpsi", "psip"};
    TString tnames[2] = {"LH2", "LD2"};
    TString pxznames[4] = {"px1_st3", "px2_st3", "pz1_st3", "pz2_st3"};

    gSystem->mkdir("imgs/hodo", 1);
    gStyle->SetOptStat(0);

    TFile* outfile = new TFile("hodo_all.root", "RECREATE");


    for(int aa = 0; aa < 2; aa++) // particle
    {
        for(int bb = 0; bb < 2; bb++) // target
        {
            for(int cc = 0; cc < 4; cc++) // hodo names
            {
                for(int dd = 0; dd < 2; dd++) // branch
                {
                    for(int ee = 0; ee < 2; ee++) // kinematics
                    {
                        for(int ff = 0; ff < 5; ff++) // binx
                        {
                            TString fmc = Form("%s_%s_mc.root", pnames[aa].Data(), tnames[bb].Data());
                            TString funmix = Form("%s_%s_unmix.root", pnames[aa].Data(), tnames[bb].Data());
                            TString fmix = Form("%s_%s_mix.root", pnames[aa].Data(), tnames[bb].Data());

                            TString hname = Form("%s_%s_%s_%s_%s_%d", pnames[aa].Data(), tnames[bb].Data(), hnames[cc].Data(), bnames[dd].Data(), knames[ee].Data(), ff); // particle, target, hodo, branch, kin, bin
                            TString kinematics = Form("%f < %s && %s < %f", bin_edges[ee][ff], knames[ee].Data(), knames[ee].Data(), bin_edges[ee][ff+1]);
                            TString bname = Form("%s_%s", bnames[dd].Data(), hnames[cc].Data()); // branch, hodo

                            std::cout << "---> using files " << fmc.Data() << " , " << funmix.Data() << " , " << fmix.Data() << std::endl;
                            std::cout << ">> histogram " << hname.Data() << std::endl;
                            std::cout << ">> kinematics " << kinematics.Data() << std::endl;
                            std:cout << ">> branch " << bname.Data() << std::endl;

                            SaveHist(fmc, funmix, fmix, hname, kinematics, bname);

                        } // bin
                    } // kinematic 
                } // branch 
            } // hodo names
        } //target
    } // particle


    for(int aa = 0; aa < 2; aa++) // particle
    {
        for(int bb = 0; bb < 2; bb++) // target
        {
            for(int cc = 0; cc < 4; cc++) // px, pz in st3
            {
                for(int dd = 0; dd < 2; dd++) // kinematics
                {
                    for(int ee = 0; ee < 5; ee++) // bins 
                    {
                        TString fmc = Form("%s_%s_mc.root", pnames[aa].Data(), tnames[bb].Data());
                        TString funmix = Form("%s_%s_unmix.root", pnames[aa].Data(), tnames[bb].Data());
                        TString fmix = Form("%s_%s_mix.root", pnames[aa].Data(), tnames[bb].Data());

                        TString hname = Form("%s_%s_%s_%s_%d", pnames[aa].Data(), tnames[bb].Data(), pxznames[cc].Data(), knames[dd].Data(), ee); // particle, target, hodo, branch, kin, bin
                        TString kinematics = Form("%f < %s && %s < %f", bin_edges[dd][ee], knames[dd].Data(), knames[dd].Data(), bin_edges[dd][ee+1]);

                        std::cout << "---> using files " << fmc.Data() << " , " << funmix.Data() << " , " << fmix.Data() << std::endl;
                        std::cout << ">> histogram " << hname.Data() << std::endl;
                        std::cout << ">> kinematics " << kinematics.Data() << std::endl;
                        std::cout << ">> branch " << pxznames[cc].Data() << std::endl;

                        SaveHist(fmc, funmix, fmix, hname, kinematics, pxznames[cc]);
                        
                    } // bins
                } // kinematics
            } // px, pz in st3
        } // target
    } // particle 

    outfile->Close();

}