# Maintainer: Connor McDermid <connor.mcdermid@mcvts.org>
pkgname=connector
pkgver=1.0
pkgrel=1
epoch=
pkgdesc="sampletext"
arch=('any')
url="https://github.com/connormcdermid/connector"
license=('GPL')
groups=('vpnmanip')
depends=('openvpn' 'cmake')
makedepends=('cmake' 'make' )
checkdepends=()
optdepends=()
provides=('connector' 'disconnector' 'vpnmanip')
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=("$pkgname-$pkgver.tar.gz")
noextract=()
md5sums=('d9c3ea36b51a8e91c24a778a3ea34446')
validpgpkeys=()

prepare() {
	cd "$pkgname-$pkgver"
}

build() {
	cd "$pkgname-$pkgver"
	cmake .
	make
}

package() {
	cd "$pkgname-$pkgver"
	make DESTDIR="$pkgdir/" install
}
