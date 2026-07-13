# 🚀 Remote System Monitor & File Transfer Server

A **multi-threaded client-server system** built with **C and POSIX Socket Programming**.

This project provides remote system monitoring, process management, authentication, role-based permission control, and file transfer capabilities through TCP communication.

---

## ✨ Features

### 🔐 Authentication & Authorization

* Username / Password authentication
* Role-Based Access Control (RBAC)
* Different permission levels for users

Example:

```text
USERNAME: pokpong
PASSWORD: 1234

LOGIN_SUCCESS admin
```

---

### 🌐 Client-Server Communication

* TCP Socket Programming
* Multi-client support
* Thread-per-client architecture using `pthread`
* Concurrent client handling

---

# 📂 Project Structure

```text
Remote-System-Monitor
│
├── server/
│   │
│   ├── server.c
│   ├── client_handler.c
│   ├── auth.c
│   ├── permission.c
│   ├── system_monitor.c
│   ├── process_monitor.c
│   ├── file_manager.c
│   ├── logger.c
│   │
│   ├── server_files/
│   │
│   └── logs/
│       └── server.log
│
├── client/
│   │
│   ├── client.c
│   └── client_downloads/
│
├── docs/
│   └── architecture.png
│
└── README.md
```

---

# 🏗️ System Architecture

```text
                 

┌──────────────┐                      ┌──────────────────┐
│              │    TCP Connection    │                  │
│    Client    │ ───────────────────► │      Server      │
│              │                      │                  │
└──────────────┘                      └───────┬──────────┘
                                              │
          ┌───────────────────────────────────┼────────────────────────┐
          │                                   │                        │
          ▼                                   ▼                        ▼
     🔐 Authentication                 🖥️ Monitoring             📁 File Manager
     - Login                            - CPU Info                - Upload
     - Password                         - Memory                  - Download
                                        - Processes               - Delete
          │                                   │                        │
          └───────────────────┬───────────────┴────────────────────────┘
                              │
                              ▼
                         📝 Logger
                         - Login Events
                         - Server Activities
```

---

# 🛠️ System Features

## 🖥️ System Monitoring

Monitor Linux system information:

* CPU information
* Memory usage
* System status

Command:

```bash
monitor
```

Example:

```bash
CPU Usage: 12%
Memory Usage: 43%
```

---

## ⚙️ Process Monitoring

View running Linux processes:

Command:

```bash
processes
```

Example:

```text
PID     COMMAND        STATE
--------------------------------
1       init           S
647     server         S
648     client         S
```

---

## 📁 File Management

Remote file operations:

| Command           | Description               |
| ----------------- | ------------------------- |
| `list`            | View server files         |
| `upload <file>`   | Upload file to server     |
| `download <file>` | Download file from server |
| `delete <file>`   | Delete server file        |

Example:

```bash
> upload test.txt

Upload success


> download test.txt

Downloaded successfully
```

---

## 📝 Logging System

Server activity logging:

```text
logs/server.log
```

Example:

```text
[Mon Jul 14 15:30:20 2026]
Client #1 user pokpong login success (admin)

[Mon Jul 14 15:31:10 2026]
Client #1 uploaded test.txt
```

---

# 🛠️ Technologies Used

| Category         | Technologies             |
| ---------------- | ------------------------ |
| Language         | C                        |
| Networking       | POSIX Sockets            |
| Concurrency      | Pthreads                 |
| Operating System | Linux                    |
| Monitoring       | Linux `/proc` Filesystem |
| Version Control  | Git & GitHub             |

---

# ▶️ How to Run

## 1. Clone Repository

```bash
git clone <repository-url>

cd Remote-System-Monitor
```

---

# 🔨 Build Server

```bash
cd server

gcc server.c \
client_handler.c \
auth.c \
permission.c \
system_monitor.c \
process_monitor.c \
file_manager.c \
logger.c \
-pthread \
-o server
```

Run:

```bash
./server
```

---

# 🔨 Build Client

```bash
cd client

gcc client.c -o client
```

Run:

```bash
./client
```

---

# 💡 Example Workflow

Start server:

```text
Server listening on port 8080...
```

Connect client:

```text
USERNAME: pokpong
PASSWORD: 1234

LOGIN_SUCCESS admin
```

Execute commands:

```text
> monitor

> processes

> list

> upload test.txt

> download test.txt

> delete test.txt

> exit
```

---

# 📚 Learning Outcomes

Through this project, I practiced:
- Client-Server Architecture
- Socket Programming
- Multi-threading
- Linux System Programming
- Access Control Design
- File Transfer Protocol Design
- Modular Software Development
- Debugging Network Applications

---

# 🔮 Future Improvements

* Password hashing
* TLS encrypted communication
* Database-based user management
* Web dashboard
* Real-time monitoring
* Docker deployment

---

# 👨‍💻 Author

**Vichayut Tubtim**
Computer Engineering Student, Thammasat University
