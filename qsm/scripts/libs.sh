#!/bin/sh

QSM_ROOT=$(dirname $(dirname $(readlink -f $0)))
QSM=$QSM_ROOT/bin/qsm
QS_PATH=$(dirname "$QSM_ROOT")

QSM_BIN="$QSM_ROOT/bin"
QSM_SCRIPTS="$QSM_ROOT/scripts"

warning()
{
	/bin/echo -e -n '\e[0;31mWarning: '
	/bin/echo "$@"
	/bin/echo -e -n '\e[0m'
}

info()
{
	/bin/echo -e -n '\e[0;34mInformation: '
	/bin/echo "$@"
	/bin/echo -e -n '\e[0m'
}

error()
{
	/bin/echo -e -n '\e[1;31mError: '
	/bin/echo "$@"
	/bin/echo -e -n '\e[0m'
	exit 1
}

usage()
{
	/bin/echo -e -n '\e[0;34mUsage: '
	/bin/echo "$@"
	/bin/echo -e -n '\e[0m'
	exit 0
}
