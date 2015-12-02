#!/bin/sh

QT_VERSION=v5.5.1

set -e

. ./scripts/libs.sh

if [ -x "$(which apt-get)" ]; then
	./scripts/dep-inst-debian.sh
elif [ -x "$(which pkg)" ]; then
	./scripts/dep-inst-freebsd.sh
else
	warning 'Cannot install dependency automatically!'
fi

if [ "$(uname)" = "FreeBSD" ]; then
	NPROC=`sysctl hw.ncpu | awk '{print $2}'`
else
	NPROC=`nproc`
fi

# clone qtbase
[ -f qtbase/.git/config ] || (
	info "Cloning Qt Base..." ""

	git clone https://github.com/qtproject/qtbase.git
)

[ "$(GIT_DIR="$PWD/qtbase/.git" git describe)" = "$QT_VERSION" ] || (
	info "Checkout $QT_VERSION..." ""

	cd qtbase
	git checkout $QT_VERSION
)

[ -f qtbase/Makefile ] || (
	info "Configuring Qt Base..." ""

	if [ "$(uname)" = "FreeBSD" ]; then
		cp -r scripts/mkspecs/freebsd-clang qtbase/mkspecs/
		PLATFORM="-platform freebsd-clang"
	fi

	cd qtbase
	./configure -opensource -confirm-license -release -static -no-gui -no-xcb -no-icu -qt-pcre $PLATFORM
)

[ -f qtbase/lib/libQt5Network.a ] || (
	info "Building Qt Base..." ""

	cd qtbase
	make sub-src-qmake_all

	cd src
	make sub-corelib sub-network -j$NPROC
)

[ -d build ] || mkdir build

[ -f build/src/libQtServer.a ] || (
	info "Building QtServer..." ""

	cd build
	cmake ..
	make -j$NPROC
)

[ -d ~/bin ] || mkdir -p ~/bin

if [ -h "$HOME/bin/qsm" ]; then 
	if [ "$(readlink -f $HOME/bin/qsm)" = "$PWD/qsm/bin/qsm" ]; then
		info "qsm is installed"
	else
		warning "Another version of QtServer is installed." \
		        "If you want to change your QtServer, remove \$HOME/bin/qsm" \
		        "and create a new symbol link to $PWD/qsm/bin/qsm"
	fi
else
	ln -s "$PWD/qsm/bin/qsm" ~/bin/qsm
	info "qsm is installed to \$HOME/bin/qsm" \
	     "If \$HOME/bin not in your \$PATH, try to update you .profile"
fi
