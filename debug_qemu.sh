#!/bin/sh

set -e
. ./iso.sh

GDB_BIN="i686-elf-gdb"
GDB_WINDOW="gdb"
KERNEL_FILE="kernel/pissos.kernel"
GDB_INIT_FILE=".gdbinit"

GDB_TARGET_PANE="${GDB_WINDOW}.1"

QEMU_CMD="qemu-system-$(./target-triplet-to-arch.sh $HOST) \
  -kernel ${KERNEL_FILE} \
  -s -S"

GDB_COMMAND="${GDB_BIN}"


tmux select-window -t ":${GDB_WINDOW}"

tmux send-keys -t ${GDB_TARGET_PANE} 'clear' Enter

tmux send-keys -t ${GDB_TARGET_PANE} "${GDB_COMMAND}" Enter

exec $QEMU_CMD
