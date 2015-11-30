#!/bin/sh

QSM_ROOT=$(dirname $(dirname $(readlink -f $0)))
QS_PATH=$(dirname "$QSM_ROOT")

QSM_BIN="$QSM_ROOT/bin"
QSM_SCRIPTS="$QSM_ROOT/scripts"

