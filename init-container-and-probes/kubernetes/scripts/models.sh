#!/usr/bin/env bash
# -*-  Mode: Shell-script -*-
# models.sh --- Create external data models store

kubectl create configmap models \
        --from-file=user-folders.fxs=resources/user-folders.fxs

# models.sh ends here
