/**
 * \addtogroup httpd
 * @{
 */

/**
 * \file
 *         Web server script interface
 * \author
 *         Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2001-2006, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: httpd-cgi.c,v 1.2 2006/06/11 21:46:37 adam Exp $
 *
 */

#include "uip.h"
#include "psock.h"
#include "httpd.h"
#include "httpd-cgi.h"
#include "httpd-fs.h"
#inclide "http_func.h"

#include "../mprintf/mprintf.h"
#include <string.h>
#include <stdio.h>

HTTPD_CGI_CALL(temp, "tag-temp", cgitemperature );
HTTPD_CGI_CALL(setp, "tag-setpoint", cgisetpoint );
HTTPD_CGI_CALL(estad, "tag-estado", cgiestado );


static const struct httpd_cgi_call *calls[] = { &temp, &setp, &estad, NULL };

/*---------------------------------------------------------------------------*/
static
PT_THREAD(nullfunction(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
httpd_cgifunction
httpd_cgi(char *name)
{
  const struct httpd_cgi_call **f;

  /* Find the matching name in the table, return the function. */
  for(f = calls; *f != NULL; ++f) {
    if(strncmp((*f)->name, name, strlen((*f)->name)) == 0) {
      return (*f)->function;
    }
  }
  return nullfunction;
}
extern unsigned char temp0, temp1, setpoint, ac_auto_on, ac_modo;

static unsigned short cgi_gettemp( void *arg )
{
	sprintf( uip_appdata, "%3d,%02d", (int)temp1, (int)temp0);

	return strlen( uip_appdata );
}

static PT_THREAD(cgitemperature(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_GENERATOR_SEND(&s->sout, cgi_gettemp, NULL);
  PSOCK_END(&s->sout);
}

static unsigned short cgi_getsetp( void *arg )
{
	sprintf( uip_appdata, "%d", (int)setpoint);

	return strlen( uip_appdata );
}

static PT_THREAD(cgisetpoint(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_GENERATOR_SEND(&s->sout, cgi_getsetp, NULL);
  PSOCK_END(&s->sout);
}

static unsigned short cgi_getestado( void *arg )
{
	switch(ac_modo)
	{
	case 0:
		strcpy( uip_appdata, "<option>ON</option> <option>AUTO</option> <option selected>OFF</option>");
		break;
	case 1:
		strcpy( uip_appdata, "<option>ON</option> <option selected>AUTO</option> <option>OFF</option>");
		break;
	case 2:
		strcpy( uip_appdata, "<option selected>ON</option> <option>AUTO</option> <option>OFF</option>");
		break;
	default:
		strcpy( uip_appdata, "<option>ON</option> <option>AUTO</option> <option selected>OFF</option>");
		break;
	}
	return strlen( uip_appdata );
}

static PT_THREAD(cgiestado(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_GENERATOR_SEND(&s->sout, cgi_getestado, NULL);
  PSOCK_END(&s->sout);
}

#define PASSWORD "sisca"

void cgiProcessFormInput( char *pcInputString, int xInputLength )
{
char *c, *d, *e, *pcText;
char tac_modo, tsetpoint;

	c = strstr( pcInputString, "?" );
    if( c )
    {
    	printf("\n\n");
    	*c = 0x00; //terminate at the filename
    	c++;
        e = strstr( c, " " );
        if( e != NULL )
        {
            *e = 0x00; //terminate the whole string
        }
    	d = strstr( c, "stpt=");
    	if (d) {
    	c=d;
    	c += strlen("stpt=");
    	d = strchr( c, '&');
    	*d = '\0';
    	tsetpoint = atoi(c);
    	printf("--[%s]--",c);
    	c=d+1;
    	}

    	d = strstr( c, "estd=");
    	if(d) {
    	c=d;
    	c += strlen("estd=");
    	d = strchr( c, '&');
    	*d = '\0';
    	if(strstr(c, "ON")) tac_modo=2;
    	if(strstr(c, "OFF")) tac_modo=0;
    	if(strstr(c, "AUTO")) tac_modo=1;
    	printf("--[%s]--",c);
    	c=d+1; }
    	d = strstr( c, "psswd=");
    	if (d)
    	{
			c=d;
			c += strlen("psswd=");
			printf("--[%s]--",c);
			if(strstr(c,PASSWORD))
			{
				setpoint = tsetpoint;
				ac_modo = tac_modo;
				printf("\n Password ok");
			}
			else
			{
				tsetpoint = setpoint;
				tac_modo = ac_modo;
				printf("\n Wrong Password");
			}
    	}

    	printf("\n Setpoint: %d",setpoint);
    	printf("\n Modo de op: %d\n",ac_modo);
    }
}





