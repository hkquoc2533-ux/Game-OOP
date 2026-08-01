# Turn Base Tower — Đồ án OOP (25C08)

Khung sườn (skeleton) một game nhập vai đánh theo lượt (turn-based) bằng
**C++17 + SFML 3.1.0**, build bằng **CMake** trong Visual Studio Community 2026.

> ⚠️ Đây là bộ khung đã chạy được về mặt kiến trúc (menu → chọn nhân vật →
> bản đồ tháp → chiến đấu → pause/save), nhưng **chưa có hình ảnh/âm thanh
> thật** (đang vẽ hình chữ nhật thay cho sprite) và một số phần được đơn
> giản hoá (không có animation, chưa có sảnh thang máy đồ hoạ...). Bạn cần
> tự bổ sung asset và tinh chỉnh trong 5 ngày còn lại — xem mục "Việc cần
> làm tiếp" bên dưới.

## Cây thư mục

```
TurnBasedTower/
├── CMakeLists.txt              # cấu hình build, tìm SFML, copy assets/DLL
├── README.md                   # file này
├── assets/
│   ├── fonts/                  # bỏ file .ttf vào đây (xem README_SFML_Setup.md)
│   ├── images/                 # sprite nhân vật/quái sau này
│   └── sounds/                 # âm thanh sau này
├── saves/                      # nơi lưu game (tự tạo file slot0.txt..slot4.txt)
├── docs/                       # báo cáo, sơ đồ UML (bạn tự thêm)
└── src/
    ├── main.cpp                # điểm bắt đầu chương trình
    ├── Game.h / Game.cpp       # vòng lặp game, quản lý cửa sổ + scene hiện tại
    ├── interfaces/
    │   ├── IGameObject.h       # interface: update() + draw()
    │   └── IMoveable.h         # interface: move(), getPosition()/setPosition()
    ├── entities/
    │   ├── Character.h/.cpp    # lớp trừu tượng cho MỌI nhân vật người chơi
    │   ├── Warrior / Mage / Archer / Tank .h/.cpp   # 4 nhân vật (đa hình)
    │   ├── Enemy.h/.cpp        # quái thường (tầng 1-2)
    │   └── BossEnemy.h/.cpp    # quái to (tầng 3), kế thừa từ Enemy
    ├── scenes/
    │   ├── Scene.h                    # interface cho mọi màn hình
    │   ├── MainMenuScene.h/.cpp       # Play / Save / Quit
    │   ├── CharacterSelectScene.h/.cpp# chọn 1P/2P + chọn nhân vật
    │   ├── TowerMapScene.h/.cpp       # hiển thị tầng hiện tại, vào combat
    │   ├── CombatScene.h/.cpp         # màn hình đánh nhau theo lượt
    │   └── PauseScene.h/.cpp          # tạm dừng, lưu game, thoát
    └── systems/
        ├── CharacterType.h            # enum 4 loại nhân vật
        ├── CharacterFactory.h/.cpp    # tạo Character* từ CharacterType
        ├── GameSession.h              # dữ liệu dùng chung giữa các scene
        ├── SaveManager.h/.cpp         # lưu/đọc tối đa 5 slot (file text)
        ├── CombatManager.h/.cpp       # kiểm tra điều kiện thắng/thua
        └── UIButton.h                 # nút bấm tái sử dụng cho mọi scene
```

## Cách các nguyên lý OOP được thể hiện (dùng cho phần báo cáo)

| Nguyên lý | Ở đâu trong code |
|---|---|
| **Trừu tượng hoá (Abstraction)** | `IGameObject`, `IMoveable` (interface thuần ảo); `Character` và `Scene` là lớp cơ sở trừu tượng |
| **Kế thừa (Inheritance)** | `Warrior/Mage/Archer/Tank` kế thừa `Character`; `Enemy` kế thừa `Character`; `BossEnemy` kế thừa `Enemy`; mọi Scene kế thừa `Scene` |
| **Đa hình (Polymorphism)** | `basicAttack()` / `useSkill()` — mỗi nhân vật/quái tự định nghĩa cách đánh riêng; `Game` gọi `currentScene->update()/draw()` mà không cần biết đang ở scene nào; `BossEnemy::takeTurn()` ghi đè `Enemy::takeTurn()` |
| **Đóng gói (Encapsulation)** | `Character` giữ `currentHp`, `currentRage` ở `protected`, chỉ cho sửa qua `takeDamage()/heal()/gainRage()/spendRage()` |

Bạn nên tự vẽ **Class Diagram** (dựa đúng theo các file trên) và một
**Sequence Diagram** cho tình huống "người chơi bấm nút Chiêu 1 trong
CombatScene" (Người chơi → CombatScene → Character::useSkill() →
Character::takeDamage() → CombatScene kiểm tra thắng/thua) để nộp báo cáo.

## Việc cần làm tiếp (gợi ý lộ trình 5 ngày)

1. **Ngày 1** — Build được project trong VS2026, chạy ra cửa sổ đen với nút
   bấm chạy được (làm theo `README_SFML_Setup.md`). Test menu → chọn nhân
   vật → vào tầng 1 → đánh thắng/thua quái.
2. **Ngày 2** — Thay hình chữ nhật bằng sprite thật cho 4 nhân vật + quái
   thường + boss (load `sf::Texture` trong `setTexture()` đã có sẵn ở
   `Character`). Thêm animation đơn giản nếu còn thời gian.
3. **Ngày 3** — Hoàn thiện `TowerMapScene` thành bản đồ có hình ảnh cửa
   thang máy/cầu thang thay vì chỉ có nút bấm; hoàn thiện màn hình chọn
   slot Save/Load rõ ràng hơn (hiện `PauseScene` đang lưu quay vòng đơn giản).
4. **Ngày 4** — Chế độ 2 Player: kiểm thử kỹ luồng chọn nhân vật không
   trùng, chỉnh cân bằng số liệu sát thương/máu/nộ nếu thấy quá dễ/khó.
5. **Ngày 5** — Quay video giới thiệu, hoàn thiện báo cáo (đưa Class Diagram,
   Sequence Diagram, ảnh chụp màn hình vào `docs/`), đóng gói `TenNhom.zip`
   theo đúng quy định (báo cáo PDF + video + mã nguồn).

## Ghi chú quan trọng

- File `shattered-pixel-dungeon-windows.zip` bạn gửi kèm chỉ nên dùng để
  **tham khảo cách tổ chức code/asset của một game 2D**, không copy nguyên
  mã nguồn của nó vào đồ án (đề bài nghiêm cấm sao chép).
- `SaveManager` hiện lưu dạng file text đơn giản (`saves/slotN.txt`), đủ để
  đáp ứng "tối đa 5 save, có thể sao chép ra nhiều bản" (`SaveManager::copySlot`).
  Bạn có thể copy tay các file `.txt` này trong Windows Explorer để tạo bản
  sao, hoặc gọi `SaveManager::copySlot()` từ code.