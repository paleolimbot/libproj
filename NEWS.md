# libproj 7.2.0-1

* Fixed linking to libtiff on Solaris and Ubuntu 20.04.
* Update to PROJ 7.2.0 and SQLite 3.34.0.
* Added `ca_bundle_path` to `libproj_configuration()`.
* `libproj_configure()` now properly checks for errors when
  calling the underlying PROJ functions.

# libproj 7.1.0-2

* Fixed linking to libtiff on MacOS (#4).
* Added a `NEWS.md` file to track changes to the package.
