/*
 * Copyright 2020 Tail-f Systems AB
 *
 * Permission to use this code as a starting point hereby granted
 *
 * Check if ConfD is ready to accept southbound connections
 */

#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <confd.h>
#include <confd_maapi.h>

#define SA struct sockaddr
#define PHASE_ONE 1

#define OK(E) ok((E), #E, __FUNCTION__, __LINE__, "FAILED")
static int ok(int res, char *expr, const char *func, int line, char *prefix)
{
    if (res == CONFD_EOF) {
        fprintf(stderr, "%s: %s, confd closed connection (CONFD_EOF), "
                "in function %s, line %d\n", prefix, expr, func, line);
        exit(1);
    }
    if (res == CONFD_ERR) {
        fprintf(stderr, "%s: %s, Error: %s (%d): %s, "
                "in function %s, line %d\n", prefix, expr,
                confd_strerror(confd_errno), confd_errno,
                (confd_errno == CONFD_ERR_OS) ? strerror(errno):
                confd_lasterr(), func, line);
        exit(1);
    }
    return res;
}

struct addrinfo *lookup(const char *addr, const char *port)
{
    struct addrinfo hints, *ai0 = NULL;
    int res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((res = getaddrinfo(addr, port, &hints, &ai0)) != 0) {
        fprintf(stderr, "Invalid address %s:%s (%s)\n",
                addr, port, gai_strerror(res));
        freeaddrinfo(ai0);
        exit(1);
    }

    char ip[INET_ADDRSTRLEN];
    for (struct addrinfo *ai = ai0; ai; ai = ai->ai_next) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)ai->ai_addr;
        inet_ntop(ai->ai_family, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
    }

    return ai0;;
}

int get_socket(const struct addrinfo *ai)
{
    return socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
}

int main(int argc, char *argv[])
{
    char *addr = "127.0.0.1";
    char *port = "4565";
    char *progname = strrchr(argv[0], (int)'/');

    /* Setup progname (without path component) */
    if (progname == NULL)
        progname = argv[0];
    else
        progname++;

    int c;
    while ((c = getopt(argc, argv, "a:dhp:")) != EOF) {
        switch (c) {
        case 'a':
            addr = optarg;
            break;
        case 'p':
            port = optarg;
            break;
        case 'h':
            fprintf(stderr, "%s [-a host] [-d|-d...] [-p port]\n", progname);
            exit(0);
            break;
        default:
            fprintf(stderr, "%s [-a host] [-d|-d...] [-p port]\n", progname);
            exit(1);
            break;
        }
    }

    /* Initialize address to confd daemon */
    struct addrinfo *ai = lookup(addr, port);

    /* Initialize ConfD library */
    confd_init(progname, 0, CONFD_SILENT);
    OK(confd_load_schemas(ai->ai_addr, ai->ai_addrlen));
    assert(confd_init_daemon(progname) != NULL);

    /* Wait for ConfD to reach start phase one, i.e. it's ready to accept
     * IPC-connections */
    int ms;
    assert((ms = get_socket(ai)) >= 0);
    OK(maapi_connect(ms, (SA *)ai->ai_addr, ai->ai_addrlen));
    OK(maapi_wait_start(ms, PHASE_ONE));

    return 0;
}
