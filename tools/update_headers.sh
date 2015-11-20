#!/bin/sh

cd ../include

rm Q*

ls *.h | sort | sed 's/.*/#include "&"/' > QtServer
echo "QtServer is created"

ls *.h | while read line; do
	cat "$line" | grep class | 
				  grep "Q_SERVER_EXPORT" | 
				  sed 's/.*Q_SERVER_EXPORT\s*\([a-zA-Z0-9_]*\).*/\1/' | 
				  grep -v "_" |
				  while read c; do
		echo "$c is created"
		echo "#include \"$line\"" > $c
	done
	cat "$line" | grep class | 
				  grep -v "Q_SERVER_EXPORT" |
				  grep -v ";" | 
				  sed 's/.*class\s*\([a-zA-Z0-9_]*\).*/Warning: class \1 not exported in '$line'/'
done

