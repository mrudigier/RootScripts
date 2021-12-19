//------------------------------------------------------------------------
// Root script for transforming root histograms into binary histogram files
// M. Rudigiier, 2020
//
// Function
// ========
// TH1_to_bin: Writes a TH1 into uncompressed binary ("one column")
// TH2_to_bin: Writes a TH2 into uncompressed binary
//               Option tv_style: 0 -- just write the matrix
//                                1 -- (default) write the matrix, as well
//                                     as two projections. Move everything
//                                     into a folder. This way the matrix
//                                     can be opened directly in tv.
//                                     (works on linux)
// TH2_to_rw:  Write a TH2 into binary radware style.
//
//------------------------------------------------------------------------

#include <stdlib.h>
#include <TH2.h>

void write_to_bin(int *array, long size, const char *filename)
{
   FILE *fp;
   fp = fopen(filename, "wb");
   fwrite(array, size, 1, fp);
   fclose(fp);
}

void TH1_to_bin (TH1 *hist, const char *filename) {
   int bins = hist->GetXaxis()->GetNbins();
   int spec[bins];
   for (int i=0; i< bins; i++) {
      spec[i] = (int) hist->GetBinContent(i+1);
   }
   write_to_bin(spec, sizeof(int) * bins, filename);
   printf("Wrote to file %s.\n", filename);
}

void TH2_to_bin (TH2 *hist, const char *filename,
                 bool tv_style=1 /*0:just matrix, 1:+folder and projections*/) {
   int binsx = hist->GetXaxis()->GetNbins();
   int binsy = hist->GetYaxis()->GetNbins();
   int spec[binsy];
   FILE *fp;
   char outname [512];
   sprintf(outname, "%s.mtx", filename);
   fp = fopen(outname, "wb");
   for (int i=0; i< binsx; i++) {
      for (int j=0; j< binsy; j++) {
         spec[j]= (int) hist->GetBinContent(i+1, j+1);
      }
      fwrite(spec, sizeof(int) * binsy, 1, fp);
   }
   fclose(fp);
   printf("Wrote to file %s, dimensions %d x %d\n", outname, binsy, binsx);
   if(tv_style) {
      char prxname[512];
      char pryname[512];
      sprintf(prxname, "%s.prx", filename);
      sprintf(pryname, "%s.pry", filename);
      TH1_to_bin(hist->ProjectionX(), prxname);
      TH1_to_bin(hist->ProjectionY(), pryname);
      char syscommand[1024];
      sprintf(syscommand, "rm -rf %s && mkdir %s", filename, filename);
      if(system(syscommand))
         printf("Something went wrong creating directory %s\n", filename);
      sprintf(syscommand, "mv %s.* %s", filename, filename);
      if(system(syscommand))
         printf("Something went wrong moving the files to %s\n", filename);
   }
}

void TH2_to_rw (TH2 *hist, const char *filename) {
   int binsx = hist->GetXaxis()->GetNbins();
   int binsy = hist->GetYaxis()->GetNbins();
   
   
   int outbinsy;
   int outbinsx;
   
  
   int outbins = 4096;
   int spec[outbins];
   FILE *fp;
   fp = fopen(filename, "wb");
  
   int i;
   for (i=0; i< binsx; i++) {
      int j;
      for (j=0; j< binsy; j++) {
         spec[j]= (int) hist->GetBinContent(i+1, j+1);
      }
      for (int p=j; p<outbins; p++) {
         spec[p] = 0;
         
      }
      fwrite(spec, sizeof(int) * outbins, 1, fp);
   }
   memset(spec, 0, sizeof(int)*outbins);
   for (int p=i; p<outbins; p++) {
      fwrite(spec, sizeof(int)*outbins, 1, fp);
   }
   
   
   fclose(fp);
   printf("Wrote to file %s, dimensions %d x %d\n", filename, outbins, outbins);
}
