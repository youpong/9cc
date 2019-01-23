#!/bin/bash
succeed=1
try() {
    expected="$1"
    input="$2"
    msg="$3"

    ./9cc -e "$input" > tmp.s
    if [ $? != 0 ]; then
	echo "$3: cann't compile"
	succeed=0
	return
    fi
    
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" != "$expected" ]; then
	echo "$msg: $expected expected, but got $actual"
	succeed=0
    fi
}

try 42    '42;'      t10
try 0     '0;'       t11
try 255   '255;'     t12

#try 99 '(1' t200 # missing ')'
#try 99 ''   't201 missing expr'
#try 99 '()' 't202 unexpected )'

#try 0 256 60

if [[ $succeed == 1 ]]; then
    echo OK
else
    echo FAILED
fi

