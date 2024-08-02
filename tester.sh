#!/bin/bash

# Colors
RED='\033[1;31m'
GRE='\033[1;32m'
ORA='\033[1;33m'
BLU='\033[1;34m'
RES='\033[0m'

DIR=".tests"
LOG="log"

nb_tests=0
success=0

VALGRIND="$1"

test() {
	echo -n "$1: "
	if [ "$1" -lt 10 ]; then
		echo -n " "
	fi

	out_1="$DIR/$LOG/out_"$1"_1"
	out_2="$DIR/$LOG/out_"$1"_2"

	err_1="$DIR/$LOG/err_"$1"_1"
	err_2="$DIR/$LOG/err_"$1"_2"

	diff_out_file="$DIR/$LOG/diff_"$1"_out"
	diff_err_file="$DIR/$LOG/diff_"$1"_err"

	if [ "$VALGRIND" = "--no-valgrind" ]; then
		./ft_ls $2 > $out_1 2>$err_1
		ls $2 > $out_2 2>$err_2
	else
		valgrind -q --leak-check=full ./ft_ls $2 > $out_1 2>$err_1
		valgrind -q --leak-check=full ls $2 > $out_2 2>$err_2
	fi

	sed 's/^ft_ls:/ls:/' -i $err_1

	diff_out=$(diff $out_1 $out_2)
	diff_err=$(diff $err_1 $err_2)

	if [ "$3" = "fail" ]; then
		diff_out="forced fail"
	fi

	if [ -n "$diff_out" ]; then
		len_diff="KO"
		if [ "$3" = "check-diff" ]; then
			len_1=$(wc -m < $out_1)
			len_2=$(wc -m < $out_2)

			len_line_1=$(head -n 1 "$out_1" | wc -m)
			len_line_2=$(head -n 1 "$out_2" | wc -m)

			len_diff=$((len_2 - len_1))
			len_diff_line=$((len_line_2 - len_line_1))

			if [ $len_diff = $len_diff_line ]; then
				len_diff="OK"
			fi
		fi
		if [ "$len_diff" = "OK" ]; then
			echo -e $BLU"OO"$RES
			echo $diff_out > $diff_out_file
			echo $diff_err > $diff_err_file
			((success++))
		else
			echo -e $RED"KO"$RES
			echo $diff_out > $diff_out_file
			echo $diff_err > $diff_err_file
		fi
	elif [ -n "$diff_err" ]; then
		echo -e $ORA"ER"$RES
		echo $diff_err > $diff_err_file
	else
		echo -e $GRE"OK"$RES
		((success++))
	fi
	((nb_tests++))
}

mkdir -p $DIR/$LOG

# Tests

#1 - Basic
test "1" "-R"

# Long Display
# 2 - -lR
test "2" "-lR"

# 3 - -l /dev
test "3" "-l /dev"

# 4 - -l on empty dir
mkdir -p $DIR/test
test "4" "-l $DIR/test"
rm -rf $DIR/test

# Symlinks
# 5 - Recursion
mkdir -p $DIR/test
ln -sf $DIR/test/a $DIR/test/a
test "5" "-l $DIR/test"
rm -rf $DIR/test

# 6 - Broken Symlink
mkdir -p $DIR/test
ln -s $DIR/test/extinct_dinosaur $DIR/test/link
test "6" "-l $DIR/test"
rm -rf $DIR/test

# 7 - Relative path
mkdir -p $DIR/target
mkdir -p $DIR/test
ln -s ../target $DIR/test/link
test "7" "-l $DIR/test"
rm -rf $DIR/test
rm -rf $DIR/target

# Permissions Errors
# 8 - No Permissions
mkdir -p $DIR/test
touch $DIR/test/a
chmod 000 $DIR/test
test "8" "-R $DIR/test"
chmod 777 $DIR/test
rm -rf $DIR/test

# 9 - Read-only
mkdir -p $DIR/test
touch $DIR/test/a
chmod 444 $DIR/test
test "9" "-R $DIR/test"
chmod 777 $DIR/test
rm -rf $DIR/test

# 10 - No Permissions
mkdir -p $DIR/test
mkdir -p $DIR/test/1
mkdir -p $DIR/test/2
mkdir -p $DIR/test/3
chmod 000 $DIR/test/2
test "10" "-R $DIR/test"
chmod 777 $DIR/test/2
rm -rf $DIR/test

# Special Permissions
# 11 - Setuid with executable rights
mkdir -p $DIR/test
touch $DIR/test/a
chmod u+x $DIR/test/a
chmod u+s $DIR/test/a
test "11" "-l $DIR/test"
rm -rf $DIR/test

# 12 - Setuid without executable rights
mkdir -p $DIR/test
touch $DIR/test/a
chmod u+s $DIR/test/a
test "12" "-l $DIR/test"
rm -rf $DIR/test

# 13 - Setgid with executable rights
mkdir -p $DIR/test
touch $DIR/test/a
chmod g+x $DIR/test/a
chmod g+s $DIR/test/a
test "13" "-l $DIR/test"
rm -rf $DIR/test

# 14 - Setgid without executable rights
mkdir -p $DIR/test
touch $DIR/test/a
chmod g+s $DIR/test/a
test "14" "-l $DIR/test"
rm -rf $DIR/test

# 15 - Sticky bit with executable rights
mkdir -p $DIR/test
touch $DIR/test/a
chmod o+x $DIR/test/a
chmod o+t $DIR/test/a
test "15" "-l $DIR/test"
rm -rf $DIR/test

# 16 - Sticky bit without executable rights
mkdir -p $DIR/test
touch $DIR/test/a
chmod o+t $DIR/test/a
test "16" "-l $DIR/test"
rm -rf "$DIR/test"

# Args
mkdir -p "$DIR/test"
mkdir -p "$DIR/test/test2"
touch "$DIR/test/file1"
touch "$DIR/test/test2/file2"

# 17 - Options after args
test "17" "$DIR/test/file1 $DIR/test/test2 -l" "check-diff"

# 18 - Options before args
test "18" "-l $DIR/test/file1 $DIR/test/test2" "check-diff"

# 19 - Options after AND before args
test "19" "-R $DIR/test/file1 $DIR/test/test2 -l" "check-diff"

# 20 - Options on the same arg
test "20" "-lRra $DIR/test/file1 $DIR/test/test2" "check-diff"

# 21 - Verbose options
test "21" "--recursive $DIR/test/file1 $DIR/test/test2 --reverse" "check-diff"

# 22 - Using '--'
test "22" "--recursive -- $DIR/test/file1 $DIR/test/test2 --reverse -l"

rm -rf "$DIR/test"

# Basic flags
# 23 - All the flags simultaneously except -t
# Because when there is two files with same last update time,
# the core utils ls sort them very weirldy (using an mp sort ??).
test "23" "* -aRrl"

# 24 - -t
mkdir -p "$DIR/test"
touch "$DIR/test/file1"
touch "$DIR/test/file2"
touch "$DIR/test/file3"
touch "$DIR/test/file4"
touch "$DIR/test/file5"

touch -t "202408011200" "$DIR/test/file1"
touch -t "202408011201" "$DIR/test/file2"
touch -t "202408011202" "$DIR/test/file3"
touch -t "202408011203" "$DIR/test/file4"
touch -t "202408011204" "$DIR/test/file5"

test "24" "$DIR/test -t"
rm -rf "$DIR/test"

# Announce results
if [ $success = $nb_tests ]; then
	echo -ne $GRE
elif [ $success -gt $(($nb_tests * 75 / 100)) ]; then
	echo -ne $ORA
else
	echo -ne $RED
fi
echo -n $success"/"$nb_tests
if [ $success = $nb_tests ]; then
	echo -n " - All Good !"
fi
echo -e $RES


# OK - Test Passed
# KO - Test Not Passed
# ER - Test Passed, but errors differs
# OO - See below

# About tests 17 -> 21 :
# Theses tests should NOT be OK in a good reimplementation of ls :
# The fact is that the original ls has a tiny error when you give
# it a file with a size inferior to 10 : It creates a pointless
# space. If you don't have it, I think it's better, because
# it does not serve any padding interest.
# To mark it, I put "OO" instead of "OK" when there is a single
# character located on the first line that differs between your
# output and the ls output.
