/*
 * dinupa3@gmail.com
 * Apply chuckcuts to data and make simple root file
*/

#include<iostream>

#include "ChamberAcceptance.hh"

using namespace ROOT;


int nbins = 50;

void PrintDf(ROOT::RDF::RNode df)
{
    auto display = df.Display({"x1_st1", "x1_st2", "x1_st3", "pT"});
    display->Print();
}


void Test()
{
    RDataFrame* dfs[4] = {df_LH2_jpsi_with_basic_cuts, df_LH2_psip_with_basic_cuts, df_LD2_jpsi_with_basic_cuts, df_LD2_psip_with_basic_cuts};

    for(int i = 0; i < 4; i++)
    {
        PrintDf(dfs[i]);
    }

}
