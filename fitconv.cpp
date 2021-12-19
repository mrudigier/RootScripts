//------------------------------------------------------------------------
// Root script for fitting a gauss-exponential convolution to a time spectrum
// M. Rudigiier, 2017
//
// Function
// ========
// fitvonc:  fit the convolution to a given histogram.
// parameters: Give some estimated parameters as starting values for the fit
//             1 -- b : constant background
//             2 -- x0: prompt mean position
//             3 -- s : sigma of the prompt position
//             4 -- l : the decay constant (inverse of mean lifetime tau)
//             5 -- A : Integram of the final distribution
//Options:     You can provide a fit limit, as well as fix individual parameters
//             All parameters are free to vary as a default.
//
//------------------------------------------------------------------------

#include <stdio.h>
#include <TMath.h>

Double_t conv (Double_t *x, Double_t *par) {
  Float_t xx = x[0];
  Double_t f = 0.5*par[0]*par[1]*TMath::Exp((TMath::Power(par[1],2))*(TMath::Power(par[2],2))*0.5)*TMath::Exp(-par[1]*(xx-par[3]))*(1-TMath::Erf(((par[1]*TMath::Power(par[2],2))-xx+par[3])/(par[2]*TMath::Sqrt(2))))+par[4];
  return f;
}


void fitconv (TH1 *h1, 
                float b, float x0, float s, float l, float A,
                float left_limit = 0, float right_limit = 4000, 
                int fit_mode = 1, /*0:leastSquares (TH1::Fit default),
                                   1:LogLikelihood ("L"),
                                   2:LogLikelihood for bin cont. <100 ("LL")
                                   If the hist has weights, add "W"
                                   below where apporpriate (see doc.).*/
                bool fix_b=0, bool fix_x0=0, bool fix_s=0, bool fix_l=0,
                bool fix_A=0) {
  TF1 *f1 = new TF1("myconv", conv, -20,h1->FindLastBinAbove(),5);
  TF1 *f2 = new TF1("prompt","gaus",-20,h1->FindLastBinAbove());
  f2->SetLineColor(4);
  h1->SetLineColor(12);
  f1->SetParameters(A, l, s, x0, b);
  if (fix_b) {
    f1->FixParameter(4, b);
  }
  if (fix_x0) {
    f1->FixParameter(3, x0);
  }
  if (fix_s) {
    f1->FixParameter(2, s);
  }
  if (fix_l) {
    f1->FixParameter(1, l);
  }
  if (fix_A) {
    f1->FixParameter(0, A);
  }
  if (fit_mode == 1) {
    h1->Fit(f1, "L", "", left_limit, right_limit);
  } else if (fit_mode == 2) {
    h1->Fit(f1, "LL", "", left_limit, right_limit);
  } else if (fit_mode == 0) {
    h1->Fit(f1, "", "", left_limit, right_limit);
  } else {
    printf("Fit_mode must be either 0,1, or 2. Did nothing.\n");
    return;
  }

  f2->SetParameters(f1->GetParameter(0)/(TMath::Sqrt(2*3.1415)*f1->GetParameter(2)),
                    f1->GetParameter(3),
                    f1->GetParameter(2));
  f2->Draw("same");

  printf("l   = %f,\ntau = %f,\nT1/2 = %f\n", f1->GetParameter(1),
                                            (1/f1->GetParameter(1)),
                                            (1/f1->GetParameter(1)*TMath::Log(2)));

}
