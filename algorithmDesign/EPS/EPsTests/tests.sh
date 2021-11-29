if [ ! -d testOutputs ]
then
    mkdir testOutputs
else
    cd testOutputs
    rm *.txt
    cd ..
fi

./ep4_exec > output1.txt << END
book-war-and-peace.txt
END

./ep4_exec > output2.txt << END
text.txt
END

./ep4_exec > output2.txt << END
text.txt
END

./ep4_exec > output3.txt << END
ep4.c
END

./ep4_exec > output4.txt << END
Makefile
END

mv output*.txt testOutputs
