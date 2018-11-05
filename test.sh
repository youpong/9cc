#!/bin/bash

try() {
    expected="$1"
    input="$2"
    msg="$3"

    ./9cc "$input" > tmp.s
    if [ $? != 0 ]; then
	echo "$3: cann't compile"
	exit 1
    fi
    
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" != "$expected" ]; then
	echo "$3: $input expected, but got $actual"
	exit 1
    fi
}

try 42 42   10
try 0 0     20
try 255 255 30

try  3 '1+2'    31
try  4 '9-5'    32
try 21 '5+20-4' 40
try 41 ' 12 + 34 - 5 ' 50

#try 0 256 60

echo OK
