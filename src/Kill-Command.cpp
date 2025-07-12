#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int main() {
    DWORD drivesMask = GetLogicalDrives();  

    if (drivesMask == 0) {
        printf("Error: Could not retrieve drive list (Error %lu)\n", GetLastError());
        return 1;
    }

    printf("Available drives in Windows:\n");
    printf("---------------------------\n");

    for (char drive = 'A'; drive <= 'Z'; ++drive) {
        if (drivesMask & (1 << (drive - 'A'))) {
            TCHAR drivePath[] = { drive, ':', '\\', '\0' };
            TCHAR volumeName[MAX_PATH + 1] = { 0 };

            // Get additional drive info
            DWORD serialNumber = 0;
            DWORD maxComponentLen = 0;
            DWORD fileSystemFlags = 0;
            TCHAR fileSystemName[MAX_PATH + 1] = { 0 };

            if (GetVolumeInformation(
                drivePath,
                volumeName,
                MAX_PATH,
                &serialNumber,
                &maxComponentLen,
                &fileSystemFlags,
                fileSystemName,
                MAX_PATH)) {

                printf("Drive %c:\\ \n", drive);
                printf("  Volume Name: %ls\n", volumeName);
                printf("  File System: %ls\n", fileSystemName);
                printf("  Serial Number: %lu\n", serialNumber);

                // Get drive type
                UINT driveType = GetDriveType(drivePath);
                const char* typeStr = "Unknown";
                switch (driveType) {
                    case DRIVE_FIXED:     typeStr = "Local Disk"; break;
                    case DRIVE_REMOVABLE: typeStr = "Removable"; break;
                    case DRIVE_CDROM:     typeStr = "CD-ROM"; break;
                    case DRIVE_REMOTE:   typeStr = "Network"; break;
                    case DRIVE_RAMDISK:   typeStr = "RAM Disk"; break;
                }
                printf("  Type: %s\n", typeStr);

                // Get free space
                ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;
                if (GetDiskFreeSpaceEx(
                    drivePath,
                    &freeBytes,
                    &totalBytes,
                    &totalFreeBytes)) {
                    printf("  Total Space: %.2f GB\n", 
                        (double)totalBytes.QuadPart / (1024 * 1024 * 1024));
                    printf("  Free Space: %.2f GB\n", 
                        (double)freeBytes.QuadPart / (1024 * 1024 * 1024));
                }
            } else {
                printf("Drive %c:\\ (No additional info available)\n", drive);
            }
            printf("---------------------------\n");
        }
    }

    return 0;
}