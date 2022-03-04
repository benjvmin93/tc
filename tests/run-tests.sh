#/bin/sh

GREEN="\e[42m"
RED="\e[41m"
END="\e[0m"
BLINK="\e[5m"

LINE="\e[1;4m"

DIR="all_tests/*"

#Browse all directories in tests directory
for d in $DIR
do
	FILES="$d/*"
	dir="${d#*/}"

	echo $dir | tr '[:lower:]' '[:upper:]'

	#Browse all files
	for f in $FILES
	do
		fi="${f#tests/*/}"
		echo Test Case $fi 
		cat $f | grep error

		#TODO run test
	done
	echo
done
