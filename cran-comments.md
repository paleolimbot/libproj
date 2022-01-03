
This submission fixes the package writing to the home directory without the
user's consent (a temporary directory is used instead). This submission
also updates the internal PROJ version to the latest available version,
8.2.0.

## R CMD check results

    * checking installed package size ... NOTE
    installed size is 13.5Mb
    sub-directories of 1Mb or more:
      libs   6.4Mb
      proj   7.6Mb
    
    0 errors v | 0 warnings v | 1 note x
