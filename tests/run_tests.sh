#/bin/sh

GREEN='\e[32m'
RED='\e[31m'
BLUE='\e[36m'
END='\e[0m'
BOLD=$(tput smso)
NOB=$(tput rmso)

#Path of tests directory
DIR="all_tests/*"

array=()

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
    echo -n "             $dir |" | tr '[:lower:]' '[:upper:]'

    case "$dir" in
        "scan")
            code_err=2
            ;;
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

    echo " Error code: $code_err"
    echo -e "$BLUE============================================================$END"

    unique_nbtest=0
    unique_nberr=0

    #Browse all files
    for f in $FILES
    do
        case "$dir" in
            "scan")
                code_err=2
                ;;
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

        ./../src/tc -X --parse $f >> filerr 2>&1


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
    array+=("$unique_nberr")

    nbgood=$(($unique_nbtest - $unique_nberr))
    percen=$((($nbgood * 100) / $unique_nbtest))
    echo -n "|  $dir: $nbgood / $unique_nbtest | $percen%"    
    if [ $unique_nberr -eq 0 ]	
    then
        echo -e " -> $GREEN╰(★‿★)╯ $END"
    else
        echo -e " -> $RED（♯▼皿▼） $END"
        echo
    fi
    cat testerror

    echo -n "" > testerror
    echo
    nbtest=$(($nbtest + $unique_nbtest))
    nberr=$(($nberr + $unique_nberr))
done

echo -e "$BLUE============================================================$END"
echo -e "$BLUE|                     TIGER TESTSUIT                       |$END"
echo -e "$BLUE============================================================$END"

echo

nbgood=$(($nbtest - $nberr))

echo "# TOTAL: $nbtest"
echo -e "#$GREEN PASS:  $nbgood$END"
echo -e "#$RED FAIL:  $nberr$END"

echo
percen=$((($nbgood * 100) / $nbtest))
echo -n "# You pass: "

#Loading bar
for val in ${array[@]}
do
    if [ $val -eq 0 ]
    then
            echo -e -n "$GREEN█$END "
    else
            echo -e -n "$RED█$END "
    fi
done
echo "$percen%"
echo
rm testerror
rm filerr

if [ $percen -gt 30 ]
then
    exit 0
else
    exit 1
fi
