# Simple_Shell

write a simple linux shell in C (This is a course project in UM-SJTU Joint Institute VE482)

compile using -std=gnu11

The tasks completed:  
1. Write a working read/parse/execute loop and an exit command;  
2. Clean exit, no memory leaks in any circumstance;  
3. Handle single commands without arguments (e.g. ls);  
4. Support commands with arguments (e.g. aptget update or pkgin update);  
5. File I/O redirection:  
  5.1. Output redirection by overwriting a file (e.g. echo 123 > 1.txt);  
  5.2. Output redirection by appending to a file (e.g. echo 465 >> 1.txt);  
  5.3. Input redirection (e.g. cat < 1.txt);  
  5.4. Combine 5.1 and 5.2 with 5.3;  
6. Support for bash style redirection syntax (e.g. cat < 1.txt 2.txt > 3.txt 4.txt);  
7. Pipes:  
  7.1. Basic pipe support (e.g. echo 123 | grep 1);  
  7.2. Run all stages of piped process in parallel. (e.g. yes ve482 | grep 482);  
  7.3. Extend 7.2 to support requirements 5 and 6 (e.g. cat < 1.txt 2.txt | grep 1 > 3.txt);  
  7.4. Extend 7.3 to support arbitrarily deep \cascade pipes" (e.g. echo 123 | grep 1 | grep | grep 1)  
8. Support CTRLD (similar to bash, when there is no/an unfinished command);   
9. Internal commands:  
  9.1. Implement pwd as a builtin command;  
  9.2. Allow changing working directory using cd;  
  9.3. Allow pwd to be piped or redirected as specified in requirement 5;  
10. Support CTRLC:  
  10.1. Properly handle CTRLC in the case of requirement 5;  
  10.2. Extend 10.1 to support subtasks 7.1 to 7.3;  
  10.3. Extend 10.2 to support requirement 8, especially on an incomplete input;  
  10.4. Extend 10.3 to support requirement 7;  
11. Wait for the command to be completed when encountering >, <, or |:  
12. Support requirements 4 and 5 together with subtasks 7.1 to 7.3;  
