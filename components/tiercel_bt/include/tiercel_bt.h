/*
 * tiercel_bt -- Bluetooth headphones, through the board's ESP32 co-processor.
 *
 * The S3 has no Classic Bluetooth and headphones need it (A2DP), so the
 * board's second chip does the radio work, running Tiercel's firmware from
 * coprocessor/. This side speaks the line protocol documented in
 * coprocessor/main/main.c and keeps a snapshot of what the ESP32 last
 * reported, for Settings > Bluetooth to show.
 *
 * Nothing here prompts anyone. At boot it only says HELLO; the ESP32
 * reconnects remembered headphones by itself, and a search happens only when
 * the Bluetooth screen asks for one.
 *
 * While headphones are connected every block the board plays (Peregrine's
 * replies) is also streamed to them, resampled to the link's 22050 Hz, and
 * the ESP32 mutes the 3.5 mm jack.
 *
 * Every function may be called from any task.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Devices one search can report -- and rows on the Bluetooth screen
 *  (BT_ROWS in GUI/tmp/screens_layout.py). */
#define TIERCEL_BT_ROWS     8
#define TIERCEL_BT_NAME_MAX 48

typedef enum {
    TIERCEL_BT_IDLE = 0,
    TIERCEL_BT_CONNECTING,
    TIERCEL_BT_CONNECTED,
} tiercel_bt_conn_t;

typedef struct {
    char addr[18];                       /* "aa:bb:cc:dd:ee:ff" */
    int8_t rssi;
    char name[TIERCEL_BT_NAME_MAX];
} tiercel_bt_dev_t;

typedef struct {
    uint32_t gen;                        /* bumps on every change */
    bool present;                        /* the ESP32 answered: our firmware */
    bool searching;
    bool failed;                         /* the last connect did not succeed */
    tiercel_bt_conn_t conn;
    tiercel_bt_dev_t peer;               /* while connecting / connected */
    bool has_saved;                      /* headphones the ESP32 remembers */
    tiercel_bt_dev_t saved;
    uint8_t ndev;
    tiercel_bt_dev_t dev[TIERCEL_BT_ROWS];   /* found by the last search */
} tiercel_bt_state_t;

/** Open the link, look for the ESP32, and tap the audio output. Call once,
 *  after tiercel_audio_init(). */
void tiercel_bt_init(void);

/** A copy of the current state. */
void tiercel_bt_get(tiercel_bt_state_t *out);

/** Row `i` of the device list: the "home" headphones first -- connected,
 *  connecting, or just remembered -- then the rest of the search results.
 *  False past the end. */
bool tiercel_bt_row(const tiercel_bt_state_t *s, int i, tiercel_bt_dev_t *out);

/** The headphones in row 0, if any: connected, connecting or remembered. */
bool tiercel_bt_home(const tiercel_bt_state_t *s, tiercel_bt_dev_t *out);

/** Search for headphones in pairing mode. Drops connected ones while it
 *  runs (the controller cannot inquire with a link up); the ESP32 reconnects
 *  them afterwards if nothing new is picked. */
void tiercel_bt_search(void);
void tiercel_bt_stop_search(void);
void tiercel_bt_connect(const char *addr);
/** Disconnect and forget the remembered headphones. */
void tiercel_bt_forget(void);

/** True while headphones are connected: sound goes to them. */
bool tiercel_bt_connected(void);

#ifdef __cplusplus
}
#endif
