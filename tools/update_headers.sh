#!/bin/sh

cd ../include

rm Q*

ls *.h | sort | sed 's/.*/#include "&"/' > QtServer
echo "QtServer is created"

ls *.h | while read line; do
	# create header named by class name
	cat "$line" | grep class | 
				  grep "Q_SERVER_EXPORT" | 
				  sed 's/.*Q_SERVER_EXPORT\s*\([a-zA-Z0-9_]*\).*/\1/' | 
				  grep -v "_" |
				  while read c; do
		echo "$c is created"
		echo "#include \"$line\"" > $c
	done
	# find out not exported classes
	cat "$line" | grep class | 
				  grep -v "Q_SERVER_EXPORT" |
				  grep -v ";" | 
				  sed 's/.*class\s*\([a-zA-Z0-9_]*\).*/Warning: class \1 not exported in '$line'/'
	# find file missing qtserverglobal.h
	if ! cat "$line" | grep -q "qtserverglobal.h"; then
		if [ "$line" != "qtserverglobal.h" ]; then
			echo "Warning: $line missing qtserverglobal.h"
		fi
	fi
done

