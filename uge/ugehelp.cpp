#include "ugehelp.h"

namespace uge {

    //+-----------------------------------
    //| 输出控制台
    //+-----------------------------------
	void Log(const char* format, ...)
	{
		char strBuffer[4096] = { 0 };
		va_list ap;
		va_start(ap, format);
		_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, format, ap);
		va_end(ap);
		std::cout << strBuffer << std::endl;

		OutputDebugString(strBuffer);
		OutputDebugString("\n");
	}

    //+-----------------------------------
    //| 保存BMP_
    //+-----------------------------------
	bool save_bmp(const char* out_filename,int width,int height,int data_size,byte * data_buffer,bool has16To32) {
        
        //bmp
        ugeBmp bmp;
        bmp.header.bfType = 0x4d42;
        bmp.header.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(bmp.colors) + data_size;
        bmp.header.bfReserved1 = 0;
        bmp.header.bfReserved2 = 0;
        bmp.header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(bmp.colors);

        bmp.info.biSize = sizeof(BITMAPINFOHEADER);
        bmp.info.biWidth = width;
        bmp.info.biHeight = height;
        bmp.info.biPlanes = 1;
        bmp.info.biBitCount = 8;
        bmp.info.biCompression = 0;
        bmp.info.biSizeImage = data_size;
        bmp.info.biXPelsPerMeter = 0;
        bmp.info.biYPelsPerMeter = 0;
        bmp.info.biClrUsed = 256;
        bmp.info.biClrImportant = 0;

        bmp.data = data_buffer;

        FILE *fp;
        fopen_s(&fp, out_filename, "wb");
        if (fp == nullptr)
        {
            return false;
        }

        fwrite(&bmp.header, sizeof(bmp.header), 1, fp);
        fwrite(&bmp.info, sizeof(bmp.info), 1, fp);
        fwrite(&bmp.colors, sizeof(bmp.colors), 1, fp);
        fwrite(bmp.data, bmp.info.biSizeImage, 1, fp);

        fclose(fp);

        return true;
	}

    //+-----------------------------------
    //| 打开文件读取到内存
    //+-----------------------------------
    bool open_file(const char* filename, void** ptr, DWORD* size)
    {
        //创建纹理
        const char* path = resoure_path(filename);
        const auto h_f = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, nullptr);
        if (h_f == INVALID_HANDLE_VALUE)
        {
            char buff[255] = {}, path[255] = {};
            GetCurrentDirectory(255, path);
            DWORD dw = GetLastError();
            sprintf_s(buff, "文件打开失败,代码:%d,路径:%s下未找到", dw, path);
            Log(buff);
            return false;
        }

        *size = GetFileSize(h_f, nullptr);
        *ptr = malloc(*size);
        if (!*ptr) {
            CloseHandle(h_f);
            return false;
        }

        if (ReadFile(h_f, *ptr, *size, size, nullptr) == 0) {
            CloseHandle(h_f);
            free(*ptr);
            return false;
        }
        CloseHandle(h_f);

        return true;
    }

    //+-----------------------------------
    //| 读取文件的绝对路径
    //+-----------------------------------
    const char* resoure_path(const char* filename)
    {
        std::string _app_path = "";
        {
            char tmp[MAX_PATH];
            GetModuleFileName(GetModuleHandle(nullptr), tmp, sizeof(tmp));

            size_t i;
            for (i = strlen(tmp) - 1; i > 0; i--) {
                if (tmp[i] == '\\') {
                    break;
                }
            }
            tmp[i + 1] = 0;
            _app_path = tmp;
        }

        static char tmp_filename_[_MAX_PATH];

        if (!filename) {
            strcpy_s(tmp_filename_, _app_path.c_str());
        }
        else if (filename[0] == '\\' || filename[0] == '/' || filename[1] == ':') {
            strcpy_s(tmp_filename_, filename);
        }
        else {
            strcpy_s(tmp_filename_, _app_path.c_str());
            strcat_s(tmp_filename_, filename);
        }

        for (auto i = 0; tmp_filename_[i]; i++) {
            if (tmp_filename_[i] == '/') {
                tmp_filename_[i] = '\\';
            }
        }
        return tmp_filename_;
    }
}