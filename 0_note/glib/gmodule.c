// http://blog.eikke.com/index.php/ikke/2005/05/18/gmodules_are_fun.html
#include <glib.h>
#include <gmodule.h>

/* A prototype of the function pointer we'll use */
/* void function(void) */
typedef void (*HelloWorldFunc)(void);

gint main(gint argc, gchar *argv[])
{
    /* We need:
     * - A handle to our module
     * - A pointer to the function we'll import
     * - Some helper strings */
    GModule *module = NULL;
    HelloWorldFunc hello = NULL;
    gchar *module_path = NULL, *curr = NULL;

    /* Check whether glib is compiled with module support */
    if (g_module_supported() == FALSE)
    {
        g_error("Modules not supported :(");
        return 1;
    }

    /* We need to figure out the path to our module. In our test case, this
     * is ".", so we want the current dir. */
    curr = g_get_current_dir();
    /* Create the path to the module. This function does quite a lot of
     * of things, check the GModule API. */
    module_path = g_module_build_path((const gchar *)curr, "module");
    /* Don't we love debugging? */
    g_debug("Module path: %s", module_path);

    /* Finally we're able to open the module. We want lazy symbol resolving.
     * This means we only want a symbol to be resolved if we request it.
     * Once more, see the API for more information. */
    module = g_module_open(module_path, G_MODULE_BIND_LAZY);

    /* Get rid of those helper strings */
    g_free(module_path);
    g_free(curr);

    /* Check whether the module was loaded successfully */
    if (module == NULL)
    {
        g_error("Unable to load module");
        return 1;
    }

    /* Load the symbol and assign it to our function pointer.
     * Check for errors */
    if (g_module_symbol(module, "m_helloworld", (gpointer *)&hello) == FALSE)
    {
        g_error("Unable to get function reference: %s", g_module_error());
        return 1;
    }

    /* Now we can call our funtion.
     * As you can see, we can call it as if it's a normal function.
     * Don't we love function pointers? */
    hello();

    /* We're nice citizens and close all references when we leave */
    if (g_module_close(module) == FALSE)
    {
        g_error("Unable to close module: %s", g_module_error());
        return 1;
    }

    return 0;
}