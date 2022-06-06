#include <stdio.h>
#include <glib.h>
#include <glib-unix.h>
#include <stdio.h>
#include <glib-object.h>

#ifdef __cplusplus
extern "C" {
#endif

// 宏, 用于当前类型
#define MY_TYPE_DEMO (my_demo_get_type())
#define MY_DEMO(object) G_TYPE_CHECK_INSTANCE_CAST((object), MY_TYPE_DEMO, MyDemo)
#define MY_DEMO_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MY_TYPE_DEMO, MyDemoClass))
#define MY_IS_DEMO(object) G_TYPE_CHECK_INSTANCE_TYPE((object), MY_TYPE_DEMO))
#define MY_IS_DEMO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MY_TYPE_DEMO))
#define MY_DEMO_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), MY_TYPE_DEMO, MyDemoClass))

typedef struct MyDemoPrivate MyDemoPrivate;

struct MyDemoPrivate
{
    gchar *name;
    gint64 age;
};

// 实例结构体
typedef struct _MyDemo MyDemo;

struct _MyDemo {
    GObject parent;
    MyDemoPrivate *priv; // 定义私有字段。也就说实例的字段是在实例结构体中定义
};

// 类结构体
typedef struct _MyDemoClass MyDemoClass;

struct _MyDemoClass {
    GObjectClass parent_class;
};

GType my_demo_get_type(void) G_GNUC_CONST;//my_demo_get_type将会使用宏G_DEFINE_TYPE去实现(G_DEFINE_TYPE(MyDemo,my_demo,G_TYPE_OBJECT))，其中的my通常表示命名空间，demo表示对象名字，get_type为固定字段

MyDemo *my_demo_new(const gchar *name, gint64 age);

#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////


enum
{
    PROP_0,
    PROP_NAME,
    PROP_AGE,
    N_PROPERTIES
};

#define MY_DEMO_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), MY_TYPE_DEMO, MyDemoPrivate))

// 实例初始化和类初始化。G_DEFINE_TYPE 定义了过后，需要手动实现这两个函数。
static void my_demo_init(MyDemo *self);
static void my_demo_class_init(MyDemoClass *klass);

// 定义类型到类型系统
/*
 * G_DEFINE_TYPE 可以让 GObject 库的数据类型系统能够识别我们所定义的 MyDemo 类类型，
 * 它接受三个参数，第一个参数是类名，即 MyDemo；第二个参数则是类的成员函数名称的前缀，例如 my_demo_get_type 函数即为 MyDemo 类的一个成员函数，"my_demo"是它的前缀；第三个参数则指明 MyDemo 类类型的父类型为G_TYPE_OBJECT。
 * G_DEFINE_TYPE中会调用一个 g_type_register_static_simple 的函数，这个函数的作用就是将用户自己定义的类型注册到系统中，
 * G_DEFINE_TYPE还定义了2个函数（my_demo_init 和 my_demo_class_init），需要自己实现这两个函数。它们是对象的初始化函数，相当于c++的构造函数，第一个函数在每个对象创建的时候都会被调用，
 * 第二个函数只有在第一次创建对象的时候被调用(比如在调用g_type_class_ref的时候，如果class没有初始化，就会调用my_demo_class_init)
*/
G_DEFINE_TYPE (MyDemo, my_demo, G_TYPE_OBJECT);


static void my_demo_dispose(GObject *object);
static void my_demo_finalize(GObject *object);

/*
//以下代码为替换宏G_DEFINE_TYPE的另外一种对my_demo_get_type函数的实现方法，代码不好理解，不建议使用这种方法
static gpointer my_demo_parent_class = ((void*) 0);
static void     my_demo_class_intern_init(gpointer klass)
{
    my_demo_parent_class = g_type_class_peek_parent(klass);
    my_demo_class_init ((MyDemoClass*)klass);
}
GType my_demo_get_type (void)
{
    static volatile gsize g_define_type_id__volatile = 0;
    if (g_once_init_enter(&g_define_type_id__volatile))
    {
        GType g_define_type_id =
        g_type_register_static_simple (((GType)((20) << (2))),
                                       g_intern_static_string("MyDemo"),
                                       sizeof(MyDemoClass),
                                       (GClassInitFunc)my_demo_class_intern_init,
                                       sizeof(MyDemo),
                                       (GInstanceInitFunc)my_demo_init,
                                       (GTypeFlags)0);
        { {{};} }
        g_once_init_leave(&g_define_type_id__volatile, g_define_type_id);
    }
    return g_define_type_id__volatile;
}
*/

static void my_demo_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    MyDemo *self;

    g_return_if_fail(object != NULL);

    self = MY_DEMO (object);
    //self->priv = MY_DEMO_GET_PRIVATE(self);

    switch (prop_id)
    {
    case PROP_NAME:
        self->priv->name = g_value_dup_string(value);
        break;
    case PROP_AGE:
        self->priv->age = g_value_get_int64(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void my_demo_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    MyDemo *self;

    g_return_if_fail(object != NULL);

    self = MY_DEMO(object);

    switch (prop_id)
    {
    case PROP_NAME:
        g_value_set_string(value, self->priv->name);
        break;
    case PROP_AGE:
        g_value_set_int64(value, self->priv->age);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

// 类结构体初始化函数
static void my_demo_class_init(MyDemoClass *klass)
{
    printf("%s\n", __FUNCTION__);

    int signal_id = g_signal_new("broadcast_msg",
        my_demo_get_type(),/*G_OBJECT_CLASS_TYPE(kclass)*/
        G_SIGNAL_RUN_LAST,
        0,
        NULL,
        NULL,
        g_cclosure_marshal_VOID__STRING,
        G_TYPE_NONE,/*返回值，因为信号没有返回，所以为NONE*/
        1,/*参数数目*/
        G_TYPE_STRING/*参数类型*/
    );

    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = my_demo_finalize;
    object_class->dispose = my_demo_dispose;

    object_class->set_property = my_demo_set_property;
    object_class->get_property = my_demo_get_property;

    g_object_class_install_property(object_class,
                                    PROP_NAME,
                                    g_param_spec_string("name",
                                                      "name",
                                                      "Specify the name of demo",
                                                      "Unknown",
                                                      G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property(object_class,
                                    PROP_AGE,
                                    g_param_spec_int64 ("age",
                                                     "age",
                                                     "Specify age number",
                                                     G_MININT64, G_MAXINT64, 0,
                                                     G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    // 如果对struct MyDemoPrivate 里面添加了成员变量，则执行下面这行代码
    g_type_class_add_private(klass, sizeof(MyDemoPrivate));
}

//实例结构体初始化函数
static void my_demo_init(MyDemo *self)
{
    printf("%s\n", __FUNCTION__);
    self->priv = MY_DEMO_GET_PRIVATE(self);
}

static void my_demo_dispose(GObject *object)
{
    printf("%s\n", __FUNCTION__);
}

static void my_demo_finalize(GObject *object)
{
    printf("%s\n", __FUNCTION__);

    MyDemo *self;

    g_return_if_fail(object != NULL);

    self = MY_DEMO(object);

    if (self->priv->name)
        g_free (self->priv->name);

    G_OBJECT_CLASS(my_demo_parent_class)->finalize(object);
}

// 创建 MyDemo 对象
MyDemo *my_demo_new(const gchar *name, gint64 age)
{
    MyDemo *demo;

    // 创建对象
    demo = MY_DEMO(g_object_new(MY_TYPE_DEMO,
                                "name", name,
                                "age", age,
                                NULL));

    printf("get property name:%s, age:%ld\n", demo->priv->name, demo->priv->age);

    return demo;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
// 开始例子
//
/////////////////////////////////////////////////////////////////////////////////////////

static GMainLoop *loop = NULL;

static gboolean signal_handler(gpointer user_data)
{
    g_main_loop_quit(loop);
    return FALSE;
}

void handler_receive_msg(GObject *sender, char *name, gpointer data)
{
    MyDemo *demo = G_TYPE_CHECK_INSTANCE_CAST(sender, my_demo_get_type(), MyDemo);
    printf("handler: [%s] from [%s: %ld]\n", name, demo->priv->name, demo->priv->age);
}

int main(int argc, char* argv[])
{
#if !GLIB_CHECK_VERSION(2,32,0)
    if (! g_thread_supported ())
        g_thread_init (NULL);
#endif
#if !GLIB_CHECK_VERSION(2,36,0)
    g_type_init ();
#endif

    loop = g_main_loop_new(NULL, FALSE);

    g_unix_signal_add_full(G_PRIORITY_DEFAULT,
                                    SIGINT,
                                    signal_handler,
                                    NULL,
                                    NULL);
    
    // 创建 3 个 demo 对象
    int i;
    MyDemo *demos[3];

    // sizeof(数组名)和sizeof(指针) 是不一样的。sizeof 是整个数组的大小
    int len = sizeof(demos) / sizeof(demos[0]);

    // JAMLEE: 3 个 demo 对象。len 是 3
    for (i = 0; i < len; i++) {
        printf("construction demos[%d]\n", i);
        // 创建对象
        //demos[i]= g_object_new(MY_TYPE_DEMO, NULL);
        /*demos[i]= g_object_new(MY_TYPE_DEMO,
                               "name", "kobe",
                               "age", i,
                               NULL);*/
        gchar *name = g_strdup_printf("kobe%d", i);
        demos[i] = my_demo_new(name, i); // 创建对象 demos
        g_free(name);

        // 绑定信号
        g_signal_connect(demos[i], "broadcast_msg", G_CALLBACK(handler_receive_msg), NULL);

        // 发送信号
        g_signal_emit_by_name(demos[i], "broadcast_msg", "lixiang");
    }

    // 析构
    for(i = 0; i < len; i++){
        printf("destruction demos[%d]\n", i);
        g_object_unref(demos[i]);
    }

    g_main_loop_run(loop);
    g_main_loop_unref(loop);

    return 0;
}