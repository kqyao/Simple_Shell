#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

static char* command_main = NULL; 

int my_gets(char* command)
{
	char tmp; 
	int command_i = 0; 
	while(1) {
		tmp = getchar(); 
		if (tmp == EOF) {
			if (command_i==0)
				return 2; 
			else 
				continue; 
		}
		if (tmp == '\n') {
			command[command_i] = '\0'; 
			break; 
		}
		else {
			command[command_i] = tmp; 
			command_i++; 
		}
	}
	return 1; 
}

int if_empty_command(const char* command) 
// 1 if empty command; 0 if non empty
{
	for(int i=0; i<strlen(command); i++) {
		if(command[i]!=' ' && command[i]!='\t')
			return 0; 
	}
	return 1; 
}

int if_consecutive_pipe(const char* command) 
{
	int pipe_last = 0; 
	for(int i=0; i<strlen(command); i++) {
		if(command[i]!='|')
			pipe_last = 0;
		else {
			if(pipe_last==0) 
				pipe_last=1; 
			else
				return 1; 
		} 
	}
	return 0; 
}

int if_pipe_first(const char* command)
{
	int i=0; 
	while(command[i]==' ' || command[i]=='\t')
		i++; 
	if(command[i]=='|')
		return 1; 
	return 0; 
}

void sigint_handler()
{
	if(command_main){
		command_main[0] = '\0'; 
	}
	printf("\nve482sh $ ");
	fflush(stdout); 
}

void sigint_handler2() {
	printf("\n");
}

int input_separate(char* command, char** file_in)
// Requires: command is dynamically allocated, file_in is NULL. 
// return: 0 if no input redirection; -1 if more than one <; 
//         1 if correct redirection. 
{
	int command_i=0; 
	while(command[command_i]!='<' && command[command_i]!='\0')
		command_i++; 
	if(command[command_i]=='\0')
		return 0;
	int symbol_place = command_i; //find < location
	command_i++; 
	while(command[command_i]!='<' && command[command_i]!='\0')
		command_i++; 
	if(command[command_i]=='<') //test if 2 '<'
		return -1;
	int command_end = command_i; 
	command_i = symbol_place + 1; 
	while(command[command_i]==' ' || command[command_i]=='\t')
		command_i++; 
	int file_begin=command_i; 
	while(command[command_i]!=' ' && command[command_i]!='\t' && command[command_i]!='\0' && command[command_i]!='>')
		command_i++; 
	int file_end = command_i-1; 
	*file_in=(char*) calloc(file_end-file_begin+1+1, sizeof(char));
	for (command_i=0; command_i<file_end-file_begin+1; command_i++) 
		(*file_in)[command_i] = command[file_begin+command_i]; 
	(*file_in)[file_end-file_begin+1]='\0'; 
	command[symbol_place] = ' '; 
	for (command_i=0; command_i<command_end - file_end; command_i++) 
		command[symbol_place+1+command_i] = command[file_end+1+command_i];
	return 1; 
}

int output_separate(char* command, char** file_out)
// Requires: command is dynamically allocated, file_out is NULL. 
// return: 0 if no output redirection; -1 if more than one >; 
//         1 if correct redirection >; 2 if >>;  
{
	int command_i=0; 
	int one_or_two=1; 
	while(command[command_i]!='>' && command[command_i]!='\0')
		command_i++; 
	if(command[command_i]=='\0')
		return 0;
	int symbol_place = command_i; //find > location
	command_i++; 
	if(command[command_i]=='>') {
		command_i++; 
		one_or_two=2; 
	}
	while(command[command_i]!='>' && command[command_i]!='\0')
		command_i++; 
	if(command[command_i]=='>') //test if 2 '>'
		return -1;
	int command_end = command_i; 
	command_i = symbol_place + one_or_two; 
	while(command[command_i]==' ' || command[command_i]=='\t')
		command_i++; 
	int file_begin=command_i; 
	while(command[command_i]!=' ' && command[command_i]!='\t' && command[command_i]!='\0' && command[command_i]!='<')
		command_i++; 
	int file_end = command_i-1; 
	*file_out=(char*) calloc(file_end-file_begin+1+1, sizeof(char));
	for (command_i=0; command_i<file_end-file_begin+1; command_i++) 
		(*file_out)[command_i] = command[file_begin+command_i]; 
	(*file_out)[file_end-file_begin+1]='\0'; 
	command[symbol_place] = ' '; 
	for (command_i=0; command_i<command_end - file_end; command_i++) 
		command[symbol_place+1+command_i] = command[file_end+1+command_i];
	return one_or_two; 
}

int get_arg_num(const char* command, const char* delim) 
// e.g. "cat a b" output 3. 
{
	char* tmp_command = (char*) calloc(strlen(command)+1, sizeof(char));
	strcpy(tmp_command, command); 
	char* result = NULL; 
	result = strtok(tmp_command, delim);
	int i=0; 
	while (result != NULL) {
		i++; 
		result = strtok(NULL, delim); 
	}
	free(tmp_command);
	return i; 
}

void get_arg_length(const char* command, int* arg_length, const char* delim)
// e.g. "cat a b" return in arg_length=[3, 1, 1]
{
	char* tmp_command = (char*) calloc(strlen(command)+1, sizeof(char));
	strcpy(tmp_command, command); 
	char* result = NULL; 
	result = strtok(tmp_command, delim);
	int i=0; 
	while (result != NULL) {
		arg_length[i] = strlen(result);
		i++; 
		result = strtok(NULL, delim); 
	}
	free(tmp_command);
}

void get_arg_list(const char* command, char** arg_list, const char* delim)
// e.g. "cat a b" output in arg_list=["cat", "a", "b"]
{
	char* tmp_command = (char*) calloc(strlen(command)+1, sizeof(char));
	strcpy(tmp_command, command); 
	char* result = NULL; 
	result = strtok(tmp_command, delim);
	int i=0; 
	while (result != NULL) {
		strcpy(arg_list[i], result); 
		i++; 
		result = strtok(NULL, delim); 
	}
	arg_list[i] = NULL; 
	free(tmp_command);
}

int if_new_line(const char* command, const int if_first_line)
// if_first_line gives 1 if the command is in the first line, 0 if already a new line
// return 1 if new line required
{
	int new_line_required = 1-if_first_line; 
	for (int i=0; i<strlen(command); i++) {
		char tmp = command[i]; 
		if(tmp == ' ' || tmp=='\t') 
			continue; 
		else if(tmp == '|' || tmp=='<' || tmp=='>')
			new_line_required=1; 
		else
			new_line_required=0; 
	}
	return new_line_required; 
}

int ifexit(const char* command) 
// return 1 if the command is exit
{
	char* tmp_command = (char*) calloc(strlen(command)+1, sizeof(char));
	strcpy(tmp_command, command); 
	char* result = NULL; 
	result = strtok(tmp_command, " \t");
	int exit_result=0;
	if (result)
		exit_result = (strcmp(result, "exit")==0);
	free(tmp_command);
	return exit_result; 
}

int exec_cd(const char* command)
// return 1 if the command is cd
{
	int arg_num = get_arg_num(command, " \t"); 
	if (arg_num==0) 
		return 0; 
	int * arg_length = (int*) malloc(arg_num*sizeof(int)); 
	get_arg_length(command, arg_length, " \t");
	char** arg_list = (char**) malloc((arg_num+1)*sizeof(char*)); 
	for (int i=0; i<arg_num; i++) {
		char* arg_single = (char*) calloc(arg_length[i]+1, sizeof(char)); 
		arg_list[i] = arg_single; 
	}
	free(arg_length);
	get_arg_list(command, arg_list, " \t"); 
	if (arg_list[0]!=NULL) {
		if (strcmp(arg_list[0], "cd")==0) {
			char dir[1024]; 
			char cd_error_output[1024]; 
			if (arg_num>=2)
				strcpy(cd_error_output, arg_list[1]); 
			if (arg_num<2) 
				strcpy(dir, getenv("HOME"));
			else if (arg_list[1][0]!='~')
				strcpy(dir, arg_list[1]); 
			else {
				strcpy(dir, getenv("HOME"));
				int arg_i = 0; 
				while (arg_list[1][arg_i+1]!='\0') {
					arg_list[1][arg_i] = arg_list[1][arg_i+1]; 
					arg_i++; 
				}
				arg_list[1][arg_i] = arg_list[1][arg_i+1]; 
				strcat(dir, arg_list[1]); 
			}
			int cd_result = chdir(dir);
			if (cd_result == -1)
		 		fprintf(stderr, "cd: %s: non-existing directory\n", cd_error_output);
		 	for (int i=0; i<arg_num; i++) 
				free(arg_list[i]); 
			free(arg_list); 
		 	return 1; 
		}
	}
	for (int i=0; i<arg_num; i++) 
		free(arg_list[i]); 
	free(arg_list); 
	return 0; 
}

int single_command(char* command, const int if_read_pipe, const int if_write_pipe) 
// execute a single command
// return -1 when ERROR
// return -2 when build in command (cd, pwd) or empty
{
	char* file_in = NULL; 
	char* file_out = NULL; 
	int in_redirection_mode = input_separate(command, &file_in); 
	int out_redirection_mode = output_separate(command, &file_out); 
	if (in_redirection_mode == -1 || (if_read_pipe && in_redirection_mode>0)) {
		fprintf(stderr, "duplicated input redirection\n");
		free(file_in);
		return -1; 
	}
	else if (in_redirection_mode == 1) {
		int fd=open(file_in, O_RDONLY); 
		if (fd==-1) {
			fprintf(stderr, "%s: non-existing file in input direction\n", file_in);
			free(file_in); 
			return -1; 
		}
		free(file_in); 
		if (dup2(fd, STDIN_FILENO)==-1) {
			fprintf(stderr, "ERROR dup2 in\n");
			return -1; 
		}
		close(fd); 
	}
	if (out_redirection_mode == -1 || (if_write_pipe && out_redirection_mode>0)) {
		fprintf(stderr, "duplicated output redirection\n");
		free(file_out);
		return -1; 
	}
	else if (out_redirection_mode >= 1) {
		int fd; 
		if (out_redirection_mode==1) 
			fd=open(file_out, O_CREAT|O_WRONLY|O_TRUNC, 0666);
		else
			fd=open(file_out, O_APPEND|O_CREAT|O_WRONLY, 0666); 
		if (fd==-1) {
			fprintf(stderr, "%s: failed to open file in output redirection\n", file_out);
			free(file_out);
			return -1; 
		}
		free(file_out);
		if (dup2(fd, STDOUT_FILENO)==-1) {
			fprintf(stderr, "ERROR dup2 out\n");
			return -1; 
		}
		close(fd); 
	}
	int arg_num = get_arg_num(command, " \t"); 
	if (arg_num==0) {
		fprintf(stderr, "missing program\n");
		return -1; 
	}
	int * arg_length = (int*) malloc(arg_num*sizeof(int)); 
	get_arg_length(command, arg_length, " \t");
	char** arg_list = (char**) malloc((arg_num+1)*sizeof(char*)); 
	for (int i=0; i<arg_num; i++) {
		char* arg_single = (char*) calloc(arg_length[i]+1, sizeof(char)); 
		arg_list[i] = arg_single; 
	}
	free(arg_length);
	get_arg_list(command, arg_list, " \t"); 
	if (arg_list[0]!=NULL) {
		if (strcmp(arg_list[0], "pwd")==0) {
			char pwd_buff[1024]; 
			if(getcwd(pwd_buff, sizeof(pwd_buff))==NULL)
				fprintf(stderr, "error getcwd\n"); 
			printf("%s\n", pwd_buff);
			for (int i=0; i<arg_num; i++) 
				free(arg_list[i]); 
			free(arg_list); 
			return -2; 
		}
		if (strcmp(arg_list[0], "cd")==0) {
			for (int i=0; i<arg_num; i++) 
				free(arg_list[i]); 
			free(arg_list); 
			return -2; 
		}
	}
	if (execvp(arg_list[0], arg_list)==-1) {
		fprintf(stderr, "%s: non-existing program\n", arg_list[0]);
		for (int i=0; i<arg_num; i++) 
			free(arg_list[i]); 
		free(arg_list); 
		return -1; 
	}
	return 0; 
}

int piped_command(const char* command)
{
	int i=0;
	int cmd_num = get_arg_num(command, "|"); 
	int * cmd_length = (int*) malloc(cmd_num*sizeof(int)); 
	get_arg_length(command, cmd_length, "|"); 
	char** cmd_list = (char**) malloc((cmd_num+1)*sizeof(char*)); 
	for (i=0; i<cmd_num; i++) {
		char* cmd_single = (char*) calloc(cmd_length[i]+1, sizeof(char)); 
		cmd_list[i] = cmd_single; 
	}
	free(cmd_length);
	get_arg_list(command, cmd_list, "|"); 
	int pipe_fd[2]; 
	int pipe_read=0; 
	pid_t cpid; 
	int if_read_pipe=0; 
	int if_write_pipe=0; 
	for (i=0; i<cmd_num; i++) {
		if (i!= cmd_num - 1) 
			if(pipe(pipe_fd)==-1){
				fprintf(stderr, "pipe error\n");
				for (i=0; i<cmd_num; i++) 
					free(cmd_list[i]); 
				free(cmd_list); 
				return -1;
			} 
		cpid = fork(); 
		if (cpid==0) { //child
			if (i!= cmd_num -1) {
				if_write_pipe=1; 
				close(pipe_fd[0]); 
				dup2(pipe_fd[1], STDOUT_FILENO); 
				close(pipe_fd[1]);
			}
			else
				if_write_pipe=0; 
			if (i!=0) {
				if_read_pipe=1; 
				dup2(pipe_read, STDIN_FILENO);
				close(pipe_read); 
			}
			else
				if_read_pipe=0; 
			int single_command_return_value=single_command(cmd_list[i], if_read_pipe, if_write_pipe); 
			if(single_command_return_value<0) {
				for (i=0; i<cmd_num; i++) 
					free(cmd_list[i]); 
				free(cmd_list); 
				return single_command_return_value; 
			}
		}
		else {
			signal(SIGINT, sigint_handler2);
			if (i!=0)
				close(pipe_read); 
			if(i!= cmd_num - 1) { 
				pipe_read = pipe_fd[0];
				close(pipe_fd[1]);
			}
		}
	}
	while(wait(NULL)>0);
	for (i=0; i<cmd_num; i++) 
		free(cmd_list[i]); 
	free(cmd_list); 
	return 0; 
}

int main(int argc, char const *argv[])
{
	while (1) {
		signal(SIGINT, sigint_handler); 
		pid_t nextpid; 
		pid_t wait_pid=0;
		printf("ve482sh $ ");
		int command_size = 1024; 
		command_main = (char*)calloc(1024+1, sizeof(char)); 
		if(my_gets(command_main)==2){
			printf("exit\n");
			free(command_main); 
			command_main=NULL; 
			break; 
		}
		char * command_new_line = (char*)calloc(1024+1, sizeof(char));
		strcpy(command_new_line, command_main); 
		int if_first_line = 1; 
		int skip = 0; 
		while(if_new_line(command_new_line, if_first_line)) {
			if_first_line = 0; 
			printf("> ");
			if(my_gets(command_new_line)==2) {
				fprintf(stderr, "error: unexpected EOF\n");
				skip = 1; 
				break; 
			}
			if(strlen(command_main)+strlen(command_new_line)+1 <= command_size){
				strcat(command_main, " "); 
				strcat(command_main, command_new_line); 
			}
			else {
				char* command_tmp = (char*) calloc(command_size+1, sizeof(char)); 
				strcpy(command_tmp, command_main); 
				free(command_main); 
				while(strlen(command_main)+strlen(command_new_line)+1 > command_size) 
					command_size+=1024;
				command_main = (char*) calloc(command_size+1, sizeof(char)); 
				strcpy(command_main, command_tmp); 
				free(command_tmp); 
				strcat(command_main, " ");
				strcat(command_main, command_new_line); 
			}
		}
		free(command_new_line); 
		command_new_line=NULL; 
		if(!skip) {
			if(if_empty_command(command_main)) {
				free(command_main); 
				command_main=NULL; 
				continue; 
			}
			if(if_consecutive_pipe(command_main) || if_pipe_first(command_main)) {
				fprintf(stderr, "missing program\n");
				free(command_main); 
				command_main=NULL; 
				continue; 
			}
			if(strchr(command_main, '|')) {
			// with pipe
				if (piped_command(command_main)<0){
					free(command_main); 
					command_main=NULL; 
					break; 
				}
			}
			else{ 
			// no pipe
				if (ifexit(command_main)) {
					free(command_main); 
					command_main=NULL; 
					break; 
				}
				if (exec_cd(command_main)) {
					free(command_main); 
					command_main=NULL; 
					continue; 
				}
				nextpid = fork();
				if (!nextpid) {
					//signal(SIGINT, sigint_handler2);
					if(single_command(command_main, 0, 0)<0) {
						free(command_main); 
						command_main=NULL; 
						break; 
					}
				} 
				else {
					signal(SIGINT, sigint_handler2);
					while(!wait_pid) {
						wait_pid = waitpid(nextpid, NULL, 0);
					}
				}
			}
		}
		free(command_main); 
		command_main=NULL; 
	}
	return 0;
}
