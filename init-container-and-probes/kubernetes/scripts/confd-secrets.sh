#!/usr/bin/env bash
# -*-  Mode: Shell-script -*-
# confd-secrets.sh --- Create ConfD secrets

kubectl create secret generic confd-secrets \
        --from-file=access.txt=resources/access.txt \
        --from-file=confd.conf=resources/confd.conf \
        --from-file=ssh_host_rsa_key.pub=resources/ssh_host_rsa_key.pub \
        --from-file=ssh_host_rsa_key=resources/ssh_host_rsa_key

# confd-secrets.sh ends here
