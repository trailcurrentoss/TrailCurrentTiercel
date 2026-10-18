/*
 * MQTT client for the Headwaters gateway.
 *
 * Subscribes to the rig's `local/...` topics, parses them into the data
 * model, and publishes commands. See docs/mqtt.md for the full contract --
 * including the three domains that have no topic yet.
 *
 * THREADING
 *
 * esp-mqtt delivers on its own event task. Parsing happens THERE, and only
 * the resulting setter calls take the LVGL lock. Doing it the other way
 * round -- taking the lock and then parsing JSON inside it -- produced
 * roughly ten seconds of UI lag on Fireside at this broker's ~200 msg/s.
 *
 * Callbacks registered here run on the MQTT task, not the LVGL task.
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "esp_err.h"
/* tiercel_mqtt_cfg_t lives here -- tiercel_mqtt_try() takes one. */
#include "tiercel_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TIERCEL_MQTT_IDLE = 0,
    TIERCEL_MQTT_CONNECTING,
    TIERCEL_MQTT_CONNECTED,
    TIERCEL_MQTT_FAILED,
} tiercel_mqtt_state_t;

const char *tiercel_mqtt_state_name(tiercel_mqtt_state_t s);

typedef void (*tiercel_mqtt_state_cb_t)(tiercel_mqtt_state_t s, void *ctx);
void tiercel_mqtt_set_state_callback(tiercel_mqtt_state_cb_t cb, void *ctx);

/** Prepare the client. Does not connect. Call after tiercel_config_init(). */
esp_err_t tiercel_mqtt_init(void);

/**
 * Connect using the broker details in tiercel_config. Call once Wi-Fi has
 * an IP -- there is no point before that, and esp-mqtt's own retry would
 * just burn cycles failing to resolve.
 *
 * Idempotent where that is actually correct: a duplicate request is ignored
 * while connected or just started, but a client that has been down for a
 * while is rebuilt. That distinction matters after a Wi-Fi reassociation,
 * when the old client holds a dead socket and treating its mere existence
 * as "nothing to do" strands the display offline.
 */
esp_err_t tiercel_mqtt_connect(void);

/** Tear down and reconnect, ignoring the idempotency check. Use after the
 *  broker settings change. */
esp_err_t tiercel_mqtt_reconnect(void);

/** Stop and free the client, releasing its TLS socket. */
esp_err_t tiercel_mqtt_stop(void);

tiercel_mqtt_state_t tiercel_mqtt_state(void);
bool tiercel_mqtt_is_connected(void);
const char *tiercel_mqtt_last_error(void);

/** Publish. Returns the message id, or -1. */
int tiercel_mqtt_publish(const char *topic, const char *payload, int len);

/** As tiercel_mqtt_publish(), at an explicit QoS (0 or 1). */
int tiercel_mqtt_publish_qos(const char *topic, const char *payload, int len,
                             int qos);

/**
 * Send one command to the Playbill this dial follows, on
 * local/playbill/<slug>/<feature>/command. `json` is the whole payload, e.g.
 * {"action":"transport.toggle"} -- Playbill validates it strictly
 * (additionalProperties: false), so send exactly the documented fields.
 * Returns the message id, or -1 if there is no Playbill or no broker.
 */
int tiercel_mqtt_playbill_command(const char *feature, const char *json);

/**
 * Drain the inbound queue and apply messages to the data model.
 * Call from the main loop. Parsing happens here, off the MQTT task.
 */
void tiercel_mqtt_process(void);

/**
 * Connect to this broker WITHOUT saving it.
 *
 * The provisioning counterpart of tiercel_wifi_try(). Provisioning uses it to
 * find out whether the broker details actually work before committing them,
 * so a typo cannot be written to NVS and leave the panel on the network
 * showing no data with no indication of why.
 *
 * On success the caller persists with tiercel_config_set_mqtt().
 */
esp_err_t tiercel_mqtt_try(const tiercel_mqtt_cfg_t *cfg);

/*
 * The Headwaters discovery broadcast.
 *
 * `local/discovery/trigger` is what makes a device appear in Overlook's
 * device list; see main/discovery.h for the full handshake. This layer does
 * two things with it and no more: it decides whether the trigger is FOR US
 * (payload `*`, or this device's own `esp32-XXXXXX` hostname), and it calls
 * this callback if so. Everything the device then does about it -- mDNS, the
 * HTTP server, the timeout -- lives in main/, because a broker client has no
 * business starting a web server.
 *
 * The callback runs on whichever task called tiercel_mqtt_process(), NOT on
 * the MQTT event task, so it may take locks. It must still return promptly;
 * spawn a task for anything that waits.
 *
 * There is deliberately no equivalent for `local/ota/trigger` yet. Nothing
 * implements OTA on this device, and a registered callback that quietly did
 * nothing would be worse than an unhandled topic, which at least logs.
 */
typedef void (*tiercel_mqtt_trigger_cb_t)(void *ctx);
void tiercel_mqtt_set_discovery_callback(tiercel_mqtt_trigger_cb_t cb,
                                         void *ctx);

/**
 * Expire values whose module has gone quiet.
 *
 * NOTHING on this platform is retained, so a value is only ever as good as
 * the last frame. A stale number that still looks live is worse than no
 * number -- the whole point of the `--` convention. Call every loop; it
 * costs nothing until a module actually times out.
 */
void tiercel_mqtt_check_watchdogs(void);

/** This device's hostname, esp32-XXXXXX from the last 3 MAC bytes. `out`
 *  needs at least 14 bytes. Returns out. */
const char *tiercel_mqtt_hostname(char *out, size_t len);

#ifdef __cplusplus
}
#endif
