

# Chainseq Development Notes

## Build requirements

```sudo apt-get install libboost-program-options-dev```


## How to build

```make```


## TODO

* Compare efficiency calculations with 3dDeconvolve
You can automatically generate the 3dDeconvolve code to facilitate this.

* Make sure I destroy whatever objects I create
Free memory!

* Make sure all precision is in doubles, never casted back to floats

* Make trials no longer TR-locked and add "granularity of time"

* Allow nulltrs to jump a whole block of trials (i.e. you can move across multiple rows of zeroes).
This may allow merging of mini blocks into bigger blocks.




### Implementing multiple events
This is the same as for a single event, except that we now have two steps:
1. first decide the order between events, by putting all in a flat list and shuffling it.
2. then decide the amount of "null TRs" between events as before.
Then during "moves" you can not only move around null TRs but you can also flip two events if they don't have any intervening null TRs.


## Technical details


### Comparing against 3dDeconvolve

```3dDeconvolve -nodata 50 1.6 -polort 3 -num_stimts 1 -stim_times 1 '1D: 19.2 40 57.6' GAM```



### Linear Algebra

[Boost uBLAS](http://www.boost.org/doc/libs/1_60_0/libs/numeric/ublas/doc/index.html)

otherwise:

Armadillo.



### Data structures

[List of structs](http://stackoverflow.com/questions/7971995/instantiating-a-list-of-structs-in-c)


### Doing Matrix Algebra


GSL sounds like it's the tool for matrix algebra
[GSL docs](http://www.gnu.org/software/gsl/manual/html_node/Matrices.html#Matrices)


Matrix inversion

[example](http://www.macapp.net/pmwiki/pmwiki.php?n=Main.InvertMatrix)



### Convolution

We also need to do convolution. It sounds like we can do 
[convolution example](https://github.com/jeremyfix/FFTConvolution/tree/master/Convolution/src)

Or maybe I could just do "copy-paste"-style? At least for the GAM function this would work.


### Interpolation

Yes, it seems GSL can do this.
[example program](https://www.gnu.org/software/gsl/manual/html_node/1D-Interpolation-Example-programs.html#g_t1D-Interpolation-Example-programs)


