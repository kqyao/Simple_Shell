# Simple_Shell
write a simple linux shell in C

compile using -std=gnu11

The tasks completed (marked in [x]):  
[x] 1. Write a working read/parse/execute loop and an exit command;  
[x] 2. Clean exit, no memory leaks in any circumstance;  
[x] 3. Handle single commands without arguments (e.g. ls);  
[x] 4. Support commands with arguments (e.g. aptget update or pkgin update);  
5. File I/O redirection:  
[x] 5.1. Output redirection by overwriting a file (e.g. echo 123 > 1.txt);  
[x] 5.2. Output redirection by appending to a file (e.g. echo 465 >> 1.txt);  
[x] 5.3. Input redirection (e.g. cat < 1.txt);  
[x] 5.4. Combine 5.1 and 5.2 with 5.3;  
[x] 6. Support for bash style redirection syntax (e.g. cat < 1.txt 2.txt > 3.txt 4.txt);  
7. Pipes:  
[x] 7.1. Basic pipe support (e.g. echo 123 | grep 1);  
[x] 7.2. Run all stages of piped process in parallel. (e.g. yes ve482 | grep 482);  
[x] 7.3. Extend 7.2 to support requirements 5 and 6 (e.g. cat < 1.txt 2.txt | grep 1 > 3.txt);  
[x] 7.4. Extend 7.3 to support arbitrarily deep \cascade pipes" (e.g. echo 123 | grep 1 | grep | grep 1)  
[x] 8. Support CTRLD (similar to bash, when there is no/an unfinished command);   
9. Internal commands:  
[x] 9.1. Implement pwd as a builtin command;  
[x] 9.2. Allow changing working directory using cd;  
[x] 9.3. Allow pwd to be piped or redirected as specified in requirement 5;  
10. Support CTRLC:  
[x] 10.1. Properly handle CTRLC in the case of requirement 5;  
[x] 10.2. Extend 10.1 to support subtasks 7.1 to 7.3;  
[x] 10.3. Extend 10.2 to support requirement 8, especially on an incomplete input;  
[x] 10.4. Extend 10.3 to support requirement 7;  
11. Wait for the command to be completed when encountering >, <, or |:  
[x] 12.1. Support requirements 4 and 5 together with subtasks 7.1 to 7.3;  
