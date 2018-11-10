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

try 42    '42;'      t10
try 0     '0;'       t11
try 255   '255;'     t12

try  3    '1+2;'     t31
try  4    '9-5;'     t32
try 21    '5+20-4;'  t40
try 41    ' 12 + 34 - 5 ; ' t50

try 18    '3*6;'     t60
try 13    '39/3;'    t70

try 47    '5+6*7;'   t71

try 40    '(40);'    t80
try  7    '(41+43) / 12;' t90
try 15    '5*(9-6);' t91
try  4    '(3+5)/2;' t92

try 10    'c=10;'    t100
try  8    'a=b=8;'   t101
try 11    'd=11;e=d;' t102

# a = 3;
# b = 5 * 6 - 8;
# a + b/2;

#try 99 '(1' t200 # missing ')'
#try 99 ''   't201 missing expr'
#try 99 '()' 't202 unexpected )'

#try 0 256 60

echo OK
