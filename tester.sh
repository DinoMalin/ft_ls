# Colors
RED='\033[1;31m'
GRE='\033[1;32m'
ORA='\033[1;33m'
RES='\033[0m'

DIR=".tests"
LOG="log"

test() {
	echo -n "$1: "

	out_1="$DIR/$LOG/out_"$1"_1"
	out_2="$DIR/$LOG/out_"$1"_2"

	err_1="$DIR/$LOG/err_"$1"_1"
	err_2="$DIR/$LOG/err_"$1"_2"

	diff_out_file="$DIR/$LOG/diff_"$1"_out"
	diff_err_file="$DIR/$LOG/diff_"$1"_err"

	valgrind -q --leak-check=full ./ft_ls $2 > $out_1 2>$err_1
	valgrind -q --leak-check=full ls $2 > $out_2 2>$err_2

	diff_out=$(diff $out_1 $out_2)
	diff_err=$(diff $err_1 $err_2)

	if [ -n "$diff_out" ]; then
		echo $RED"KO"$RES
		echo $diff_out > $diff_out_file
		echo $diff_err > $diff_err_file
	elif [ -n "$diff_err" ]; then
		echo $ORA"ER"$RES
		echo $diff_err > $diff_err_file
	else
		echo $GRE"OK"$RES
	fi
}

mkdir -p $DIR/$LOG

# Tests

#1 - Basic
test "1" "-R lib"

# Long Display
# 2 - -lR
test "2" "-lR"

# 3 - -l /dev
test "3" "-l /dev"

# 4 - -l on empty dir
mkdir -p $DIR/test
test "4" "-l $DIR/test"
rm -rf $DIR/test

#Symlinks
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

#Perms
# 8 - 0 Perms
mkdir -p $DIR/test
touch $DIR/test/a
chmod 000 $DIR/test
test "8" "-R $DIR/test"
chmod 777 $DIR/test
rm -rf $DIR/test

# 9 - Stat error
mkdir -p $DIR/test
touch $DIR/test/a
chmod 444 $DIR/test
test "9" "-R $DIR/test"
chmod 777 $DIR/test
rm -rf $DIR/test

