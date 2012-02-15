/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
whiteboardserver_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		ClientData addclient_1_arg;
		ClientData delclient_1_arg;
		AddLineArg addline_1_arg;
		ClientData sendallmylines_1_arg;
		int query_1_arg;
		char *newserver_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case addclient:
		_xdr_argument = (xdrproc_t) xdr_ClientData;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) addclient_1_svc;
		break;

	case delclient:
		_xdr_argument = (xdrproc_t) xdr_ClientData;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) delclient_1_svc;
		break;

	case addline:
		_xdr_argument = (xdrproc_t) xdr_AddLineArg;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) addline_1_svc;
		break;

	case sendallmylines:
		_xdr_argument = (xdrproc_t) xdr_ClientData;
		_xdr_result = (xdrproc_t) xdr_Linep;
		local = (char *(*)(char *, struct svc_req *)) sendallmylines_1_svc;
		break;

	case query:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_BBoard;
		local = (char *(*)(char *, struct svc_req *)) query_1_svc;
		break;

	case newserver:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) newserver_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

#undef WhiteBoardServer
int getTransientProgNumber(int version);
//void startserver(int, int);


int main(int argc, char *argv[])
{
	SVCXPRT *transp;
	int WhiteBoardServer;

	WhiteBoardServer = getTransientProgNumber(WhiteBoardServerVersion);
	if (WhiteBoardServer <= 0) {
	    fprintf(stderr, "%s: getTransientProgNumber(%d) returned %d!\n",
		    argv[0], WhiteBoardServerVersion, WhiteBoardServer);
	    exit(2);
	}
	pmap_unset (WhiteBoardServer, WhiteBoardServerVersion);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, WhiteBoardServer, WhiteBoardServerVersion, whiteboardserver_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (WhiteBoardServer, WhiteBoardServerVersion, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, WhiteBoardServer, WhiteBoardServerVersion, whiteboardserver_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (WhiteBoardServer, WhiteBoardServerVersion, tcp).");
		exit(1);
	}

	fprintf(stderr,	"startserver(%d, %d)\n",
			WhiteBoardServer, WhiteBoardServerVersion);
//	startserver(WhiteBoardServer, WhiteBoardServerVersion);

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}