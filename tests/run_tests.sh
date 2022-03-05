#/bin/sh

GREEN='\e[32m'
RED='\e[31m'
BLUE='\e[36m'
END='\e[0m'
BOLD=$(tput smso)
NOB=$(tput rmso)

#Path of tests directory
DIR="all_tests/*"

#Creation of the testerror file and initialization of variables
touch testerror
nbtest=0
nberr=0

touch filerr

#Browse all directories in tests directory
for d in $DIR
do
    code_err=0

    #Add '/*' to the path file"
    FILES="$d/*"
    #Remove what's before '*/' to the path file"
    dir="${d#*/}"

    #Print directory name
    echo -e "$BLUE============================================================$END"
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

        ./../src/tc -X --parse $f >> filerr


        if [ $? -eq $code_err ]
        then
            echo -e -n "$GREEN█$END "
            unique_nbtest=$(($unique_nbtest + 1))
        else
            echo -e -n "$RED█$END "
            unique_nberr=$(($unique_nberr + 1))
            unique_nbtest=$(($unique_nbtest + 1))
        fi
        cat $f | grep error >> testerror
    done

    nbgood=$(($unique_nbtest - $unique_nberr))
    percen=$((($nbgood * 100) / $unique_nbtest))
    echo -n "|  $dir: $nbgood / $unique_nbtest | $percen%"    
    if [ $unique_nberr -eq 0 ]	
    then
        echo -e " -> $GREEN★ $END"
    else
        echo -e " -> $RED★ $END"
    fi
    echo
    cat testerror
    echo -n "" > testerror
    echo
    nbtest=$(($nbtest + $unique_nbtest))
    nberr=$(($nberr + $unique_nberr))
done

echo -e "$BLUE============================================================$END"
echo -e "$BLUE|                     TIGER TESTSUIT                       |$END"
echo -e "$BLUE============================================================$END"

nbgood=$(($nbtest - $nberr))

echo "# TOTAL: $nbtest"
echo -e "#$GREEN PASS:  $nbgood$END"
echo -e "#$RED FAIL:  $nberr$END"

echo
percen=$((($nbgood * 100) / $nbtest))
echo "# You pass: $percen%"
echo

rm testerror
rm filerr

if [ $percen -gt 30 ]
then
    exit 0
else
    exit 1
fi
