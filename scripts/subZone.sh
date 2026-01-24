#!/bin/bash

ROOT_ZONE=$1
ZONES=$(find "/usr/share/zoneinfo/$ROOT_ZONE" -type f)

for ZONE in ${ZONES[@]}; do
    RES=${ZONE##*/};
    if [[ \"$RES\" != \"zoneinfo\" ]]; then
        echo "$RES" >> /tmp/subZone.tmp
    fi
done