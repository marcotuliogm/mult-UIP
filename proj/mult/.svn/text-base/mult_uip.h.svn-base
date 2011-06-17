/**
 * @file mult_uip.h
 *
 * @brief Declaracao das bibliotecas e servicos usados na camada mult
 *
 *
 * @author Marco Tulio Martins
 *
 */

#ifndef MULT_UIP_H_
#define MULT_UIP_H_


//#include "uipopt.h"

#include "../cli/debug.h"

#include "api_services.h" //contem as variaveis de comunicação.


//typedef struct services_state uip_tcp_appstate_t;

/* Finally we define the application function to be called by uIP. */
void services_appcall(void);

//SERVIÇOS DE TCP
#ifndef UIP_APPCALL
#define UIP_APPCALL services_appcall
#endif /* UIP_APPCALL */

//SERVICOS DE UDP
typedef struct dhcpc_state uip_udp_appstate_t;
#define UIP_UDP_APPCALL dhcpc_appcall



void services_init(void);


#endif /* MULT_UIP_H_ */
