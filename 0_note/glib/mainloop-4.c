// https://blog.csdn.net/huang987246510/article/details/90738137

#include <glib.h>                                       
                                                                                                                                                                                                               
typedef struct _MySource MySource;

/* 自定义事件源，继承自Glib的GSource类型*/
struct _MySource
{
    GSource _source;	// 基类
    GIOChannel *channel;
    GPollFD fd;
};

/*事件源回调函数，读出iochannel中的内容，打印其长度*/
static gboolean watch(GIOChannel *channel)
{
    gsize len = 0;
    gchar *buffer = NULL;

    g_io_channel_read_line(channel, &buffer, &len, NULL, NULL);
    if(len > 0)
    	g_print("filesize: %d\n", len);
    g_free(buffer);

    return TRUE;
}

/*
状态机prepare回调函数，timeout等于-1告诉poll如果IO没有准备好，一直等待，即阻塞IO
返回FALSE指示需要poll来检查事件源是否准备好，如果是TRUE表示跳过poll
*/
static gboolean prepare(GSource *source, gint *timeout)
{
    *timeout = -1;
    return FALSE;
}

/*
状态机check回调函数，检查自己感兴趣的fd状态（events）是否准备好
用户通过设置events标志设置感兴趣的fd状态（包括文件可读，可写，异常等）
revents是poll的返回值，由内核设置，表明fd哪些状态是准备好的
函数功能：
当感兴趣的状态和poll返回的状态不相同，表示fd没有准备好，返回FALSE，Glib不发起调度
反之返回TRUE，Glib发起调度
*/
static gboolean check(GSource *source)
{
    MySource *mysource = (MySource *)source;

    if(mysource->fd.revents != mysource->fd.events)
    	return FALSE;

    return TRUE;
}
/*
状态机dispatch回调函数，prepare和check其中只要有一个返回TRUE，Glib就会直接调用此接口
函数逻辑是执行用户注册的回调函数
*/
static gboolean dispatch(GSource *source, GSourceFunc callback, gpointer user_data)
{
    MySource *mysource = (MySource *)source;

    if(callback)
      callback(mysource->channel);

    return TRUE;
}
/*
* 当事件源不再被引用时，这个接口被回调
*/
static void finalize(GSource *source)
{
    MySource *mysource = (MySource *)source;

    if(mysource->channel)
      g_io_channel_unref(mysource->channel);
}

int main(int argc, char* argv[])
{
    GError *error = NULL;
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);	// 从默认上下文获取事件循环实例
    GSourceFuncs funcs = {prepare, check, dispatch, finalize};	// 声明用户定义的状态机回调
    
    // 自定义事件源
    /*
	Glib允许用户自己定义事件源，但需要把Glib的事件源作为"基类"，具体实现是把GSource
	作为自定义事件源的第一个成员，在创建事件源时传入状态机回调函数和自定义事件源的结构体大小
	*/
    GSource *source = g_source_new(&funcs, sizeof(MySource));
    MySource *mysource = (MySource *)source; // 强制转换为自定义事件源

	/*
	创建一个文件类型的GIOChannel，GIOChannel就是Glib对文件描述符的封装，实现其平台可移植性
	GIOChannel在所有Unix平台上都可移植，在Windows平台上部分可移植
	GIOChannel的fd类型可以是文件，pipe和socket
	*/
    if (!(mysource->channel = g_io_channel_new_file("README.md", "r", &error))) {
        if (error != NULL)
            g_print("Unable to get test file channel: %s\n", error->message);

        return -1;
    }
	/*获取GIOChannel的fd，放到GPollFD的fd域中*/
    mysource->fd.fd = g_io_channel_unix_get_fd(mysource->channel);
    /*设置感兴趣的文件状态，这里时文件可读状态*/
    mysource->fd.events = G_IO_IN;
    /*
    传给poll的文件描述符结构体
	struct GPollFD {
  		gint		fd;			// 文件描述符
  		gushort 	events;		// 感兴趣的文件状态
  		gushort 	revents;	// 返回值，由内核设置
	};
	*/
    g_source_add_poll(source, &mysource->fd);	// 将文件描述符添加到事件源中
    g_source_set_callback(source, (GSourceFunc)watch, NULL, NULL);	// 设置事件源的回调函数
    /* 
    设置事件源优先级，如果多个事件源在同一个上下文，这个事件源都准备好了，优先级高的事件源会被Glib优先调度
    */
    g_source_set_priority(source, G_PRIORITY_DEFAULT_IDLE);	
    g_source_attach(source, NULL);	//将事件源添加到Glib的上下文，此处上下文为NULL，表示默认的上下文
    g_source_unref(source);

    g_main_loop_run(loop);	// Glib开始执行默认上下文的事件循环

    g_main_context_unref(g_main_loop_get_context(loop));
    g_main_loop_unref(loop);

    return 0;
}
