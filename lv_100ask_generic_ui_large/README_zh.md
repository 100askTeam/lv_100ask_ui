# 用法

1. 将`lv_100ask_generic_ui_large` 目录放在你的项目中的 `lvgl` 库目录的旁边。

与 `lv_conf.h` 类似，此仓库示例也有一个配置文件。它就是`lv_100ask_generic_ui_large_conf.h`。

1. 将 `lv_100ask_generic_ui_large/lv_100ask_generic_ui_large_conf_template.h` 复制到 `lv_100ask_generic_ui_large` 目录旁边
2. 将 `lv_100ask_generic_ui_large_conf_template.h` 重命名为 `lv_100ask_generic_ui_large_conf.h`
3. 将重命名之后的 `lv_100ask_generic_ui_large_conf.h` 中的第一个 `#if 0` 更改为 `#if 1` 以启用文件的内容
4. 启用或禁用具体的课堂示例
5. 在main函数引用启动的示例的接口函数
6. 编译运行，即可查看效果。

# 演示视频

TODO
