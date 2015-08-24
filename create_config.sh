#! /bin/sh

echo "" > config.h

if test `uname | grep "FreeBSD"`; then

	if test "`uname -r | cut -c 1 -`" -le "4"; then

		echo "#define HIDE_CURSOR_WORKAROUND" >> config.h

	fi

fi


