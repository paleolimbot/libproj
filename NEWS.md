# libproj 8.1.0-1

* Rewrite the configure script using autoconf and test it on
  all the platforms supported by rhub.
* Update the internal PROJ copy to 8.1.0.
* Rewrite tests to make sure the C API can be linked to
  and don't skip them on any platform.
* Test using a wrapper package that uses most of the
  exported functions
  (see <https://github.com/paleolimbot/rlibproj>)
* Update the README with examples of how to make use of this
  package (optionally using Rcpp or cpp11 as a helper)
* Correctly declare system requirements.
* Update configuration options to be more robust across
  environments.

# libproj 7.2.0-1

* Fixed linking to libtiff on Solaris and Ubuntu 20.04.
* Update to PROJ 7.2.0 and SQLite 3.34.0.
* Added `ca_bundle_path` to `libproj_configuration()`.
* `libproj_configure()` now properly checks for errors when
  calling the underlying PROJ functions.

# libproj 7.1.0-2

* Fixed linking to libtiff on MacOS (#4).
* Added a `NEWS.md` file to track changes to the package.
