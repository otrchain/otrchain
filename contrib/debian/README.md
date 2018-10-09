
Debian
====================
This directory contains files used to package OTRchaind/OTRchain-qt
for Debian-based Linux systems. If you compile OTRchaind/OTRchain-qt yourself, there are some useful files here.

## OTRchain: URI support ##


OTRchain-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install OTRchain-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your OTRchainqt binary to `/usr/bin`
and the `../../share/pixmaps/OTRchain128.png` to `/usr/share/pixmaps`

OTRchain-qt.protocol (KDE)

