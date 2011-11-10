/*
 * Datei:               netfind.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Sa, 15. Okt 2011 22:40:25
 *
 */

#ifndef __NETFIND_H__
#define __NETFIND_H__

#include <stdint.h>

#define NETFIND_CLIENT_PORT             6799
#define NETFIND_SERVER_PORT             6800

#define NETFIND_STATE_IDLE              0
#define NETFIND_STATE_WAIT              1
#define NETFIND_STATE_SEND_ANSWER       2

struct netfind_state {
        uint8_t state;

        uint32_t send_answer_time;
};

void netfind_init(void);
void netfind_app_call(void);
void netfind_send_answer(void);

#endif

