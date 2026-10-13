# Ray Tracer from Scratch

一个使用 C++17 从零实现的简单光线追踪器。当前场景包含漫反射球体、镜面球体、漫反射地面、点光源、平行光和渐变天空背景。

## 渲染效果

![光线追踪场景](images/render.png)

## 构建

需要安装 CMake 3.16 或更高版本，以及支持 C++17 的编译器。

```powershell
cmake -S . -B build
cmake --build build
```

## 运行

从仓库根目录运行程序，渲染结果会写入 `images/P3.ppm`：

```powershell
.\build\ray_tracer.exe
```

输出为 1280 × 720 的 PPM 图像。可使用支持 PPM 的图像查看器打开；`images/render.png` 是该渲染结果的 PNG 预览。

## 项目结构

- `main.cpp`：场景、相机、光照和逐像素渲染逻辑
- `data_structure/`：向量、颜色、射线和相机
- `objects/`：场景对象基类、球体和墙面
- `material/`：漫反射与镜面材质
- `light_source/`：点光源、平行光和环境光
- `images/`：渲染输出和预览图
