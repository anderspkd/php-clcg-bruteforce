Program for bruteforcing the internal state of PHPs [CLCG](https://en.wikipedia.org/wiki/Combined_Linear_Congruential_Generator)
PRG using the values exposed by `uniqid('', true)`, i.e., the
`more_entropy` option that can be set when calling the `uniqid`[1](http://php.net/manual/en/function.uniqid.php)
function.


## Usage

To compile run

`$ gcc -fopenmp main.c -o main`

To run do

`$ main [x1] [x2] [x3]`

where `x1`, `x2` and `x3` are three consequtive outputs from PHPs CLCG
(e.g., from `more_entropy` part of `uniqid`).


## Why?

After
watching
[this](https://www.youtube.com/watch?v=WiGif0D3fIc&feature=youtu.be) I
wondered if `uniqid` was also insecure even when the `more_entropy`
option was set. Turns out that it is (in some circumstances).


## How it works

Suppose it is possible to observe three consequtive calls to
`uniqid('', true)`. These will have the form

```
T1 --- T13X1.X2 --- X8
```

Where `X=X1.X2 --- X8` corresponds to `10*CLCG()` truncated to 8 decimal
places. It is these values that we will use.

The following is then done:

1. Write `X = S1 - S2 mod m` and make a guess at `S1`, `S2`.
2. Advance the CLCG one step and compare with `X2`.
3. If it matches, advance yet another step and compare with `X3`
4. If this matches, output `S1`, `S2`.


Note that this will probably work in a more general setting, so long
as we know the distance between each call to `uniqid`.


## Timing/testing

```
$ lscpu | grep "Arch\|Model\ \|^CPU("
Architecture:          x86_64
CPU(s):                4
Model name:            Intel(R) Core(TM) i5-3230M CPU @ 2.60GHz
```

Timing execution. See `generate-test-values.php` for how the test
values was generated (no suprises there), and `process.py` for
extracting information from the output of a call to `uniqid`.

```
$ php generate-test-values.php > testvals
$ time ./main $(./process.py $(cat testvals) | awk -F'-' '{print $2}' | tr -d ' ' | head -n3)
[snip]

real	39m29,076s
user	157m18,952s
sys	    0m0,217s
$ cat output
:1008469547:329414316:0
```

Compare

```
$ pr -m -t <(./process.py $(cat testvals) | awk -F':|-' '{print $5}' | tr -d ' ') <(./clcg.py $(cat output | awk -F: '{print $2 " " $3}'))
3.16209742			    3.16209746
8.14131920			    8.14131930
8.87498792			    8.87498803
9.39615853			    9.39615864
4.77255658			    4.77255664
3.13084848			    3.13084852
8.32636214			    8.32636224
5.15919248			    5.15919254
7.60551973			    7.60551983
5.83177053			    5.83177061
```

First column is original testvalues, second is the CLCG values
recomputed from the computed result. There seems to consistent +[0 -
10] deviance. No idea where that comes from...


## Dude, wtf, your code is slow and ugly!

I know. I'm in no way a C programmer, so any oddities or
in-efficiancies (due to the programming) is a result of that.


Also, the approach taken is very naive. The first two inputs will
satisfy

```
X1 = S1 - S2 mod m
X2 = (a*S1 mod m1) - (b*S2 mod m2) mod m
```

Where `a`, `b`, `m1`, `m2` and `m` are known. That is, it is almost
two equations in two unknowns. However, all the modular stuff kinda
ruins it :-(
