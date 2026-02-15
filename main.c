#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Return codes
#define SUCCESS 0
#define ERR_CMD_FAIL 1

void print_help(void) {
    printf("Usage: gic [options]\n");
    printf("Options:\n");
    printf("  -f    Force push\n");
    printf("  -v    Verbose output\n");
    printf("  -h    Show this help\n");
}

// Run a shell command
int run_command(const char *cmd, bool verbose) {
    if (verbose) {
        printf("$ %s\n", cmd);
    }

    int result = system(cmd);
    if (result != 0) {
        fprintf(stderr, "Command failed: %s\n", cmd);
        return ERR_CMD_FAIL;
    }
    return SUCCESS;
}

// Show git status
int show_status(void) {
    FILE *fp = popen("git status --short", "r");
    if (fp == NULL) {
        perror("Failed to run git");
        return ERR_CMD_FAIL;
    }

    char output[1024];
    while (fgets(output, sizeof(output), fp) != NULL) {
        printf("%s", output);
	printf("\nCurrent Branch: ");
	system("git branch --show-current");
	printf("\n");
    }
    
    pclose(fp);
    return SUCCESS;
}

// Stage a file
int stage_file(const char *filename, bool verbose) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "git add %s", filename);
    return run_command(cmd, verbose);
}

// Commit with message
int commit_changes(const char *msg, bool verbose) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "git commit -m \"%s\"", msg);
    return run_command(cmd, verbose);
}

// Push changes
int push_changes(bool force, bool verbose) {
    char cmd[256];
    
    if (force) {
        snprintf(cmd, sizeof(cmd), "git push --force");
    } else {
        snprintf(cmd, sizeof(cmd), "git push");
    }
    
    return run_command(cmd, verbose);
}

// Remove trailing newline from fgets input
void strip_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main(int argc, char *argv[]) {
    bool force_push = false;
    bool verbose = false;
    char buffer[256];

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            force_push = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = true;
        } else if (strcmp(argv[i], "-h") == 0) {
            print_help();
            return SUCCESS;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help();
            return ERR_CMD_FAIL;
        }
    }

    // Show current status
    show_status();

    // Stage files
    printf("File to stage: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return ERR_CMD_FAIL;
    }
    strip_newline(buffer);
    
    if (stage_file(buffer, verbose) != SUCCESS) {
        return ERR_CMD_FAIL;
    }

    // Commit
    printf("Commit message: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return ERR_CMD_FAIL;
    }
    strip_newline(buffer);
    
    if (commit_changes(buffer, verbose) != SUCCESS) {
        return ERR_CMD_FAIL;
    }

    // Push
    printf("Pushing files...\n");
    if (push_changes(force_push, verbose) != SUCCESS) {
        return ERR_CMD_FAIL;
    }

    printf("Done!\n");
    return SUCCESS;
}
