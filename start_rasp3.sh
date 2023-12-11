#!/bin/bash

qemu-system-aarch64 \
    -M raspi3b \
    -serial stdio \
    -append "width=800 height=480 logdev=ttyS1 loglevel=5" \
    -kernel bin/raspi/kernel8.img \
    -s