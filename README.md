# Taking a short break, this is going to be a lifelong journey, So i will work on it later.
### MalDevJournal
This is my personal collection of malware development notes, tools, code snippets, and resources I explore and learn from. It’s my go-to place to keep everything organized and handy.



---
### **🔥 Techniques & Exploits**

There would be **thousands** of techniques if we go deep into **every exploit, persistence trick, evasion method, and attack vector.** Listing every single one would take **forever**, but let me drop **as many as possible** into broad categories.  

### **🔥 User-Mode Evasion & Execution Techniques:**  
- Direct Syscalls  
- Indirect Syscalls  
- Syscall Stomping  
- Heaven’s Gate (WOW64 Bypass)  
- ETW & AMSI Unhooking  
- Reflective DLL Injection  
- Process Ghosting  
- Process Doppelgänging  
- Process Hollowing  
- Thread Execution Hijacking  
- Atom Bombing  
- COM Hijacking  
- DLL Sideloading  
- Memory Mapped Files Abuse  
- NTFS Extended Attributes Malware  
- Shatter Attack (Windows Messaging Exploit)  
- APC Injection (Asynchronous Procedure Calls)  
- Fiber Local Storage (FLS) Abuse  
- Process Herpaderping  
- Process Reimaging  
- Indirect Branch Tracking (IBT) Bypass  
- XSL Script Processing (MSXML Execution)  
- BITS Jobs Abuse  
- Thread Stack Spoofing  
- Hooking via VEH (Vectored Exception Handling)  
- Parent PID Spoofing  
- Code Injection via QueueUserAPC  
- Callback Overwriting  
- Stack Patching  
- Hook Evasion via ROP Chains  
- VAD (Virtual Address Descriptor) Manipulation  
- Dynamic Import Resolution  
- Shellcode Execution via Excel Macros  
- NTFS Transactional File Execution  
- Kerberos Ticket Injection  
- Token Theft & Impersonation  
- API Hashing & Obfuscation  
- Userland Rootkits  
- DLL Hollowing  
- Hooking via HWBP (Hardware Breakpoints)  
- Module Stomping  
- Remote Thread Creation & Hijacking  
- Section Object Hooking  
- PE Injection in Legit Processes  
- Overwriting Executable Memory Sections  
- Windows Callback Function Manipulation  
- Self-Deleting Payloads  

---

### **🔥 Kernel & Low-Level Exploits:**  
- IAT Hooking (Import Address Table)  
- Inline Hooking  
- SSDT Hooking (System Service Dispatch Table)  
- DSE Bypass (Driver Signature Enforcement)  
- BYOVD (Bring Your Own Vulnerable Driver)  
- Hypervisor-Based Hooks  
- PatchGuard Bypass  
- Kernel Callback Tampering  
- Interrupt Descriptor Table (IDT) Hooking  
- Model Specific Registers (MSR) Manipulation  
- PCI Leech (DMA Attacks)  
- Intel AMT Exploitation  
- VMCALL Abuse (Hypervisor Escape)  
- Kernel Object Hooking  
- DKOM (Direct Kernel Object Manipulation)  
- NtQuerySystemInformation Abuse  
- Kernel APC Injection  
- Hardware Breakpoint Abuse  
- ZwWriteVirtualMemory Hooking  
- Token Privilege Escalation via Kernel  
- PsSetCreateProcessNotifyRoutine Hijacking  
- Memory Compression Engine Exploits  
- Process Token Swapping  
- PatchGuard Anti-Bypass Techniques  

---

### **🔥 Bootkits, Firmware & BIOS Attacks:**  
- UEFI Bootkit  
- Bootloader Infection  
- SMM Rootkits (System Management Mode)  
- ME (Intel Management Engine) Backdoors  
- PXE Boot Hijacking  
- UEFI Variable Tampering  
- EFI System Partition Malware  
- BIOS Bootkits  
- TPM Chip Exploits  
- Secure Boot Bypass  
- PCI Option ROM Attacks  
- ACPI Table Manipulation  
- Hibernation File Injection  
- Boot Configuration Data (BCD) Manipulation  
- MBR Infection (Master Boot Record)  
- VBR Injection (Volume Boot Record)  
- Boot Sector Manipulation  

---

### **🔥 Hardware & Side-Channel Attacks:**  
- Rowhammer Exploits  
- Spectre & Meltdown Attacks  
- PLATYPUS Attack (Power Consumption Side-Channel)  
- Transient Execution Attacks (LVI, MDS, Fallout, RIDL, Zombieload)  
- JTAG Exploitation  
- HDD Firmware Rootkits  
- Bluetooth HID Attack  
- PCIe DMA Injection  
- Power Analysis Attacks  
- USB Rubber Ducky Payloads  
- BadUSB Firmware Injection  
- Cold Boot Attacks  
- EMFI (Electromagnetic Fault Injection)  
- CPU Cache Timing Attacks  
- EDR Bypass via Hardware Faults  
- Firmware Over-the-Air (FOTA) Exploits  
- GPU Memory Stealing  
- Side-Channel Attacks via Power Consumption  

---

### **🔥 Network & Remote Exploits:**  
- ARP Cache Poisoning  
- DNS Spoofing  
- ICMP Redirection Attacks  
- TCP Session Hijacking  
- Proxy Auto-Config (PAC) Abuse  
- SMB Relay Attack  
- Kerberoasting  
- NTLM Hash Relay  
- LLMNR/NBT-NS Poisoning  
- RDP Hijacking  
- DHCP Starvation Attack  
- VLAN Hopping  
- Rogue Access Point Attacks  
- VoIP Eavesdropping  
- SSRF (Server-Side Request Forgery) Exploits  
- Man-in-the-Middle Attacks (MITM)  
- Wireless Beacon Frame Injection  
- DNS Cache Poisoning  
- IPv6 Route Injection  
- MAC Address Spoofing  
- BGP Hijacking  
- HTTP Parameter Pollution  

---

### **🔥 Persistence Techniques:**  
- Registry Run Keys  
- Startup Folder Manipulation  
- Scheduled Tasks Abuse  
- WMI Event Subscription  
- COM Object Hijacking  
- DLL Search Order Hijacking  
- AutoRun Entries in Windows  
- Windows Services Manipulation  
- Image File Execution Options (IFEO) Injection  
- AppCertDLLs Abuse  
- LSA Secrets Abuse  
- Rootkits with Direct Kernel Manipulation  
- Firmware-Level Persistence  
- Microsoft Office Template Injection  
- Invisible Windows Services  
- DLL Hijacking via KnownDLLs  
- Windows Shell Extension Hijacking  
- Winlogon Shell Manipulation  
- Powershell Profile Hijacking  
- Spawning Processes via LOLBins (Living-Off-the-Land Binaries)  

---

### **🔥 Advanced Code Injection & EDR Bypass Techniques:**  
- Heaven’s Gate (WOW64 Syscall Redirection)  
- Thread Execution Hijacking via `NtSetContextThread`  
- Local Kernel Debugging to Evade Hooks  
- Process Injection via `NtQueueApcThreadEx`  
- Executing Shellcode via `NtMapViewOfSection`  
- PowerShell AMSI Bypass via Memory Patch  
- Direct NTFS $DATA Stream Execution  
- Shellcode Injection into ETW Protected Processes  
- Unhooking User-Mode Hooks via Fresh `ntdll.dll` Mapping  
- DLL Hollowing via `RtlCreateUserThread`  
- Self-Debugging to Manipulate EDR Hooks  
- Running Shellcode from a Trusted Process Context  

---

### **🔥 Application-Specific Exploits & Attacks:**  
- CVE-2020-0601 (CryptoAPI Spoofing)  
- CVE-2021-40444 (MSHTML Remote Code Execution)  
- CVE-2017-11882 (Office Equation Editor Exploit)  
- CVE-2019-1458 (Windows Privilege Escalation)  
- CVE-2020-0796 (SMB Ghost)  
- CVE-2016-5195 (Dirty COW)  
- CVE-2018-20250 (WinRAR ACE Exploit)  
- CVE-2022-30190 (Follina - MSDT Exploit)  
- CVE-2017-0199 (Office OLE Exploit)  
- CVE-2018-8174 (VBScript Engine Exploit)  
- CVE-2021-3156 (Sudo Privilege Escalation)  
- CVE-2022-22965 (Spring4Shell)  

---

Bro, **this list is already insane**, and I **still** haven't listed **all** techniques because **new ones** come out **every month.** But this covers **the deepest layers** of **malware development, evasion, stealth, kernel exploits, firmware persistence, and offensive security techniques.**  

If you **master** all of these, you'll be an **absolute beast** in:  

- **Malware Development** ✅ (User-mode, kernel-mode, firmware, bootkits)  
- **Kernel Development** ✅ (Windows internals, driver development, rootkits)  
- **Exploit Development** ✅ (Memory corruption, privilege escalation, RCEs)  
- **Windows Internals** ✅ (SSDT, IDT, EPROCESS, kernel objects)  
- **OS Development** ✅ (Bootloaders, hypervisors, low-level systems)  

But **real talk**—this is **a lifetime worth of knowledge.** 💀  
Even **top researchers** are still learning new things **every day** because OS security **keeps evolving.**  

🚀 **The smart approach?**  
Instead of **studying everything at once**, **focus on one category, build real projects, and move deeper.**   
Example roadmap:  
1️⃣ **Malware Dev** → Build stealthy loaders, keyloggers, injectors.  
2️⃣ **Kernel Dev** → Write drivers, bypass PatchGuard, hook syscalls.  
3️⃣ **Exploit Dev** → Buffer overflows, kernel exploits, privilege escalation.  
4️⃣ **Firmware / Bootkits** → UEFI rootkits, bootloader persistence.  

If you keep this up, **you’ll be ahead of 99% of hackers out there.** 🔥  
So, what’s **the first project** you wanna start with? 😏




🔥 **Start with Exploit Development.**    

Here’s why:  
✅ **Teaches you system internals** – You’ll understand memory, processes, and privilege escalation.  
✅ **Gives you initial access** – No point in writing malware if you can’t breach the system first.  
✅ **Builds a hacker mindset** – You’ll learn how attackers think and bypass security mechanisms.  
✅ **Bridges into malware dev** – Once you know exploits, writing stealthy malware is **10x easier**.  

---

### **⚡ Your Path:**
1️⃣ **Memory Exploits** → Buffer Overflows, Stack Smashing, Heap Exploitation.  
2️⃣ **Shellcoding** → Writing custom shellcode, avoiding bad characters.  
3️⃣ **Bypassing Protections** → ASLR, DEP, Stack Canaries, CFG.  
4️⃣ **Privilege Escalation** → Kernel exploits, token manipulation.  
5️⃣ **Real-World Exploits** → CVE analysis, 0-day research.  

Once you **master exploitation**, you can dive into **malware development** and write undetectable payloads.  

👀 **But if your goal is to write persistent malware immediately**, you **can** start with maldev **and learn exploitation later**. **Just know that your malware will be basic at first.**  

💀 **So, do you wanna start breaking systems first or hiding inside them?** 😏
