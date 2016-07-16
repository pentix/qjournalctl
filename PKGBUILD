# This is an example PKGBUILD file. Use this as a start to creating your own,
# and remove these comments. For more information, see 'man PKGBUILD'.
# NOTE: Please fill out the license field for your package! If it is unknown,
# then please put 'unknown'.

# Maintainer: Patrick Eigensatz <patrick.eigensatz@gmail.com>
pkgname=journalctlgui
pkgver=0.1
pkgrel=1
epoch=
pkgdesc="GUI for the systemd journalctl command"
arch=('i686' 'x86_64')
url=""
license=('GPL')
groups=()
depends=()
makedepends=('make')
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
source=('http://www.blokx5.ch:4566/pentix/journalctl_test/archive/master.tar.gz')
noextract=()
md5sums=()
sha256sums=('290b18e19e822335678d12ca89e4e67178adcd9173a94d9bcc5ed9ab8b12c960')
validpgpkeys=()

#prepare() {
#}

build() {
	./autogen.sh
	make -j9
}

#check() {
#}

package() {
	mkdir -p $pkgdir/usr/bin/
	cp qt_creator_test $pkgdir/usr/bin/
}
