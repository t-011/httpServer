# HTTP Server

A lightweight HTTP server built from scratch using Linux socket API and TCP.

## Features

- HTTP request parsing and response handling
- Built with raw socket programming (no frameworks)
- Logging support
- Cross-platform Linux support

## Requirements

- Linux
- C++
- Make

## Build & Run
```bash
make
./server
```

Server listens on default HTTP port. View logs in `log_file.txt`.

## Architecture

- `http_tcpServer_linux.cpp/h` — HTTP protocol and TCP socket handling
- `server_linux.cpp` — Main server entry point
- `utils.h` — Utility functions

This is a learning project demonstrating low-level network programming.
