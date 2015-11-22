#!/bin/sh

cd ../include

rm Q*

echo "#include <QtCore/QtCore>" > QtServer
echo "#include <QtNetwork/QtNetwork>" >> QtServer
ls *.h | sort | sed 's/.*/#include "&"/' >> QtServer
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

	# check macro
	EXP_MACRO=`echo $line | awk '{print toupper($0)}' | sed 's/\./_/g'`
	if ! cat $line | sed '1p' | grep -q $EXP_MACRO; then
		echo "Warning: $line macro mot matched expect $EXP_MACRO"
	fi
	if ! cat $line | sed '2p' | grep -q $EXP_MACRO; then
		echo "Warning: $line macro mot matched expect $EXP_MACRO"
	fi
done

