// generated automatically by data-raw/update-libproj-api.R - do not edit by hand!
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include "R-libproj/proj.h"

// we need a utility function to get the runtime version in a form that is
// queryable from the inst/include/libproj.c, because future PROJ versions
// will add to the C API. The ability to do a runtime check around R_GetCCallable()
// lets newer packages link to multiple versions of libproj.
#define LIBPROJ_VERSION_INT(major, minor, patch) (patch + minor * 100 + major * 10000)

int libproj_version_int() {
  return LIBPROJ_VERSION_INT(PROJ_VERSION_MAJOR, PROJ_VERSION_MINOR, PROJ_VERSION_PATCH);
}

SEXP libproj_c_register_c_callables() {
  /* used by external packages linking to libproj from C */
  R_RegisterCCallable("libproj", "libproj_version_int", (DL_FUNC) &libproj_version_int);
    R_RegisterCCallable("libproj", "proj_context_create", (DL_FUNC) &proj_context_create);
  R_RegisterCCallable("libproj", "proj_context_destroy", (DL_FUNC) &proj_context_destroy);
  R_RegisterCCallable("libproj", "proj_context_clone", (DL_FUNC) &proj_context_clone);
  R_RegisterCCallable("libproj", "proj_context_set_file_finder", (DL_FUNC) &proj_context_set_file_finder);
  R_RegisterCCallable("libproj", "proj_context_set_search_paths", (DL_FUNC) &proj_context_set_search_paths);
  R_RegisterCCallable("libproj", "proj_context_set_ca_bundle_path", (DL_FUNC) &proj_context_set_ca_bundle_path);
  R_RegisterCCallable("libproj", "proj_context_use_proj4_init_rules", (DL_FUNC) &proj_context_use_proj4_init_rules);
  R_RegisterCCallable("libproj", "proj_context_get_use_proj4_init_rules", (DL_FUNC) &proj_context_get_use_proj4_init_rules);
  R_RegisterCCallable("libproj", "proj_context_set_fileapi", (DL_FUNC) &proj_context_set_fileapi);
  R_RegisterCCallable("libproj", "proj_context_set_sqlite3_vfs_name", (DL_FUNC) &proj_context_set_sqlite3_vfs_name);
  R_RegisterCCallable("libproj", "proj_context_set_network_callbacks", (DL_FUNC) &proj_context_set_network_callbacks);
  R_RegisterCCallable("libproj", "proj_context_set_enable_network", (DL_FUNC) &proj_context_set_enable_network);
  R_RegisterCCallable("libproj", "proj_context_is_network_enabled", (DL_FUNC) &proj_context_is_network_enabled);
  R_RegisterCCallable("libproj", "proj_context_set_url_endpoint", (DL_FUNC) &proj_context_set_url_endpoint);
  R_RegisterCCallable("libproj", "proj_context_get_url_endpoint", (DL_FUNC) &proj_context_get_url_endpoint);
  R_RegisterCCallable("libproj", "proj_context_get_user_writable_directory", (DL_FUNC) &proj_context_get_user_writable_directory);
  R_RegisterCCallable("libproj", "proj_grid_cache_set_enable", (DL_FUNC) &proj_grid_cache_set_enable);
  R_RegisterCCallable("libproj", "proj_grid_cache_set_filename", (DL_FUNC) &proj_grid_cache_set_filename);
  R_RegisterCCallable("libproj", "proj_grid_cache_set_max_size", (DL_FUNC) &proj_grid_cache_set_max_size);
  R_RegisterCCallable("libproj", "proj_grid_cache_set_ttl", (DL_FUNC) &proj_grid_cache_set_ttl);
  R_RegisterCCallable("libproj", "proj_grid_cache_clear", (DL_FUNC) &proj_grid_cache_clear);
  R_RegisterCCallable("libproj", "proj_is_download_needed", (DL_FUNC) &proj_is_download_needed);
  R_RegisterCCallable("libproj", "proj_download_file", (DL_FUNC) &proj_download_file);
  R_RegisterCCallable("libproj", "proj_create", (DL_FUNC) &proj_create);
  R_RegisterCCallable("libproj", "proj_create_argv", (DL_FUNC) &proj_create_argv);
  R_RegisterCCallable("libproj", "proj_create_crs_to_crs", (DL_FUNC) &proj_create_crs_to_crs);
  R_RegisterCCallable("libproj", "proj_create_crs_to_crs_from_pj", (DL_FUNC) &proj_create_crs_to_crs_from_pj);
  R_RegisterCCallable("libproj", "proj_normalize_for_visualization", (DL_FUNC) &proj_normalize_for_visualization);
  R_RegisterCCallable("libproj", "proj_assign_context", (DL_FUNC) &proj_assign_context);
  R_RegisterCCallable("libproj", "proj_destroy", (DL_FUNC) &proj_destroy);
  R_RegisterCCallable("libproj", "proj_area_create", (DL_FUNC) &proj_area_create);
  R_RegisterCCallable("libproj", "proj_area_set_bbox", (DL_FUNC) &proj_area_set_bbox);
  R_RegisterCCallable("libproj", "proj_area_destroy", (DL_FUNC) &proj_area_destroy);
  R_RegisterCCallable("libproj", "proj_angular_input", (DL_FUNC) &proj_angular_input);
  R_RegisterCCallable("libproj", "proj_angular_output", (DL_FUNC) &proj_angular_output);
  R_RegisterCCallable("libproj", "proj_degree_input", (DL_FUNC) &proj_degree_input);
  R_RegisterCCallable("libproj", "proj_degree_output", (DL_FUNC) &proj_degree_output);
  R_RegisterCCallable("libproj", "proj_trans", (DL_FUNC) &proj_trans);
  R_RegisterCCallable("libproj", "proj_trans_array", (DL_FUNC) &proj_trans_array);
  R_RegisterCCallable("libproj", "proj_trans_generic", (DL_FUNC) &proj_trans_generic);
  R_RegisterCCallable("libproj", "proj_coord", (DL_FUNC) &proj_coord);
  R_RegisterCCallable("libproj", "proj_roundtrip", (DL_FUNC) &proj_roundtrip);
  R_RegisterCCallable("libproj", "proj_lp_dist", (DL_FUNC) &proj_lp_dist);
  R_RegisterCCallable("libproj", "proj_lpz_dist", (DL_FUNC) &proj_lpz_dist);
  R_RegisterCCallable("libproj", "proj_xy_dist", (DL_FUNC) &proj_xy_dist);
  R_RegisterCCallable("libproj", "proj_xyz_dist", (DL_FUNC) &proj_xyz_dist);
  R_RegisterCCallable("libproj", "proj_geod", (DL_FUNC) &proj_geod);
  R_RegisterCCallable("libproj", "proj_context_errno", (DL_FUNC) &proj_context_errno);
  R_RegisterCCallable("libproj", "proj_errno", (DL_FUNC) &proj_errno);
  R_RegisterCCallable("libproj", "proj_errno_set", (DL_FUNC) &proj_errno_set);
  R_RegisterCCallable("libproj", "proj_errno_reset", (DL_FUNC) &proj_errno_reset);
  R_RegisterCCallable("libproj", "proj_errno_restore", (DL_FUNC) &proj_errno_restore);
  R_RegisterCCallable("libproj", "proj_errno_string", (DL_FUNC) &proj_errno_string);
  R_RegisterCCallable("libproj", "proj_context_errno_string", (DL_FUNC) &proj_context_errno_string);
  R_RegisterCCallable("libproj", "proj_log_level", (DL_FUNC) &proj_log_level);
  R_RegisterCCallable("libproj", "proj_log_func", (DL_FUNC) &proj_log_func);
  R_RegisterCCallable("libproj", "proj_factors", (DL_FUNC) &proj_factors);
  R_RegisterCCallable("libproj", "proj_info", (DL_FUNC) &proj_info);
  R_RegisterCCallable("libproj", "proj_pj_info", (DL_FUNC) &proj_pj_info);
  R_RegisterCCallable("libproj", "proj_grid_info", (DL_FUNC) &proj_grid_info);
  R_RegisterCCallable("libproj", "proj_init_info", (DL_FUNC) &proj_init_info);
  R_RegisterCCallable("libproj", "proj_list_operations", (DL_FUNC) &proj_list_operations);
  R_RegisterCCallable("libproj", "proj_list_ellps", (DL_FUNC) &proj_list_ellps);
  R_RegisterCCallable("libproj", "proj_list_prime_meridians", (DL_FUNC) &proj_list_prime_meridians);
  R_RegisterCCallable("libproj", "proj_torad", (DL_FUNC) &proj_torad);
  R_RegisterCCallable("libproj", "proj_todeg", (DL_FUNC) &proj_todeg);
  R_RegisterCCallable("libproj", "proj_dmstor", (DL_FUNC) &proj_dmstor);
  R_RegisterCCallable("libproj", "proj_rtodms", (DL_FUNC) &proj_rtodms);
  R_RegisterCCallable("libproj", "proj_cleanup", (DL_FUNC) &proj_cleanup);
  R_RegisterCCallable("libproj", "proj_string_list_destroy", (DL_FUNC) &proj_string_list_destroy);
  R_RegisterCCallable("libproj", "proj_context_set_autoclose_database", (DL_FUNC) &proj_context_set_autoclose_database);
  R_RegisterCCallable("libproj", "proj_context_set_database_path", (DL_FUNC) &proj_context_set_database_path);
  R_RegisterCCallable("libproj", "proj_context_get_database_path", (DL_FUNC) &proj_context_get_database_path);
  R_RegisterCCallable("libproj", "proj_context_get_database_metadata", (DL_FUNC) &proj_context_get_database_metadata);
  R_RegisterCCallable("libproj", "proj_context_get_database_structure", (DL_FUNC) &proj_context_get_database_structure);
  R_RegisterCCallable("libproj", "proj_context_guess_wkt_dialect", (DL_FUNC) &proj_context_guess_wkt_dialect);
  R_RegisterCCallable("libproj", "proj_create_from_wkt", (DL_FUNC) &proj_create_from_wkt);
  R_RegisterCCallable("libproj", "proj_create_from_database", (DL_FUNC) &proj_create_from_database);
  R_RegisterCCallable("libproj", "proj_uom_get_info_from_database", (DL_FUNC) &proj_uom_get_info_from_database);
  R_RegisterCCallable("libproj", "proj_grid_get_info_from_database", (DL_FUNC) &proj_grid_get_info_from_database);
  R_RegisterCCallable("libproj", "proj_clone", (DL_FUNC) &proj_clone);
  R_RegisterCCallable("libproj", "proj_create_from_name", (DL_FUNC) &proj_create_from_name);
  R_RegisterCCallable("libproj", "proj_get_type", (DL_FUNC) &proj_get_type);
  R_RegisterCCallable("libproj", "proj_is_deprecated", (DL_FUNC) &proj_is_deprecated);
  R_RegisterCCallable("libproj", "proj_get_non_deprecated", (DL_FUNC) &proj_get_non_deprecated);
  R_RegisterCCallable("libproj", "proj_is_equivalent_to", (DL_FUNC) &proj_is_equivalent_to);
  R_RegisterCCallable("libproj", "proj_is_equivalent_to_with_ctx", (DL_FUNC) &proj_is_equivalent_to_with_ctx);
  R_RegisterCCallable("libproj", "proj_is_crs", (DL_FUNC) &proj_is_crs);
  R_RegisterCCallable("libproj", "proj_get_name", (DL_FUNC) &proj_get_name);
  R_RegisterCCallable("libproj", "proj_get_id_auth_name", (DL_FUNC) &proj_get_id_auth_name);
  R_RegisterCCallable("libproj", "proj_get_id_code", (DL_FUNC) &proj_get_id_code);
  R_RegisterCCallable("libproj", "proj_get_remarks", (DL_FUNC) &proj_get_remarks);
  R_RegisterCCallable("libproj", "proj_get_scope", (DL_FUNC) &proj_get_scope);
  R_RegisterCCallable("libproj", "proj_get_area_of_use", (DL_FUNC) &proj_get_area_of_use);
  R_RegisterCCallable("libproj", "proj_as_wkt", (DL_FUNC) &proj_as_wkt);
  R_RegisterCCallable("libproj", "proj_as_proj_string", (DL_FUNC) &proj_as_proj_string);
  R_RegisterCCallable("libproj", "proj_as_projjson", (DL_FUNC) &proj_as_projjson);
  R_RegisterCCallable("libproj", "proj_get_source_crs", (DL_FUNC) &proj_get_source_crs);
  R_RegisterCCallable("libproj", "proj_get_target_crs", (DL_FUNC) &proj_get_target_crs);
  R_RegisterCCallable("libproj", "proj_identify", (DL_FUNC) &proj_identify);
  R_RegisterCCallable("libproj", "proj_get_geoid_models_from_database", (DL_FUNC) &proj_get_geoid_models_from_database);
  R_RegisterCCallable("libproj", "proj_int_list_destroy", (DL_FUNC) &proj_int_list_destroy);
  R_RegisterCCallable("libproj", "proj_get_authorities_from_database", (DL_FUNC) &proj_get_authorities_from_database);
  R_RegisterCCallable("libproj", "proj_get_codes_from_database", (DL_FUNC) &proj_get_codes_from_database);
  R_RegisterCCallable("libproj", "proj_get_celestial_body_list_from_database", (DL_FUNC) &proj_get_celestial_body_list_from_database);
  R_RegisterCCallable("libproj", "proj_celestial_body_list_destroy", (DL_FUNC) &proj_celestial_body_list_destroy);
  R_RegisterCCallable("libproj", "proj_get_crs_list_parameters_create", (DL_FUNC) &proj_get_crs_list_parameters_create);
  R_RegisterCCallable("libproj", "proj_get_crs_list_parameters_destroy", (DL_FUNC) &proj_get_crs_list_parameters_destroy);
  R_RegisterCCallable("libproj", "proj_get_crs_info_list_from_database", (DL_FUNC) &proj_get_crs_info_list_from_database);
  R_RegisterCCallable("libproj", "proj_crs_info_list_destroy", (DL_FUNC) &proj_crs_info_list_destroy);
  R_RegisterCCallable("libproj", "proj_get_units_from_database", (DL_FUNC) &proj_get_units_from_database);
  R_RegisterCCallable("libproj", "proj_unit_list_destroy", (DL_FUNC) &proj_unit_list_destroy);
  R_RegisterCCallable("libproj", "proj_insert_object_session_create", (DL_FUNC) &proj_insert_object_session_create);
  R_RegisterCCallable("libproj", "proj_insert_object_session_destroy", (DL_FUNC) &proj_insert_object_session_destroy);
  R_RegisterCCallable("libproj", "proj_get_insert_statements", (DL_FUNC) &proj_get_insert_statements);
  R_RegisterCCallable("libproj", "proj_suggests_code_for", (DL_FUNC) &proj_suggests_code_for);
  R_RegisterCCallable("libproj", "proj_string_destroy", (DL_FUNC) &proj_string_destroy);
  R_RegisterCCallable("libproj", "proj_create_operation_factory_context", (DL_FUNC) &proj_create_operation_factory_context);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_destroy", (DL_FUNC) &proj_operation_factory_context_destroy);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_set_desired_accuracy", (DL_FUNC) &proj_operation_factory_context_set_desired_accuracy);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_set_area_of_interest", (DL_FUNC) &proj_operation_factory_context_set_area_of_interest);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_set_crs_extent_use", (DL_FUNC) &proj_operation_factory_context_set_crs_extent_use);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_set_spatial_criterion", (DL_FUNC) &proj_operation_factory_context_set_spatial_criterion);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_set_grid_availability_use", (DL_FUNC) &proj_operation_factory_context_set_grid_availability_use);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_set_use_proj_alternative_grid_names", (DL_FUNC) &proj_operation_factory_context_set_use_proj_alternative_grid_names);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_set_allow_use_intermediate_crs", (DL_FUNC) &proj_operation_factory_context_set_allow_use_intermediate_crs);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_set_allowed_intermediate_crs", (DL_FUNC) &proj_operation_factory_context_set_allowed_intermediate_crs);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_set_discard_superseded", (DL_FUNC) &proj_operation_factory_context_set_discard_superseded);
  R_RegisterCCallable("libproj", "proj_operation_factory_context_set_allow_ballpark_transformations", (DL_FUNC) &proj_operation_factory_context_set_allow_ballpark_transformations);
  R_RegisterCCallable("libproj", "proj_create_operations", (DL_FUNC) &proj_create_operations);
  R_RegisterCCallable("libproj", "proj_list_get_count", (DL_FUNC) &proj_list_get_count);
  R_RegisterCCallable("libproj", "proj_list_get", (DL_FUNC) &proj_list_get);
  R_RegisterCCallable("libproj", "proj_list_destroy", (DL_FUNC) &proj_list_destroy);
  R_RegisterCCallable("libproj", "proj_get_suggested_operation", (DL_FUNC) &proj_get_suggested_operation);
  R_RegisterCCallable("libproj", "proj_crs_is_derived", (DL_FUNC) &proj_crs_is_derived);
  R_RegisterCCallable("libproj", "proj_crs_get_geodetic_crs", (DL_FUNC) &proj_crs_get_geodetic_crs);
  R_RegisterCCallable("libproj", "proj_crs_get_horizontal_datum", (DL_FUNC) &proj_crs_get_horizontal_datum);
  R_RegisterCCallable("libproj", "proj_crs_get_sub_crs", (DL_FUNC) &proj_crs_get_sub_crs);
  R_RegisterCCallable("libproj", "proj_crs_get_datum", (DL_FUNC) &proj_crs_get_datum);
  R_RegisterCCallable("libproj", "proj_crs_get_datum_ensemble", (DL_FUNC) &proj_crs_get_datum_ensemble);
  R_RegisterCCallable("libproj", "proj_crs_get_datum_forced", (DL_FUNC) &proj_crs_get_datum_forced);
  R_RegisterCCallable("libproj", "proj_datum_ensemble_get_member_count", (DL_FUNC) &proj_datum_ensemble_get_member_count);
  R_RegisterCCallable("libproj", "proj_datum_ensemble_get_accuracy", (DL_FUNC) &proj_datum_ensemble_get_accuracy);
  R_RegisterCCallable("libproj", "proj_datum_ensemble_get_member", (DL_FUNC) &proj_datum_ensemble_get_member);
  R_RegisterCCallable("libproj", "proj_dynamic_datum_get_frame_reference_epoch", (DL_FUNC) &proj_dynamic_datum_get_frame_reference_epoch);
  R_RegisterCCallable("libproj", "proj_crs_get_coordinate_system", (DL_FUNC) &proj_crs_get_coordinate_system);
  R_RegisterCCallable("libproj", "proj_cs_get_type", (DL_FUNC) &proj_cs_get_type);
  R_RegisterCCallable("libproj", "proj_cs_get_axis_count", (DL_FUNC) &proj_cs_get_axis_count);
  R_RegisterCCallable("libproj", "proj_cs_get_axis_info", (DL_FUNC) &proj_cs_get_axis_info);
  R_RegisterCCallable("libproj", "proj_get_ellipsoid", (DL_FUNC) &proj_get_ellipsoid);
  R_RegisterCCallable("libproj", "proj_ellipsoid_get_parameters", (DL_FUNC) &proj_ellipsoid_get_parameters);
  R_RegisterCCallable("libproj", "proj_get_celestial_body_name", (DL_FUNC) &proj_get_celestial_body_name);
  R_RegisterCCallable("libproj", "proj_get_prime_meridian", (DL_FUNC) &proj_get_prime_meridian);
  R_RegisterCCallable("libproj", "proj_prime_meridian_get_parameters", (DL_FUNC) &proj_prime_meridian_get_parameters);
  R_RegisterCCallable("libproj", "proj_crs_get_coordoperation", (DL_FUNC) &proj_crs_get_coordoperation);
  R_RegisterCCallable("libproj", "proj_coordoperation_get_method_info", (DL_FUNC) &proj_coordoperation_get_method_info);
  R_RegisterCCallable("libproj", "proj_coordoperation_is_instantiable", (DL_FUNC) &proj_coordoperation_is_instantiable);
  R_RegisterCCallable("libproj", "proj_coordoperation_has_ballpark_transformation", (DL_FUNC) &proj_coordoperation_has_ballpark_transformation);
  R_RegisterCCallable("libproj", "proj_coordoperation_get_param_count", (DL_FUNC) &proj_coordoperation_get_param_count);
  R_RegisterCCallable("libproj", "proj_coordoperation_get_param_index", (DL_FUNC) &proj_coordoperation_get_param_index);
  R_RegisterCCallable("libproj", "proj_coordoperation_get_param", (DL_FUNC) &proj_coordoperation_get_param);
  R_RegisterCCallable("libproj", "proj_coordoperation_get_grid_used_count", (DL_FUNC) &proj_coordoperation_get_grid_used_count);
  R_RegisterCCallable("libproj", "proj_coordoperation_get_grid_used", (DL_FUNC) &proj_coordoperation_get_grid_used);
  R_RegisterCCallable("libproj", "proj_coordoperation_get_accuracy", (DL_FUNC) &proj_coordoperation_get_accuracy);
  R_RegisterCCallable("libproj", "proj_coordoperation_get_towgs84_values", (DL_FUNC) &proj_coordoperation_get_towgs84_values);
  R_RegisterCCallable("libproj", "proj_coordoperation_create_inverse", (DL_FUNC) &proj_coordoperation_create_inverse);
  R_RegisterCCallable("libproj", "proj_concatoperation_get_step_count", (DL_FUNC) &proj_concatoperation_get_step_count);
  R_RegisterCCallable("libproj", "proj_concatoperation_get_step", (DL_FUNC) &proj_concatoperation_get_step);

  return R_NilValue;
}
