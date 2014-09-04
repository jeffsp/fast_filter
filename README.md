# Fast Filter Operations

Efficiently filter an image over KxK blocks.

## Description

This algorithm was developed in order to quickly compute RMS contrast of KxK blocks at each image pixel.

Rather than iterating over KxK pixels at each image location \[i,j\], for a total of K^2\*N operations, the algorithm
makes only 2 passes over the image for a total of 2\*N operations.

## Other Operations

This code also supports:

* Convolution using a Square Wave Filter
* Computing Variance over KxK blocks

&copy; Jeffrey S. Perry 2007-2014
