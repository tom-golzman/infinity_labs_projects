#ifndef __ILRD_EXT2_H__
#define __ILRD_EXT2_H__

/************************************ Includes *************************************/


/************************************ Functions ************************************/
int EXT2PrintSuperblock(const char* device_path_);

int EXT2PrintGroupDesc(const char* device_path_);

int Ex2PrintFileContentInRoot(const char* device_path_, const char* file_name_);

int Ex2PrintFileContent(const char* device_path_, const char* file_path_);

#endif /*__ILRD_EXT2_H__*/