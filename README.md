
# Traffic Management System

A hybrid Node.js web dashboard backend paired with a native C-based execution simulation engine. This repository has been optimized to contain only critical source files for deployment, continuous integration, or clean archiving.

---

## 📂 Project Directory Structure

```text
TRAFFIC-PROJECT/
├── public/
│   └── dash.html         # Interactive traffic metrics UI dashboard page
├── index.html            # Application entry portal landing page
├── package.json          # Node.js project manifest and module configuration
├── package-lock.json     # Node.js dependency structural lockfile
├── server.js             # Asynchronous web app routing dashboard backend controller
└── sim.c                 # Source logic execution engine for core simulation matrices

```

---

## 🛠️ Prerequisites

Before executing the compilation or execution pipeline, ensure that the following toolchains are accessible inside your active shell terminal path:

* **Node.js Runtime Environment:** Version 16.x or newer recommended.
* **C Compiler Toolchain:** GCC, Clang, or MSVC Build tools configured natively for target execution.

---

## 🚀 Step-by-Step Guide to Run the Program

Follow these consecutive instructions carefully to initialize, assemble, and run the complete system loop:

### Step 1: Initialize System Dependencies

Open up your command prompt or console workspace directory bounds and parse out tracking components safely:

```bash
npm install

```

*(This freshly retrieves the third-party framework layers needed by the backend application wrapper without bloating structural file weights).*

### Step 2: Compile the Simulation Architecture Matrix

The web dashboard initializes and interfaces directly with a target compiled executable program logic loop. Compile the native file framework wrapper (`sim.c`) matching your deployment system:

* **On Linux / macOS Systems:**
```bash
gcc -O3 sim.c -o sim

```


* **On Windows Platforms (Command Prompt / PowerShell):**
```bash
gcc sim.c -o sim.exe

```


*(If building using MSVC Developer tools instead, invoke `cl /O2 sim.c /Fesim.exe`).*

> 📌 **Important Rule:** Ensure that the compiled output executable (`sim` or `sim.exe`) is located inside the same folder level as your `server.js` controller script so the paths hook up correctly.

### Step 3: Launch the Core Control Server

Boot up the central dashboard logic processing stack utilizing your local machine runtime loop:

```bash
node server.js

```

### Step 4: Interact via Operating Interface

When the processing console outputs standard operational status confirmations, route directly via browser instances to stream data:

```text
http://localhost:3000

```

*(If your specific file variables isolate structural interfaces explicitly under dedicated routing tokens, you can access the operational workspace matrix directly at `http://localhost:3000/dash.html`).*

---

## 🛡️ Production Best Practices

* **Workspace Hygiene:** Keep structural snapshot footprints clean by tracking compiled execution targets (`sim`, `sim.exe`) or runtime analytics records (`*.log`) outside repository bounds via your tracking configuration filters.
* **Cross-Platform Deployments:** Native executable frameworks should never be moved universally across variable target hardware; always compile from the raw `sim.c` codebase on your respective target server directly.

```

```
