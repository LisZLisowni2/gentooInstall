#!/bin/bash

ZONES=$(find /usr/share/zoneinfo -type d)

for ZONE in ${ZONES[@]}; do
    RES=${ZONE##*/};
    if [[ \"$RES\" != \"zoneinfo\" ]]; then
        echo "$RES" >> /tmp/UnsortedZone.tmp
    fi
done

sort --output=/tmp/zone.tmp /tmp/UnsortedZone.tmp
