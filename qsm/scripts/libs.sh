#!/bin/sh

QSM_ROOT=$(dirname $(dirname $(readlink -f $0)))
QSM=$QSM_ROOT/bin/qsm
QS_PATH=$(dirname "$QSM_ROOT")

QSM_BIN="$QSM_ROOT/bin"
QSM_SCRIPTS="$QSM_ROOT/scripts"

_message()
{
	local TEXT="$1"
	shift
	local COLOR="$1"
	shift
	for x; do
		printf "\033[%sm%s%s\033[0m\n" "$COLOR" "$TEXT" "$x"
		TEXT=`echo "$TEXT" | sed 's/./ /g'`
	done 
}

warning()
{
	_message "Warning: " "0;31" "$@"
}

info()
{
	_message "Information: " "0;34" "$@"
}

error()
{
	_message "Error: " "1;31" "$@"
	exit 1
}

usage()
{
	_message "Usage: " "0;34" "$@"
	exit 0
}
