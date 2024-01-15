/*
 * dinupa3@gmail.com
 * Apply chuckcuts to data and make simple root file
*/

#include<iostream>

#include "DataFrames.hh"

using namespace ROOT;


int nbins = 50;

void Test()
{
    auto display = df_real_with_basic_cuts.Display({"x1_st1", "x1_st2", "x1_st3", "pT"});
    display->Print();
}
