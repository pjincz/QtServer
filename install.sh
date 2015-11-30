#!/bin/sh

QT_VERSION=v5.5.1

. ./install/libs.sh

if [ -f $(which apt-get) ]; then
	./install/dep-inst-debian.sh
else
	warning 'Cannot install dependency automatically!'
fi


# clone qtbase
[ -f qtbase/.git/config ] || (
	info "Cloning Qt Base..."
	echo

	git clone https://github.com/qtproject/qtbase.git
)

[ "$(GIT_DIR="$PWD/qtbase/.git" git describe)" = "$QT_VERSION" ] || (
	info "Checkout $QT_VERSION..."
	echo

	cd qtbase
	git checkout $QT_VERSION
)

[ -f qtbase/Makefile ] || (
	info "Configuring Qt Base..."
	echo

	cd qtbase
	./configure -opensource -confirm-license -release -static -no-gui -no-xcb -no-icu
)

[ -f qtbase/lib/libQt5Network.a ] || (
	info "Building Qt Base..."
	echo

	cd qtbase
	make sub-src-qmake_all

	cd src
	make sub-corelib sub-network
)
