// generated automatically by data-raw/update-libproj-api.R - do not edit by hand!
#ifndef LIBPROJ_H
#define LIBPROJ_H

#ifndef __cplusplus
# include <stddef.h> /* for size_t definition */
#else
# include <cstddef>
using std::size_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define PROJ_DLL

#define PROJ_VERSION_MAJOR 8
#define PROJ_VERSION_MINOR 1
#define PROJ_VERSION_PATCH 0

/* Note: the following 3 defines have been introduced in PROJ 8.0.1 */
/* Macro to compute a PROJ version number from its components */
#define PROJ_COMPUTE_VERSION(maj,min,patch) ((maj)*10000+(min)*100+(patch))

/* Current PROJ version from the above version numbers */
#define PROJ_VERSION_NUMBER                 \
    PROJ_COMPUTE_VERSION(PROJ_VERSION_MAJOR, PROJ_VERSION_MINOR, PROJ_VERSION_PATCH)

/* Macro that returns true if the current PROJ version is at least the version specified by (maj,min,patch) */
#define PROJ_AT_LEAST_VERSION(maj,min,patch) \
    (PROJ_VERSION_NUMBER >= PROJ_COMPUTE_VERSION(maj,min,patch))


// how integer versions are calculated
#define LIBPROJ_VERSION_INT(major, minor, patch) (patch + minor * 100 + major * 10000)

// the runtime version of libproj
extern int (*libproj_version_int)();

// the compile-time version of libPROJ
#define LIBPROJ_VERSION_COMPILE_INT LIBPROJ_VERSION_INT(PROJ_VERSION_MAJOR, PROJ_VERSION_MINOR, PROJ_VERSION_PATCH)

union PJ_COORD;
typedef union PJ_COORD PJ_COORD;

struct PJ_AREA;
typedef struct PJ_AREA PJ_AREA;

struct P5_FACTORS {                  /* Common designation */
    double meridional_scale;               /* h */
    double parallel_scale;                 /* k */
    double areal_scale;                    /* s */

    double angular_distortion;             /* omega */
    double meridian_parallel_angle;        /* theta-prime */
    double meridian_convergence;           /* alpha */

    double tissot_semimajor;               /* a */
    double tissot_semiminor;               /* b */

    double dx_dlam, dx_dphi;
    double dy_dlam, dy_dphi;
};
typedef struct P5_FACTORS PJ_FACTORS;

/* Data type for projection/transformation information */
struct PJconsts;
typedef struct PJconsts PJ;         /* the PJ object herself */

/* Data type for library level information */
struct PJ_INFO;
typedef struct PJ_INFO PJ_INFO;

struct PJ_PROJ_INFO;
typedef struct PJ_PROJ_INFO PJ_PROJ_INFO;

struct PJ_GRID_INFO;
typedef struct PJ_GRID_INFO PJ_GRID_INFO;

struct PJ_INIT_INFO;
typedef struct PJ_INIT_INFO PJ_INIT_INFO;

/* Data types for list of operations, ellipsoids, datums and units used in PROJ.4 */
struct PJ_LIST {
    const char  *id;                /* projection keyword */
    PJ          *(*proj)(PJ *);     /* projection entry point */
    const char  * const *descr;     /* description text */
};

typedef struct PJ_LIST PJ_OPERATIONS;

struct PJ_ELLPS {
    const char  *id;    /* ellipse keyword name */
    const char  *major; /* a= value */
    const char  *ell;   /* elliptical parameter */
    const char  *name;  /* comments */
};
typedef struct PJ_ELLPS PJ_ELLPS;

struct PJ_UNITS {
    const char  *id;        /* units keyword */
    const char  *to_meter;  /* multiply by value to get meters */
    const char  *name;      /* comments */
    double      factor;     /* to_meter factor in actual numbers */
};
typedef struct PJ_UNITS PJ_UNITS;

struct PJ_PRIME_MERIDIANS {
    const char  *id;        /* prime meridian keyword */
    const char  *defn;      /* offset from greenwich in DMS format. */
};
typedef struct PJ_PRIME_MERIDIANS PJ_PRIME_MERIDIANS;


/* Geodetic, mostly spatiotemporal coordinate types */
typedef struct { double   x,   y,  z, t; }  PJ_XYZT;
typedef struct { double   u,   v,  w, t; }  PJ_UVWT;
typedef struct { double lam, phi,  z, t; }  PJ_LPZT;
typedef struct { double o, p, k; }          PJ_OPK;  /* Rotations: omega, phi, kappa */
typedef struct { double e, n, u; }          PJ_ENU;  /* East, North, Up */
typedef struct { double s, a1, a2; }        PJ_GEOD; /* Geodesic length, fwd azi, rev azi */

/* Classic proj.4 pair/triplet types - moved into the PJ_ name space */
typedef struct { double   u,   v; }  PJ_UV;
typedef struct { double   x,   y; }  PJ_XY;
typedef struct { double lam, phi; }  PJ_LP;

typedef struct { double   x,   y,  z; }  PJ_XYZ;
typedef struct { double   u,   v,  w; }  PJ_UVW;
typedef struct { double lam, phi,  z; }  PJ_LPZ;


/* Avoid preprocessor renaming and implicit type-punning: Use a union to make it explicit */
union PJ_COORD {
     double v[4];   /* First and foremost, it really is "just 4 numbers in a vector" */
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
    int         major;              /* Major release number                 */
    int         minor;              /* Minor release number                 */
    int         patch;              /* Patch level                          */
    const char  *release;           /* Release info. Version + date         */
    const char  *version;           /* Full version number                  */
    const char  *searchpath;        /* Paths where init and grid files are  */
                                    /* looked for. Paths are separated by   */
                                    /* semi-colons on Windows, and colons   */
                                    /* on non-Windows platforms.            */
    const char * const *paths;
    size_t path_count;
};

struct PJ_PROJ_INFO {
    const char  *id;                /* Name of the projection in question                       */
    const char  *description;       /* Description of the projection                            */
    const char  *definition;        /* Projection definition                                    */
    int         has_inverse;        /* 1 if an inverse mapping exists, 0 otherwise              */
    double      accuracy;           /* Expected accuracy of the transformation. -1 if unknown.  */
};

struct PJ_GRID_INFO {
    char        gridname[32];       /* name of grid                         */
    char        filename[260];      /* full path to grid                    */
    char        format[8];          /* file format of grid                  */
    PJ_LP       lowerleft;          /* Coordinates of lower left corner     */
    PJ_LP       upperright;         /* Coordinates of upper right corner    */
    int         n_lon, n_lat;       /* Grid size                            */
    double      cs_lon, cs_lat;     /* Cell size of grid                    */
};

struct PJ_INIT_INFO {
    char        name[32];           /* name of init file                        */
    char        filename[260];      /* full path to the init file.              */
    char        version[32];        /* version of the init file                 */
    char        origin[32];         /* origin of the file, e.g. EPSG            */
    char        lastupdate[16];     /* Date of last update in YYYY-MM-DD format */
};

typedef enum PJ_LOG_LEVEL {
    PJ_LOG_NONE  = 0,
    PJ_LOG_ERROR = 1,
    PJ_LOG_DEBUG = 2,
    PJ_LOG_TRACE = 3,
    PJ_LOG_TELL  = 4,
    PJ_LOG_DEBUG_MAJOR = 2, /* for proj_api.h compatibility */
    PJ_LOG_DEBUG_MINOR = 3  /* for proj_api.h compatibility */
} PJ_LOG_LEVEL;

typedef void (*PJ_LOG_FUNCTION)(void *, int, const char *);


/* The context type - properly namespaced synonym for pj_ctx */
struct pj_ctx;
typedef struct pj_ctx PJ_CONTEXT;
#ifdef __cplusplus
#define PJ_DEFAULT_CTX nullptr
#else
#define PJ_DEFAULT_CTX 0
#endif
typedef const char* (*proj_file_finder) (PJ_CONTEXT *ctx, const char*, void* user_data);
/** Opaque structure for PROJ for a file handle. Implementations might cast it to their
 * structure/class of choice. */
typedef struct PROJ_FILE_HANDLE PROJ_FILE_HANDLE;

/** Open access / mode */
typedef enum PROJ_OPEN_ACCESS
{
    /** Read-only access. Equivalent to "rb" */
    PROJ_OPEN_ACCESS_READ_ONLY,

    /** Read-update access. File should be created if not existing. Equivalent to "r+b" */
    PROJ_OPEN_ACCESS_READ_UPDATE,

    /** Create access. File should be truncated to 0-byte if already existing. Equivalent to "w+b" */
    PROJ_OPEN_ACCESS_CREATE
} PROJ_OPEN_ACCESS;

/** File API callbacks */
typedef struct PROJ_FILE_API
{
    /** Version of this structure. Should be set to 1 currently. */
    int version;

    /** Open file. Return NULL if error */
    PROJ_FILE_HANDLE* (*open_cbk)(PJ_CONTEXT *ctx, const char *filename, PROJ_OPEN_ACCESS access, void* user_data);

    /** Read sizeBytes into buffer from current position and return number of bytes read */
    size_t            (*read_cbk)(PJ_CONTEXT *ctx, PROJ_FILE_HANDLE*, void* buffer, size_t sizeBytes, void* user_data);

    /** Write sizeBytes into buffer from current position and return number of bytes written */
    size_t            (*write_cbk)(PJ_CONTEXT *ctx, PROJ_FILE_HANDLE*, const void* buffer, size_t sizeBytes, void* user_data);

    /** Seek to offset using whence=SEEK_SET/SEEK_CUR/SEEK_END. Return TRUE in case of success */
    int               (*seek_cbk)(PJ_CONTEXT *ctx, PROJ_FILE_HANDLE*, long long offset, int whence, void* user_data);

    /** Return current file position */
    unsigned long long (*tell_cbk)(PJ_CONTEXT *ctx, PROJ_FILE_HANDLE*, void* user_data);

    /** Close file */
    void              (*close_cbk)(PJ_CONTEXT *ctx, PROJ_FILE_HANDLE*, void* user_data);

    /** Return TRUE if a file exists */
    int (*exists_cbk)(PJ_CONTEXT *ctx, const char *filename, void* user_data);

    /** Return TRUE if directory exists or could be created  */
    int (*mkdir_cbk)(PJ_CONTEXT *ctx, const char *filename, void* user_data);

    /** Return TRUE if file could be removed  */
    int (*unlink_cbk)(PJ_CONTEXT *ctx, const char *filename, void* user_data);

    /** Return TRUE if file could be renamed  */
    int (*rename_cbk)(PJ_CONTEXT *ctx, const char *oldPath, const char *newPath, void* user_data);
} PROJ_FILE_API;
/** Opaque structure for PROJ for a network handle. Implementations might cast it to their
 * structure/class of choice. */
typedef struct PROJ_NETWORK_HANDLE PROJ_NETWORK_HANDLE;

/** Network access: open callback
 * 
 * Should try to read the size_to_read first bytes at the specified offset of
 * the file given by URL url,
 * and write them to buffer. *out_size_read should be updated with the actual
 * amount of bytes read (== size_to_read if the file is larger than size_to_read).
 * During this read, the implementation should make sure to store the HTTP
 * headers from the server response to be able to respond to
 * proj_network_get_header_value_cbk_type callback.
 *
 * error_string_max_size should be the maximum size that can be written into
 * the out_error_string buffer (including terminating nul character).
 *
 * @return a non-NULL opaque handle in case of success.
 */
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

/** Network access: close callback */
typedef void (*proj_network_close_cbk_type)(PJ_CONTEXT* ctx,
                                            PROJ_NETWORK_HANDLE* handle,
                                            void* user_data);

/** Network access: get HTTP headers */
typedef const char* (*proj_network_get_header_value_cbk_type)(
                                            PJ_CONTEXT* ctx,
                                            PROJ_NETWORK_HANDLE* handle,
                                            const char* header_name,
                                            void* user_data);

/** Network access: read range
 *
 * Read size_to_read bytes from handle, starting at offset, into
 * buffer.
 * During this read, the implementation should make sure to store the HTTP
 * headers from the server response to be able to respond to
 * proj_network_get_header_value_cbk_type callback.
 *
 * error_string_max_size should be the maximum size that can be written into
 * the out_error_string buffer (including terminating nul character).
 *
 * @return the number of bytes actually read (0 in case of error)
 */
typedef size_t (*proj_network_read_range_type)(
                                            PJ_CONTEXT* ctx,
                                            PROJ_NETWORK_HANDLE* handle,
                                            unsigned long long offset,
                                            size_t size_to_read,
                                            void* buffer,
                                            size_t error_string_max_size,
                                            char* out_error_string,
                                            void* user_data);
/* Apply transformation to observation - in forward or inverse direction */
enum PJ_DIRECTION {
    PJ_FWD   =  1,   /* Forward    */
    PJ_IDENT =  0,   /* Do nothing */
    PJ_INV   = -1    /* Inverse    */
};
typedef enum PJ_DIRECTION PJ_DIRECTION;
/** Error codes typically related to coordinate operation initialization
 * Note: some of them can also be emitted during coordinate transformation,
 * like PROJ_ERR_INVALID_OP_FILE_NOT_FOUND_OR_INVALID in case the resource loading
 * is deferred until it is really needed.
 */
#define PROJ_ERR_INVALID_OP                           1024                        /* other/unspecified error related to coordinate operation initialization */
#define PROJ_ERR_INVALID_OP_WRONG_SYNTAX              (PROJ_ERR_INVALID_OP+1)     /* invalid pipeline structure, missing +proj argument, etc */
#define PROJ_ERR_INVALID_OP_MISSING_ARG               (PROJ_ERR_INVALID_OP+2)     /* missing required operation parameter */
#define PROJ_ERR_INVALID_OP_ILLEGAL_ARG_VALUE         (PROJ_ERR_INVALID_OP+3)     /* one of the operation parameter has an illegal value */
#define PROJ_ERR_INVALID_OP_MUTUALLY_EXCLUSIVE_ARGS   (PROJ_ERR_INVALID_OP+4)     /* mutually exclusive arguments */
#define PROJ_ERR_INVALID_OP_FILE_NOT_FOUND_OR_INVALID (PROJ_ERR_INVALID_OP+5)     /* file not found (particular case of PROJ_ERR_INVALID_OP_ILLEGAL_ARG_VALUE) */

/** Error codes related to transformation on a specific coordinate */
#define PROJ_ERR_COORD_TRANSFM                           2048                           /* other error related to coordinate transformation */
#define PROJ_ERR_COORD_TRANSFM_INVALID_COORD             (PROJ_ERR_COORD_TRANSFM+1)     /* for e.g lat > 90deg */
#define PROJ_ERR_COORD_TRANSFM_OUTSIDE_PROJECTION_DOMAIN (PROJ_ERR_COORD_TRANSFM+2)     /* coordinate is outside of the projection domain. e.g approximate mercator with |longitude - lon_0| > 90deg, or iterative convergence method failed */
#define PROJ_ERR_COORD_TRANSFM_NO_OPERATION              (PROJ_ERR_COORD_TRANSFM+3)     /* no operation found, e.g if no match the required accuracy, or if ballpark transformations were asked to not be used and they would be only such candidate */
#define PROJ_ERR_COORD_TRANSFM_OUTSIDE_GRID              (PROJ_ERR_COORD_TRANSFM+4)     /* point to transform falls outside grid or subgrid */
#define PROJ_ERR_COORD_TRANSFM_GRID_AT_NODATA            (PROJ_ERR_COORD_TRANSFM+5)     /* point to transform falls in a grid cell that evaluates to nodata */

/** Other type of errors */
#define PROJ_ERR_OTHER                                   4096
#define PROJ_ERR_OTHER_API_MISUSE                        (PROJ_ERR_OTHER+1)             /* error related to a misuse of PROJ API */
#define PROJ_ERR_OTHER_NO_INVERSE_OP                     (PROJ_ERR_OTHER+2)             /* no inverse method available */
#define PROJ_ERR_OTHER_NETWORK_ERROR                     (PROJ_ERR_OTHER+3)             /* failure when accessing a network resource */
/** \brief Type representing a NULL terminated list of NULL-terminate strings. */
typedef char **PROJ_STRING_LIST;

/** \brief Guessed WKT "dialect". */
typedef enum
{
    /** \ref WKT2_2019 */
    PJ_GUESSED_WKT2_2019,

    /** Deprecated alias for PJ_GUESSED_WKT2_2019 */
    PJ_GUESSED_WKT2_2018 = PJ_GUESSED_WKT2_2019,

    /** \ref WKT2_2015 */
    PJ_GUESSED_WKT2_2015,

    /** \ref WKT1 */
    PJ_GUESSED_WKT1_GDAL,

    /** ESRI variant of WKT1 */
    PJ_GUESSED_WKT1_ESRI,

    /** Not WKT / unrecognized */
    PJ_GUESSED_NOT_WKT
} PJ_GUESSED_WKT_DIALECT;

/** \brief Object category. */
typedef enum
{
    PJ_CATEGORY_ELLIPSOID,
    PJ_CATEGORY_PRIME_MERIDIAN,
    PJ_CATEGORY_DATUM,
    PJ_CATEGORY_CRS,
    PJ_CATEGORY_COORDINATE_OPERATION,
    PJ_CATEGORY_DATUM_ENSEMBLE
} PJ_CATEGORY;

/** \brief Object type. */
typedef enum
{
    PJ_TYPE_UNKNOWN,

    PJ_TYPE_ELLIPSOID,

    PJ_TYPE_PRIME_MERIDIAN,

    PJ_TYPE_GEODETIC_REFERENCE_FRAME,
    PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME,
    PJ_TYPE_VERTICAL_REFERENCE_FRAME,
    PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME,
    PJ_TYPE_DATUM_ENSEMBLE,

    /** Abstract type, not returned by proj_get_type() */
    PJ_TYPE_CRS,

    PJ_TYPE_GEODETIC_CRS,
    PJ_TYPE_GEOCENTRIC_CRS,

    /** proj_get_type() will never return that type, but
     * PJ_TYPE_GEOGRAPHIC_2D_CRS or PJ_TYPE_GEOGRAPHIC_3D_CRS. */
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

/** Comparison criterion. */
typedef enum
{
    /** All properties are identical. */
    PJ_COMP_STRICT,

    /** The objects are equivalent for the purpose of coordinate
    * operations. They can differ by the name of their objects,
    * identifiers, other metadata.
    * Parameters may be expressed in different units, provided that the
    * value is (with some tolerance) the same once expressed in a
    * common unit.
    */
    PJ_COMP_EQUIVALENT,

    /** Same as EQUIVALENT, relaxed with an exception that the axis order
    * of the base CRS of a DerivedCRS/ProjectedCRS or the axis order of
    * a GeographicCRS is ignored. Only to be used
    * with DerivedCRS/ProjectedCRS/GeographicCRS */
    PJ_COMP_EQUIVALENT_EXCEPT_AXIS_ORDER_GEOGCRS,
} PJ_COMPARISON_CRITERION;


/** \brief WKT version. */
typedef enum
{
    /** cf osgeo::proj::io::WKTFormatter::Convention::WKT2 */
    PJ_WKT2_2015,
    /** cf osgeo::proj::io::WKTFormatter::Convention::WKT2_SIMPLIFIED */
    PJ_WKT2_2015_SIMPLIFIED,
    /** cf osgeo::proj::io::WKTFormatter::Convention::WKT2_2019 */
    PJ_WKT2_2019,
    /** Deprecated alias for PJ_WKT2_2019 */
    PJ_WKT2_2018 = PJ_WKT2_2019,
    /** cf osgeo::proj::io::WKTFormatter::Convention::WKT2_2019_SIMPLIFIED */
    PJ_WKT2_2019_SIMPLIFIED,
    /** Deprecated alias for PJ_WKT2_2019 */
    PJ_WKT2_2018_SIMPLIFIED = PJ_WKT2_2019_SIMPLIFIED,
    /** cf osgeo::proj::io::WKTFormatter::Convention::WKT1_GDAL */
    PJ_WKT1_GDAL,
    /** cf osgeo::proj::io::WKTFormatter::Convention::WKT1_ESRI */
    PJ_WKT1_ESRI
} PJ_WKT_TYPE;

/** Specify how source and target CRS extent should be used to restrict
  * candidate operations (only taken into account if no explicit area of
  * interest is specified. */
typedef enum
{
    /** Ignore CRS extent */
    PJ_CRS_EXTENT_NONE,

    /** Test coordinate operation extent against both CRS extent. */
    PJ_CRS_EXTENT_BOTH,

    /** Test coordinate operation extent against the intersection of both
        CRS extent. */
    PJ_CRS_EXTENT_INTERSECTION,

    /** Test coordinate operation against the smallest of both CRS extent. */
    PJ_CRS_EXTENT_SMALLEST
} PROJ_CRS_EXTENT_USE;

/** Describe how grid availability is used. */
typedef enum {
    /** Grid availability is only used for sorting results. Operations
        * where some grids are missing will be sorted last. */
    PROJ_GRID_AVAILABILITY_USED_FOR_SORTING,

    /** Completely discard an operation if a required grid is missing. */
    PROJ_GRID_AVAILABILITY_DISCARD_OPERATION_IF_MISSING_GRID,

    /** Ignore grid availability at all. Results will be presented as if
        * all grids were available. */
    PROJ_GRID_AVAILABILITY_IGNORED,

    /** Results will be presented as if grids known to PROJ (that is
    * registered in the grid_alternatives table of its database) were
    * available. Used typically when networking is enabled.
    */
    PROJ_GRID_AVAILABILITY_KNOWN_AVAILABLE,
} PROJ_GRID_AVAILABILITY_USE;

/** \brief PROJ string version. */
typedef enum
{
    /** cf osgeo::proj::io::PROJStringFormatter::Convention::PROJ_5 */
    PJ_PROJ_5,
    /** cf osgeo::proj::io::PROJStringFormatter::Convention::PROJ_4 */
    PJ_PROJ_4
} PJ_PROJ_STRING_TYPE;

/** Spatial criterion to restrict candidate operations. */
typedef enum {
    /** The area of validity of transforms should strictly contain the
        * are of interest. */
    PROJ_SPATIAL_CRITERION_STRICT_CONTAINMENT,

    /** The area of validity of transforms should at least intersect the
        * area of interest. */
    PROJ_SPATIAL_CRITERION_PARTIAL_INTERSECTION
} PROJ_SPATIAL_CRITERION;

    /** Describe if and how intermediate CRS should be used */
typedef enum {
    /** Always search for intermediate CRS. */
    PROJ_INTERMEDIATE_CRS_USE_ALWAYS,

    /** Only attempt looking for intermediate CRS if there is no direct
        * transformation available. */
    PROJ_INTERMEDIATE_CRS_USE_IF_NO_DIRECT_TRANSFORMATION,

    /* Do not attempt looking for intermediate CRS. */
    PROJ_INTERMEDIATE_CRS_USE_NEVER,
} PROJ_INTERMEDIATE_CRS_USE;

/** Type of coordinate system. */
typedef enum
{
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

/** \brief Structure given overall description of a CRS.
 *
 * This structure may grow over time, and should not be directly allocated by
 * client code.
 */
typedef struct
{
    /** Authority name. */
    char* auth_name;
    /** Object code. */
    char* code;
    /** Object name. */
    char* name;
    /** Object type. */
    PJ_TYPE type;
    /** Whether the object is deprecated */
    int deprecated;
    /** Whereas the west_lon_degree, south_lat_degree, east_lon_degree and
     * north_lat_degree fields are valid. */
    int bbox_valid;
    /** Western-most longitude of the area of use, in degrees. */
    double west_lon_degree;
    /** Southern-most latitude of the area of use, in degrees. */
    double south_lat_degree;
    /** Eastern-most longitude of the area of use, in degrees. */
    double east_lon_degree;
    /** Northern-most latitude of the area of use, in degrees. */
    double north_lat_degree;
    /** Name of the area of use. */
    char* area_name;
    /** Name of the projection method for a projected CRS. Might be NULL even
     *for projected CRS in some cases. */
    char* projection_method_name;

    /** Name of the celestial body of the CRS (e.g. "Earth").
     * @since 8.1
     */
    char* celestial_body_name;
} PROJ_CRS_INFO;

/** \brief Structure describing optional parameters for proj_get_crs_list();
 *
 * This structure may grow over time, and should not be directly allocated by
 * client code.
 */
typedef struct
{
    /** Array of allowed object types. Should be NULL if all types are allowed*/
    const PJ_TYPE* types;
    /** Size of types. Should be 0 if all types are allowed*/
    size_t typesCount;

    /** If TRUE and bbox_valid == TRUE, then only CRS whose area of use
     * entirely contains the specified bounding box will be returned.
     * If FALSE and bbox_valid == TRUE, then only CRS whose area of use
     * intersects the specified bounding box will be returned.
     */
    int crs_area_of_use_contains_bbox;
    /** To set to TRUE so that west_lon_degree, south_lat_degree,
     * east_lon_degree and north_lat_degree fields are taken into account. */
    int bbox_valid;
    /** Western-most longitude of the area of use, in degrees. */
    double west_lon_degree;
    /** Southern-most latitude of the area of use, in degrees. */
    double south_lat_degree;
    /** Eastern-most longitude of the area of use, in degrees. */
    double east_lon_degree;
    /** Northern-most latitude of the area of use, in degrees. */
    double north_lat_degree;

    /** Whether deprecated objects are allowed. Default to FALSE. */
    int allow_deprecated;

    /** Celestial body of the CRS (e.g. "Earth"). The default value, NULL,
     *  means no restriction
     * @since 8.1
     */
    const char* celestial_body_name;
} PROJ_CRS_LIST_PARAMETERS;

/** \brief Structure given description of a unit.
 *
 * This structure may grow over time, and should not be directly allocated by
 * client code.
 * @since 7.1
 */
typedef struct
{
    /** Authority name. */
    char* auth_name;

    /** Object code. */
    char* code;

    /** Object name. For example "metre", "US survey foot", etc. */
    char* name;

    /** Category of the unit: one of "linear", "linear_per_time", "angular",
     * "angular_per_time", "scale", "scale_per_time" or "time" */
    char* category;

    /** Conversion factor to apply to transform from that unit to the
     * corresponding SI unit (metre for "linear", radian for "angular", etc.).
     * It might be 0 in some cases to indicate no known conversion factor. */
    double conv_factor;

    /** PROJ short name, like "m", "ft", "us-ft", etc... Might be NULL */
    char* proj_short_name;

    /** Whether the object is deprecated */
    int deprecated;
} PROJ_UNIT_INFO;

/** \brief Structure given description of a celestial body.
 *
 * This structure may grow over time, and should not be directly allocated by
 * client code.
 * @since 8.1
 */
typedef struct
{
    /** Authority name. */
    char* auth_name;

    /** Object name. For example "Earth" */
    char* name;

} PROJ_CELESTIAL_BODY_INFO;
typedef struct PJ_OBJ_LIST PJ_OBJ_LIST;
typedef struct PJ_INSERT_SESSION PJ_INSERT_SESSION;
typedef struct PJ_OPERATION_FACTORY_CONTEXT PJ_OPERATION_FACTORY_CONTEXT;

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
