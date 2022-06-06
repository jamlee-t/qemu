// 暂时未编译验证

#include "object.h"

#define TYPE_ANIMAL "animal"

typedef struct AnimalClass {
    /*< private >*/
    ObjectClass parent_class;
} AnimalClass;

typedef struct Animal
{
    Object parent;
    int age;
} Animal;

// 初始化实例
static void animal_initfn(Object *obj) {
    Animal *animal = Animal(obj);
    animal->age = 10;
}


// JAMLEE: 如果 TypeInfo 的父类是抽象类，定义类型也不需要那么多函数了。
// 因为父类已经给了，所以就像 qobject.h 中注释的例子那样
// 参考例子：
// static const TypeInfo device_type_info = {
//     .name = TYPE_DEVICE,
//     .parent = TYPE_OBJECT,
//     .instance_size = sizeof(DeviceState),
//     .instance_init = device_initfn,
//     .instance_post_init = device_post_init,
//     .instance_finalize = device_finalize,
//     .class_base_init = device_class_base_init,
//     .class_init = device_class_init,
//     .abstract = true,
//     .class_size = sizeof(DeviceClass),
// };
static const TypeInfo  animal_info = {
     .name = TYPE_ANIMAL,
     .parent = TYPE_OBJECT,
     .class_init = NULL,
     .instance_init = animal_initfn,
     .instance_post_init = NULL,
     .instance_finalize = NULL,
     .instance_size = sizeof(Animal),
     .class_size = sizeof(AnimalClass),
     .abstract = false,
};

static void animal_register_types(void)
{
     type_register_static(&animal_info);
}
type_init(animal_register_types)