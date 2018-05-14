#!/bin/bash

SUBDIRS=$(find . -maxdepth 1 -not -path . -type d)
opt=""

function SetForceMake ()
{
    if ! make -q -f ../Modules.mk headers ; then
        opt="-B"
    fi
}

for d in $SUBDIRS; do
    cd $d
    SetForceMake
    make -f ../Modules.mk headers
    cd ../
done

failed=false

for d in $SUBDIRS; do
    cd $d

    make $opt -f ../Modules.mk objs
    if [ $? != 0 ] ; then
        failed=true
        break
    fi

    cd ../
done

if [ $failed == false ] ; then
    make
fi
