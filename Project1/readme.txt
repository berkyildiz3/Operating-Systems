
Threads and message queue are missing. For the thread side I tried to handle by book examples but could not take an output. My thread file just creates an useless single thread. So i did not add that file in the makefile. Makefile will execute findtopk.c and it will produce output named "findtopk.c"

Program has some constraints.
The input file names should be test1.txt/test2.txt etc.
If the number of input files are 3 then input names should be test1.txt, test2.txt and test3.txt. Same constraint is valid for all other input numbers.

There should be files named out1.txt out2.txt. out3.txt out4.txt and out5.txt in the folder. I dont know the reason but without these files I got open failed error.

In the implementation side, the current program is hard coded for 800 inputs per file but in can be changed manually by updating the read buffer sizes. 
Program is not working for more than 800 inputs per file because I could not read the data correctly from the file. For example when the data is 62 689 332
I could read that data as 626, 893 and 32. I spent hours for correct way of reading but could not fix it. I thought even the test data is incorrect it would be better to present a output to show.

Thank you for your attention :)

