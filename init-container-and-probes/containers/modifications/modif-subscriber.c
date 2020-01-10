/*********************************************************************
 * ConfD cdb_get_modifications() example
 *
 * (C) 2017 Tail-f Systems
 * Permission to use this code as a starting point hereby granted
 * This is ConfD Sample Code
 *
 * See the README file for more information
 ********************************************************************/

#include <getopt.h>
#include <libgen.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <confd.h>
#include <confd_cdb.h>

#define _TRACE_DECLARE
#include <traceh.h>

#include "user-folders.h"

#include "modif-printer.h"

/********************************************************************/
#define DAEMON_NAME "subscriber-modifications"
#define ROOT_PATH "/folder-user"

static int ss;
static int sp;

struct addrinfo *lookup(const char *addr, const char *port)
{
    INFO_ENTER("");

    struct addrinfo hints, *ai0 = NULL;
    int ret;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    fprintf(stdout, "look up address %s:%s\n", addr, port);
    if ((ret = getaddrinfo(addr, port, &hints, &ai0)) != 0) {
        FATAL("getaddrinfo: %s:%s (%s)\n",
              addr, port, gai_strerror(ret));
        exit(1);
    }

    char ip[INET_ADDRSTRLEN];
    for (struct addrinfo *ai = ai0; ai; ai = ai->ai_next) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)ai->ai_addr;
        inet_ntop(ai->ai_family, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
        fprintf(stdout, "Got address %s\n", ip);
    }

    INFO_EXIT("(%p)", ai0);
    return ai0;;
}

int init_subscriber(struct addrinfo *ai)
{
    INFO_ENTER("");

    int ret = CONFD_ERR;

    ret = confd_load_schemas(ai->ai_addr, ai->ai_addrlen);
    if (CONFD_OK != ret) {
        FATAL("Failed to load schemas from ConfD\n");
        goto term;
    }

    if ((ss = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
        FATAL("Failed to open socket\n");
        goto term;
    }

    ret = cdb_connect(ss, CDB_SUBSCRIPTION_SOCKET, ai->ai_addr, ai->ai_addrlen);
    if (CONFD_OK != ret) {
        FATAL("Failed to cdb_connect() to ConfD\n");
        goto term;
    }

    ret = cdb_subscribe(ss, 1, folders__ns, &sp, ROOT_PATH);
    if (CONFD_OK != ret) {
        FATAL("subscribe %d\n");
        goto term;
    }

    ret = cdb_subscribe_done(ss);
    if (CONFD_OK != ret) {
        FATAL("cdb_subscribe_done() failed");
        goto term;
    }

    TRACE("subscription point = %d", sp);

term:
    INFO_EXIT("(%i)", ret);
    return ret;
}


int process_modifications(void)
{
    INFO_ENTER("");
    int ret = CONFD_OK;

    confd_tag_value_t *values = NULL;
    int values_cnt = 0;

    // get all the data in the subtree, including the sublists...
    int flags = CDB_GET_MODS_INCLUDE_LISTS;

    // get list of all the changes done in triggering commit
    ret = cdb_get_modifications(ss, sp, flags, &values, &values_cnt, NULL);
    if (ret != CONFD_OK) {
        FATAL("cdb_get_modifications() failed");
        goto term;
    }

    TRACE("number of modifications: %d", values_cnt);

    // pass the changes to a custom "writer" to create a NETCONF-like
    // content for corresponding edit-config message
    char *result_str = pr_write_value_array(values, values_cnt);
    printf("modifications read by subscriber:\n%s", result_str);

    free(result_str);
    result_str = NULL;

    int i;
    for (i = 0; i < values_cnt; i++) {
        confd_free_value(CONFD_GET_TAG_VALUE(&values[i]));
    }

    if (NULL != values) {
        free(values);
        values = NULL;
        values_cnt = 0;
    }

term:
    INFO_EXIT("(%x)", ret);
    return ret;
}


int main(int argc, char *argv[])
{
    INFO_ENTER("argc %i", argc);
    int ret = CONFD_OK;
    struct addrinfo *ai = NULL;
    int debuglevel = CONFD_TRACE;
    char *addr = "127.0.0.1";
    char *port = "4565";

    int c;
    while ((c = getopt(argc, argv, "a:dhp:qrt")) != -1) {
        switch (c) {
        case 'a':
            addr = optarg;
        case 'd':
            debuglevel = CONFD_DEBUG;
            break;
        case 'h':
            fprintf(stderr, "usage: simple [-dhpqt]\n");
            exit(0);
        case 'p':
            port = optarg;
        case 'q':
            debuglevel = CONFD_SILENT;
            break;
        case 'r':
            debuglevel = CONFD_PROTO_TRACE;
            break;
        case 't':
            debuglevel = CONFD_TRACE;
            break;
        default:
            fprintf(stderr, "usage: simple [-qdtp]\n");
            exit(1);
        }
    }

    confd_init(DAEMON_NAME, stderr, debuglevel);

again:
    ai = lookup(addr, port);
    if ((ret = init_subscriber(ai)) != CONFD_OK) {
        INFO("Failed to initialize subscriber");
        sleep(2);
        goto again;
    }
    freeaddrinfo(ai);

    const int subs_cnt = cdb_active_subscriptions;

    while (1) {
        struct pollfd set[subs_cnt];
        set[0].fd = ss;
        set[0].events = POLLIN;
        set[0].revents = 0;

        if (poll(set, sizeof set/sizeof *set, -1) < 0) {
            FATAL("Poll failed:");
            break;
        }

        if ((set[0].revents & POLLHUP)) {
            /* Socket closed, assume current ConfD failed */
            INFO("Lost ConfD connection (POLLHUP) - HA failover?");
            goto again;
        }

        if ((set[0].revents & POLLERR)) {
            /* Socket closed, assume current ConfD failed */
            INFO("Lost ConfD connection (POLLERR) - HA failover?");
            goto again;
        }

        if (!(set[0].revents & POLLIN)) {
            continue;
        }

        int sub_points[subs_cnt];
        int reslen;

        ret = cdb_read_subscription_socket(ss, &sub_points[0], &reslen);
        if (CONFD_OK != ret) {
            FATAL("sub_read: %d\n", ret);
            goto again;
            break;
        }

        if (sub_points[0] == sp) {
            TRACE("subscription triggered");
            ret = process_modifications();
        }

        ret = cdb_sync_subscription_socket(ss, CDB_DONE_PRIORITY);
        if (CONFD_OK != ret) {
            FATAL("failed to sync subscription: %d\n", ret);
            goto again;
            break;
        }
    }

    INFO_EXIT("ret %i", ret);
    return ret;
}
