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

try 42 42   t10
try 0 0     t20
try 255 255 t30

try  3 '1+2'    t31
try  4 '9-5'    t32
try 21 '5+20-4' t40
try 41 ' 12 + 34 - 5 ' t50

try 18 '3*6' t60
#try 13   '39/3'  70

#try 40   '(40)'  80
#try  7   '(41+43) / 12' 90

#try 0 256 60

echo OK
