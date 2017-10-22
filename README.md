# Simple_Shell
write a simple linux shell in C
compile using -std=gnu11

The tasks completed: 
[x] 1. Write a working read/parse/execute loop and an exit command; [5]
[x] 2. Clean exit, no memory leaks in any circumstance; [5]
[x] 3. Handle single commands without arguments (e.g. ls); [5]
[x] 4. Support commands with arguments (e.g. aptget update or pkgin update); [5]
5. File I/O redirection: [5+5+5+2]
[x] 5.1. Output redirection by overwriting a file (e.g. echo 123 > 1.txt);
[x] 5.2. Output redirection by appending to a file (e.g. echo 465 >> 1.txt);
[x] 5.3. Input redirection (e.g. cat < 1.txt);
[x] 5.4. Combine 5.1 and 5.2 with 5.3;
[x] 6. Support for bash style redirection syntax (e.g. cat < 1.txt 2.txt > 3.txt 4.txt); [8]
7. Pipes: [5+5+5+10]
[x] 7.1. Basic pipe support (e.g. echo 123 j grep 1);
[x] 7.2. Run all stages of piped process in parallel. (e.g. yes ve482 j grep 482);
[x] 7.3. Extend 7.2 to support requirements 5 and 6 (e.g. cat < 1.txt 2.txt j grep 1 > 3.txt);
[x] 7.4. Extend 7.3 to support arbitrarily deep \cascade pipes" (e.g. echo 123 j grep 1 j grep 1
j grep 1)
Note: the subprocesses must be reaped in order to be awarded the marks.
[?] 8. Support CTRLD (similar to bash, when there is no/an unfinished command); [5]
9. Internal commands: [5+5+5]
[x] 9.1. Implement pwd as a builtin command;
[x] 9.2. Allow changing working directory using cd;
[x] 9.3. Allow pwd to be piped or redirected as specified in requirement 5;
10. Support CTRLC: [5+3+2+10]
[x] 10.1. Properly handle CTRLC in the case of requirement 5;
[x] 10.2. Extend 10.1 to support subtasks 7.1 to 7.3;
[x] 10.3. Extend 10.2 to support requirement 8, especially on an incomplete input;
[x] 10.4. Extend 10.3 to support requirement 7;
11. Support quotes: [5+2+3+5]
[ ] 11.1. Handle single and double quotes (e.g. echo "de'f' ghi" '123"a"bc' a b c);
[ ] 11.2. Extend 11.1 to support requirement 5 and subtasks 7.1 to 7.3;
[ ] 11.3. Extend 11.2 in the case of incomplete quotes (e.g. Input echo "de, hit enter and input
cd");
[ ] 11.4. Extend 11.3 to support requirements 5 and 7, together with subtask 10.3;
12. Wait for the command to be completed when encountering >, <, or j: [3+2]
[x] 12.1. Support requirements 4 and 5 together with subtasks 7.1 to 7.3;
[ ] 12.2. Extend 12.1 to support requirement 11
