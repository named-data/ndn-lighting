#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ccn/ccn.h>
#include <ccn/uri.h>

#define URI "/light1"

#define UNUSED(expr) UNUSED_ ## expr __attribute__((unused))

static enum ccn_upcall_res
incoming_handler(
		struct ccn_closure* UNUSED(selfp),
		enum ccn_upcall_kind kind,
		struct ccn_upcall_info* info)
{
	unsigned int i;
	int res;
	const unsigned char *ptr;
	size_t length;

	printf("Received response (kind: %d)\n", kind);
	switch(kind) {
		case CCN_UPCALL_FINAL:
			printf("deregistering handler\n");
			return CCN_UPCALL_RESULT_OK;

		case CCN_UPCALL_CONTENT:
			printf("received content\n");
			res = ccn_content_get_value(info->content_ccnb, info->pco->offset[CCN_PCO_E], info->pco, &ptr, &length);

			printf("Output: ");
			for (i = 0; i < length; i++) {
				putchar(ptr[i]);
			}
			break;

		case CCN_UPCALL_INTEREST_TIMED_OUT:
			printf("request timed out - retrying\n");
			return CCN_UPCALL_RESULT_REEXPRESS;

		case CCN_UPCALL_CONTENT_UNVERIFIED:
		case CCN_UPCALL_CONTENT_BAD:
			printf("error\n");
			return CCN_UPCALL_RESULT_ERR;

		default:
			printf("Unexpected response\n");
			return CCN_UPCALL_RESULT_ERR;
	}

	return CCN_UPCALL_RESULT_OK;
}

int main(void)
{
	int res;
	struct ccn *ccn = NULL;
	struct ccn_charbuf *name = NULL;
	struct ccn_closure *incoming;

	name = ccn_charbuf_create();

	res = ccn_name_from_uri(name, URI);
	if (res < 0) {
		fprintf(stderr, "bad ccn URI: %s\n", URI);
		exit(1);
	}
	ccn = ccn_create();

	res = ccn_connect(ccn, NULL);
	if (res < 0) {
		fprintf(stderr, "can't connect: %d\n", res);
		ccn_perror(ccn, "ccn_connect");
		exit(1);
	}

	incoming = calloc(1, sizeof(*incoming));
	incoming->p = incoming_handler;
	res = ccn_express_interest(ccn, name, incoming, NULL);

	printf("Waiting for response to interest packet\n");

	while (res >= 0) {
		printf("running ccn_run...\n");
		res = ccn_run(ccn, -1);
	}

	if (res < 0) {
		ccn_perror(ccn, "ccn_run");
		exit(1);
	}

	ccn_charbuf_destroy(&name);
	ccn_destroy(&ccn);

	return 0;
}
