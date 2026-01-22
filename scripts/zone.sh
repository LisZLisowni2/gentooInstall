#!/bin/bash

ZONES=$(find /usr/share/zoneinfo -type d)

for ZONE in ${ZONES[@]}; do
    RES=${ZONE##*/};
    if [[ \"$RES\" != \"zoneinfo\" ]]; then
        echo "$RES" >> /tmp/zone.tmp
    fi
done