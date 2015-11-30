#!/bin/sh

set -e

. $(dirname $0)/libs.sh

install()
{
	info "Trying to install $1 automatically, sudo maybe required!"
	sudo apt-get install $1 -y
}

[ -x "$(which git)" ] || install git
[ -x "$(which make)" ] || install make
[ -x "$(which gcc)" ] || install gcc
[ -x "$(which g++)" ] || install g++
