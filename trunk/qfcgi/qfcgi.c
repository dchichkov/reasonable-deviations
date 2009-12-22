/*
 * The MIT License
 *  
 * Copyright (c) 2009 Dmitry Chichkov
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE
 */

#include <fcgi_config.h>
#include <fcgiapp.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <string.h>
#include <quantis.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


enum 
{
	MAX_N = 1000,			// max random numbers per request
	MAX_RECORDS = 100000,	// maximum records per file
	cardnumber = 0			// quantis card number
};



int extract_int_parameter(char *str, char *param, int def)
{
    char *s = strstr(str, param);
    if(s == NULL) return def;
    s += strlen(param);
    return strtol(s, NULL, 10);
}


void accept_request(FCGX_Request *request)
{
	char POST[100];
	int i, j, len = FCGX_GetStr(POST, sizeof(POST) / sizeof(POST[0]), request->in);
	POST[sizeof(POST) / sizeof(POST[0]) - 1] = 0;	// sanitizing the request
	if(len <= 0)
	{
		FCGX_FPrintF(request->out,
			"Content-type: text/plain\r\n"
			"\r\n"
			"\r\n Error: Empty request."
			"\r\n The request should contain an user ID and a number N.");
		return;
	}

	else	// nonempty request
	{
		unsigned char data[MAX_N];
		char result[100 + MAX_N * 4];
		char *IP = FCGX_GetParam("REMOTE_ADDR", request->envp);
		int ID = extract_int_parameter(POST, "id=", -1);
		int APP = extract_int_parameter(POST, "app=", -1);
		int N = extract_int_parameter(POST, "n=", 0);

		if(N < 0 || N > MAX_N) N = 0;
		
		if (N != quantisRead(cardnumber, (char *) data, N))
		{
			syslog (LOG_INFO, "Warning. Cannot read card %d. Failed in quantisRead.\n", cardnumber);
			
			if (quantisBoardReset(cardnumber))
			{
				FCGX_FPrintF(request->out,
					"Content-type: text/plain\r\n"
					"\r\n"
					"\r\n Error: Failed to reset the QRNG device.");
			
				syslog (LOG_ERR, "Warning. Cannot reset the card %d. Failed in quantisBoardReset.\n", cardnumber);
				return;
			}
			if (N != quantisRead(cardnumber, (char *) data, N))
			{
				FCGX_FPrintF(request->out,
					"Content-type: text/plain\r\n"
					"\r\n"
					"\r\n Error: Failed to read data from the QRNG device.");
				syslog (LOG_ERR, "Warning. Cannot read card %d. Failed in quantisRead.\n", cardnumber);
				return;
			
			}
		}
		
		// for(i = 0; i < N; i++) data[i] = (char)rand();	// fill with 'random' data

		i = snprintf(result, sizeof(result) / sizeof(result[0]), "%d,%s,%d,%d,%d", (int)time(NULL), IP ? IP : "0.0.0.0", ID, APP, N);
		for(j = 0; j < N; j++)
			i += snprintf(result + i,  sizeof(result) / sizeof(result[0]) - i, ",%d", data[j]);
		snprintf(result + i,  sizeof(result) / sizeof(result[0]) - i, "\r\n");

		// we have a new record. write it to a file.
		if(IP != NULL && ID != -1 && APP != -1 && N != 0)
		{   
			static FILE *f = NULL;
			static int records_count = 0;

			if(++records_count >= MAX_RECORDS) 
			{
				records_count = 0;
				if(f != NULL) fclose(f);
				f = NULL;
			}

			if(f == NULL)
			{
				char fname[100];
				snprintf(fname, 100, "data/%d", (int)time(NULL));
				f = fopen(fname, "ab");
				if(f == NULL)
				{
					syslog (LOG_ERR, "Program aborted. Failed creating a file %s.", fname);
					abort();
				}
			}

			if(f != NULL)
				fputs(result, f);
		}


		FCGX_FPrintF(request->out,
			"Content-type: text/plain\r\n"
			"\r\n");

		FCGX_PutS(result, request->out);
	}
}





int main ()
{
	FCGX_Request request;
	int listen_socket;

	// Init SysLog
	setlogmask (LOG_UPTO (LOG_NOTICE));
	openlog ("qrv-fcgi-server", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	syslog (LOG_NOTICE, "Program started by User %d", getuid ());

	// Init QRNG
	if (!quantisCount() || (quantisCount() < cardnumber+1)) {
		syslog (LOG_ERR, "card %d not found\n",cardnumber);
		abort();
	}

	if (quantisBoardReset(cardnumber)){
		syslog (LOG_ERR, "cannot reset card %d\n",cardnumber);
		abort();
	}

	// Init FCGX
	listen_socket = FCGX_OpenSocket(":9000", 100);
	if (listen_socket < 0)
	{
		syslog (LOG_ERR, "Program aborted. The socket :9000 is unavailable.");
		abort();
	}

	if (FCGX_Init()) 
	{
		syslog (LOG_ERR, "Program aborted. Failed in the FCGX_Init().");
		abort();
	}

	if (FCGX_InitRequest(&request, listen_socket, 0)) 
	{
		syslog (LOG_ERR, "Program aborted. Failed in the FCGX_InitRequest().");
		abort();
	}

	while (FCGX_Accept_r(&request) >= 0) 
	{
		accept_request(&request);
	}

	syslog (LOG_ERR, "Program aborted. Failed in the FCGX_Accept_r().");
	closelog ();
	return -1;
}
