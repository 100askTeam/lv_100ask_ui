# 用法

1. 将`lv_100ask_generic_ui_small` 目录放在你的项目中的 `lvgl` 库目录的旁边。与 `lv_conf.h` 类似，此仓库示例也有一个配置文件。它就是`lv_100ask_generic_ui_small_conf.h`。
2. 将 `lv_100ask_generic_ui_small/lv_100ask_generic_ui_small_conf_template.h` 复制到 `lv_100ask_generic_ui_small` 目录旁边
3. 将 `lv_100ask_generic_ui_small_conf_template.h` 重命名为 `lv_100ask_generic_ui_small_conf.h`
4. 将重命名之后的 `lv_100ask_generic_ui_small_conf.h` 中的第一个 `#if 0` 更改为 `#if 1` 以启用文件的内容
5. 启用或禁用具体的模块功能
6. 在main函数调用入口接口函数
7. 编译运行，查看效果。

> 注意：一些页面使用到了 lv_lib_100ask 仓库，因此同时需要加入此仓库到你的工程当中。

lv_lib_100ask仓库地址：

- GitHub： [https://github.com/100askTeam/lv_lib_100ask](https://github.com/100askTeam/lv_lib_100ask)
- Gitee： [https://gitee.com/weidongshan/lv_lib_100ask](https://gitee.com/weidongshan/lv_lib_100ask)

# 演示视频

- YouTube: [https://youtu.be/ZNKxmB3X2gY](https://youtu.be/ZNKxmB3X2gY)
- B站：[https://www.bilibili.com/video/BV1Tw4m1k7C3](https://www.bilibili.com/video/BV1Tw4m1k7C3)
