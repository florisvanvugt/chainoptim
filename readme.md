# Chain-based MRI sequence generator

This is a tool that allows you to search for efficient MRI rapid event-related designs.



## Running chainseq


### Specifying scan parameters

Parameters:

`--ntp N` : number of scan time points

`--tr N` : temporal resolution of acquisition (in seconds)


### Specifying trial constraints

Parameters:

`--ntrials N` : set the number of trials to run

`--trial_duration N` : set the duration of a trial, in number of TRs


### Specifying the analysis method

Parameters:

`--hrf [GAM|BLOCK]` : set the hypothesised haemodynamic response function. Default is `GAM`.

`--npolort N` : set the number of orthogonal polynomials to use as baseline regressors. Default is SOMETHING.

### Fine-tuning optimisation strategy

Parameters:

`--nchain N` : number of chains to run sequentially. Running more chains means you are more likely to get a better chain. The program will by default only return the best chain.

`--move [max|random]` : set the criterion for choosing the move at each iteration in the chain. Setting this to `max` will cause the script to always perform the move that maximises the gain in statistical efficiency of your design. Setting this to `random` means the script will at each iteration randomly choose any of the moves that improve the statistical efficiency, not necessarily the greatest change.

`--maxstep N` : maximum number of items in the chain. That is, after `N` iterations each chain will terminate, regardless of whether further improvements are possible. If you don't supply any value or `0` then there is no limit to the number of moves and the algorithm will continue each chain until no more moves are possible.



### Fine-tuning the script output

Parameters:

`--Xout FILENAME` : save the design matrix to the given file name.



## Examples

```./chainseq --ntp 12 --tr 3.3 --ntrials 2 --trial_duration 2```



## Algorithm details

TODO


