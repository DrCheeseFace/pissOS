#!/bin/sh

set -e
. ./iso.sh

KERNEL_FILE="kernel/pissos.kernel"

QEMU_CMD="qemu-system-$(./target-triplet-to-arch.sh $HOST) \
  -kernel ${KERNEL_FILE} \
  -s -S"

exec $QEMU_CMD
