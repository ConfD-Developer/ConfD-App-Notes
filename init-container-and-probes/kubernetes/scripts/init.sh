#!/usr/bin/env bash
# -*-  Mode: Shell-script -*-
# models.sh --- Create external init store

kubectl create configmap init-config \
        --from-file=aaa_init.xml=resources/aaa_init.xml

# init.sh ends here
