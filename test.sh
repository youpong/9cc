#!/bin/bash

try() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" != "$expected" ]; then
	echo "$input expected, but got $actual"
	exit 1
    fi
}

try 42 42
try 0 0
try 255 255

try 21 '5+20-4'
try 41 ' 12 + 34 - 5 '

#try 0 256

echo OK
