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


## Timing

```
$ php generate-test-values.php > testvals
$ time ./main $(head -n3 testvals | awk -F'.' '{print $2}')
[snip]
some output
$ test result
```

## Dude, wtf, your code is slow and ugly!

I know. I'm in no way a C programmer. There's probably also better
(i.e., faster) ways to do this.
