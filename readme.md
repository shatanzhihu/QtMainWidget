1. ~~本例程用于演示接口的使用方法，基于QT5.5.0开发，使用之前请先安装QT5.5.0或以上版本。~~
   本工程使用的是QT5.12.0.

2. 本例程使用MV Viewer版本为2.1.2_Build20180323，使用之前请先安装2.1.2_Build20180323版本MV Viewer

3. 本例程不包含修改相机的IP，使用之前请先用MV Viewer修改相机的IP到网卡所在的网段。

4. 目录结构如下

	```
	/ShowImage
		├ Bin      ： ImageConvert.dll用于图像格式转换，VideoRender.dll用来显示图像。
		│
		├ Include  ： 头文件
		│   ├ Media
		│   ├ GenICam     : SDK库的头文件。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Include中)
		│   ├ Infra       ：SDK库的头文件。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Include中)
		│   └ Memory      ：SDK库的头文件。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Include中)
		│
		├ Depends  ：  编译时依赖库
		│   ├ ImageConvert.lib
		│   ├ VideoRender.lib  
		│   └ MVSDKmd.lib     ： SDK库。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Lib中)
		│
		├ Src
		│   ├ Dahua.h         ： 客户需要的适配层接口类
		│   ├ dahua.cpp       ： 客户需要的适配层接口类
		│   ├ MessageQue.h    ： 帧缓存队列数据结构（必需！）
		│   ├ main.cpp        ： <例程>为了演示接口的使用方法而提供的，实际使用接口时不需要包含
		│   ├ form.h          ： <例程>为了演示接口的使用方法而提供的，实际使用接口时不需要包含
		│   ├ form.cpp        ： <例程>为了演示接口的使用方法而提供的，实际使用接口时不需要包含
		│   ├ form.ui         ： <例程>为了演示接口的使用方法而提供的，实际使用接口时不需要包含
		│   └cammerwidget.ui  ： <例程>为了演示接口的使用方法而提供的，实际使用接口时不需要包含
		│
		└ ShowQImage.pro

	```
5. 接口使用时需要注意顺序。

	1. 先发现相机（CameraCheck），再连接相机（CameraOpen），连接相机以后才可以修改曝光和增益、改变触发方式。
	2. 开始采集（CameraStart）以后才可以显示图像，否则缓存队列为空。

6. 注意事项。
	1. 默认连接的是发现列表中的第一个相机。
	2. ShowImage接口没有获取图像功能（图像在回调函数中获取），只做显示用，需要给它传入一个指针pRgbFrameBuf，该指针指向要显示的图像数据。
	3. 目前工程只支持Mono8和RGB24图像数据转化成QImage，Mono8不需要转码可以直接生成QImage,其余格式通过ImageConvert库先转换成RGB24再生成QImage
	4. Sample中通过QLabel显示图像，QLabel只支持QPixmap。所以先将QImage转换为QPixmap，转换会造成图像显示延迟。
	5. 如何生成QImage请参照dahua.cpp中的ShowImage函数， 非mono8格式转码为RGB24请参照dahua.cpp中的DisplayThreadProc函数

<center> END </center>

----

### 未实现功能

1. 参数修改。自动提取识别到相关的参数信息。并将参数写进到配置文件中。能够**保存**每一本书的参数信息，能够修改没本书的参数信息
2. 参数的自动拟合，自动修正功能。能够自动修正参数信息，提供给用户是否要自动修复参数。
3. 

----

### 2020.03.14 总结一下已经实现的工作：打开相机、显示图像这些功能已经在原始的代码中实现。
- 新添加的功能有：将相机改为硬件除法、并将图像显示在label上，label_2显示图书部分，Label_3显示是否有误。
- 添加了识别部分的代码，已经测试功能正常。
- 新加了LCD显示屏显示数量，但是效果不好，想着改为label显示
- 添加了数据库，不知道能否正确实现增删改查功能，不知道如何查看

### 20200316周：要实现自动化训练部分所需要的脚本代码，能实现一键训练功能。具体的功能如下：

- [ ] 1. 新书拍照。在菜单中添加拍照的菜单，实现将实时视频流保存为图片。
	- [x] 保存在指定位置。(位置移动)
- [x] 2. 新书截图。借助之前的画模板的程序
	- [x] 在新书图片截取图书，并将图书位置等相关信息输出。对新书图片裁剪。
	- [x] 新书的批量复制（代码已实现，具体位置之后再定）
	- [x] 图片文件进行处理：调整亮度实现多张照片的效果。
	- [x] 图片文件移动到相应的文件夹中
- [x] 3. 图书标记。
	- [x] 调用打标签的程序打标签
	- [ ] 标签文件的修改和训练数据的产生（程序在新机器上）
	- [x] 标签的批量复制
	- [x] 标记文件移动到相应的文件夹中
- [x] 4. 修改训练配置文件
- [x] 5. 训练模型。**代码已实现，但是未运行测试**
- [ ] 6. 得到的模型移动到指定位置？或者采用final weight？(确定之后在配置文件中确定此项)

照片和标记文件的移动和复制代码在F:\\code\\python\\WechatPicTrans\\copyfile.py
 <!--<u>**训练模块完成**</u>-->

### 20200407周：
- 实现了参数调整窗口。在主窗体菜单栏点击菜单选项可以打开参数设置窗口，并将现在使用的参数传递到参数设置窗口中，参数修改完成后可以将参数保存。
  修改的参数包括黑色小块的个数N，线性拟合的三个参数k/b/s。
- 实现了出现错误时的三种报警信息：声音报警、弹窗报警、气缸弹书。识别到错误时，会发出错误信号，执行错误指令。
- 添加了日常使用菜单。包括参数调整，图书位置框选。
- 将配置信息保存在配置文件中

参数的自动化调整？区间阈值标准参数等。


## 


## 使用手册

### 日常使用
    点击打开摄像头->开始识别，开始识别使用

### 新书训练
新书训练按照新书训练主菜单中的顺序依次点击完成即可
![1](Img/1.png)

1. 新书拍照。把书放到传送带上，**摆正**，（添加一个仿射变换？）然后点击拍照。
2. 新书裁剪。新书裁剪菜单中先打开裁剪窗口，框选图书部位，然后导出模板，关闭窗口。之后再点击裁剪图片菜单，完成图片裁剪。
3. 新书标记。点击新书标记裁剪，打开标记软件，框选中书中的每一个黑色标记，尽量准确。保存标记文件，完成标记工作。
4. 新书图像和标记文件的批量生成，位置移动，只需点击其他准备工作按钮即可。
5. 开始训练，点击此按钮后开始学习新的图书。此过程大概需要一个小时左右。