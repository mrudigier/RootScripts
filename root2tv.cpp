//------------------------------------------------------------------------
// Root script to convert a TH1 into an ASCII histogram
// M. Rudigiier, 2017
//
// Function
// ========
// Give the name of the output file and the TH1 to be converted.
// By default, the output ASCII file will be "tv style", i.e. one column,
// where the line number corresponds to the bin number. This means that any
// calibration will be lost.
// By setting make_tv to 0 the result will be a two-cloumn ASCII file where
// the first column corresponds to the calibrated bin value.
//------------------------------------------------------------------------

void root2tv (const char *outfilename, TH1 *thishist, bool make_tv = 1) {
  
  FILE *ofp = fopen(outfilename, "w");
  int range, i;
  range = thishist->GetNbinsX();
  for (i = 0; i < range; i++) {
    if (make_tv) {
      fprintf(ofp, "%lf\n", thishist->GetBinContent(i));
    } else {
      fprintf(ofp, "%f  %lf\n", thishist->GetBinLowEdge(i), thishist->GetBinContent(i));
    }
  }
  fclose(ofp);
  return; 
}
