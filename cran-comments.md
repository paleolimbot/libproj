
This submission fixes the following check errors on various platforms:

* Updated rwinlibs libraries to work on UCRT
* Properly clean up home directory files that may have been written by tests

As the package was archived without a warning period, there was no opportunity
ensure that the CRAN checks were clean for all platforms before burdening
the CRAN system with an additional submission.

## R CMD check results

    * checking installed package size ... NOTE
    installed size is 13.5Mb
    sub-directories of 1Mb or more:
      libs   6.3Mb
      proj   7.0Mb
    
    0 errors v | 0 warnings v | 1 note x
