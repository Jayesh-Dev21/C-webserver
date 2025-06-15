# C-Webserver

An HTTP protocol-based web server written in C for **CSOC'25**.

```json
{
"Name": "Jayesh Krishan Puri",
"Roll no.": 24155058,
"Branch": "B.Tech Mining Engineering"
}
```

---

## 📜 Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Project Structure](#project-structure)
- [How to Build and Run](#how-to-build-and-run)
- [How the Code Works (Beginner Level)](#how-the-code-works-beginner-level)
- [Cleaning Build Files](#cleaning-build-files)
- [License](#license)

---

## 🧠 Introduction

This project is a minimalist web server built using the **C programming language**. It listens on your computer’s `localhost` and serves files over HTTP — the protocol that powers the web. This server is designed to help you understand the fundamentals of:

- TCP sockets  
- HTTP request handling  
- Static file serving  
- Logging mechanisms  

It's a great entry point into low-level networking and systems programming!

---

## 🚀 Features

- 📡 Handles HTTP requests on **port 3003**
- 📂 Serves static files from the `public/` directory
- 📄 Logs **all incoming requests** and **errors**
- ⚙️ Clean and build using a **Makefile**

---

## 🗂️ Project Structure

```
.
├── webserver              # Compiled binary
├── src/                   # Source code files
│   ├── main.c             # Main server logic
│   ├── content_headers.c/h  # Sets MIME types for files
│   ├── error_logging.c/h    # Error handling and logging
│   ├── request_logging.c/h  # Request access logging
│   └── headers.h          # HTTP and server config
├── public/                # Static HTML/CSS/JS/assets to serve
├── logs/                  # Log output directory
│   └── server_activity.log
├── Makefile               # Build and clean instructions
├── README.md              # Project documentation
└── LICENSE                # License file
```

---

## 🛠️ How to Build and Run

### 1. Clone the Repository

```bash
git clone https://github.com/Jayesh-Dev21/C-webserver.git
cd C-webserver
```

### 2. Build the Server

```bash
make
```

### 3. Run the Server

```bash
./webserver
```

Open your browser and go to:

```
http://localhost:3003/
```

You should see the default web page (served from `public/index.html`) or any other static files you’ve added.

---

## 👨‍🏫 How the Code Works (Beginner Level)

- The server setup and logic is inside `src/main.c`.
- It uses a socket to listen for HTTP requests from browsers.
- When a request comes in:
  - It reads the path and determines which file to serve.
  - It fetches the correct MIME type using `content_headers.c`.
  - If the file exists, it serves it with the right HTTP headers.
  - If there’s an issue (e.g., file not found), it logs the error and returns an error page.
- Logs for **all requests** and **errors** are stored in the `logs/` directory.

### 📘 Summary of Key Files

| File | Description |
|------|-------------|
| `main.c` | Starts the server and handles connections |
| `content_headers.c/h` | Assigns MIME types to files |
| `error_logging.c/h` | Logs runtime and file errors |
| `request_logging.c/h` | Logs all HTTP requests with metadata |
| `headers.h` | Common declarations and configurations |
| `Makefile` | Handles build and clean operations |

> 🔰 **New to C or web servers?** Read the files in this order for best understanding:
> 1. `main.c`  
> 2. `content_headers.c/h`  
> 3. `request_logging.c/h`  
> 4. `error_logging.c/h`  
> 5. `headers.h`

---

## 🧹 Cleaning Build Files

To remove compiled files and logs:

```bash
make clean
```

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).  
Feel free to explore, modify, and share!

---

## 🧪 Explore More

Add your own `.html`, `.css`, `.png`, or `.js` files into the `public/` directory and access them via your browser. The server will serve them just like a real production web server.

---
