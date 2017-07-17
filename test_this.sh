#!/bin/bash

if [ -f foo ]
then
    echo this script requies use of a file foo
    exit 1
fi

if [ -f bar ]
then
    echo this script requies use of a file bar
    exit 1
fi

for X in *
do
    if [ -f "$X" ]
    then
        echo checking $X
        filesize=$(stat -c '%s' $X)
        printf "uncompressed size:\t\t%d\n" $filesize
        ./sample -k4 -c -i $X -o foo
        ./sample -k4 -d -i foo -o bar
        diff $X bar
        filesize=$(stat -c '%s' foo)
        printf "compressed with -k4 size:\t%d\n" $filesize
        rm foo
        rm bar
        ./sample -k5 -c -i $X -o foo
        ./sample -k5 -d -i foo -o bar
        diff $X bar
        filesize=$(stat -c '%s' foo)
        printf "compressed with -k5 size:\t%d\n\n" $filesize
        rm foo
        rm bar
    fi
done

exit 0
