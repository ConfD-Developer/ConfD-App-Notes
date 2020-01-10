#!/usr/bin/env bash
# -*-  Mode: Shell-script -*-
# app-secrets.sh --- Create app secrets for ConfD access

kubectl create secret generic app-secrets \
        --from-literal=CONFD_IPC_ACCESS_FILE=/app/secrets/access.txt \
        --from-file=access.txt=resources/access.txt

# app-secrets.sh ends here
