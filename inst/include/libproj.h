// generated automatically by data-raw/update-libproj-api.R - do not edit by hand!
#ifndef LIBPROJ_H
#define LIBPROJ_H

#ifndef __cplusplus
# include <stddef.h>
#else
# include <cstddef>
using std::size_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
#define PJ_DEFAULT_CTX nullptr
#else
#define PJ_DEFAULT_CTX 0
#endif

#define PROJ_VERSION_MAJOR 8
#define PROJ_VERSION_MINOR 2
#define PROJ_VERSION_PATCH 1

// how integer versions are calculated
#define LIBPROJ_VERSION_INT(major, minor, patch) (patch + minor * 100 + major * 10000)

// the runtime version of libproj
extern int (*libproj_version_int)();

// the compile-time version of libproj
#define LIBPROJ_VERSION_COMPILE_INT LIBPROJ_VERSION_INT(PROJ_VERSION_MAJOR, PROJ_VERSION_MINOR, PROJ_VERSION_PATCH)

typedef union PJ_COORD PJ_COORD;
typedef struct PJ_AREA PJ_AREA;
typedef struct P5_FACTORS PJ_FACTORS;
typedef struct PJconsts PJ;
typedef struct PJ_INFO PJ_INFO;
typedef struct PJ_PROJ_INFO PJ_PROJ_INFO;
typedef struct PJ_GRID_INFO PJ_GRID_INFO;
typedef struct PJ_INIT_INFO PJ_INIT_INFO;
typedef struct PJ_LIST PJ_OPERATIONS;
typedef struct PJ_ELLPS PJ_ELLPS;
typedef struct PJ_UNITS PJ_UNITS;
typedef struct PJ_PRIME_MERIDIANS PJ_PRIME_MERIDIANS;
typedef struct pj_ctx PJ_CONTEXT;
typedef struct PROJ_FILE_HANDLE PROJ_FILE_HANDLE;
typedef struct PROJ_NETWORK_HANDLE PROJ_NETWORK_HANDLE;
typedef struct PJ_OBJ_LIST PJ_OBJ_LIST;
typedef struct PJ_INSERT_SESSION PJ_INSERT_SESSION;
typedef struct PJ_OPERATION_FACTORY_CONTEXT PJ_OPERATION_FACTORY_CONTEXT;
typedef char **PROJ_STRING_LIST;

typedef enum PJ_LOG_LEVEL {
  PJ_LOG_NONE = 0,
  PJ_LOG_ERROR = 1,
  PJ_LOG_DEBUG = 2,
  PJ_LOG_TRACE = 3,
  PJ_LOG_TELL = 4,
  PJ_LOG_DEBUG_MAJOR = 2,
  PJ_LOG_DEBUG_MINOR = 3
} PJ_LOG_LEVEL;

typedef enum PROJ_OPEN_ACCESS {
  PROJ_OPEN_ACCESS_READ_ONLY,
  PROJ_OPEN_ACCESS_READ_UPDATE,
  PROJ_OPEN_ACCESS_CREATE
} PROJ_OPEN_ACCESS;

enum PJ_DIRECTION {
  PJ_FWD = 1,
  PJ_IDENT = 0,
  PJ_INV = -1
};

typedef enum {
  PJ_GUESSED_WKT2_2019,
  PJ_GUESSED_WKT2_2018 = PJ_GUESSED_WKT2_2019,
  PJ_GUESSED_WKT2_2015,
  PJ_GUESSED_WKT1_GDAL,
  PJ_GUESSED_WKT1_ESRI,
  PJ_GUESSED_NOT_WKT
} PJ_GUESSED_WKT_DIALECT;

typedef enum {
  PJ_CATEGORY_ELLIPSOID,
  PJ_CATEGORY_PRIME_MERIDIAN,
  PJ_CATEGORY_DATUM,
  PJ_CATEGORY_CRS,
  PJ_CATEGORY_COORDINATE_OPERATION,
  PJ_CATEGORY_DATUM_ENSEMBLE
} PJ_CATEGORY;

typedef enum {
  PJ_TYPE_UNKNOWN,
  PJ_TYPE_ELLIPSOID,
  PJ_TYPE_PRIME_MERIDIAN,
  PJ_TYPE_GEODETIC_REFERENCE_FRAME,
  PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME,
  PJ_TYPE_VERTICAL_REFERENCE_FRAME,
  PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME,
  PJ_TYPE_DATUM_ENSEMBLE,
  PJ_TYPE_CRS,
  PJ_TYPE_GEODETIC_CRS,
  PJ_TYPE_GEOCENTRIC_CRS,
  PJ_TYPE_GEOGRAPHIC_CRS,
  PJ_TYPE_GEOGRAPHIC_2D_CRS,
  PJ_TYPE_GEOGRAPHIC_3D_CRS,
  PJ_TYPE_VERTICAL_CRS,
  PJ_TYPE_PROJECTED_CRS,
  PJ_TYPE_COMPOUND_CRS,
  PJ_TYPE_TEMPORAL_CRS,
  PJ_TYPE_ENGINEERING_CRS,
  PJ_TYPE_BOUND_CRS,
  PJ_TYPE_OTHER_CRS,
  PJ_TYPE_CONVERSION,
  PJ_TYPE_TRANSFORMATION,
  PJ_TYPE_CONCATENATED_OPERATION,
  PJ_TYPE_OTHER_COORDINATE_OPERATION,
  PJ_TYPE_TEMPORAL_DATUM,
  PJ_TYPE_ENGINEERING_DATUM,
  PJ_TYPE_PARAMETRIC_DATUM,
} PJ_TYPE;

typedef enum {
  PJ_COMP_STRICT,
  PJ_COMP_EQUIVALENT,
  PJ_COMP_EQUIVALENT_EXCEPT_AXIS_ORDER_GEOGCRS,
} PJ_COMPARISON_CRITERION;

typedef enum {
  PJ_WKT2_2015,
  PJ_WKT2_2015_SIMPLIFIED,
  PJ_WKT2_2019,
  PJ_WKT2_2018 = PJ_WKT2_2019,
  PJ_WKT2_2019_SIMPLIFIED,
  PJ_WKT2_2018_SIMPLIFIED = PJ_WKT2_2019_SIMPLIFIED,
  PJ_WKT1_GDAL,
  PJ_WKT1_ESRI
} PJ_WKT_TYPE;

typedef enum {
  PJ_CRS_EXTENT_NONE,
  PJ_CRS_EXTENT_BOTH,
  PJ_CRS_EXTENT_INTERSECTION,
  PJ_CRS_EXTENT_SMALLEST
} PROJ_CRS_EXTENT_USE;

typedef enum {
  PROJ_GRID_AVAILABILITY_USED_FOR_SORTING,
  PROJ_GRID_AVAILABILITY_DISCARD_OPERATION_IF_MISSING_GRID,
  PROJ_GRID_AVAILABILITY_IGNORED,
  PROJ_GRID_AVAILABILITY_KNOWN_AVAILABLE,
} PROJ_GRID_AVAILABILITY_USE;

typedef enum {
  PJ_PROJ_5,
  PJ_PROJ_4
} PJ_PROJ_STRING_TYPE;

typedef enum {
  PROJ_SPATIAL_CRITERION_STRICT_CONTAINMENT,
  PROJ_SPATIAL_CRITERION_PARTIAL_INTERSECTION
} PROJ_SPATIAL_CRITERION;

typedef enum {
  PROJ_INTERMEDIATE_CRS_USE_ALWAYS,
  PROJ_INTERMEDIATE_CRS_USE_IF_NO_DIRECT_TRANSFORMATION,
  PROJ_INTERMEDIATE_CRS_USE_NEVER,
} PROJ_INTERMEDIATE_CRS_USE;

typedef enum {
  PJ_CS_TYPE_UNKNOWN,
  PJ_CS_TYPE_CARTESIAN,
  PJ_CS_TYPE_ELLIPSOIDAL,
  PJ_CS_TYPE_VERTICAL,
  PJ_CS_TYPE_SPHERICAL,
  PJ_CS_TYPE_ORDINAL,
  PJ_CS_TYPE_PARAMETRIC,
  PJ_CS_TYPE_DATETIMETEMPORAL,
  PJ_CS_TYPE_TEMPORALCOUNT,
  PJ_CS_TYPE_TEMPORALMEASURE
} PJ_COORDINATE_SYSTEM_TYPE;

typedef enum PJ_DIRECTION PJ_DIRECTION;

typedef void (*PJ_LOG_FUNCTION)(void *, int, const char *);

typedef const char* (*proj_file_finder) (PJ_CONTEXT *ctx, const char*, void* user_data);

typedef PROJ_NETWORK_HANDLE* (*proj_network_open_cbk_type)(
  PJ_CONTEXT* ctx,
  const char* url,
  unsigned long long offset,
  size_t size_to_read,
  void* buffer,
  size_t* out_size_read,
  size_t error_string_max_size,
  char* out_error_string,
  void* user_data);

typedef void (*proj_network_close_cbk_type)(PJ_CONTEXT* ctx,
  PROJ_NETWORK_HANDLE* handle,
  void* user_data);

typedef const char* (*proj_network_get_header_value_cbk_type)(
  PJ_CONTEXT* ctx,
  PROJ_NETWORK_HANDLE* handle,
  const char* header_name,
  void* user_data);

typedef size_t (*proj_network_read_range_type)(
  PJ_CONTEXT* ctx,
  PROJ_NETWORK_HANDLE* handle,
  unsigned long long offset,
  size_t size_to_read,
  void* buffer,
  size_t error_string_max_size,
  char* out_error_string,
  void* user_data);

struct PJ_LIST {
  const char *id;
  PJ *(*proj)(PJ *);
  const char * const *descr;
};

struct PJ_ELLPS {
  const char *id;
  const char *major;
  const char *ell;
  const char *name;
};

struct PJ_UNITS {
  const char *id;
  const char *to_meter;
  const char *name;
  double factor;
};

struct PJ_PRIME_MERIDIANS {
  const char *id;
  const char *defn;
};

typedef struct { double x, y, z, t; } PJ_XYZT;

typedef struct { double u, v, w, t; } PJ_UVWT;

typedef struct { double lam, phi, z, t; } PJ_LPZT;

typedef struct { double o, p, k; } PJ_OPK;

typedef struct { double e, n, u; } PJ_ENU;

typedef struct { double s, a1, a2; } PJ_GEOD;

typedef struct { double u, v; } PJ_UV;

typedef struct { double x, y; } PJ_XY;

typedef struct { double lam, phi; } PJ_LP;

typedef struct { double x, y, z; } PJ_XYZ;

typedef struct { double u, v, w; } PJ_UVW;

typedef struct { double lam, phi, z; } PJ_LPZ;

union PJ_COORD {
  double v[4];
  PJ_XYZT xyzt;
  PJ_UVWT uvwt;
  PJ_LPZT lpzt;
  PJ_GEOD geod;
  PJ_OPK opk;
  PJ_ENU enu;
  PJ_XYZ xyz;
  PJ_UVW uvw;
  PJ_LPZ lpz;
  PJ_XY xy;
  PJ_UV uv;
  PJ_LP lp;
};

struct PJ_INFO {
  int major;
  int minor;
  int patch;
  const char *release;
  const char *version;
  const char *searchpath;
  const char * const *paths;
  size_t path_count;
};

struct PJ_PROJ_INFO {
  const char *id;
  const char *description;
  const char *definition;
  int has_inverse;
  double accuracy;
};

struct PJ_GRID_INFO {
  char gridname[32];
  char filename[260];
  char format[8];
  PJ_LP lowerleft;
  PJ_LP upperright;
  int n_lon, n_lat;
  double cs_lon, cs_lat;
};

struct PJ_INIT_INFO {
  char name[32];
  char filename[260];
  char version[32];
  char origin[32];
  char lastupdate[16];
};

typedef struct PROJ_FILE_API {
  int version;
  PROJ_FILE_HANDLE* (*open_cbk)(PJ_CONTEXT *ctx, const char *filename, PROJ_OPEN_ACCESS access, void* user_data);
  size_t (*read_cbk)(PJ_CONTEXT *ctx, PROJ_FILE_HANDLE*, void* buffer, size_t sizeBytes, void* user_data);
  size_t (*write_cbk)(PJ_CONTEXT *ctx, PROJ_FILE_HANDLE*, const void* buffer, size_t sizeBytes, void* user_data);
  int (*seek_cbk)(PJ_CONTEXT *ctx, PROJ_FILE_HANDLE*, long long offset, int whence, void* user_data);
  unsigned long long (*tell_cbk)(PJ_CONTEXT *ctx, PROJ_FILE_HANDLE*, void* user_data);
  void (*close_cbk)(PJ_CONTEXT *ctx, PROJ_FILE_HANDLE*, void* user_data);
  int (*exists_cbk)(PJ_CONTEXT *ctx, const char *filename, void* user_data);
  int (*mkdir_cbk)(PJ_CONTEXT *ctx, const char *filename, void* user_data);
  int (*unlink_cbk)(PJ_CONTEXT *ctx, const char *filename, void* user_data);
  int (*rename_cbk)(PJ_CONTEXT *ctx, const char *oldPath, const char *newPath, void* user_data);
} PROJ_FILE_API;

typedef struct {
  char* auth_name;
  char* code;
  char* name;
  PJ_TYPE type;
  int deprecated;
  int bbox_valid;
  double west_lon_degree;
  double south_lat_degree;
  double east_lon_degree;
  double north_lat_degree;
  char* area_name;
  char* projection_method_name;
  char* celestial_body_name;
} PROJ_CRS_INFO;

typedef struct {
  const PJ_TYPE* types;
  size_t typesCount;
  int crs_area_of_use_contains_bbox;
  int bbox_valid;
  double west_lon_degree;
  double south_lat_degree;
  double east_lon_degree;
  double north_lat_degree;
  int allow_deprecated;
  const char* celestial_body_name;
} PROJ_CRS_LIST_PARAMETERS;

typedef struct {
  char* auth_name;
  char* code;
  char* name;
  char* category;
  double conv_factor;
  char* proj_short_name;
  int deprecated;
} PROJ_UNIT_INFO;

typedef struct {
  char* auth_name;
  char* name;
} PROJ_CELESTIAL_BODY_INFO;

extern PJ_CONTEXT* (*proj_context_create)(void);
extern PJ_CONTEXT* (*proj_context_destroy)(PJ_CONTEXT*);
extern PJ_CONTEXT* (*proj_context_clone)(PJ_CONTEXT*);
extern void (*proj_context_set_file_finder)(PJ_CONTEXT*, proj_file_finder, void*);
extern void (*proj_context_set_search_paths)(PJ_CONTEXT*, int, const char* const*);
extern void (*proj_context_set_ca_bundle_path)(PJ_CONTEXT*, const char*);
extern void (*proj_context_use_proj4_init_rules)(PJ_CONTEXT*, int);
extern int (*proj_context_get_use_proj4_init_rules)(PJ_CONTEXT*, int);
extern int (*proj_context_set_fileapi)( PJ_CONTEXT*, const PROJ_FILE_API*, void*);
extern void (*proj_context_set_sqlite3_vfs_name)(PJ_CONTEXT*, const char*);
extern int (*proj_context_set_network_callbacks)( PJ_CONTEXT*, proj_network_open_cbk_type, proj_network_close_cbk_type, proj_network_get_header_value_cbk_type, proj_network_read_range_type, void*);
extern int (*proj_context_set_enable_network)(PJ_CONTEXT*, int);
extern int (*proj_context_is_network_enabled)(PJ_CONTEXT*);
extern void (*proj_context_set_url_endpoint)(PJ_CONTEXT*, const char*);
extern const char* (*proj_context_get_url_endpoint)(PJ_CONTEXT*);
extern const char* (*proj_context_get_user_writable_directory)(PJ_CONTEXT*, int);
extern void (*proj_grid_cache_set_enable)(PJ_CONTEXT*, int);
extern void (*proj_grid_cache_set_filename)(PJ_CONTEXT*, const char*);
extern void (*proj_grid_cache_set_max_size)(PJ_CONTEXT*, int);
extern void (*proj_grid_cache_set_ttl)(PJ_CONTEXT*, int);
extern void (*proj_grid_cache_clear)(PJ_CONTEXT*);
extern int (*proj_is_download_needed)(PJ_CONTEXT*, const char*, int);
extern int (*proj_download_file)(PJ_CONTEXT*, const char*, int, int*);
extern PJ* (*proj_create)(PJ_CONTEXT*, const char*);
extern PJ* (*proj_create_argv)(PJ_CONTEXT*, int, char**);
extern PJ* (*proj_create_crs_to_crs)(PJ_CONTEXT*, const char*, const char*, PJ_AREA*);
extern PJ* (*proj_create_crs_to_crs_from_pj)(PJ_CONTEXT*, const PJ*, const PJ*, PJ_AREA*, const char* const*);
extern PJ* (*proj_normalize_for_visualization)(PJ_CONTEXT*, const PJ*);
extern void (*proj_assign_context)(PJ*, PJ_CONTEXT*);
extern PJ* (*proj_destroy)(PJ*);
extern PJ_AREA* (*proj_area_create)(void);
extern void (*proj_area_set_bbox)(PJ_AREA*, double, double, double, double);
extern void (*proj_area_destroy)(PJ_AREA*);
extern int (*proj_angular_input)(PJ*, enum PJ_DIRECTION);
extern int (*proj_angular_output)(PJ*, enum PJ_DIRECTION);
extern int (*proj_degree_input)(PJ*, enum PJ_DIRECTION);
extern int (*proj_degree_output)(PJ*, enum PJ_DIRECTION);
extern PJ_COORD (*proj_trans)(PJ*, PJ_DIRECTION, PJ_COORD);
extern int (*proj_trans_array)(PJ*, PJ_DIRECTION, size_t, PJ_COORD*);
extern size_t (*proj_trans_generic)( PJ*, PJ_DIRECTION, double*, size_t, size_t, double*, size_t, size_t, double*, size_t, size_t, double*, size_t, size_t nt );
extern int (*proj_trans_bounds)( PJ_CONTEXT*, PJ*, PJ_DIRECTION, double, double, double, double, double*, double*, double*, double*, int densify_pts );
extern PJ_COORD (*proj_coord)(double, double, double, double);
extern double (*proj_roundtrip)(PJ*, PJ_DIRECTION, int, PJ_COORD*);
extern double (*proj_lp_dist)(const PJ*, PJ_COORD, PJ_COORD);
extern double (*proj_lpz_dist)(const PJ*, PJ_COORD, PJ_COORD);
extern double (*proj_xy_dist)(PJ_COORD, PJ_COORD);
extern double (*proj_xyz_dist)(PJ_COORD, PJ_COORD);
extern PJ_COORD (*proj_geod)(const PJ*, PJ_COORD, PJ_COORD);
extern int (*proj_context_errno)(PJ_CONTEXT*);
extern int (*proj_errno)(const PJ*);
extern int (*proj_errno_set)(const PJ*, int);
extern int (*proj_errno_reset)(const PJ*);
extern int (*proj_errno_restore)(const PJ*, int);
extern const char* (*proj_errno_string)(int);
extern const char* (*proj_context_errno_string)(PJ_CONTEXT*, int);
extern PJ_LOG_LEVEL (*proj_log_level)(PJ_CONTEXT*, PJ_LOG_LEVEL);
extern void (*proj_log_func)(PJ_CONTEXT*, void*, PJ_LOG_FUNCTION);
extern PJ_FACTORS (*proj_factors)(PJ*, PJ_COORD);
extern PJ_INFO (*proj_info)(void);
extern PJ_PROJ_INFO (*proj_pj_info)(PJ*);
extern PJ_GRID_INFO (*proj_grid_info)(const char*);
extern PJ_INIT_INFO (*proj_init_info)(const char*);
extern const PJ_OPERATIONS* (*proj_list_operations)(void);
extern const PJ_ELLPS* (*proj_list_ellps)(void);
extern const PJ_PRIME_MERIDIANS* (*proj_list_prime_meridians)(void);
extern double (*proj_torad)(double);
extern double (*proj_todeg)(double);
extern double (*proj_dmstor)(const char*, char**);
extern char* (*proj_rtodms)(char*, double, int, int);
extern void (*proj_cleanup)(void);
extern void (*proj_string_list_destroy)(PROJ_STRING_LIST);
extern void (*proj_context_set_autoclose_database)(PJ_CONTEXT*, int);
extern int (*proj_context_set_database_path)(PJ_CONTEXT*, const char*, const char *const*, const char* const*);
extern const char* (*proj_context_get_database_path)(PJ_CONTEXT*);
extern const char* (*proj_context_get_database_metadata)(PJ_CONTEXT*, const char*);
extern PROJ_STRING_LIST (*proj_context_get_database_structure)( PJ_CONTEXT*, const char* const*);
extern PJ_GUESSED_WKT_DIALECT (*proj_context_guess_wkt_dialect)(PJ_CONTEXT*, const char*);
extern PJ* (*proj_create_from_wkt)(PJ_CONTEXT*, const char*, const char* const*, PROJ_STRING_LIST*, PROJ_STRING_LIST*);
extern PJ* (*proj_create_from_database)(PJ_CONTEXT*, const char*, const char*, PJ_CATEGORY, int, const char* const*);
extern int (*proj_uom_get_info_from_database)(PJ_CONTEXT*, const char*, const char*, const char**, double*, const char**);
extern int (*proj_grid_get_info_from_database)(PJ_CONTEXT*, const char*, const char**, const char**, const char**, int*, int*, int*);
extern PJ* (*proj_clone)(PJ_CONTEXT*, const PJ*);
extern PJ_OBJ_LIST* (*proj_create_from_name)(PJ_CONTEXT*, const char*, const char*, const PJ_TYPE*, size_t, int, size_t, const char* const*);
extern PJ_TYPE (*proj_get_type)(const PJ*);
extern int (*proj_is_deprecated)(const PJ*);
extern PJ_OBJ_LIST* (*proj_get_non_deprecated)(PJ_CONTEXT*, const PJ*);
extern int (*proj_is_equivalent_to)(const PJ*, const PJ*, PJ_COMPARISON_CRITERION);
extern int (*proj_is_equivalent_to_with_ctx)(PJ_CONTEXT*, const PJ*, const PJ*, PJ_COMPARISON_CRITERION);
extern int (*proj_is_crs)(const PJ*);
extern const char* (*proj_get_name)(const PJ*);
extern const char* (*proj_get_id_auth_name)(const PJ*, int);
extern const char* (*proj_get_id_code)(const PJ*, int);
extern const char* (*proj_get_remarks)(const PJ*);
extern const char* (*proj_get_scope)(const PJ*);
extern int (*proj_get_area_of_use)(PJ_CONTEXT*, const PJ*, double*, double*, double*, double*, const char**);
extern const char* (*proj_as_wkt)(PJ_CONTEXT*, const PJ*, PJ_WKT_TYPE, const char* const*);
extern const char* (*proj_as_proj_string)(PJ_CONTEXT*, const PJ*, PJ_PROJ_STRING_TYPE, const char* const*);
extern const char* (*proj_as_projjson)(PJ_CONTEXT*, const PJ*, const char* const*);
extern PJ* (*proj_get_source_crs)(PJ_CONTEXT*, const PJ*);
extern PJ* (*proj_get_target_crs)(PJ_CONTEXT*, const PJ*);
extern PJ_OBJ_LIST* (*proj_identify)(PJ_CONTEXT*, const PJ*, const char*, const char* const*, int**);
extern PROJ_STRING_LIST (*proj_get_geoid_models_from_database)( PJ_CONTEXT*, const char*, const char*, const char *const*);
extern void (*proj_int_list_destroy)(int*);
extern PROJ_STRING_LIST (*proj_get_authorities_from_database)(PJ_CONTEXT*);
extern PROJ_STRING_LIST (*proj_get_codes_from_database)(PJ_CONTEXT*, const char*, PJ_TYPE, int);
extern PROJ_CELESTIAL_BODY_INFO* (*proj_get_celestial_body_list_from_database)( PJ_CONTEXT*, const char*, int*);
extern void (*proj_celestial_body_list_destroy)(PROJ_CELESTIAL_BODY_INFO**);
extern PROJ_CRS_LIST_PARAMETERS* (*proj_get_crs_list_parameters_create)(void);
extern void (*proj_get_crs_list_parameters_destroy)( PROJ_CRS_LIST_PARAMETERS*);
extern PROJ_CRS_INFO* (*proj_get_crs_info_list_from_database)( PJ_CONTEXT*, const char*, const PROJ_CRS_LIST_PARAMETERS*, int*);
extern void (*proj_crs_info_list_destroy)(PROJ_CRS_INFO**);
extern PROJ_UNIT_INFO* (*proj_get_units_from_database)( PJ_CONTEXT*, const char*, const char*, int, int*);
extern void (*proj_unit_list_destroy)(PROJ_UNIT_INFO**);
extern PJ_INSERT_SESSION* (*proj_insert_object_session_create)(PJ_CONTEXT*);
extern void (*proj_insert_object_session_destroy)(PJ_CONTEXT*, PJ_INSERT_SESSION*);
extern PROJ_STRING_LIST (*proj_get_insert_statements)(PJ_CONTEXT*, PJ_INSERT_SESSION*, const PJ*, const char*, const char*, int, const char *const*, const char *const*);
extern char* (*proj_suggests_code_for)(PJ_CONTEXT*, const PJ*, const char*, int, const char *const*);
extern void (*proj_string_destroy)(char*);
extern PJ_OPERATION_FACTORY_CONTEXT* (*proj_create_operation_factory_context)( PJ_CONTEXT*, const char*);
extern void (*proj_operation_factory_context_destroy)( PJ_OPERATION_FACTORY_CONTEXT*);
extern void (*proj_operation_factory_context_set_desired_accuracy)( PJ_CONTEXT*, PJ_OPERATION_FACTORY_CONTEXT*, double);
extern void (*proj_operation_factory_context_set_area_of_interest)( PJ_CONTEXT*, PJ_OPERATION_FACTORY_CONTEXT*, double, double, double, double);
extern void (*proj_operation_factory_context_set_crs_extent_use)( PJ_CONTEXT*, PJ_OPERATION_FACTORY_CONTEXT*, PROJ_CRS_EXTENT_USE);
extern void (*proj_operation_factory_context_set_spatial_criterion)( PJ_CONTEXT*, PJ_OPERATION_FACTORY_CONTEXT*, PROJ_SPATIAL_CRITERION);
extern void (*proj_operation_factory_context_set_grid_availability_use)( PJ_CONTEXT*, PJ_OPERATION_FACTORY_CONTEXT*, PROJ_GRID_AVAILABILITY_USE);
extern void (*proj_operation_factory_context_set_use_proj_alternative_grid_names)( PJ_CONTEXT*, PJ_OPERATION_FACTORY_CONTEXT*, int);
extern void (*proj_operation_factory_context_set_allow_use_intermediate_crs)( PJ_CONTEXT*, PJ_OPERATION_FACTORY_CONTEXT*, PROJ_INTERMEDIATE_CRS_USE);
extern void (*proj_operation_factory_context_set_allowed_intermediate_crs)( PJ_CONTEXT*, PJ_OPERATION_FACTORY_CONTEXT*, const char* const*);
extern void (*proj_operation_factory_context_set_discard_superseded)( PJ_CONTEXT*, PJ_OPERATION_FACTORY_CONTEXT*, int);
extern void (*proj_operation_factory_context_set_allow_ballpark_transformations)( PJ_CONTEXT*, PJ_OPERATION_FACTORY_CONTEXT*, int);
extern PJ_OBJ_LIST* (*proj_create_operations)( PJ_CONTEXT*, const PJ*, const PJ*, const PJ_OPERATION_FACTORY_CONTEXT*);
extern int (*proj_list_get_count)(const PJ_OBJ_LIST*);
extern PJ* (*proj_list_get)(PJ_CONTEXT*, const PJ_OBJ_LIST*, int);
extern void (*proj_list_destroy)(PJ_OBJ_LIST*);
extern int (*proj_get_suggested_operation)(PJ_CONTEXT*, PJ_OBJ_LIST*, PJ_DIRECTION, PJ_COORD);
extern int (*proj_crs_is_derived)(PJ_CONTEXT*, const PJ*);
extern PJ* (*proj_crs_get_geodetic_crs)(PJ_CONTEXT*, const PJ*);
extern PJ* (*proj_crs_get_horizontal_datum)(PJ_CONTEXT*, const PJ*);
extern PJ* (*proj_crs_get_sub_crs)(PJ_CONTEXT*, const PJ*, int);
extern PJ* (*proj_crs_get_datum)(PJ_CONTEXT*, const PJ*);
extern PJ* (*proj_crs_get_datum_ensemble)(PJ_CONTEXT*, const PJ*);
extern PJ* (*proj_crs_get_datum_forced)(PJ_CONTEXT*, const PJ*);
extern int (*proj_datum_ensemble_get_member_count)(PJ_CONTEXT*, const PJ*);
extern double (*proj_datum_ensemble_get_accuracy)(PJ_CONTEXT*, const PJ*);
extern PJ* (*proj_datum_ensemble_get_member)(PJ_CONTEXT*, const PJ*, int);
extern double (*proj_dynamic_datum_get_frame_reference_epoch)(PJ_CONTEXT*, const PJ*);
extern PJ* (*proj_crs_get_coordinate_system)(PJ_CONTEXT*, const PJ*);
extern PJ_COORDINATE_SYSTEM_TYPE (*proj_cs_get_type)(PJ_CONTEXT*, const PJ*);
extern int (*proj_cs_get_axis_count)(PJ_CONTEXT*, const PJ*);
extern int (*proj_cs_get_axis_info)(PJ_CONTEXT*, const PJ*, int, const char**, const char**, const char**, double*, const char**, const char**, const char**);
extern PJ* (*proj_get_ellipsoid)(PJ_CONTEXT*, const PJ*);
extern int (*proj_ellipsoid_get_parameters)(PJ_CONTEXT*, const PJ*, double*, double*, int*, double*);
extern const char* (*proj_get_celestial_body_name)(PJ_CONTEXT*, const PJ*);
extern PJ* (*proj_get_prime_meridian)(PJ_CONTEXT*, const PJ*);
extern int (*proj_prime_meridian_get_parameters)(PJ_CONTEXT*, const PJ*, double*, double*, const char**);
extern PJ* (*proj_crs_get_coordoperation)(PJ_CONTEXT*, const PJ*);
extern int (*proj_coordoperation_get_method_info)(PJ_CONTEXT*, const PJ*, const char**, const char**, const char**);
extern int (*proj_coordoperation_is_instantiable)(PJ_CONTEXT*, const PJ*);
extern int (*proj_coordoperation_has_ballpark_transformation)(PJ_CONTEXT*, const PJ*);
extern int (*proj_coordoperation_get_param_count)(PJ_CONTEXT*, const PJ*);
extern int (*proj_coordoperation_get_param_index)(PJ_CONTEXT*, const PJ*, const char*);
extern int (*proj_coordoperation_get_param)(PJ_CONTEXT*, const PJ*, int, const char**, const char**, const char**, double*, const char**, double*, const char**, const char**, const char**, const char**);
extern int (*proj_coordoperation_get_grid_used_count)(PJ_CONTEXT*, const PJ*);
extern int (*proj_coordoperation_get_grid_used)(PJ_CONTEXT*, const PJ*, int, const char**, const char**, const char**, const char**, int*, int*, int*);
extern double (*proj_coordoperation_get_accuracy)(PJ_CONTEXT*, const PJ*);
extern int (*proj_coordoperation_get_towgs84_values)(PJ_CONTEXT*, const PJ*, double*, int, int);
extern PJ* (*proj_coordoperation_create_inverse)(PJ_CONTEXT*, const PJ*);
extern int (*proj_concatoperation_get_step_count)(PJ_CONTEXT*, const PJ*);
extern PJ* (*proj_concatoperation_get_step)(PJ_CONTEXT*, const PJ*, int);

void libproj_init_api();

#ifdef __cplusplus
}
#endif

#endif
