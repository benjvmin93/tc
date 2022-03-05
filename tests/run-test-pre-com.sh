#!/bin/sh

#Initialization of colors
GREEN="\e[42m"
RED="\e[41m"
END="\e[0m"
BLINK="\e[5m"
LINE="\e[1;4m"

#Path of tests directory
DIR="../../tests/all_tests/*"

#Creation of the testerror file and initialization of variables
nbtest=0
nberr=0

#Browse all directories in tests directory
for d in $DIR
do
	code_err=0

	#Add '/*' to the path file"
	FILES="$d/*"
	#Remove what's before '*/' to the path file"
	dir="${d#*/*/*/*/}"

	#Print directory name
	echo "             $dir" | tr '[:lower:]' '[:upper:]'
	
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
	
	unique_nbtest=0
	unique_nberr=0

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
			unique_nbtest=$(($unique_nbtest + 1))
		else
			echo -n "$RED░$END "
			unique_nberr=$(($unique_nberr + 1))
			unique_nbtest=$(($unique_nbtest + 1))
		fi
	done

	nbgood=$(($unique_nbtest - $unique_nberr))
	percen=$((($nbgood * 100) / $unique_nbtest))
	echo -n "  $dir: $nbgood / $unique_nbtest | $percen%"    
	if [ $unique_nberr -eq 0 ]	
	then
		echo " -> $GREEN✩ $END"
	else
		echo " -> $RED✩ $END"
	fi

	echo
	nbtest=$(($nbtest + $unique_nbtest))
	nberr=$(($nberr + $unique_nberr))
done

echo "------------------------------------------------------------"
echo "▒                     TIGER TESTSUIT                       ▒"
echo "------------------------------------------------------------"
percen=$((($nberr * 100) / $nbtest))
echo "  Error : $nberr / $nbtest | $percen%"
echo

nbgood=$(($nbtest - $nberr))
percen=$((($nbgood * 100) / $nbtest))
echo "You pass: $nbgood / $nbtest | $percen%"
echo

if [ $percen -gt 30 ]
then
	exit 0
else
	exit 1
fi
