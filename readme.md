Program for bruteforcing the internal state of PHPs [CLCG](wiki link)
PRG using the values exposed by `uniqid('', true)`, i.e., the
`more_entropy` option that can be set when calling the `uniqid`[1](php link)
function.


## Usage

To compile run

`$ gcc -fopenmp main.c -o main`

To run do

`$ main [x1] [x2] [x3]`

where `x1`, `x2` and `x3` are three consequtive outputs from PHPs CLCG
(e.g., from `more_entropy` part of `uniqid`).


## Why?

After watching [this](youtube link) I wondered if `uniqid` was also
insecure even when the `more_entropy` option was set. Turns out that
it is (in some circumstances).


## (some circumstances)

The program works under the assumption that we are able to observe
three consequtive calls to `uniqid('', true)`.


## How it works

It bruteforces the internal state, that's how :-)

It works because a CLCG behaves kinda like a linear function in two
unknowns. Therefore, if we can get three consequtive points, we can
recreate the "line" used by the generator.

Although I havn't tested it, this program should be modifiable to work
in a more general situation. That is, the distance between points are
not so important. What's important is that is it known.


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

I know. I'm in no way a C programmer. There's probably also better
(i.e., faster) ways to do this.
