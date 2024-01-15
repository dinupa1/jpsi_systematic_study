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
    auto display = df_real_with_basic_cuts.Display();
    display->Print();
}
