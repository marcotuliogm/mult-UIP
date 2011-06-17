/**
 * \addtogroup exampleapps
 * @{
 */

/**
 * \defgroup ftp client
 * @{
 *
 * The uIP ftp client is a very simplistic implementation of an FTP
 * client. It can upload a file to a server ad it cannot download anything.
 *
 */


/**
 * \file
 * FTP client.
 * \author Fabio Giovagnini <fabio.giovagnini@aurion-tech.com>
 */

/*
 * Copyright (c) 2006, Aurion s.r.l.
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
 * $Id: ftp.c,v 1.0.0.0 2007/01/31 21:21:01 fabio Exp $
 *
 */
#include <ctype.h>
#include <stdlib.h>
#include "uip.h"
#include "ftp.h"
#include "../mult/api_services.h"

//#define DEBUG
#ifdef DEBUG
#include "arnDebug.h"
#define PRINT(x) arnDEBUG("%s",x) /*printf("%s", x)*/
#define PRINTLN(x) arnDEBUG("%s\n",x) /*printf("%s\n", x)*/
#define PRINTNUM(x) arnDEBUG("%d",x) 
#define PRINTNUMLN(x) arnDEBUG("%d\n",x)
#else /* DEBUG */
#define PRINT(x)
#define PRINTLN(x)
#define PRINTNUMLN(x)
#define PRINTNUM(x)
#endif /* NOT DEBUG */

#ifdef __cplusplus
extern "C" {
#endif

static arn_BOOL HostToIp(arn_CHAR* host,arn_uSHORT ipaddr[2]);

static void _abort(struct ftp_state *s);
static void _timeout(struct ftp_state *s);
static void _close(struct ftp_state *s);
static void _connect(struct ftp_state *s);
static void _newdata(struct ftp_state *s);
static void _ack(struct ftp_state *s);
static void _poll(struct ftp_state *s);
static void _senddata(struct ftp_state *s);
static void _retrasmit(struct ftp_state *s);

static void _abort_data(struct ftp_state *s);
static void _timeout_data(struct ftp_state *s);
static void _close_data(struct ftp_state *s);
static void _connect_data(struct ftp_state *s);
static void _newdata_data(struct ftp_state *s);
static void _ack_data(struct ftp_state *s);
static void _poll_data(struct ftp_state *s);
static void _senddata_data(struct ftp_state *s);
static void _retrasmit_data(struct ftp_state *s);

static const arn_CHAR cmd_cwd_P[] = "CWD";
static const arn_CHAR cmd_dele_P[] = "DELE";
static const arn_CHAR cmd_list_P[] = "LIST";
static const arn_CHAR cmd_mkd_P[] = "MKD";
static const arn_CHAR cmd_xmkd_P[] = "XMKD";
static const arn_CHAR cmd_nlst_P[] = "NLST";
static const arn_CHAR cmd_noop_P[] = "NOOP";
static const arn_CHAR cmd_pass_P[] = "PASS";
static const arn_CHAR cmd_pasv_P[] = "PASV";
static const arn_CHAR cmd_port_P[] = "PORT";
static const arn_CHAR cmd_pwd_P[] = "PWD";
static const arn_CHAR cmd_xpwd_P[] = "XPWD";
static const arn_CHAR cmd_quit_P[] = "QUIT";
static const arn_CHAR cmd_retr_P[] = "RETR";
static const arn_CHAR cmd_rmd_P[] = "RMD";
static const arn_CHAR cmd_xrmd_P[] = "XRMD";
static const arn_CHAR cmd_size_P[] = "SIZE";
static const arn_CHAR cmd_stor_P[] = "STOR";
static const arn_CHAR cmd_syst_P[] = "SYST";
static const arn_CHAR cmd_type_P[] = "TYPE";
static const arn_CHAR cmd_user_P[] = "USER";

static const arn_CHAR okCode125[] = "125";
static const arn_CHAR okCode150[] = "150";
static const arn_CHAR okCode200[] = "200";
static const arn_CHAR okCode215[] = "215";
static const arn_CHAR okCode220[] = "220";
static const arn_CHAR okCode221[] = "221";
static const arn_CHAR okCode225[] = "225";
static const arn_CHAR okCode226[] = "226";
static const arn_CHAR okCode230[] = "230";
static const arn_CHAR okCode331[] = "331";

static const arn_CHAR FailCode500[] =  "500";
static const arn_CHAR FailCode501[] =  "501";
static const arn_CHAR FailCode502[] =  "502";
static const arn_CHAR FailCode504[] =  "504";
static const arn_CHAR FailCode421[] =  "421";


#define CMDSND_NONE							0
#define CMDSND_CONNECT						(CMDSND_NONE + 1)
#define CMDSND_LOGIN						(CMDSND_CONNECT + 1)
#define CMDSND_SYST							(CMDSND_LOGIN + 1)
#define CMDSND_PORT							(CMDSND_SYST + 1)
#define CMDSND_STOR							(CMDSND_PORT + 1)
#define CMDSND_QUIT							(CMDSND_STOR + 1)
#define CMDSND_PASS							(CMDSND_QUIT + 1)

#define ANSRCV_NONE							0
#define ANSRCV_WELCOME						(ANSRCV_NONE + 1)
#define ANSRCV_WELCOMERR					(ANSRCV_WELCOME + 1)
#define ANSRCV_LOGGEDIN						(ANSRCV_WELCOMERR + 1)
#define ANSRCV_LOGGEDERR					(ANSRCV_LOGGEDIN + 1)
#define ANSRCV_SYSTOK						(ANSRCV_LOGGEDERR + 1)
#define ANSRCV_SYSTERR						(ANSRCV_SYSTOK + 1)
#define ANSRCV_PORTOK						(ANSRCV_SYSTERR + 1)
#define ANSRCV_PORTERR						(ANSRCV_PORTOK + 1)
#define ANSRCV_STOROK						(ANSRCV_PORTERR + 1)
#define ANSRCV_STORERR						(ANSRCV_STOROK + 1)
#define ANSRCV_QUITOK						(ANSRCV_STORERR + 1)
#define ANSRCV_QUITERR						(ANSRCV_QUITOK + 1)
#define ANSRCV_PASSERR						(ANSRCV_QUITERR + 1)
#define ANSRCV_PASSREQ						(ANSRCV_PASSERR + 1)
//------------------------------------------------------------
//----------- API AND NEEDED CALLBACK ------------------------
//------------------------------------------------------------
ftp_cmd_status_t ftp_connect(arn_CHAR* host,arn_uSHORT rport,struct uip_conn **sconn) 
{
	char deb[100];
	DEBUG_ENTER;
	if (*sconn == NULL) {
		arn_uSHORT ipHostAddr[2];
		DEBUG_FTP(host);
		if (!HostToIp(host,ipHostAddr)) return ftpcs_FAILED;
		*sconn = uip_connect(ipHostAddr, htons(rport));
		DEBUG_FTP("Conectando..");
		if(sconn == NULL) return ftpcs_FAILED;
		DEBUG_FTP("Conectado com sucesso.");
		((struct ftp_state *)((*sconn)->appstate.ftp_st))->remotectrlport = rport;
		DEBUG_FTP("porta");
		((struct ftp_state *)((*sconn)->appstate.ftp_st))->CmdSnt = CMDSND_CONNECT;
		DEBUG_FTP("CmdSnt");
		((struct ftp_state *)((*sconn)->appstate.ftp_st))->Status = STS_WAITFORCONN;
		DEBUG_FTP("Configurado conexao.");
		}
	else {
		struct ftp_state *s = (struct ftp_state *)(*sconn)->appstate.ftp_st;
		DEBUG_FTP("ja esta conectado..");
		sprintf(deb, "Status: %s\t %d",s->Status,s->Status);
		DEBUG_FTP(deb);
		if (s->Status == STS_CONNECTED) {
			if (s->AnsRcv == ANSRCV_WELCOME) {
				s->CmdSnt = CMDSND_NONE; s->AnsRcv = ANSRCV_NONE;
				s->dconn = 0;
				return ftpcs_SUCCESS;
				}
			else if (s->AnsRcv == ANSRCV_WELCOMERR) {
				s->Status = STS_ERROR;
				s->errorCode = ERRC_CONN;
				return ftpcs_FAILED;
				}
			}
		}
	DEBUG_EXIT;
	return ftpcs_INPROGRESS;
}

ftp_cmd_status_t ftp_login(const struct uip_conn *sconn,arn_CHAR* username)
{
	struct ftp_state *s = (struct ftp_state *)sconn->appstate.ftp_st;
	if (s->Status != STS_CONNECTED) return ftpcs_FAILED;
	if (s->CmdSnt == CMDSND_NONE) {
		strcpy((arn_CHAR*)s->data,cmd_user_P);
		strcat((arn_CHAR*)s->data," ");
		strcat((arn_CHAR*)s->data,username);
		strcat((arn_CHAR*)s->data,"\r\n");
		s->CmdSnt = CMDSND_LOGIN;
		s->flags |= FTP_FLAG_CMDREQUEST;
		}
	else if (s->CmdSnt == CMDSND_LOGIN) {
		if (s->AnsRcv == ANSRCV_LOGGEDIN) {
			s->CmdSnt = CMDSND_NONE; s->AnsRcv = ANSRCV_NONE; 
			return ftpcs_SUCCESS;
			}
		else if (s->AnsRcv == ANSRCV_PASSREQ) {
			s->CmdSnt = CMDSND_NONE; s->AnsRcv = ANSRCV_NONE; 
			return ftpcs_PASSWORD_REQ;
			}
		else if (s->AnsRcv == ANSRCV_LOGGEDERR) {
			s->Status = STS_ERROR;
			s->errorCode = ERRC_LOGIN;
			return ftpcs_FAILED;
			}
		}
	return ftpcs_INPROGRESS;
}

ftp_cmd_status_t ftp_password(const struct uip_conn *sconn,arn_CHAR* password)
{
	struct ftp_state *s = (struct ftp_state *)sconn->appstate.ftp_st;
	if (s->Status != STS_CONNECTED) return ftpcs_FAILED;
	if (s->CmdSnt == CMDSND_NONE) {
		strcpy((arn_CHAR*)s->data,cmd_pass_P);
		strcat((arn_CHAR*)s->data," ");
		strcat((arn_CHAR*)s->data,password);
		strcat((arn_CHAR*)s->data,"\r\n");
		s->CmdSnt = CMDSND_PASS;
		s->flags |= FTP_FLAG_CMDREQUEST;
		}
	else if (s->CmdSnt == CMDSND_PASS) {
		if (s->AnsRcv == ANSRCV_LOGGEDIN) {
			s->CmdSnt = CMDSND_NONE; s->AnsRcv = ANSRCV_NONE; 
			return ftpcs_SUCCESS;
			}
		else if (s->AnsRcv == ANSRCV_PASSERR) {
			s->Status = STS_ERROR;
			s->errorCode = ERRC_PASS;
			return ftpcs_FAILED;
			}
		}
	return ftpcs_INPROGRESS;
}

ftp_cmd_status_t ftp_syst(const struct uip_conn *sconn,arn_CHAR* res)
{
	struct ftp_state *s = (struct ftp_state *)sconn->appstate.ftp_st;
	if (s->Status != STS_CONNECTED) return ftpcs_FAILED;
	if (s->CmdSnt == CMDSND_NONE) {
		strcpy((arn_CHAR*)s->data,cmd_syst_P);
		strcat((arn_CHAR*)s->data,"\r\n");
		s->CmdSnt = CMDSND_SYST;
		s->flags |= FTP_FLAG_CMDREQUEST;
		}
	else if (s->CmdSnt == CMDSND_SYST) {
		if (s->AnsRcv == ANSRCV_SYSTOK) {
			s->CmdSnt = CMDSND_NONE; s->AnsRcv = ANSRCV_NONE;
			strcpy(res,(const arn_CHAR*)(s->data));
			return ftpcs_SUCCESS;
			}
		else if (s->AnsRcv == ANSRCV_SYSTERR) {
			s->Status = STS_ERROR;
			s->errorCode = ERRC_SYST;
			return ftpcs_FAILED;
			}
		}
	return ftpcs_INPROGRESS;
}

ftp_cmd_status_t ftp_port(const struct uip_conn *sconn,arn_uSHORT ldataport)
{
	struct ftp_state *s = (struct ftp_state *)sconn->appstate.ftp_st;
	if (s->Status != STS_CONNECTED) return ftpcs_FAILED;
	if (s->CmdSnt == CMDSND_NONE) {
		strcpy((arn_CHAR*)s->data,cmd_port_P);
		sprintf((arn_CHAR*)(s->data + (sizeof(cmd_port_P) - 1))," %u,%u,%u,%u,%u,%u\r\n",
			 htons(uip_hostaddr[0]) >> 8,
			 htons(uip_hostaddr[0]) & 0xff,
			 htons(uip_hostaddr[1]) >> 8,
			 htons(uip_hostaddr[1]) & 0xff,
			 htons(ldataport) >> 8,
			 htons(ldataport) & 0xff
			);
		s->localdataport = ldataport;
		s->remotedataport = 20;
		s->CmdSnt = CMDSND_PORT;
		s->flags |= FTP_FLAG_CMDREQUEST;
		}
	else if (s->CmdSnt == CMDSND_PORT) {
		if (s->AnsRcv == ANSRCV_PORTOK) {
			s->CmdSnt = CMDSND_NONE; s->AnsRcv = ANSRCV_NONE;
			return ftpcs_SUCCESS;
			}
		else if (s->AnsRcv == ANSRCV_PORTERR) {
			s->Status = STS_ERROR;
			s->errorCode = ERRC_PORT;
			return ftpcs_FAILED;
			}
		}
	return ftpcs_INPROGRESS;
}

ftp_cmd_status_t ftp_pasv(const struct uip_conn *sconn)
{
	return ftpcs_FAILED;
}

ftp_cmd_status_t ftp_send(const struct uip_conn *sconn,arn_CHAR* filename,arn_uCHAR* sadd,arn_uLONG size)
{
	struct ftp_state *s = (struct ftp_state *)sconn->appstate.ftp_st;
	if (s->Status != STS_CONNECTED) return ftpcs_FAILED;
	if (s->CmdSnt == CMDSND_NONE) {
		strcpy((arn_CHAR*)s->data,cmd_stor_P);
		strcat((arn_CHAR*)s->data," ");
		strcat((arn_CHAR*)s->data,filename);
		strcat((arn_CHAR*)s->data,"\r\n");
		s->fSAddr = sadd;
		s->fSize = size;
		s->CmdSnt = CMDSND_STOR;
		s->flags |= FTP_FLAG_CMDREQUEST;
		}
	else if (s->CmdSnt == CMDSND_STOR) {
		if (s->AnsRcv == ANSRCV_STOROK) {
			s->CmdSnt = CMDSND_NONE; s->AnsRcv = ANSRCV_NONE;
			return ftpcs_SUCCESS;
			}
		else if (s->AnsRcv == ANSRCV_STORERR) {
			s->Status = STS_ERROR;
			s->errorCode = ERRC_STOR;
			return ftpcs_FAILED;
			}
		}
	return ftpcs_INPROGRESS;
}

ftp_cmd_status_t ftp_quit(const struct uip_conn *sconn)
{
	struct ftp_state *s = (struct ftp_state *)sconn->appstate.ftp_st;
	if (s->Status != STS_CONNECTED) return ftpcs_FAILED;
	if (s->CmdSnt == CMDSND_NONE) {
		strcpy((arn_CHAR*)s->data,cmd_quit_P);
		strcat((arn_CHAR*)s->data,"\r\n");
		s->CmdSnt = CMDSND_QUIT;
		s->flags |= FTP_FLAG_CMDREQUEST;
		}
	else if (s->CmdSnt == CMDSND_QUIT) {
		if (s->AnsRcv == ANSRCV_QUITOK) {
			s->CmdSnt = CMDSND_NONE; s->AnsRcv = ANSRCV_NONE;
			return ftpcs_SUCCESS;
			}
		else if (s->AnsRcv == ANSRCV_QUITERR) {
			s->Status = STS_ERROR;
			s->errorCode = ERRC_QUIT;
			return ftpcs_FAILED;
			}
		}
	return ftpcs_INPROGRESS;
}

arn_uCHAR ftp_getStatus(const struct uip_conn *sconn)
{
	struct ftp_state *s = (struct ftp_state *)sconn->appstate.ftp_st;
	return s->Status;
}

arn_uCHAR ftp_getErrorCode(const struct uip_conn *sconn)
{
	struct ftp_state *s = (struct ftp_state *)sconn->appstate.ftp_st;
	return s->errorCode;
}

//------------------------------------------------------------
//----------- END - API AND NEEDED CALLBACK ------------------
//------------------------------------------------------------

void ftp_init(void)
{
}

void ftp_appcall(void)
{
	struct ftp_state *s = (struct ftp_state *)uip_conn->appstate.ftp_st;
	//DEBUG_FTP("FTP APPCALL");
	if (uip_conn->rport == htons(s->remotectrlport)) {
	// preparazione in relazione agli aventi
		if(uip_connected()) {_connect(s);return;}
		if(uip_closed()) {_close(s);return;}
		if(uip_aborted()) {_abort(s);return;}
		if(uip_timedout()) {_timeout(s);return;}
		if(uip_acked()) {_ack(s);}
		if(uip_newdata()) {_newdata(s);}
		if(uip_poll()) {_poll(s);}
	//END --- preparazione in relazione agli aventi
	// eventi che determinano una eventuale trasmissione
		if(uip_rexmit() ||
			uip_newdata() ||
			uip_acked()) {
			_senddata(s);
			}
		else if (uip_poll()) {
			_senddata(s);
			}
	//END --- eventi che determinano una eventuale trasmissione
		}
	else if (uip_conn->rport == HTONS(20)/*htons(s->remotedataport)*/) {
	// preparazione in relazione agli aventi
		if(uip_closed()) {_close_data(s);return;}
		if(uip_aborted()) {_abort_data(s);return;}
		if(uip_timedout()) {_timeout_data(s);return;}
		if(uip_connected()) {_connect_data(s);return;}
		if(uip_acked()) {_ack_data(s);}
		if(uip_newdata()) {_newdata(s);}
		if(uip_poll()) {_poll_data(s);}
	//END --- preparazione in relazione agli aventi
	// eventi che determinano una eventuale trasmissione
		if(uip_connected() ||
			uip_rexmit() ||
			uip_newdata() ||
			uip_acked()) {
			_senddata_data(s);
			}
		else if (uip_poll()) {
			_senddata_data(s);
			}
	//END --- eventi che determinano una eventuale trasmissione
		}
}


void _abort(struct ftp_state *s)
{
}

void _timeout(struct ftp_state *s)
{
	//uip_close();
}
//test test test
arn_uCHAR TSTclose;
//END
void _close(struct ftp_state *s)
{
//test test test
TSTclose++;
//END
	if (s->Status == STS_CONNECTED) {
		switch (s->CmdSnt) {
			case CMDSND_QUIT:
				s->AnsRcv = ANSRCV_QUITOK;
				break;
			default: break;
			}
		}
}

// test test test
arn_uCHAR TSTconn;
arn_CHAR TSTgetBff[8][16];
//END
void _connect(struct ftp_state *s)
{
	s->Status = STS_CONNECTED;
	uip_len = 0;
}

void _newdata(struct ftp_state *s)
{
// test test test
strncpy(TSTgetBff[TSTconn],(arn_CHAR*)uip_appdata,16);
TSTconn++;
if (TSTconn == 8) TSTconn = 0;
//END
	uip_len = 0;
	if (s->Status == STS_CONNECTED) {
		switch (s->CmdSnt) {
			case CMDSND_CONNECT:
				if (!memcmp(okCode220,(void*)uip_appdata,sizeof(okCode220)-1)) s->AnsRcv = ANSRCV_WELCOME;
				else s->AnsRcv = ANSRCV_WELCOMERR;
				break;
			case CMDSND_LOGIN:
				if (!memcmp(okCode230,(void*)uip_appdata,sizeof(okCode230)-1)) s->AnsRcv = ANSRCV_LOGGEDIN;
				else if (!memcmp(okCode331,(void*)uip_appdata,sizeof(okCode331)-1)) s->AnsRcv = ANSRCV_PASSREQ;
				else s->AnsRcv = ANSRCV_LOGGEDERR;
				break;
			case CMDSND_PASS:
				if (!memcmp(okCode230,(void*)uip_appdata,sizeof(okCode230)-1)) s->AnsRcv = ANSRCV_LOGGEDIN;
				else s->AnsRcv = ANSRCV_LOGGEDERR;
				break;
			case CMDSND_SYST:
				if (!memcmp(okCode215,(void*)uip_appdata,sizeof(okCode215)-1)) {
					strncpy((arn_CHAR*)(s->data),(arn_CHAR*)(uip_appdata + sizeof(okCode215)),24);
					s->AnsRcv = ANSRCV_SYSTOK;
					}
				else s->AnsRcv = ANSRCV_SYSTERR;
				break;
			case CMDSND_PORT:
				if (!memcmp(okCode200,(void*)uip_appdata,sizeof(okCode200)-1)) s->AnsRcv = ANSRCV_PORTOK;
				else s->AnsRcv = ANSRCV_PORTERR;
				break;
			case CMDSND_STOR:
				if (s->dconn) {
					struct ftp_state *cs = (struct ftp_state*)((s->dconn)->appstate.ftp_st);
					if (!(cs->flags & FTP_FLAG_CMDREQUEST) &&
						!memcmp(okCode150,(void*)uip_appdata,sizeof(okCode150)-1)) {
						cs->flags |= (FTP_FLAG_CMDREQUEST | FTP_FLAG_DATAPENDING);
						}
					else if (!(cs->flags & FTP_FLAG_DATAPENDING)) s->AnsRcv = ANSRCV_STOROK;
					else s->AnsRcv = ANSRCV_STORERR;
					}
				else s->AnsRcv = ANSRCV_STORERR;
				break;
/*			case CMDSND_QUIT:
				if (!memcmp(okCode221,(void*)uip_appdata,sizeof(okCode221)-1)) s->AnsRcv = ANSRCV_QUITOK;
				else s->AnsRcv = ANSRCV_QUITERR;
				break;*/
			default: break;
			}
		}
}

// test test test
arn_uCHAR TSTack;
//END
void _ack(struct ftp_state *s)
{
TSTack++;
uip_len = 0;
}
// test test test
arn_CHAR TSTpss[32];
//END
void _poll(struct ftp_state *s)
{
	arn_uSHORT lLen;
	if ((s->Status == STS_CONNECTED) && (s->flags & FTP_FLAG_CMDREQUEST)) {
		switch (s->CmdSnt) {
			case CMDSND_STOR:
				uip_listen(s->localdataport);
				s->flags |= FTP_FLAG_CTRL;
				break;
			case CMDSND_QUIT:
				s->flags &= ~FTP_FLAG_CMDREQUEST;
				uip_close();
				//s->AnsRcv = ANSRCV_QUITOK;
				return;
			default: 
				break;
			}
// test test test
if (s->CmdSnt == CMDSND_PASS) strcpy(TSTpss,(const arn_CHAR*)s->data);
//END
		strcpy((arn_CHAR*)uip_appdata,(const arn_CHAR*)s->data);
		lLen = strlen((arn_CHAR*)uip_appdata);
		uip_len = ((lLen > uip_mss()) ? uip_mss() : lLen);
		s->flags &= ~FTP_FLAG_CMDREQUEST;
		}
}

void _senddata(struct ftp_state *s)
{
	if (uip_len > 0) {
		uip_send(uip_appdata,uip_len);
		}
}

void _retrasmit(struct ftp_state *s)
{
	arn_uSHORT lLen;
	if (s->Status == STS_CONNECTED) {
		switch (s->CmdSnt) {
			case CMDSND_STOR:
				uip_listen(s->localdataport);
				s->flags |= FTP_FLAG_CTRL;
				break;
			default: 
				break;
			}
		strcpy((arn_CHAR*)uip_appdata,(const arn_CHAR*)s->data);
		lLen = strlen((arn_CHAR*)uip_appdata);
		uip_len = ((lLen > uip_mss()) ? uip_mss() : lLen);
		}
}


arn_BOOL HostToIp(arn_CHAR* host,arn_uSHORT ipaddr[2])
{
	arn_uCHAR ipTmp[4];
	arn_uCHAR indx = 0;
	arn_CHAR* pHost = host;
	if (!pHost) return arnFALSE;
	if (!isdigit(pHost[0])) return arnFALSE;
	while (*pHost) {
		if (*pHost == '.') {
			ipTmp[indx++] = atoi(host);
			pHost++;
			host = pHost;
			}
		pHost++;
		}
	ipTmp[indx++] = atoi(host);
	if (indx < 4) return arnFALSE;
	uip_ipaddr(ipaddr,ipTmp[0],ipTmp[1],ipTmp[2],ipTmp[3]);
	return arnTRUE;
}


//--------------------------------------------------------------------------------

// data part

void _abort_data(struct ftp_state *s)
{
}

void _timeout_data(struct ftp_state *s)
{
}

void _close_data(struct ftp_state *s)
{
//	fc_state.DataStatus = STS_DATACONN_CLOSED_OK;
}

void _connect_data(struct ftp_state *s)
{
	arn_uCHAR i;
	s->flags = 0;
	for (i = 0; i < UIP_CONNS; i++) {
		struct uip_conn* cn = (&(uip_conns[i]));
		if (cn->tcpstateflags == UIP_ESTABLISHED && (cn->rport == FTP_PORT)) {
			struct ftp_state *cs = (struct ftp_state *)cn->appstate.ftp_st;
			if (!(cs->dconn) && (cs->flags & FTP_FLAG_CTRL)) {
				cs->dconn = uip_conn;
				s->fSAddr = cs->fSAddr;
				s->fSize = cs->fSize;
				break;
				}
			}
		}
	if (i == UIP_CONNS) {
		uip_abort(); 
		return;
		}
	s->flags |= FTP_FLAG_DATA;
	s->Status = STS_CONNECTED;
}

void _newdata_data(struct ftp_state *s)
{
}

void _ack_data(struct ftp_state *s)
{
	if (s->flags & FTP_FLAG_DATAPENDING) {
		if (!(s->flags & FTP_FLAG_LASTDATAACK)) {
			if (s->count >= uip_mss()) {
				s->dataptr += uip_mss();
				s->count -= uip_mss();
				}
			else {
				s->dataptr = 0;
				s->count = 0;
				s->flags &= ~FTP_FLAG_DATAPENDING;
				uip_close();
				}
			if (s->count >= uip_mss()) {
				strncpy((arn_CHAR*)uip_appdata,(const arn_CHAR*)(s->dataptr),uip_mss());
				uip_len = uip_mss();
				}
			else {
				strncpy((arn_CHAR*)uip_appdata,(const arn_CHAR*)(s->dataptr),s->count);
				uip_len = s->count;
				s->flags |= FTP_FLAG_LASTDATAACK;
				}
			}
		else {
			s->count = 0;
			s->dataptr = 0;
			s->flags &= ~FTP_FLAG_DATAPENDING;
			uip_close();
			}
		}
}

void _poll_data(struct ftp_state *s)
{
	if (s->flags & FTP_FLAG_CMDREQUEST) {
		s->dataptr = s->fSAddr;
		s->count = s->fSize;
		if (s->count >= uip_mss()) {
			strncpy((arn_CHAR*)uip_appdata,(const arn_CHAR*)(s->dataptr),uip_mss());
			uip_len = uip_mss();
			}
		else {
			strncpy((arn_CHAR*)uip_appdata,(const arn_CHAR*)(s->dataptr),s->count);
			uip_len = s->count;
			}
		s->flags &= ~FTP_FLAG_CMDREQUEST;
		}
}

void _senddata_data(struct ftp_state *s)
{
	if (uip_len > 0) {
		uip_send(uip_appdata,uip_len);
		}
}

void _retrasmit_data(struct ftp_state *s)
{
	if (s->count >= uip_mss()) {
		strncpy((arn_CHAR*)uip_appdata,(const arn_CHAR*)(s->dataptr),uip_mss());
		uip_len = uip_mss();
		}
	else {
		strncpy((arn_CHAR*)uip_appdata,(const arn_CHAR*)(s->dataptr),s->count);
		uip_len = s->count;
		}
}

#ifdef __cplusplus
}
#endif
