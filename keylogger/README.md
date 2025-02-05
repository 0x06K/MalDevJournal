# Windows Keylogger Development Guide

## 📌 Overview
This guide provides a structured list of **Windows API functions** you need to understand to develop a **Windows Keylogger** with **persistence and stealth capabilities**. It is divided into **essential functions for the project** and **advanced functions for long-term growth**.

---

## **1️⃣ Essential Functions for Keylogger Development**

### **🔹 Keystroke Logging Functions**
| Function | Purpose |
|----------|---------|
| `SetWindowsHookExA()` | Hooks the keyboard globally to capture keystrokes |
| `CallNextHookEx()` | Passes the event to the next hook in the chain |
| `UnhookWindowsHookEx()` | Removes the keyboard hook |
| `GetAsyncKeyState()` | Polls the keyboard to check if a key is pressed |
| `MapVirtualKey()` | Converts virtual key codes to characters |

### **🔹 File I/O (For Logging Keystrokes)**
| Function | Purpose |
|----------|---------|
| `CreateFile()` | Creates or opens a file to store logged keystrokes |
| `WriteFile()` | Writes keystrokes to the log file |
| `ReadFile()` | Reads data from the log file |
| `SetFileAttributes()` | Hides log files using attributes (e.g., Hidden, System) |

### **🔹 Process Manipulation (For Stealth)**
| Function | Purpose |
|----------|---------|
| `CreateToolhelp32Snapshot()` | Captures system processes, modules, and threads |
| `Process32First()` | Gets the first process in the snapshot |
| `Process32Next()` | Iterates through running processes |
| `OpenProcess()` | Opens a handle to another process |
| `VirtualAllocEx()` | Allocates memory in another process (for injection) |
| `WriteProcessMemory()` | Writes data to a remote process (for injection) |
| `CreateRemoteThread()` | Creates a thread in another process (for execution) |

### **🔹 Persistence (Making the Keylogger Survive Reboots)**
| Function | Purpose |
|----------|---------|
| `RegCreateKeyExA()` | Creates or opens a registry key |
| `RegSetValueExA()` | Modifies the registry to add a startup entry |
| `CreateService()` | Registers the keylogger as a Windows service |
| `MoveFileEx()` | Moves or renames files (useful for self-replication) |
| `ShellExecute()` | Executes a process (used for persistence methods) |
| `sc.exe` | Command-line tool to create and manage Windows services |

---

## **2️⃣ Advanced Functions for Further Learning**

### **🔹 Advanced Persistence Techniques**
| Function | Purpose |
|----------|---------|
| `CoRegisterClassObject()` | Registers a COM object for persistence |
| `CoCreateInstance()` | Creates a COM object (used for stealth persistence) |
| `IWbemServices::ExecQuery()` | Executes WMI queries (for WMI persistence) |
| `IWbemLocator::ConnectServer()` | Connects to WMI (used for stealthy execution) |
| `schtasks /create` | Schedules a task to execute the keylogger on boot |

### **🔹 AV/EDR Evasion Techniques**
| Function | Purpose |
|----------|---------|
| `NtAllocateVirtualMemory()` | Allocates memory using direct syscalls |
| `NtCreateThreadEx()` | Creates a new thread using direct syscalls |
| `NtProtectVirtualMemory()` | Modifies memory protections |
| `VirtualProtect()` | Changes memory permissions to bypass hooks |
| `LoadLibraryA()` | Dynamically loads DLLs |
| `GetProcAddress()` | Resolves function addresses dynamically |
| `NtCreateTransaction()` | Used for process doppelgänging (AV evasion) |
| `NtSetInformationProcess()` | Modifies process execution details |

### **🔹 Rootkit-Level Stealth Techniques**
| Function | Purpose |
|----------|---------|
| `PsSetCreateProcessNotifyRoutine()` | Hooks process creation at the kernel level |
| `ObRegisterCallbacks()` | Registers object manager callbacks for stealth |
| `NtQuerySystemInformation()` | Retrieves system process information (can be hooked for stealth) |
| `ZwQueryDirectoryFile()` | Hides files by modifying directory listing results |

---

## **📌 Learning Roadmap**
1️⃣ **Learn Windows API Basics** (File I/O, Process Management)  
2️⃣ **Study Windows Hooks (`SetWindowsHookExA`)**  
3️⃣ **Implement a Basic Keylogger Using Hooks**  
4️⃣ **Learn Windows Registry for Persistence (`RegSetValueExA`)**  
5️⃣ **Study Process Injection & Memory Manipulation**  
6️⃣ **Explore AV/EDR Evasion Techniques (Direct Syscalls)**  
7️⃣ **Implement Process Hiding & Stealth Methods**  
8️⃣ **Go Deep Into Kernel Mode Rootkit Development**  

---

## **📌 Recommended Resources**
- **Windows API Documentation**: [https://docs.microsoft.com/en-us/windows/win32/apiindex/windows-api-list](https://docs.microsoft.com/en-us/windows/win32/apiindex/windows-api-list)
- **Windows Internals (6th Edition) - Mark Russinovich** (Deep dive into OS internals)
- **Malware Analysis Techniques - Alexey Kleymenov** (Reverse engineering and evasion)

---

## **🚀 Next Steps**
- Start with a simple **keylogger** using `SetWindowsHookExA()`.
- Implement **persistence using Windows Registry (`RegSetValueExA()`)**.
- Move to **advanced stealth techniques (Direct Syscalls, Process Hollowing)**.
- Explore **rootkit development for deeper system access**.

---

This document is intended for **educational and ethical research purposes only**.
