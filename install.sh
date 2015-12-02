#!/bin/sh

QT_VERSION=v5.5.1

. ./scripts/libs.sh

if [ -f $(which apt-get) ]; then
	./scripts/dep-inst-debian.sh
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
	make sub-corelib sub-network -j`nproc`
)

[ -d build ] || mkdir build

[ -f build/src/libQtServer.a ] || (
	info "Building QtServer..."
	echo 

	cd build
	cmake ..
	make -j`nproc`
)

[ -d ~/bin ] || mkdir -p ~/bin

if [ -h "$HOME/bin/qsm" ]; then 
	if [ "$(readlink -f $HOME/bin/qsm)" = "$PWD/qsm/bin/qsm" ]; then
		info "qsm is installed"
	else
		warning "Another version of QtServer is installed."
		warning "If you want to change your QtServer, remove \$HOME/bin/qsm"
		warning "and create a new symbol link to $PWD/qsm/bin/qsm"
	fi
else
	ln -s "$PWD/qsm/bin/qsm" ~/bin/qsm
	info "qsm is installed to \$HOME/bin/qsm"
	info "If \$HOME/bin not in your \$PATH, try to update you .profile"
fi
