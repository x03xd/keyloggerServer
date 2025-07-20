# 🧩 Keylogger Server Project

This project contains server-side components for a keylogger system built at the **Layer 4 (L4)** level of the OSI model — it operates directly over **TCP sockets** without using any application-layer protocols like HTTP.

The server behavior is determined by the `--mode` argument provided at runtime, allowing two different modes:

---

## 📡 Modes of Operation

- **`--mode payload`**  
  Starts a server that sends the keylogger payload to connecting clients. It handles binary transmission and initial setup.

- **`--mode keylog`**  
  Starts a server that listens for incoming keystroke data. It processes the received content along with `userId`, allowing user-specific logging and persistence.

---

## 📍 Default Address & Ports

- All servers bind by default to IP address: `127.0.0.9`
- **Payload server** (`--mode payload`) uses port: `1234`
- **Keylogger server** (`--mode keylog`) uses port: `4321`

These defaults can be changed directly in the source code.

---

## 🔧 Core Architecture

The project is implemented in C using the Windows Sockets API (WinSock). Below is a high-level overview:

### Server Initialization

```c
BOOL initializeServer(...) {
    WSAStartup(...);
    socket(...);
    bind(...);
    listen(...);
}
