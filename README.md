# 🧩 Keylogger Server Project

This project contains server-side components for a keylogger system built at the **Layer 4 (L4)** level of the OSI model - it operates directly over **TCP sockets** without using any application-layer protocols like HTTP.

In practice, network communications that aim to blend into normal traffic often use protocols, traffic patterns, and destination ports that resemble legitimate network activity. For this reason, using HTTPS over 443 can be an effective approach, as it aligns with traffic commonly observed in many environments. Encrypting communications may also be beneficial despite the additional overhead, since it prevents the payload from being directly inspected and classified based solely on its contents. However, modern security solutions typically analyze more than just port numbers, including protocol behavior, traffic patterns, metadata, and destination reputation.

It is important to note that many enterprise environments deploy security controls such as TLS-intercepting proxies. In these setups, an organization-controlled root CA certificate is installed on managed endpoints, allowing TLS connections to be terminated, inspected, and analyzed before the traffic is re-encrypted and forwarded to its destination if it complies with organizational security policies.

The server behavior is determined by the `--mode` argument provided at runtime, allowing two different modes:

---

## 📡 Modes of Operation

- **`--mode payload`**  
  Starts a server that sends the keylogger payload to connecting clients. It handles binary transmission and initial setup.

- **`--mode keylog`**  
  Starts a server that listens for incoming keystroke data.  
  It processes the received content along with a `userId`, and **logs the data to a file specific to that user**, enabling persistent, user-specific storage.

---

## 📍 Default Address & Ports

- All servers bind by default to IP address: `127.0.0.9`
- **Payload server** (`--mode payload`) uses port: `1234`
- **Keylogger server** (`--mode keylog`) uses port: `4321`

These defaults can be changed directly in the source code.

---

## 🔧 Core Architecture

The project is implemented in C using the Windows Sockets API (WinSock). To maximize performance, each client connection is processed in its own dedicated thread.
