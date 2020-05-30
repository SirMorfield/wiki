#!/bin/bash
runner () {
	clear
	rm c;
	status=$?

	gcc $1 -o c;

	res1=$(date +%s.%N)

	cmd="./c"
	$cmd
	status=$?

	res2=$(date +%s.%N)
	dt=$(echo "$res2 - $res1" | bc)
	dd=$(echo "$dt/86400" | bc)
	dt2=$(echo "$dt-86400*$dd" | bc)
	dh=$(echo "$dt2/3600" | bc)
	dt3=$(echo "$dt2-3600*$dh" | bc)
	dm=$(echo "$dt3/60" | bc)
	ds=$(echo "$dt3-60*$dm" | bc)
	LC_NUMERIC=C printf "\nRuntime: %d:%02d:%02d:%02.3f\n" $dd $dh $dm $ds
	echo "exit $status"
}
runner $1

inotifywait -q -m -e close_write $1 |
while read -r filename event; do
	runner $1
done

# gcc main.c -o main.c.compiled; ./main.c.compiled
# gcc temp.c -o temp.c.compiled; ./temp.c.compiled
