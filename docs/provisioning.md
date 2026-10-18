# Provisioning

A new Tiercel needs only two things entered by hand: **which Wi-Fi network**
and **which broker**. Everything else (Peregrine's address and CA, which
Playbill it controls, its devices and alarms) comes from Headwaters after
the unit has been discovered. The flow is Capstan's.

## 1. Setup portal (first boot, or after a factory reset)

With no saved network, Tiercel raises its own access point and shows the
**Setup** screen:

- network name `Tiercel-XXXXXX` (the last three bytes of the unit's MAC)
- a random 8-character password, kept in NVS so it survives a reboot
  mid-setup
- `http://192.168.4.1`

Join that network from a phone. The captive portal opens on its own (or
browse to the address). The page scans for networks and asks for:

| Field | Default |
|---|---|
| Wi-Fi network, security, password | — |
| Broker host | `headwaters.local` |
| Port | `8883` (Headwaters' broker is TLS-only) |
| Username / password | the broker credentials |

**Nothing is saved until it works.** The unit joins the network and
completes a TLS handshake with the broker while the AP stays up, and only
then writes both to NVS. A wrong password or an unreachable broker is
reported on the phone, and nothing changes.

Once verified, the dial tears the AP down, joins the network with the saved
credentials, and goes to Settings, where Wi-Fi and MQTT show as connected.

## 2. Discovery

When Headwaters scans for modules, it publishes `local/discovery/trigger`.
Tiercel stops MQTT, advertises `_trailcurrent._tcp` over mDNS (TXT
`type=tiercel`, `fw=<version>`) and serves `GET /discovery/confirm` on port
80 until Headwaters confirms it or three minutes pass. Then it reconnects.
After that, Headwaters lists the unit under its hostname, `esp32-XXXXXX`.

## 3. Configuration from Headwaters

Headwaters publishes three **retained** topics for the hostname, and Tiercel
keeps each one in NVS so it works before the broker reconnects after a power
cut:

| Topic | Carries |
|---|---|
| `local/config/tiercel/<hostname>/services` | Peregrine URL and CA, the Playbill device to control |
| `local/config/tiercel/<hostname>/controls` | the lights and relays on the Devices carousel |
| `local/config/panel/<hostname>/alarms` | which inputs raise an alert, per rig mode |

Payloads are in [mqtt.md](mqtt.md#per-device-configuration). Each topic
replaces its list wholesale, and an empty payload clears it. That is how
removing something in the Headwaters PWA reaches the dial.

> **Headwaters support.** Headwaters publishes all three: `controls` and
> `alarms` from the Tiercel's module settings, and `services` from its
> Peregrine settings (its URL and CA). They are re-sent on every module
> or Peregrine settings save, so nothing about Peregrine is entered on the
> dial. See [mqtt.md](mqtt.md#what-headwaters-sends).

## Starting over

**Settings → Factory Reset**, tapped twice within five seconds, erases NVS
and reboots into the setup portal.
