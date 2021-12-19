# Root scripts

Collection of useful scriptsscripts for working with nuclear spectroscopy data
using ROOT, the analysis frame work developed at cern (http:://root.cern).
Please feel free to use these as they are, or to modify and ibase your own scripts on.

All scripts have been written for root6, but will probably also work with the old root5
verison which is still used in some places.
Load them in the root prompt using `.L <script_name.cpp>++`. The `++` will compile
the script, so you can check that it will work on your installation of root.
You can then use the provided functions in the root terminal (including tab completion).

Author: M. Rudigier

## Find time shift values for a large number of histograms:
#### FindTimeShift.cpp              
Initial calibrations of every fast timing experiment involves time alignment. With
large detector arrays this can become very tideous, so this script provides a couple
of ways to determine these shifts automatically.

## Convert root histograms
into other formats for use with programmes like tv, radware, or for plotting with gnuplot:
#### root2tv.cpp
Convert TH1 to human readable ascii

#### TH_to_bin.cpp
Convert TH1 or TH2 to binary (e.g. for use with tv or radware)

## Delayed coincidence time spectrum analysis:
#### fitconv.cpp
Fit a Gauss-ExpDecay convolution function to a time spectrum to extract tau.

#### Slope_with_stat_sample_method.cpp
Use a statistical sampling method to extract the mean lifetime from a time spectrum.
