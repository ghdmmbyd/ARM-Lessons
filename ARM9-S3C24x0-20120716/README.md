﻿
课程安排
=========

# day1:
	1. 开发环境搭建

	2. 芯片手册导读

	3. GPIO 控制器

# day2:
	4. CLOCK 时钟管理

	5. UART 控制器

# day3:
	6. SDRAM 控制器

	7. NandFlash 控制器

# day4:
	项目实践1 - Bootloader 启动代码

# day5:
	8. Exception 异常处理

	9. Interrupt 控制器

	10. PWM Timer 定时器

# day6:
	11. DMA 控制器

	12. LCD 控制器

	13. BMP 图片显示

# day7:
	14. IIS 音频总线

	15. 音频芯片驱动 UDA1341

	16. WAV 音频播放

# day8:
	项目实践2 - DPF 数码相框



ARM 课程笔记
=============

day1: 
======

#1 环境搭建

## 网络设置
	ip 地址设置为自动分配	
	测试网关 ping 192.168.1.1 
	测试外网 ping 8.8.8.8 （域名）
	测试网页 chrome www.google.com
	测试共享 \\192.168.1.100
	测试 VNC 桌面共享： 	192.168.1.100 有线 akaedu	
				192.168.1.131 无线 123

## 凌波多媒体教学软件
	\\192.168.1.100\tools\lingbo6.94
	安装学生端，应用破解补丁
	连接教师机，远程查看桌面

## 课程安排介绍
	8天 * 7.5学时 = 60 学时
	班长发言

## 工具安装
	ADS1.2
		1. 选择 full （不是 typical） 包含 pdf 目录
		2. install licence from file (browser zip)
			选择 LICENSE.DAT
		成功之后运行 CodeWarrior 代码战士，看到 IDE 就是成功
		启动 cmd 窗口，运行 armcc
		path 命令查看安装路径: C:\Program Files\ARM\ADSv1_2\bin
	FoxitReader
	Ultraedit
	make.exe
	H-jtag.exe
	Terminal.exe
	Git for windows

## 开发板连接和测试
	开发板测试
		2440 
			有7段数码管和 8个LED灯
			串口线 （双母头）
		2410 
			没有7段数码管和 8个LED灯
			      
		1. 电源线 5v 内正+ 外负- 
		2. 串口线 公口，母口
		3. 并口线 + Jtag 小板子
		4. 计算机 pc 的并口
		
	测试成功： 串口有输出 115200
	如果不成功： 	1. 波特率有问题 改为 19200 
			2. 程序错误，改为用 Jtag 工具检测
				安装 H-JTAG V0.9.2.EXE

	H-Jtag 测试
		检测 CPU -> ARM920T
		烧写 bin -> S3C2440/S3C2410 + K9F1208 -> RAM at 0 -> Plain Binary
	
	串口测试
		uboot 3秒之内按回车键，进入到 shell
		vivi 3秒之内按空格键，进入到 shell

	USB转串口	usbtoserial product 

	超级终端 （Terminal） hypertrm.exe (dll)
		1. 波特率 115200
		2. 数据流控制 无

## bin 文件的下载和执行
	### 2440.bin
	AKAE2440 # loadb
	## Ready for binary (kermit) download to 0x32000000 at 115200 bps...
	## Total Size      = 0x00000058 = 88 Bytes
	## Start Addr      = 0x32000000
	AKAE2440 #
	AKAE2440 # go 0x32000000
	## Starting application at 0x32000000 ...


	### 2410.bin
	vivi> load
	invalid 'load' command: too few(many) arguments
	Usage:
	  load <flash|ram> [ <partname> | <addr> <size> ] <x|y|z|t>
	vivi> load ram 0x32000000 128 x	
	AKAE2440 # loadb
	## Ready for binary (kermit) download to 0x32000000 at 115200 bps...
	## Total Size      = 0x00000058 = 88 Bytes
	## Start Addr      = 0x32000000
	AKAE2440 #
	AKAE2440 # go 0x32000000
	## Starting application at 0x32000000 ...

#2 芯片手册导读

## 福昕阅读器工具使用技巧
	书签功能
	ctrl + 滚轮 实现放大缩小
	ctrl + f 搜索功能
	图形标注工具 (菜单 -> 视图 -> 工具栏 下面)
	
## 硬件原理图介绍
	核心板
	SoC: s3c2440 (289-pin)
	NandFlash： K9F1208 (48-pin)	64M (NorFlash)
	SDRAM:  K4S561632 (2chips) 32M * 2 = 64M 
	Y2: 12M 外部时钟供给SoC
	
	底板
	J1 J2 插头
	LED0 - LED7 8个 LED 灯
	Beep 蜂鸣器
	SW1 - SW4 用户按键 + Reset 键
	4 位拨码开关
	SEG7 7段数码管
	MAX3232 电平转换，给串口用 5v - 15v
	UART0, UART1 串口
	USB (J8, J7)
	SD-CARD (J10)
	CS8900A (U7) 网卡
	CH7005C (U16) VGA 转换芯片 -> VGA 接口
	UDA1341 (U8) 声卡芯片 -> 耳机接口
	IMP811 (U14) 复位芯片 -> 输出reset信号
	JTAG (J19) Jtag接口(14/20针) (TDI/TDO, TCK, TMS, TRST)
	CPLD (U15) 可改变硬件连接逻辑
	IDE (J15) 40针 
	LCD (J12) 50针
	
#3 基本开发流程
	make.exe - copy it to c:\program files\arm\adsv1_2\bin
	armcc test.c -o test.axf
	fromelf -bin test.axf -o test.bin
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	







