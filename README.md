# gic/p - Git Interactive Commit and Push

A simple command-line tool that makes git commits faster by combining add, commit, and push into one interactive prompt.

## What it does

Instead of typing three separate git commands, gic asks you what to stage, what your commit message is, and then pushes automatically.

## Installation
```bash
make
sudo make install
```

## Usage
```bash
gic [options]
```

The program will ask you:
1. Which file to stage
2. Your commit message
3. Then automatically push

### Options

- `-f` - Force push (use carefully!)
- `-v` - Show all git commands as they run
- `-h` - Show help

### Examples

Basic usage:
```bash
gic
```

Force push with verbose output:
```bash
gic -f -v
```

## Requirements

- Git must be installed
- Must be run inside a git repository

## License

GPL-3.0
