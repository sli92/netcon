/*
 * Datei:               netfind.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Do, 01. Dez 2011 10:34:47
 *
 */

#ifndef __NETFIND_H__
#define __NETFIND_H__

#include <stdint.h>

#define NETFIND_VERSION                 0

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
int filter_ethaddr(uint8_t *addr1, uint8_t *addr2);
void netfind_handle_request(void);
void netfind_send_answer(void);

#endif

