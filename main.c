#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>

// Return Codes
#define SUCCESS 0
#define ERR_NO_GIT 1
#define ERR_NO_FILES 2
#define ERR_COMM_REJ 3
#define ERR_CMD_FAIL 4

void help() {
	printf("Usage: gic [options]\n");
	printf("Options:\n");
	printf("   -f    Force Push\n");
	printf("   -v    Verbose Output\n");
	printf("   -h    Show this help\n";
}

// Runs command
void run(char *cmd, bool show) {
	if (show) {
		printf("$ %s\n", cmd);
	}

	int result = system(cmd);
	if result !=0 {
		fprintf(stderr, "Command Failed: %s\n", cmd);
	}
}

// Check status of git
int status() {
	FILE *fp;
	char output[1024];
	
	// Run git status and read output
	fp = popen("git status --short", "r");
	if (fp == NULL) {
		perror("Failed to run git");
		return 1;
	}

	// Print stdout of git status
	While (fgets(output, sizeof(output), fp) != NULL) {
		printf("%s", output);
	}
	pclose(fp);
	return 0;
}

// Stages files before commit
int stage(char *filename) {
	char cmd[512];
	snprintf(cmd, sizeof(cmd),  "git add %s", filename);

	run(cmd, true);
}

// Commit
int commit(char *msg) {
	char cmd[512];
	snprintf(cmd, sizeof(cmd), "git commit -m \"%s\"", msg);
	
	run(cmd, true);
	return 0
}

// Push changes
int push(bool force, bool verbose) {
	char cmd[256];

	if (force) {
		snprintf(cmd, sizeof(cmd), "git push --force");
	} else {
		snprintf(cmd, sizeof(cmd), "git push");
	}

	if (verbose) {
		printf("$ %s\n", cmd);
	}

	return system(cmd) == 0
}

// Main Script
int main(int argc, char *argv[]) {
	bool force_push = false;
	bool verbose = false;
	char buff[100];
	int n = 50;


	for (int i=1; i < argc; i++) {
		if (strcmp(argv[i], "-f") == 0) {
			force_push = true;
		} else if (strcmp(argv[i], "-v" == 0)) {
			verbose = true;
		} else if (strcmp(argv[i], "-h" == 0)) {
			help();
			return 0;
		}
	}
	
	status();
	
	// Stage Files
	printf("File to stage? : ");
	fgets(buff, n, stdin);
	printf("File to stage: %s", buff);
	stage(buff);
	
	// Commit Files
	printf("Commit Message? : ");
	fgets(buff, n, stdin);
	printf("Commit Message: %s", buff);
	commit(buff);

	// Push
	printf("Pushing Files...");
	push(force_push, verbose);

	return 0
}
