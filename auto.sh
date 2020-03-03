#!/bin/bash

pkgname="connector"
pkgver="1.0"

package-arch() {
	echo Packaging for Arch Linux...
	echo Cleaning build directory...
	make clean
	echo Starting...
	cd production
	mkdir $pkgname-$pkgver
	echo Copying...
	cp ../*.1 ../disconnector ../*.cpp ../*.txt $pkgname-$pkgver
	echo Compressing...
	tar czvf $pkgname-$pkgver.tar.gz $pkgname-$pkgver
	echo Including PKGBUILD...
	rm -rf $pkgname-$pkgver
	cd ..
	cp PKGBUILD production
	echo Generating package hashes...
	cd production && updpkgsums
	echo Compressing...
	cd ..
	tar czvf connector-archlinux.tar.gz production/
	echo Done.
}

package-debian() {
	echo Packaging for Debian-based...
	echo Cleaning build directory...
	make clean
	echo Starting...
	cd production/
	cmake ..
	cmake --build .
	cpack
	cd ..

}
