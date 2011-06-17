/***************************************************************************
                          ftp.h  -  description
                             -------------------
    begin                : mer 31 01 2007
    copyright            : (C) 2007 by Aurion s.r.l.
    email                : fabio.giovagnini@aurion-tech.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * \addtogroup ftp
 * @{
 */

/**
 * \file
 * Header file for the ftp client.
 * \author Fabio Giovagnini <fabio.giovagnini@aurion-tech.com>
 */

/*
 * Copyright (c) 2007, Aurion s.r.l.
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
 * $Id: ftp.h,v 1.0.0.0 2007/01/31 21:23:49 fabio Exp $
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ARNFTP_H__
#define __ARNFTP_H__

typedef unsigned char		arn_uCHAR;
typedef unsigned short		arn_uSHORT;
typedef unsigned long		arn_uLONG;
typedef arn_uLONG		arn_uINT;

typedef signed char		arn_sCHAR;
typedef signed short		arn_sSHORT;
typedef signed long		arn_sLONG;
typedef arn_sLONG		arn_sINT;

typedef char    		arn_CHAR;
typedef short   		arn_SHORT;
typedef long    		arn_LONG;
typedef int    			arn_INT;

/* END - C Native Types */

#define arnTRUE			((arn_uCHAR) 1)
#define arnFALSE		((arn_uCHAR) 0)
typedef arn_uCHAR		arn_BOOL;

#define MAX_FILENAME_LEN		128
#define FTP_FLAG_CMDREQUEST		0x01
#define FTP_FLAG_DATAPENDING	0x02
#define FTP_FLAG_LASTDATAACK	0x04
#define FTP_FLAG_CTRL			0x80
#define FTP_FLAG_DATA			0x40

struct ftp_state {
	struct uip_conn * dconn;	/*!<  connection for data management */
	arn_uCHAR*	dataptr;		/*!<  Pointer to the current chunk of data */
	arn_uLONG	count;			/*!<  Counter of current chunk of data */
	arn_uCHAR*	fSAddr;			/*!<  Pointer to the base address of the file */
	arn_uLONG	fSize;			/*!<  curret size of the file */
	arn_uSHORT	errorCode;		/*!< ErrorCode */
	arn_uSHORT localdataport;	/*!< default = 5001 */
	arn_uSHORT remotedataport;	/*!< default = 20*/
	arn_uSHORT remotectrlport;	/*!< default = 21 */
	arn_uCHAR	Status;			/*!< State of the protocol */
	arn_uCHAR	CmdSnt;			/*!< Comando inviato */
	arn_uCHAR	AnsRcv;			/*!< Risposta ricevuta */
	arn_uCHAR	flags;		/*!< allineamento */
	arn_CHAR	fname[MAX_FILENAME_LEN];	/*!<  file nale to upload */
	arn_uCHAR	data[MAX_FILENAME_LEN + 8];
	};


void ftp_init(void);
void ftp_appcall(void);

// callback functions and API
typedef enum ftp_cmd_status {
	ftpcs_NONE = 0,
	ftpcs_INPROGRESS,
	ftpcs_SUCCESS,
	ftpcs_FAILED,
	ftpcs_USERNAME_REQ,
	ftpcs_PASSWORD_REQ
	} ftp_cmd_status_t;
ftp_cmd_status_t ftp_connect(arn_CHAR* host,arn_uSHORT rport,struct uip_conn **sconn);
ftp_cmd_status_t ftp_login(const struct uip_conn *sconn,arn_CHAR* username);
ftp_cmd_status_t ftp_password(const struct uip_conn *sconn,arn_CHAR* password);
ftp_cmd_status_t ftp_syst(const struct uip_conn *sconn,arn_CHAR* res);
ftp_cmd_status_t ftp_port(const struct uip_conn *sconn,arn_uSHORT ldataport);
ftp_cmd_status_t ftp_pasv(const struct uip_conn *sconn);
ftp_cmd_status_t ftp_send(const struct uip_conn *sconn,arn_CHAR* filename,arn_uCHAR* sadd,arn_uLONG size);
ftp_cmd_status_t ftp_quit(const struct uip_conn *sconn);


#define STS_NONE						0
#define STS_ERROR						(STS_NONE + 1)
#define STS_WAITFORCONN					(STS_ERROR + 1)
#define STS_CONNECTED					(STS_WAITFORCONN + 1)
#define STS_SENDINGFILE					(STS_CONNECTED + 1)
#define STS_UPOLOADOK					(STS_SENDINGFILE + 1)
#define STS_LOGGINGIN					(STS_UPOLOADOK + 1)
#define STS_LOGGEDIN					(STS_LOGGINGIN + 1)
#define STS_SYST_INPROG					(STS_LOGGEDIN + 1)
#define STS_SYST_OK						(STS_SYST_INPROG + 1)
#define STS_PORT_INPROG					(STS_SYST_OK + 1)
#define STS_PORT_OK						(STS_PORT_INPROG + 1)
#define STS_DATACONN_INPROG				(STS_PORT_OK + 1)
#define STS_DATACONN_OK					(STS_DATACONN_INPROG + 1)
#define STS_STOR_INPROG					(STS_DATACONN_OK + 1)
#define STS_START_DATASTORE				(STS_STOR_INPROG + 1)
#define STS_SENDDATA_INPROG				(STS_START_DATASTORE + 1)		// 16 , 0x10
#define STS_SENDDATA_ENDING				(STS_SENDDATA_INPROG + 1)
#define STS_SENDDATA_OK					(STS_SENDDATA_ENDING + 1)
#define STS_DATACONN_CLOSED_OK			(STS_SENDDATA_OK + 1)
#define STS_QUIT_INPROG					(STS_DATACONN_CLOSED_OK + 1)
#define STS_QUIT_OK						(STS_QUIT_INPROG + 1)
#define STS_IDLE						(STS_QUIT_OK + 1)
arn_uCHAR ftp_getStatus(const struct uip_conn *sconn);

#define ERRC_NONE						0
#define ERRC_HOSTNAMENOTIP				(ERRC_NONE + 1)
#define ERRC_CONN						(ERRC_HOSTNAMENOTIP + 1)
#define ERRC_FNAMETOLOONG				(ERRC_CONN + 1)
#define ERRC_SRV_WM						(ERRC_FNAMETOLOONG + 1)
#define ERRC_LOGIN						(ERRC_SRV_WM + 1)
#define ERRC_SYST						(ERRC_LOGIN + 1)
#define ERRC_PORT						(ERRC_SYST + 1)
#define ERRC_CONN_DATA					(ERRC_PORT + 1)
#define ERRC_STOR						(ERRC_CONN_DATA + 1)
#define ERRC_QUIT						(ERRC_STOR + 1)
#define ERRC_PASS						(ERRC_QUIT + 1)
arn_uCHAR ftp_getErrorCode(const struct uip_conn *sconn);


/* UIP_APPCALL: the name of the application function. This function
   must return void and take no arguments (i.e., C type "void
   appfunc(void)"). */
//#ifndef UIP_APPCALL
//#define UIP_APPCALL     ftp_appcall
//#endif


/* UIP_APPSTATE_SIZE: The size of the application-specific state
   stored in the uip_conn structure. */
#ifndef UIP_APPSTATE_SIZE
#define UIP_APPSTATE_SIZE (sizeof(struct ftp_state))
#endif


#endif /* __ARNFTPD_H__ */

#ifdef __cplusplus
}
#endif
