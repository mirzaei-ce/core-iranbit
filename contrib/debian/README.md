
Debian
====================
This directory contains files used to package iranbitd/iranbit-qt
for Debian-based Linux systems. If you compile iranbitd/iranbit-qt yourself, there are some useful files here.

## iranbit: URI support ##


iranbit-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install iranbit-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your iranbit-qt binary to `/usr/bin`
and the `../../share/pixmaps/iranbit128.png` to `/usr/share/pixmaps`

iranbit-qt.protocol (KDE)

