#include "help.h"

namespace wzl {

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
    //| 打开文件读取到内存
    //+-----------------------------------
    void* open_file(const char* filename, int* size)
    {
        unsigned long _size = 0;
        void* data = nullptr;

        //创建纹理
        const char* path = resoure_path(filename);
        const auto h_f = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, nullptr);
        if (h_f == INVALID_HANDLE_VALUE)
        {
            char buff[255] = {}, path[255] = {};
            GetCurrentDirectory(255, path);
            DWORD dw = GetLastError();
            sprintf_s(buff, "文件打开失败,代码:%d,路径:%s\\%s", dw, path, filename);
            Log(buff);
            return nullptr;
        }

        _size = GetFileSize(h_f, nullptr);
        data = malloc(_size);
        if (!data) {
            CloseHandle(h_f);
            return nullptr;
        }

        if (ReadFile(h_f, data, _size, &_size, nullptr) == 0) {
            CloseHandle(h_f);
            free(data);
            return nullptr;
        }
        CloseHandle(h_f);

        if (_size)
        {
            *size = _size;
        }

        return data;
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

    //+-----------------------------------
    //| 获取资源的名称
    //+-----------------------------------
    std::string get_file_name(const char* filename)
    {
        char sz_name[_MAX_PATH];
        std::string name;

        strcpy_s(sz_name, filename);
        size_t len = strlen(filename);
        int j = 0;
        for (int i = 0; sz_name[i]; i++) {
            if (sz_name[i] == '/' || sz_name[i] == '\\') {
                j = i;
            }
        }
        name = &sz_name[j + 1];

        return name;
    }
}