/*
 * dinupa3@gmail.com
 * Apply chuckcuts to data and make simple root file
*/

#include<iostream>

#include "ChamberAcceptance.hh"

using namespace ROOT;


int nbins = 50;

void PrintDf(RDF::RNode df)
{
    auto display = df.Display({"x1_st1", "x1_st2", "x1_st3", "pT"});
    display->Print();
}


void Test()
{
    PrintDf(df_LH2_psip_with_basic_cuts);
}
