#!/bin/bash

FIFO="${PGRAMS_EXE_DIR}/ctl"

start() {
  cd ${PGRAMS_EXE_DIR}
  # making FIFO
  rm -f "$FIFO"
  mkfifo -m 600 "$FIFO"
  exec 3<> "$FIFO"

  id=`date "+%Y%m%d%H%M%S"`
  exec ./neu_test_20260128.rb <&3
  end_status=$?
}

stop() {
  if [[ ! -p "$FIFO" ]]; then
    exit 0
  fi
  printf ".q\n" > "$FIFO"
  rm -f "$FIFO"
}

case "${1:-}" in
  start) start ;;
  stop)  stop ;;
  *) echo "Usage: $0 {start|stop}" >&2; exit 2 ;;
esac