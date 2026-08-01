#include "Paths.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <climits>
#include <cstdlib>
#include <unistd.h>
#endif

namespace Paths {

static std::filesystem::path ComputeExecutableDir() {
#if defined(_WIN32)
    // GetModuleFileNameW luon tra ve duong dan DAY DU cua chinh file .exe
    // dang chay, bat ke ban goi no tu dau (double-click, terminal, VS
    // debug, shortcut...). Day chinh la cai ta can, khac voi CWD.
    wchar_t buffer[MAX_PATH];
    DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    if (length == 0 || length == MAX_PATH) {
        // That bai bat thuong (rat hiem) -> danh du CWD lam phuong an cuoi.
        return std::filesystem::current_path();
    }
    std::filesystem::path exePath(buffer);
    return exePath.parent_path();
#else
    // Phuong an cho cac he dieu hanh khac Windows (vd: khi ban syntax-check
    // hay build thu tren Linux/macOS). Du an chinh thuc chi target Windows
    // + VS2026 nhu README da ghi, nhung de code van compile duoc noi khac.
    char buffer[PATH_MAX];
    ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (length != -1) {
        buffer[length] = '\0';
        return std::filesystem::path(buffer).parent_path();
    }
    return std::filesystem::current_path();
#endif
}

const std::filesystem::path& ExecutableDir() {
    // static local -> chi tinh 1 lan duy nhat, lan sau goi lai dung ket qua
    // da cache (thread-safe theo chuan C++11 tro len).
    static const std::filesystem::path dir = ComputeExecutableDir();
    return dir;
}

std::filesystem::path Resolve(const std::string& relativePath) {
    return ExecutableDir() / relativePath;
}

} // namespace Paths
