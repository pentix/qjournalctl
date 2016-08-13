# This is an example PKGBUILD file. Use this as a start to creating your own,
# and remove these comments. For more information, see 'man PKGBUILD'.
# NOTE: Please fill out the license field for your package! If it is unknown,
# then please put 'unknown'.

# Maintainer: Patrick Eigensatz <patrick.eigensatz@gmail.com>
pkgname=qjournalctl-git
pkgver=0.2
pkgrel=1
epoch=
pkgdesc="Qt-based graphical user interface for systemd's journalctl command"
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
source=('http://www.blokx5.ch:4566/pentix/qjournalctl/archive/master.tar.gz')
noextract=()
md5sums=(SKIP)
#sha256sums=(SKIP)
validpgpkeys=()

#prepare() {
#}

build() {
	cd qjournalctl
	./autogen.sh
	make -j9
}

#check() {
#}

package() {
	mkdir -p $pkgdir/usr/bin/
	cp qjournalctl/qjournalctl $pkgdir/usr/bin/
}
