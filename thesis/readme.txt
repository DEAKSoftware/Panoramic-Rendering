-----------------------------------
 Trial Programs Used in the Thesis
-----------------------------------


This directory contains all the scene script files and executables to perform 
the trials described in my thesis. To run the trials contained in this directory,
simply copy all the files and subdirectories into the root path of the panoramic 
rendering project.

This directory contains the following structure:

test_fast_lod0.bat          <- Batch file for executing the lookup table method test.
test_slow_lod0.bat          <- Batch file for executing the direct transformation method test.
test_subdiv_01.bat          <- Batch file for executing the subdivision tests, with the subdivision threshold set to 0.1.
test_subdiv_01.bat          <- Batch file for executing the subdivision tests, with the subdivision threshold set to 0.2.

cosmic_ray_tau.exe          <- Win32 executable, with the lookup table method implemented
cosmic_ray_tau_slow.exe     <- Win32 executable, with the direct transformation method implemented

stats                       <- Directory where all the script files and results are saved.
stats/_fast_lod0            <- Results for the lookup table method are stored here.
stats/_slow_lod0            <- Results for the direct transformation method are stored here.
stats/_subdiv_01            <- Results for the subdivision tests, with the subdivision threshold set to 0.1.
stats/_subdiv_02            <- Results for the subdivision tests, with the subdivision threshold set to 0.2.
stats/sc_lod0               <- Scene script files for the lookup table method, and the direct transformation method tests.
stats/sc_subdiv_01          <- Scene script files for the subdivision tests, with the subdivision threshold set to 0.1.
stats/sc_subdiv_02          <- Scene script files for the subdivision tests, with the subdivision threshold set to 0.2.

To run the actual test, execute one of the batch files listed above. The batch file 
will load the appropriate scene script file, and executes the 3D engine. Once the 
renderer is running, quit the program after 1 minutes. The batch file will load 
the next scene with different a setting, and again you must terminate the program
after 1 minutes. This process will continue for a number of settings. Note that this is 
a time consuming process, as some batch files will run up to 20 scenes sequentially.
