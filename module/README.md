### Modules For SysyPlus Compiler

编译方法：
```shell
mkdir build && cd build
cmake ..
make
make install
```

编译完成后，库存放在module/lib内，为当前编译架构的目标文件

使用方法：

```shell
g++ sysyplus源文件 xxx.module -o 可执行文件路径
```