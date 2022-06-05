// https://blog.csdn.net/huang987246510/article/details/90738137

#include <glib.h>
/*
函数打印标准输入中读到内容的长度
*/
gboolean io_watch(GIOChannel *channel,
                  GIOCondition condition,
                  gpointer data)
{
    gsize len = 0;
    gchar *buffer = NULL;

    g_io_channel_read_line(channel, &buffer, &len, NULL, NULL);

    if(len > 0)
        g_print("%d\n", len);

    g_free(buffer);

    return TRUE;
}

int main(int argc, char* argv[])
{
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);	// 获取一个上下文的事件循环实例，context为NULL则获取默认的上下文循环
    GIOChannel* channel = g_io_channel_unix_new(1);	// 将标准输入描述符转化成GIOChannel，方便操作

    if(channel) {
        g_io_add_watch(channel, G_IO_IN, io_watch, NULL);	
        // 将针对channel事件源的回调注册到默认上下文，告诉Glib自己对channel的输入（G_IO_IN）感兴趣
        // 当输入准备好之后，调用自己注册的回调io_watch，并传入参数NULL。
        g_io_channel_unref(channel);
    }

    g_main_loop_run(loop);	// 执行默认上下文的事件循环
    g_main_context_unref(g_main_loop_get_context(loop));
    g_main_loop_unref(loop);

    return 0;
}
