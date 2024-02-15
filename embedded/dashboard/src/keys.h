//
// Created by methi on 06/02/2024.
//

#ifndef KEYS_H
#define KEYS_H

#define TZ_OFFSET (-3 * 60 * 60)
#define NTP_SERVER "south-america.pool.ntp.org", "a.ntp.br", "br.pool.ntp.org"

#define K_PREF_EMPTY_DISTANCE "wEmptyDist"
#define K_PREF_FULL_DISTANCE "wFullDist"
#define K_PREF_PUMP_ACTIVE_LEVEL "wActLevel"
#define K_PREF_PUMP_SCHEDULE_ACTIVE "psActive"
#define K_PREF_PUMP_SCHEDULE_MIN_INTERVAL "psMInterval"
#define K_PREF_PUMP_SCHEDULE_MAX_ON "psMaxON"
#define K_PREF_PUMP_SCHEDULE_MIN_HOUR "psMinH"
#define K_PREF_PUMP_SCHEDULE_MAX_HOUR "psMaxH"

#define MQTT_TOPIC_SET_BUTTON_VALUE "weatherLite2/setButtonValue"
#define MQTT_TOPIC_SET_WATER_PREFS "weatherLite2/setWaterPrefs"
#define MQTT_TOPIC_SET_PUMP_SCHEDULE_PREFS "weatherLite2/setPumpSchedulePrefs"
#define MQTT_TOPIC_ACK "weatherLite2/ack"

#endif //KEYS_H
