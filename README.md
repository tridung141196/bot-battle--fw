#BATTLE BOT

Repo này sẽ bao gồm firmware điều khiển robot ( dựa trên chip NRF51822 trên Board VBLUno) với [mbed-cli](https://github.com/ARMmbed/mbed-cli).

#THIẾT LẬP MÔI TRƯỜNG

Để build được ứng dụng, trước hết ta phải cài đặt các phần mềm như:
* [CMake](http://www.cmake.org/download/).
* [mbed-cli](https://github.com/ARMmbed/mbed-cli). Please note that **mbed-cli has its own set of dependencies**, listed in the installation instructions.
* [Python](https://www.python.org/downloads/).
* [ARM GCC toolchain 4.9.x](https://launchpad.net/gcc-arm-embedded/+milestone/4.9-2015-q3-update).
* Giả lập serial monitor (ví dụ screen, pySerial

#HARDWARE

Về hardware chúng ta sẽ sử dụng board [VBLUno](https://vngiotlab.github.io/vbluno/vi/mydoc_introduction_vi.html). do VNG sản xuất dựa trên chip NRF51822 của NORDIC.

#BUILD VÀ TEST CHƯƠNG TRÌNH

__Để build một chương trình dựa trên Mbed OS ta thực hiện các bước như sau:__
1. CLone về dự án ( ví dụ như bot battle)

	```
	$git clone https://dungvt@git.nanochip.vn/iot/bot-battle-fw.git
	```

Hoặc là bạn có thể tạo một dự án mẫu bằng lênh

	```
	$mbed new tenduan	
	```

2. Di chuyển đến thư mục chứa dự án.

	```
	cd bot-battle-fw
	```

3. Update source tree

	```
	mbed update
	```

4. Thiết lập toolchain mặc định

	```
	mbed toolchain GCC_ARM
	```

5. Thiết lập target mặc định ( lựa chọn chip)

Chương trình sẽ chạy trên chip NRF51822 nên target sẽ chọn là **NRF51_DK**

	```
	mbed target NRF51_DK
	```

6. Compile chương trình

	```
	mbed compile
	```

7. Sau khi Compile thành công, file HEX được tạo sẽ nằm trong thư mục có cấu trúc như sau: ```BUILD/<target-name>/<toolchain>```

Trong project này thư mục sẽ là ```BUILD/NRF51_DK/GCC_ARM```

**Lưu ý** Tùy vào target và file được tạo có thể là file `.HEX` hoặc `.BIN`

8. Kết nối Board VBLUno với máy tính bằng cổng COM. Thư mục DAPLINK sẽ xuất hiện và bạn chỉ cần copy file HEX vừa được tạo vào trong thư mục đó để nạp chương trình

9. Reset lại chip.
