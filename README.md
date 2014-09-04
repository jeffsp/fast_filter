# Fast Filter Operations

Efficiently filter an image over KxK blocks.

## Description

This algorithm was developed in order to quickly compute RMS contrast of KxK blocks at each image pixel.

Rather than iterating over KxK pixels at each image location \[i,j\], for a total of K^2\*N operations, the algorithm
uses some temporary storage and makes a few passes over the image.  This results in speedups that in some cases are
factors of ten or greater.

## Operations

This code supports:

* 2D convolution using a square wave filter
* Computing variance over KxK blocks
* Computing RMS contrast over KxK blocks

## Example

    jsp@uni ~/Projects/fast_filter $ make run
    running test1
    ./test1
    running test2
    ./test2
    running test3
    ./test3
    running test4
    ./test4
    running test5
    ./test5
    t1: 3277ms
    t2: 166ms
    19.741X speedup

## See also

http://elynxsdk.free.fr/ext-docs/Blur/Fast_box_blur.pdf
