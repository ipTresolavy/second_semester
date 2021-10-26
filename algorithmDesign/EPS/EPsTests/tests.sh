#!/bin/bash

if [ ! -d testOutputs ]
then
    mkdir testOutputs
else
    cd testOutputs
    rm *.txt
    cd ..
fi

./ep2 > output1.txt << END
6 6
0 1 0 0 1 0
1 1 1 1 -1 1
0 1 0 0 1 0
0 1 0 0 1 0
1 1 1 1 1 1
0 1 0 0 1 0
END

mv output*.txt testOutputs

./ep2 > output2.txt << END
7 7 
0 0 1 1 1 0 0 
0 0 1 1 1 0 0 
1 1 1 1 1 1 1 
1 1 1 -1 1 1 1 
1 1 1 1 1 1 1 
0 0 1 1 1 0 0 
0 0 1 1 1 0 0 
END

mv output*.txt testOutputs

./ep2 > output3.txt << END
6 6
1 1 1 1 1 1
1 1 1 1 1 1
1 1 1 1 1 1
1 1 1 1 1 1
1 1 1 1 -1 1
1 1 1 1 1 1
END

mv output*.txt testOutputs

./ep2 > output4.txt << END
5 6
0 1 1 0 0 0
0 1 1 0 0 0
1 1 1 1 1 -1
0 1 1 0 0 0
0 1 1 0 0 0
END

mv output*.txt testOutputs

./ep2 > output5.txt << END
7 7
0 0 1 1 1 0 0 
0 0 1 1 1 0 0 
1 1 1 1 1 1 1 
1 1 1 1 -1 1 1 
1 1 1 1 1 1 1 
0 0 1 1 1 0 0 
0 0 1 1 1 0 0
END

mv output*.txt testOutputs

./ep2 > output6.txt << END
6 4
1 1 1 1
1 1 1 1
-1 -1 -1 -1
-1 -1 -1 -1
1 1 1 1
1 1 1 1
END

mv output*.txt testOutputs

./ep2 > output7.txt << END
9 9
0 0 0 0 1 0 0 0 0
0 0 0 1 1 1 0 0 0
0 0 1 1 1 1 1 0 0
0 1 1 1 1 1 1 1 0
1 1 1 1 -1 1 1 1 1
0 1 1 1 1 1 1 1 0
0 0 1 1 1 1 1 0 0
0 0 0 1 1 1 0 0 0
0 0 0 0 1 0 0 0 0
END

mv output*.txt testOutputs

./ep2 > output8.txt << END
7 7
0 0 1 1 1 0 0 
0 1 1 1 1 1 0 
1 1 1 -1 1 1 1 
1 1 1 1 1 1 1 
1 1 1 1 1 1 1 
0 1 1 1 1 1 0 
0 0 1 1 1 0 0
END

mv output*.txt testOutputs

./ep2 > output9.txt << END
3 6
-1 1 1 1 1 -1
-1 1 1 1 1 -1
-1 1 1 1 1 -1
END

mv output*.txt testOutputs