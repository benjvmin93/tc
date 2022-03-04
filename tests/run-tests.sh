#!/bin/sh

GREEN="\e[42m"
RED="\e[41m"
END="\e[0m"
BLINK="\e[5m"
LINE="\e[1;4m"

DIR="all_tests/*"

touch testerror
nbtest=0
nberr=0

#Browse all directories in tests directory
for d in $DIR
do
	code_err=0

	FILES="$d/*"
	dir="${d#*/}"

	echo $dir | tr '[:lower:]' '[:upper:]'
	
	case "$dir" in
		"syntax")
			code_err=3
			;;
		"bind")
			code_err=4
			;;
		"type")
			code_err=5
			;;
	esac

	#Browse all files
	for f in $FILES
	do
		case "$dir" in
			"syntax")
				code_err=3
				;;
			"bind")
				code_err=4
				;;
			"type")
				code_err=5
				;;
		esac
		#bash ./../src/tc -X --parse $f

		if [ $? -eq $code_err ]
		then
			echo -n "$GREEN░$END "
			nbtest=$(($nbtest + 1))
		else
			echo -n "$RED░$END "
			nberr=$(($nberr + 1))
			nbtest=$(($nbtest + 1))
		fi
		cat $f | grep error >> testerror
	done
	echo
	cat testerror
	echo -n "" > testerror
	echo
done

echo "------------------------------------------------------------"
echo "|                     TIGER TESTSUIT                       |"
echo "------------------------------------------------------------"

echo "$nberr / $nbtest" 

rm testerror








