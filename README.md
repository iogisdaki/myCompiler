<u>NOTE:</u>
There is a problem with the mixal IN and OUT commands. It compiles but it can't find the in/out devices so you can't give it input form outside the program (only with assignment) or print any output.

<u>Requirements:</u>
You need to have flex installed if you're on debian linux write:
```
sudo apt-get update
sudo apt-get install flex
```
Also, you need to have gnu bison intalled
Finally, you need the GNU MIX Development Kit (MDK) to compile and run the mixal code you can intall it with:
```
sudo apt update
sudo apt install mdk
```

<u>About the code:</u>

To make the parser run:
```
bison -d myparser.y
```
To make the flexer run:
```
flex mylexer.l
```
To create the executable run:
```
gcc myparser.tab.c lex.yy.c parseTree.c mixal.c symbolTable.c -lfl
```
Now run the executable a.out with a test, e.g.:
```
./a.out < tests/test10.in
```
There are two resulting files:
 - result.mixal that has the generated mixal code
 - tree.txt that has the parse tree

To run the mixal code run:
```
mixasm result.mixal
```
And then run:
```
mixvm result.mix 
```
and then inside the vm write:
```
run
```
and you can check the registers using 
```
pmem <register number>
```
to interactively check the registers or:
```
mixvm -r result.mix
```
to run it uninteractively.
