/*
 * Datei:               netfind.h
 * Author:              dev00
 * Beschreibung:
 *
 * Aenderungsdatum:     Mo, 09. Jän 2012 03:20:48
 *
 */

#ifndef __NETFIND_H__
#define __NETFIND_H__

#include <stdint.h>
#include <avr/pgmspace.h>

#define NETFIND_VERSION                 0

#define NETFIND_CLIENT_PORT             50000
#define NETFIND_SERVER_PORT             50001

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
void netfind_add_string_P(uint8_t *dest, const uint8_t *src, uint8_t pad_up);
void netfind_send_answer(void);

#endif

