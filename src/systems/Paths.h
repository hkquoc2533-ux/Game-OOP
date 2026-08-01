#pragma once
#include <filesystem>
#include <string>

// Tien ich lay duong dan tuyet doi den file/tro trong game, KHONG phu thuoc
// vao "thu muc lam viec hien tai" (current working directory) luc chay .exe.
//
// Van de truoc day: code dung duong dan tuong doi kieu "assets/..." hay
// "saves/...". Duong dan tuong doi nay duoc tinh tu CWD - CWD lai khac nhau
// tuy vao CACH ban chay chuong trinh (double-click trong out/build/...,
// chay tu terminal o thu muc goc, tao shortcut o Desktop, VS debug...).
// Vi vay chuong trinh "chi chay dung" khi dung trong thu muc out/ (noi
// CMake da copy san assets/saves toi) va loi khi chay tu noi khac.
//
// Cach sua: luon lay duong dan THU MUC CHUA FILE .exe (khong doi, du ban
// chay tu dau) bang GetModuleFileNameW tren Windows, roi ghep voi
// "assets/..." hay "saves/..." de ra duong dan TUYET DOI. Nho vay chuong
// trinh chay dung o bat ky thu muc nao, mien la assets/ va saves/ nam
// canh file .exe (CMakeLists.txt da tu dong copy chung toi do roi).
namespace Paths {

// Tra ve thu muc chua file .exe dang chay (duong dan tuyet doi, cache lai
// sau lan goi dau tien nen goi nhieu lan cung khong ton chi phi).
const std::filesystem::path& ExecutableDir();

// Ghep ExecutableDir() voi mot duong dan tuong doi, vd:
//   Paths::Resolve("assets/fonts/font.ttf")
//   Paths::Resolve("saves/slot0.txt")
std::filesystem::path Resolve(const std::string& relativePath);

} // namespace Paths
