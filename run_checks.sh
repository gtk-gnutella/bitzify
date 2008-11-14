#! /bin/sh
# THIS FILE IS PUBLIC DOMAIN. NO WARRANTY OF ANY KIND. NO RIGHTS RESERVED.

missing=
for prog in mplayer curl file bitter bc ogginfo pkg-config
do
	command -v "$prog" >/dev/null || {
		missing="$missing $prog"
		echo Cannot find "$prog"
	}
done

case "$missing" in
'')	exit 0
	;;
*)
	echo 'Some required programs are missing:'
	echo "$missing"
	exit 1
	;;
esac
