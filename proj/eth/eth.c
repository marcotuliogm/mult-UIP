#include <string.h>
#include "eth.h"
#include "../timer/timer.h"
#include "../lpc_config.h"
#include "../uart/uart.h"
#include "../mac/emac.h"
#include "../lcd/lcd.h"
#include "../cli/cli.h"
#include "../cli/debug.h"

/* uip includes. */
#include "uip-conf.h"
#include "../mult/mult_uip.h"
#include <uip.h>
#include <uip_arp.h>
#include <timer.h>
#include "clock-arch.h"

/**
 * @brief Envia a mensagem para a PHY
 *
 * Pega mensagem de saida do UIP envia para transmissao pela PHY
 */
static void ENET_Send(void);

/**
 * @brief Inicializa o clock.
 */
void clock_init( void );
clock_time_t clock_time( void );

/**
 * Funcao para inicializar o clock T0
 */
void clock_init(void)
{
	T0Init(1);
}

/**
 * @brief Coloco o clock em operação
 *
 */
void clock_start(void)
{
	T0Start();
}

/**
 * @brief reinicializa o clock.
 */
void clock_reset(void)
{
	T0Init(1);
	T0Start();
}

/**
 * @brief Retorna o tempo de T0
 */
clock_time_t clock_time( void )
{
	return T0Get();
}

extern void ( EMAC_ISR_Handler )( void );


struct timer periodic_timer, arp_timer;

/**
 * @brief Inicializa protocolo Ethernet.
 *
 * Configura endereco de MAC
 * Inicializa  os clocks.
 * Inicializa a pilha do UIP (tempo, portas, conexoes e buffer)
 * Configura enderecos de IP, Gateway e Mascara FIXOS
 * Inicializa camada mult.
 * Inicializa MAC
 *
 */
void ethernet_init()
{
	int i;
	uip_ipaddr_t xIPAddr;
	char buff[40];
	struct uip_eth_addr xAddr;
	xAddr.addr[ 0 ] = uipMAC_ADDR0;
	xAddr.addr[ 1 ] = uipMAC_ADDR1;
	xAddr.addr[ 2 ] = uipMAC_ADDR2;
	xAddr.addr[ 3 ] = uipMAC_ADDR3;
	xAddr.addr[ 4 ] = uipMAC_ADDR4;
	xAddr.addr[ 5 ] = uipMAC_ADDR5;

	clock_init();
	clock_start();

	/* Initialize the uIP stack. */
	timer_set( &periodic_timer, 1000 );
	timer_set( &arp_timer, 10000 );
	uip_init();

	/* The next two lines define an IP adress */
	set_eth_addr();

	//dhcpc_init(&xAddr, 6); //DHCP Client not working yet
	services_init();

   	/* Initialise the MAC. */
   	while( Init_EMAC() != PASS )
   	{
   	    delayMs( uipINIT_WAIT );
   	}
   	disableIRQ();
   	MAC_INTENABLE = INT_RX_DONE | INT_TX_DONE | INT_TX_UNDERRUN | INT_RX_OVERRUN | INT_TX_FIN | INT_RX_FIN;
   	VICIntSelect &= ~0x00200000;
   	VICIntEnable |=  0x00200000;
   	VICVectAddr21 = ( int ) EMAC_ISR_Handler;
   	uip_setethaddr( xAddr ); // Set hardware address (MAC) on uIP stack
   	enableIRQ();
}
/**
 * @brief Funcao que recebe dados da PHY
 *
 * Funcao e chamada por interrupcao
 * Quando um dado e recebido para ser processado vindo da rede.
 * Verifica se é uma mensagem com protocolo IP ou uma solicitacao de ARP
 * Verificacao do tempo das conexoes periodicas para o protocolo TCP verificando timeout.
 */
void ethernet_task()
{
	int i;

	/* Is there received data ready to be processed? */
	uip_len = uiGetEMACRxData( uip_buf );

	if( uip_len > 0 )
	{
		/* Standard uIP loop taken from the uIP manual. */
		if( xHeader->type == htons( UIP_ETHTYPE_IP ) )
		{
			//uart_putc('.');
			uip_arp_ipin();
			uip_input();

			/* If the above function invocation resulted in data that
			should be sent out on the network, the global variable
			uip_len is set to a value > 0. */
			if( uip_len > 0 )
			{
				//uart_putc('-');
				uip_arp_out();
				ENET_Send();
			}
		}
		else if( xHeader->type == htons( UIP_ETHTYPE_ARP ) )
		{
			//uart_putc('\\');
			uip_arp_arpin();

			/* If the above function invocation resulted in data that
			should be sent out on the network, the global variable
			uip_len is set to a value > 0. */
			if( uip_len > 0 )
			{
				//uart_putc('/');
				ENET_Send();
			}
		}
	}
	else
	{
		if( timer_expired( &periodic_timer ) )
		{
			//uart_putc('|');
			timer_reset( &periodic_timer );
			for( i = 0; i < UIP_CONNS; i++ )
			{
				uip_periodic( i );

				/* If the above function invocation resulted in data that 
				should be sent out on the network, the global variable 
				uip_len is set to a value > 0. */
				if( uip_len > 0 )
				{
					//uart_putc('@');
					uip_arp_out();
					ENET_Send();
				}
			}
#if UIP_UDP
			for(i = 0; i < UIP_UDP_CONNS; i++) {
				uip_udp_periodic(i);
				/* If the above function invocation resulted in data that
				should be sent out on the network, the global variable
				uip_len is set to a value > 0. */
				if(uip_len > 0) {
					//uart_putc('%');
					uip_arp_out();
					ENET_Send();
				}
			}
#endif /* UIP_UDP */

			/* Call the ARP timer function every 10 seconds. */
			if( timer_expired( &arp_timer ) )
			{
				//uart_putc('#');
				timer_reset( &arp_timer );
				uip_arp_timer();
			}
		}
	}
}

/**
 * @brief Envia dados para o EMAC
 *
 * Funcao que pega as informacoes geradas pelo protocolo UIP para serem enviadas e
 * coloca na pilha de saida do EMAC.
 */
static void ENET_Send(void)
{
    RequestSend();

    /* Copy the header into the Tx buffer. */
    CopyToFrame_EMAC( uip_buf, uipTOTAL_FRAME_HEADER_SIZE );
    if( uip_len > uipTOTAL_FRAME_HEADER_SIZE )
    {
        CopyToFrame_EMAC( uip_appdata, ( uip_len - uipTOTAL_FRAME_HEADER_SIZE ) );
    }

    DoSend_EMAC( uip_len );
}

/**
 * @brief Configura enderecos iniciais da rede.
 */
void set_eth_addr(void){
	set_IpDefault();
	set_GatewayDefault();
	set_MaskDefault();
}
