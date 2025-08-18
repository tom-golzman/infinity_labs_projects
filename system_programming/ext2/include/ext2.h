#ifndef __ILRD_EXT2_H__
#define __ILRD_EXT2_H__

/************************************ Includes *************************************/


/************************************ Functions ************************************/
int EXT2PrintSuperblock(const char* device_path);

int EXT2PrintGroupDesc(const char* device_path);

int Ex2PrintFileContentInRoot(const char* device_path, const char* file_name);

int Ex2PrintFileContent(const char* device_path, const char* file_path);

#endif /*__ILRD_EXT2_H__*/