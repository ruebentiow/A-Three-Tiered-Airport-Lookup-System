/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "PlacesToAirport.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

returnTypeA *
ptoa_1(LatLon *argp, CLIENT *clnt)
{
	static returnTypeA clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, PToA,
		(xdrproc_t) xdr_LatLon, (caddr_t) argp,
		(xdrproc_t) xdr_returnTypeA, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
