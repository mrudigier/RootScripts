//------------------------------------------------------------------------
// Root script to extract mean values from a large number of similar histograms.
// Main purpose is the determination of time offset values of time spectra
// M. Rudigiier, 2018
//
// Description:
// There are three different functions in this script.
//
// FindTimeShiftTH1D:  Find the centroid in a single TH1 histogram and write
//                     it to a given output file.
// FindTimeShiftTDir:  Apply FindTimeShiftTH1D to all histograms in a given
//                     TDirectory which fullfill a certain condition.
//                     Please adjust the condition according to your naming
//                     convention
// FindTimeShiftTH2D:  Extracts TH1 histograms from a TH2 histogram where
//                     the x-axis indexes the detector number and the y-axis
//                     contains the histograms to extract the time spectra from.
//
// All functions give some feedback output. For example there will be a
// note if statistics was too low in a given histogram to determine a reliable
// result.
//------------------------------------------------------------------------

#include <TROOT.h>
#include <TH2.h>
#include <TH1.h>
#include <TKey.h>

void FindTimeShiftTH2D (TH2D * thishist, int first, int last, int refdet, int offset = 0) {
   int current;
   long long max, entries;
   FILE *tsfile = fopen("TShifts", "w");
  printf("#Time shifts, reference det%2d\n#Gives the offset with respect to desired value\n#Id   timeshift (ns)\n", refdet);
   TH1D *projX;
   for (current = first; current < last; current++) {
      thishist->ProjectionX("tmp", current+1, current+1);
      projX = (TH1D*) gDirectory->Get("tmp");
      max = projX->GetMaximum();
      entries = projX->GetEntries();
      if (max < 2) {
        if (current == refdet) {
           fprintf(tsfile, "%02d  %d\n", current, 0);
           printf("%02d      reference\n", current);
        } else {
           printf("%02d          empty\n", current);
        }
      } else if ( (double) max/entries > 0.05 ) {
        projX->GetMaximumBin();
       fprintf(tsfile, "%02d  %d\n", current, (int)( projX->GetXaxis()->GetBinLowEdge(projX->GetMaximumBin()) - offset));
       printf("%02d  %13d  (%lf)\n", current, (int)( projX->GetXaxis()->GetBinLowEdge(projX->GetMaximumBin()) - offset), (double) max/entries );
//       printf("%02d  %lf\n", current,(double) max/entries);
      } else {
        printf("%02d  no clear peak  (%lf)\n", current,  (double) max/entries );
      }
      delete projX;
   }

   fclose(tsfile);
}

void FindTimeShiftTH1D (TH1D * thishist, FILE *thisoutfile, int offset = 0) {
   int id;
   long long max, entries;
      if (!sscanf(thishist->GetName(), "hdt_%05d", &id)) {
         printf("no valid id\n");
         return;
      }
      max = thishist->GetMaximum();
      entries = thishist->GetEntries();

      if (max < 2) {
           printf("%d          empty\n", id);
           fprintf(thisoutfile, "%d  %d\n", id, -12000);
      } else if ( (double) max/entries > 0.02 ) {
        thishist->GetMaximumBin();
        fprintf(thisoutfile, "%d  %d\n", id, (int)( thishist->GetXaxis()->GetBinLowEdge(thishist->GetMaximumBin()) - offset));
        printf("%d  %13d  (%lf)\n", id, (int)( thishist->GetXaxis()->GetBinLowEdge(thishist->GetMaximumBin()) - offset), (double) max/entries );
      } else {
        printf("%d  no clear peak  (%lf)\n", id,  (double) max/entries );
        fprintf(thisoutfile, "%d  %d\n", id, -12000);
      }
}


void FindTimeShiftTDir (char *thisDir, int refdet, int offset = 0){
   FILE *tsfile = fopen("TShifts", "w");
   printf("#Time shifts, reference det%2d\n#Gives the offset with respect to desired value\n#Id   timeshift (ns)\n", refdet);
   gDirectory->cd(thisDir);
   TIter next(gDirectory->GetListOfKeys());
   TKey *key;
   TH1D *h;
   while ((key = (TKey*)next())) {
      h = (TH1D*) gDirectory->Get(key->GetName());
      //printf("%s\n", h->GetName());
      FindTimeShiftTH1D(h, tsfile);
   }
   fclose(tsfile);
}
























