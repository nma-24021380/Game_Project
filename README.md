# Adventure Card Game
> Game 2D turn-based thẻ bài.

## 🛠 Công Nghệ Sử Dụng
- **C++:**  Ngôn ngữ lập trình chính để phát triển game.
- **SDL2, SDL_image, SDL_mixer, SDL_ttf:**  Thư viện xử lý đồ họa 2D, sự kiện, âm thanh, font chữ.
- **Code::Blocks:**  IDE sử dụng để phát triển và debug project.
- **Git & GitHub:**  Lưu trữ source code.

## 🎯 Tính năng
- Điều khiển nhân vật di chuyển để tìm lối ra, trên đường đi sẽ gặp kẻ địch.
- Chạm vào kẻ địch sẽ chuyển sang chế độ chiến đấu sử dụng thẻ bài.
- Có thể ghép 2 thẻ cùng loại và cùng cấp sao thành thẻ có cấp sao cao hơn, thẻ 3 sao có hiệu ứng riêng.

## 🎮 Gameplay
![Di chuyển nhân vật, chạm vào kẻ địch](demo_images/start.png)
Di chuyển nhân vật (nhấn lên-xuống-trái-phải), khi chạm phải kẻ địch sẽ chuyển sang chế độ chiến đấu.<br>

![Bộ thẻ](demo_images/battle.png)
Nhân vật có 6 thẻ, có thể chọn/ghép thẻ (nhấn G để xem hướng dẫn), sau mỗi lượt chơi sẽ bổ sung thẻ mới vào những thẻ đã dùng.<br>

![Ghép thẻ](demo_images/card_merging.png)
Ghép 2 thẻ có cùng loại và cấp sao thành thẻ có cấp sao cao hơn (tối đa 3).<br>

![Hoàn thành trò chơi](demo_images/ending.png)
Đi đến cánh cửa để hoàn thành trò chơi.<br>

![Màn hình chiến thắng](demo_images/game_win.png)
Màn hình chiến thắng.<br>

![Màn hình thất bại](demo_images/game_over.png)
Màn hình thất bại khi nhân vật còn 0 HP.<br>

### 🃏 Danh sách Thẻ Bài

| Thẻ Bài | Ảnh | Mô Tả |
|:---|:---:|:---|
| **Attack** | ![Attack Card](assets/cards/attack_1.png) ![Attack Card](assets/cards/attack_2.png) | Gây sát thương lên kẻ địch.<br>⭐⭐ Tăng sát thương.<br>⭐⭐⭐ Chuyển thành thẻ Stun. |
| **Heal** | ![Heal Card](assets/cards/heal_1.png) ![Heal Card](assets/cards/heal_2.png) | Hồi máu cho bản thân.<br>⭐⭐ Tăng lượng hồi máu.<br>⭐⭐⭐ Chuyển thành thẻ Shield. |
| **Stun** | ![Stun Card](assets/cards/stun.png) | Gây thêm hiệu ứng choáng (không thể hành động trong turn tiếp theo).|
| **Shield** | ![Shield Card](assets/cards/shield.png) | Tạo lá chắn (chặn đòn đánh tiếp theo). |

## 🎬 Demo Gameplay
👉 [Xem video demo trên Google Drive](https://drive.google.com/file/d/1ChRQzgmRAo1bwS1PR8Dd4Y-GEEJxqCd-/view?usp=drive_link)

## 📑 Cập nhật
- 18/03/2025: Thêm adventure scene và battle scene (chạm vào kẻ địch sẽ chuyển scene sang "battle"), điều khiển nhân vật, thẻ đơn giản.
- 21/03/2025: Thêm các loại thẻ (attack & heal, thẻ attack 3 sao = thẻ debuff, thẻ heal 3 sao = thẻ shield (tránh 1 đòn đánh)), thêm cơ chế ghép thẻ, thêm bộ thẻ (tối đa 6 thẻ).
- 25/03/2025: Thêm animation cho nhân vật, render thanh máu và tên trong battle.
- 29/3/2025: Thêm cơ chế chọn thẻ.
- 12/04/2025: Thêm lại thẻ cho đủ 6 thẻ sau mỗi turn, người chơi có tối đa 3 lượt hành động (chọn/ghép thẻ).
- 13/04/2025: Thêm nút xác nhận (continue) và reset lựa chọn (cancel).
- 25/04/2025: Thêm hướng dẫn trong adventure và battle, animation cho kẻ địch, hiệu ứng khi dùng thẻ, thêm âm thanh; thêm quy tắc chọn thẻ đơn giản cho kẻ địch; khi bị đánh bại/hoàn thành game sẽ có lựa chọn chơi lại (từ đầu).

## 📚 Nguồn Tham Khảo
- **Nhạc nền và hiệu ứng âm thanh:** [OpenGameArt](https://opengameart.org)
- **Sprite nhân vật và kẻ địch:** [Ralsei](https://www.spriters-resource.com/fullview/110469/)
- **Sprite hiệu ứng tấn công:** [Explosion](https://opengameart.org/content/pixel-explosion-12-frames)
- **Font chữ:** [Font](https://www.fontspace.com/determination-mono-web-font-f23209)
- **Tham khảo cách set up và game loop:** [Let's Make Games](https://www.youtube.com/watch?v=QQzAHcojEKg&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx)
