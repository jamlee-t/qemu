
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

//实例结构体
typedef struct _MyDemo MyDemo;

struct _MyDemo {
    GObject parent;
    MyDemoPrivate *priv;
};

//类结构体
typedef struct _MyDemoClass MyDemoClass;

struct _MyDemoClass {
    GObjectClass parent_class;
};

GType my_demo_get_type(void) G_GNUC_CONST;//my_demo_get_type将会使用宏G_DEFINE_TYPE去实现(G_DEFINE_TYPE(MyDemo,my_demo,G_TYPE_OBJECT))，其中的my通常表示命名空间，demo表示对象名字，get_type为固定字段

MyDemo *my_demo_new(const gchar *name, gint64 age);

#ifdef __cplusplus
}
#endif