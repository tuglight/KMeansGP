# KMeansGP
Using Genetic Programming to Reduce Features in K-Means Clustering
for a Problem Class

This code is for a fun project that I am doing in my Clustering class.  The
idea is that given a set of points for a problem class, a parse tree can be
trained to reduce all the features into one feature.  If you were to need to
run k-means clustering on another dataset of the same problem class, you could
use the parse tree to reduce features of those points into a single feature.  
The hope was that all the training time would lead you to a faster solve time
of k-means, while not sacrificing the purity of the solution.  This is a
testbed for the idea and practice for my C++ skills.  You are welcome to use
the code however you want and the idea is open to the public.  If you have any
questions regarding this project, email me at jmt275@mst.edu.  Below I will
explain how to get the code up and running on your machine.

The program requires C++11 to compile.  If you are on Windows and you have
GCC installed in your Path, then you can run build_gcc.bat from the Command
Line.  If you are on Windows and want to compile the program with Visual C++,
you must have the Developer Command Prompt for Visual Studio.  To compile with
Visual C++, open the Developer Command Prompt and run build_windows.bat.  To
open the Visual C++ executable in Visual Studio without making a new project,
run the following:
devenv kmeansgp.exe.  

If you're on Linux and you want to run this code, then you still need GCC.  I
have not tested compilation on Linux, but the following should work for
compiling the code from your terminal:
g++ -O3 -std=c++11 -o kmeansgp *.cpp

** // Purpose of this is to remove the italics caused by the previous line.

Before you run the program, you will need a settings file.  The settings folder
has some examples that should make it easy to make a new one.  The first row of
the dataset csv files contain the column names of the data.  The first column
for the subsequent rows is the classification for the point and the remaining
columns contain the features.  Take note, the feature columns need to be of a
numeric type and there cannot be missing values.  The training and testing
datasets use the same format.  After all this has been done, run the executable
kmeansgp.exe.  Input the settings file and you should then be off to the races!
