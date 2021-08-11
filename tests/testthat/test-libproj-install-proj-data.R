
test_that("libproj_install_proj_data() works", {
  skip("run test manually to check data installer (downloads ~2 GB)")

  tmp <- tempfile()
  dir.create(tmp)

  quiet <- FALSE

  expect_true(file.create(file.path(tmp, "some-file")))
  expect_error(libproj_install_proj_data(tmp, overwrite = FALSE, quiet = quiet), "Use `overwrite = TRUE`")
  expect_error(libproj_install_proj_data(tmp, overwrite = NA, quiet = quiet), "Use `overwrite = TRUE`")

  libproj_install_proj_data(tmp, overwrite = TRUE, quiet = quiet)

  expect_true(file.exists(file.path(tmp, "some-file")))
  expect_true(file.exists(file.path(tmp, ".libproj_install_proj_data")))

  expect_false(libproj_install_proj_data(tmp, overwrite = NA, quiet = quiet))
  expect_false(libproj_install_proj_data(tmp, overwrite = FALSE, quiet = quiet))
  expect_true(libproj_install_proj_data(tmp, overwrite = TRUE, quiet = quiet))
  expect_true(libproj_has_proj_data(tmp))

  unlink(tmp, recursive = TRUE)
})
