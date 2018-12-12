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
	echo "$msg: $expected expected, but got $actual"
	exit 1
    fi
}

try 42    '42;'      t10
try 0     '0;'       t11
try 255   '255;'     t12

try  3    '1+2;'     t31
try  4    '9-5;'     t32
try 21    '5+20-4;'  t40
try 44    '46-7+5;'  t41
try 41    ' 12  + 34 - 5 ; ' t50

try 18    '3*6;'     t60
try 13    '39/3;'    t70

try 47    '5+6*7;'   t71
try 18    '24 / 4 * 3;' t72

try 40    '(40);'    t80
try  7    '(41+43) / 12;' t90
try 15    '5*(9-6);' t91
try  4    '(3+5)/2;' t92

try 10    'c=10;'    t100
try  8    'a=b=8;'   t101
try 54    'a=b=54;a;' t102
try 55    'a=b=55;b;' t103
try 11    'd=11;e=d;' t104

try 14    'a = 3; b = 5 * 6 - 8; a + b/2;' t110
try  0    '19==22;'    t120
try  1    '19!=22;'    t121
try  1    '16==33-17;' t121
try  1    '23+24 != 25;' t122
try  0    '26+27 != 53;' t123
try 29    'aa=28+1;'     t130

#try 99 '(1' t200 # missing ')'
#try 99 ''   't201 missing expr'
#try 99 '()' 't202 unexpected )'

#try 0 256 60

echo OK
