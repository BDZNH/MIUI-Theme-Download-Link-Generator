[English](readme.md)

# 生成小米主题下载链接
![无标题.jpg](https://i.loli.net/2018/09/09/5b94e4dbedde9.jpg)
-----
如果你想自己编译的话，你需要 [libcurl](https://curl.haxx.se/download.html) 和 [jsoncpp](https://github.com/open-source-parsers/jsoncpp) ,并且编译这两个库的时候相同的编译参数。
 - 对于Release版本，在运行库选项中你需要使用`多线程 DLL (/MD)`
 - 对于Debug版本，在运行库选项中你需要使用`多线程调试 DLL (MDd)`

-----
# 依赖
 - [imgui](https://github.com/ocornut/imgui)(with glfw and opengl)
 - [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
 - [libcurl](https://curl.haxx.se/download.html)

 -----
 如果你遇到一些奇怪的显示问题，比如下图这样的
 [![无标题3.jpg](https://i.loli.net/2018/08/20/5b7ad04a38da3.jpg)](https://i.loli.net/2018/08/20/5b7ad04a38da3.jpg)
尝试删除同级目录的文件`imgui.ini`。