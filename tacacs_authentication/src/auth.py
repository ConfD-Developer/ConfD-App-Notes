#!/usr/bin/env python
from tacacs_plus.client import TACACSClient
import socket
import sys

line = sys.stdin.readline()
line = line.replace("[", "")
line = line.replace("]", "")
token = line.split(";")  # token[0] username, token[1] password


def get_av_pair(arguments, key, default=None):
    ret = default
    for av in arguments:
        avf = av.split("=")
        if avf[0] == key:
            ret = avf[1]
            break
    return ret


cli = TACACSClient('localhost', 49, 'testing123', timeout=10,
                   family=socket.AF_INET)
authen = cli.authenticate(token[0], token[1])
if authen.valid == True:
    auth = cli.authorize(token[0], arguments=["service=tailf"])
    groups = get_av_pair(auth.arguments, key="groups")
    if groups != None:
        uid = get_av_pair(auth.arguments, key="uid", default=9000)
        gid = get_av_pair(auth.arguments, key="gid", default=100)
        home = "/var/confd/homes/{}".format(token[0])
        print("accept {} {} {} {}".format(groups, uid, gid, home))
    else:
        print(
            "reject Cannot retrieve groups AV pair (tailf service) for user {}"
                .format(token[0]))
else:
    print("reject")
