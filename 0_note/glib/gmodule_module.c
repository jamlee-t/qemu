#include <glib.h>
#include <gmodule.h>

G_MODULE_EXPORT void m_helloworld() {
        g_print("Hello modular world!\n");
}