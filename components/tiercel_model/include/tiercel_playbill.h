/*
 * Playbill -- what the media box in this dial's room is doing.
 *
 * Playbill publishes its state per feature, retained, on
 *
 *     local/playbill/<deviceId>/<feature>/status
 *
 * and takes commands on the sibling `.../command` topic. tiercel_mqtt parses
 * the JSON and calls the setters here; the UI reads through the getters. The
 * contract is documented in docs/mqtt.md, and it is Playbill's own
 * (controller/src/schema/commands.schema.json in TrailCurrentPlaybill) -- this
 * file invents none of it.
 *
 * WHICH PLAYBILL
 *
 * A rig can have several. Headwaters names the one this dial controls in the
 * services config (tiercel_config_get_services()). With none named, the first
 * Playbill that reports itself online is adopted, which is right for the
 * common single-Playbill rig and is logged so it is never a mystery.
 *
 * POSITION
 *
 * Transport status is throttled to one publish a second and the throttle keeps
 * the FIRST update in each window with no trailing publish, so the position
 * arrives late and in steps. tiercel_playbill_get() interpolates from the last
 * report while playing, which is what Playbill's own docs ask a remote to do.
 *
 * THREADING
 *
 * Setters run on the service task, getters on the LVGL task. Both take the
 * internal mutex and the getter returns a copy.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIERCEL_PB_SLUG_MAX   33
#define TIERCEL_PB_NAME_MAX   33
#define TIERCEL_PB_TITLE_MAX  96
#define TIERCEL_PB_SUB_MAX    96
#define TIERCEL_PB_SOURCE_MAX 16
#define TIERCEL_PB_STATIONS   24

typedef enum {
    TIERCEL_PB_BAND_FM = 0,
    TIERCEL_PB_BAND_AM,
    TIERCEL_PB_BAND_OTHER,
} tiercel_pb_band_t;

typedef struct {
    bool     known;                          /**< a Playbill has been adopted */
    bool     online;                         /**< system/status online:true */
    char     slug[TIERCEL_PB_SLUG_MAX];      /**< deviceId, the topic segment */
    char     name[TIERCEL_PB_NAME_MAX];      /**< "Living Room" */

    /* source/status */
    char     source[TIERCEL_PB_SOURCE_MAX];  /**< "radio", "youtube", ... or "" */

    /* transport/status (nowPlaying) */
    bool     has_media;                      /**< something is loaded in mpv */
    bool     paused;
    char     title[TIERCEL_PB_TITLE_MAX];
    char     subtitle[TIERCEL_PB_SUB_MAX];   /**< artist / channel / year */
    uint32_t position_ms;                    /**< interpolated while playing */
    uint32_t duration_ms;                    /**< 0 when unknown (live) */

    /* volume/status */
    bool     volume_known;
    uint8_t  volume_pct;
    bool     muted;

    /* radio/status */
    bool              radio_running;
    tiercel_pb_band_t band;
    uint32_t          frequency_hz;

    /* radio/status lastScan.stations, ascending. Playbill keeps no station
     * names and presets cannot be read over MQTT, so a completed scan is the
     * only list of real stations a remote can see. */
    uint8_t           station_count;
    uint32_t          stations_hz[TIERCEL_PB_STATIONS];
} tiercel_playbill_t;

void tiercel_playbill_init(void);

/**
 * Which deviceId to follow. Empty adopts the first online Playbill.
 * Changing it forgets everything held for the previous one.
 */
void tiercel_playbill_set_target(const char *slug);

/**
 * True if `slug` is the device being followed. With no explicit target, the
 * first slug offered while nothing is adopted is adopted -- so call this
 * BEFORE the setters, with the slug from the topic.
 */
bool tiercel_playbill_accept(const char *slug, bool online_hint);

void tiercel_playbill_set_presence(bool online, const char *name);
void tiercel_playbill_set_source(const char *source);
void tiercel_playbill_set_transport(bool has_media, bool paused,
                                    const char *title, const char *subtitle,
                                    int64_t position_ms, int64_t duration_ms);
void tiercel_playbill_set_volume(int pct, bool muted);
void tiercel_playbill_set_radio(bool running, const char *band,
                                uint32_t frequency_hz);
/** Replace the scanned station list (FM only; sorted here). */
void tiercel_playbill_set_stations(const uint32_t *hz, int count);

/**
 * Optimistic local echo of a command this dial just sent, so the knob does
 * not visibly lag while Playbill's status catches up. Status from Playbill
 * always wins on its next arrival.
 */
void tiercel_playbill_echo_volume(int pct);
void tiercel_playbill_echo_frequency(uint32_t frequency_hz);

void tiercel_playbill_get(tiercel_playbill_t *out);

#ifdef __cplusplus
}
#endif
