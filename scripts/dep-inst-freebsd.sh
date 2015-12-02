#!/bin/sh

set -e

. $(dirname $0)/libs.sh

install()
{
	info "Trying to install $1 automatically, sudo maybe required!"
	sudo pkg install -y $1
}

[ -x "$(which git)" ] || install git
[ -x "$(which make)" ] || install make
[ -x "$(which cmake)" ] || install cmake
