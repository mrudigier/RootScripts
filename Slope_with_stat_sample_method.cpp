//------------------------------------------------------------------------
// Root script for extracting the lifetime of a delayed coin. time spectrum
// using a statistical sample method.
// M. Rudigiier, 2017
//
// Description:
// This program uses the statistical sample method described
// in G. Lorusso et al., EPL, 120 (2017) 22001
// doi: 10.1209/0295-5075/120/22001
// Can be used to determine the lifetime of a delayed coincidence time distribution.
// Advantages are not clear from the paper. Seems to work ok-ish. But don't know
// exactly how to estimate the uncertainty from this...
//
// Creates two histograms, lambda_result and tau_result, which can be used to
// find the values for lambda and tau respectively.
// tau_result will be drawn at the end of the procedure and its mean printed.
//------------------------------------------------------------------------

#include <TROOT.h>
#include <TH1.h>
#include <TKey.h>
#include <TF1.h>
#include <TMath.h>


void FindSlope (TH1 * dthist,
                int limit_low, int limit_high,
                double psPerBin = 1, double constBG = 0) {

   TH1D *lambda_result = new TH1D("lambda_result", "Find lambda from average slope", 100000, -2, 2);
   TH1D *tau_result = new TH1D("tau_result", "Find tau from average slope", 100000, 0, 200000);
   double thisSlope = 1;
   double dF=0;

   for (int i=limit_low; i<limit_high; i++) {
      for (int j=i+10; j<limit_high; j++) {
         dF = TMath::Log(dthist->GetBinContent(j) - constBG) - TMath::Log(dthist->GetBinContent(i) - constBG);
         thisSlope = dF / (j-i);
         printf("%d, %d  --  %lf  --  %lf\n", i, (j-i), dF, thisSlope);
         tau_result->Fill(-psPerBin/thisSlope);
         lambda_result->Fill(thisSlope);
      }
   }
   printf("Done.\n");
   printf("Mean lifetime of the distribution is %4.8 ps\n",tau_result->GetMean());
   tau_result->Draw();
}

