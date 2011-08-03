#!/bin/sh

TARG="gdpe@expdev.net:/home/gdpe/public_html/bigraph/bigmc"

git pull origin master
./autogen.sh
./configure

VERSION="20110803"

make pdf

if [ $? -ne 0 ]
then
	echo "make pdf failed, aborting."
	exit 1
fi

make html

if [ $? -ne 0 ]
then
	echo "make html failed, aborting."
        exit 1
fi

make dist
if [ $? -ne 0 ]
then
	echo "make dist failed, aborting."
        exit 1
fi

cd doc/manual

./format_manual.sh

cd ../../

scp doc/website/* $TARG/
scp doc/website/manual/* $TARG/manual/
scp doc/manual/bigmc.pdf $TARG/bigmc.pdf
scp bigmc-$VERSION.tar.gz $TARG/release/
scp bin/BigMC-$VERSION.pkg $TARG/release/

