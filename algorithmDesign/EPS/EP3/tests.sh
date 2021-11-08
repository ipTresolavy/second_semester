#!/bin/bash

if [ ! -d testOutputs ]
then
    mkdir testOutputs
else
    cd testOutputs
    rm *.txt
    cd ..
fi

./ep3_exec > testOutput1.txt << END
7
3 5 2 1 7 6 4
END
mv testOutput*.txt testOutputs

./ep3_exec > testOutput3.txt << END
10
6 7 5 2 3 9 5 8 10 4
END
mv testOutput*.txt testOutputs

./ep3_exec > testOutput5.txt << END
6
3 4 1 6 5 2
END
mv testOutput*.txt testOutputs

./ep3_exec > testOutput6.txt << END
7
1 7 2 6 3 4 5
END
mv testOutput*.txt testOutputs

# Too slow
# ./ep3_exec > testOutput4.txt << END
# 14
# 8  3  5  6  -1  0  -2  2  -1  7  2  1  4  7
# END
# mv testOutput*.txt testOutputs

# For the burnt pancake problem
# 8
# -8 7 5 -2 3 6 -1 -4
