# Smart Energy Manager Simulator
Dự án phát triển Hệ thống quản lý năng lượng thông minh.

---

## 🌟 Tổng quan về Dự án (Overview)
Đây là dự án mô phỏng quá trình hoạt động của một Trí tuệ Hệ thống Quản lí Điện Năng (Smart Energy System) trong một chu kỳ **24 Giờ**. 

Hệ thống sẽ chạy mô phỏng việc con người (User) ngẫu nhiên bật tắt các đồ điện dân dụng, qua đó đối soát với thuật toán an toàn điện và lượng Pin dự trữ hiện hành để quyết định có nên tự động can thiệp (ngắt bớt tải) hay không. Mọi tương tác theo dòng thời gian đều được lưu vết chi tiết ra định dạng log chuyên nghiệp và in báo cáo thống kê mức tiêu thụ ở cuối vòng mô phỏng.

## 🔄 Luồng Mô phỏng 24H (Workflow)
Hệ thống sẽ chạy một vòng lặp từ giờ thứ 1 đến giờ thứ 24, tại mỗi giờ sẽ diễn ra chung một kịch bản theo đúng thứ tự logic như sau:

1. `Nhân tố Hỗn loạn`: Cứ mỗi giờ, sự kiện tự nhiên (User) sẽ ra quyết định ngẫu nhiên vào việc Bật hoặc Tắt ngẫu nhiên 20% lượng thiết bị trong nhà.
2. `Nhân tố Kiểm soát`: Hệ thống (System) sau đó sẽ quét tổng thể 2 tình trạng là Mức Pin và Điện áp quá tải, qua đó gạt công tắc cấm (OFF) những thiết bị thuộc thứ tự kém ưu tiên.
3. `Cập nhật Dữ liệu`: Trừ lượng điện tiêu hao đi vào cục Pin dự phòng và cộng bảng thống kê ngốn điện của riêng các thiết bị đó.
4. `Sạc Pin định kỳ`: Sau mỗi 4 tiếng hoạt động giả lập, Nguồn sẽ tự động kích sạc đầy thêm 20% dung lượng bù vào Pin.

## 💡 Nền tảng thuật toán (Features logic)
* **Mode Pin (Mức dung lượng):** 
  * Gồm 4 mức: `HIGH` (> 60%), `MEDIUM`, `LOW`, `CRITICAL` (< 10%). Khi pin càng tụt hạng, thuật toán sẽ ép gạt tắt các đồ điện từ thứ hạn ưu tiên thấp nhất.
* **Mức độ Ưu Tiên Thiết bị (Priority Level):**
  * `ESSENTIAL` (P1 - Cốt lõi): Luôn được cấp điện (Như Tủ lạnh, Camera...).
  * `IMPORTANT` (P2 - Quan trọng): Sẽ bị ngắt khi rớt vào Mode Critical.
  * `NORMAL` (P3 - Bình thường): Sẽ bị hệ thống tắt khi pin vào Mode Low
  * `OPTIONAL` (P4 - Tùy chọn, Đèn trang trí...): Sẽ bị hệ thống chủ động ngắt ngay khi Pin rơi khỏi Mode High.
* **Chống Quá tải (Max Power Trigger):** Nếu lượng điện lôi cùng lúc của nhiều thiết bị vượt ngưỡng cho phép, hệ thống sẽ tắt dần các thiết bị từ yêu tiên thấp đến cao cho đến khi công suất trở lại mức an toàn

---

## 📂 Tổ chức mã nguồn & Khái Quát Vai Trò
Để đảm bảo cấu trúc, mã nguồn được chia theo tính Modul hóa (Component based). Những ai đọc code có thể truy xuất logic theo vai trò sau:

* **1. Thư mục mã logic Core (`src/`) và File liên kết Header (`inc/`)**
  * ⚙️ `main.c` -- Bộ điều khiển Timer, chạy file log gốc, kết xuất Báo Cáo.
  * ⚙️ `system.c` & `system.h` -- "Bộ Não" của quản lý điện. Chứa logic tính toán điện (Capacity, Mode), thuật toán tắt thiết bị khi quá tải, lưu trữ đếm luỹ kế tiêu thụ.
  * ⚙️ `device.c` & `device.h` -- Xử lý định dạng của Thiết bị mạng điện (Tên, W, Priority, State) và quét tài liệu từ file.
  * ⚙️ `user_sim.c` & `user_sim.h` -- Bộ tác nhân mô phỏng con người. Kịch bản ngẫu nhiên hóa giả lập việc dùng tùy hứng.
  * ⚙️ `logger.c` & `logger.h` -- Modul Ghi Nhật Ký. Nhận các chuỗi từ nơi khác ném về rồi nêm nếm Timestamp gọt format đẩy vào file `log.txt`.

* **2. Thư mục Tài liệu cung cấp (`docs/`)**
  * 📄 `devices.txt`: Cung cấp mảng linh kiện có sãn cho gia đình và mức tiêu thụ (W).
  * 📄 `system.txt`: Cung cấp Battery ban đầu và MaxPower giới hạn điện áp.

* **3. Thư mục kết xuất (`build/`)**
  * Thư mục tự thiết lập khi biên dịch xong dự án trên máy. Nơi chứa `.exe`

---

## 🚀 Hướng dẫn Biên dịch và Chạy (Local Execute)
### Bước 1: Biên dịch (Build)
Để ghép các file code hoạt động, mở Terminal tại thư mục gốc của dự án và gõ dòng lệnh sau:

```bash
gcc -Iinc src/*.c -o build/energy_manager.exe
```
Lệnh `gcc` sẽ quét toàn bộ code trong `src/` và nhả ra tệp chạy cuối cùng vào bên trong thư mục `build/`. 

### Bước 2: Chạy chương trình
**Trên máy tính Windows:**
```bash
.\build\energy_manager.exe
```

**Trên MacOS / Linux:**
```bash
./build/energy_manager
```

