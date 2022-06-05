#include <glib.h>
#include <unistd.h> 
#include <sys/syscall.h>

GMainLoop* loop;

gint counter = 10;
gboolean callback(gpointer arg)
{
    pid_t tid = syscall(__NR_gettid);
    g_print("current thread id is %d\n", tid);
    g_print(".");
    if(--counter ==0){
        g_print("\n");
        //退出循环
        g_main_loop_quit(loop);
        //注销定时器
        return FALSE;
    }
    //定时器继续运行
    return TRUE;
}

int main(int argc, char* argv[])
{
    pid_t tid = syscall(__NR_gettid);
    g_print("current thread id is %d\n", tid);

    // g_thread_init是必需的，GMainLoop需要gthread库的支持。
    if(g_thread_supported() == 0)
        g_thread_init(NULL);
    // 创建一个循环体，先不管参数的意思。
    g_print("g_main_loop_new\n");
    loop = g_main_loop_new(NULL, FALSE);

    g_timeout_add(100,callback, NULL);

    // 让这个循环体跑起来
    g_print("g_main_loop_run\n");
    g_main_loop_run(loop);

    // 循环运行完成后，计数器减一
    // glib的很多结构类型和c++的智能指针相似，拥有一个计数器
    // 当计数器为0时，自动释放资源。
    g_print("g_main_loop_unref\n");
    g_main_loop_unref(loop);
    return 0;
}