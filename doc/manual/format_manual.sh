#!/bin/sh

for i in bigmc.html/*.html
do
	BNAME=`echo $i | cut -f 2 -d "/" | cut -f 1 -d "."`
	echo "$i -> $BNAME.inc"
	sed -n '/<body>/,/<\/body>/p' $i | sed -e '1s/.*<body>//' -e '$s/<\/body>.*//' \
		| sed 's/\([a-zA-Z\-]*\)\.html/index.php?page=\1/g' > ../website/manual/$BNAME.inc
done

