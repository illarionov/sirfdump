/* Sv Status Information For Android or Other OS*/

/** Represents SV information. */
typedef struct {
    /** Pseudo-random number for the SV. */
    int     prn;
    /** Signal to noise ratio. */
    float   snr;
    /** Elevation of SV in degrees. */
    float   elevation;
    /** Azimuth of SV in degrees. */
    float   azimuth;
} SiRFGpsSvInfo;


/** Maximum number of SVs for gps_sv_status_callback(). */
#define SIRF_GPS_MAX_SVS 32

typedef struct {
    /** Number of SVs currently visible. */
    int         num_svs;

    /** Contains an array of SV information. */
    SiRFGpsSvInfo  sv_list[SIRF_GPS_MAX_SVS];

    /** Represents a bit mask indicating which SVs
     * have ephemeris data.
     */
    uint32_t    ephemeris_mask;

    /** Represents a bit mask indicating which SVs
     * have almanac data.
     */
    uint32_t    almanac_mask;

    /**
     * Represents a bit mask indicating which SVs
     * were used for computing the most recent position fix.
     */
    uint32_t    used_in_fix_mask;
    uint16_t    nav_valid;
}SiRFGpsSvStatus;

