#ifndef __MOMOS_MACROS__
#define __MOMOS_MACROS__

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdio.h>
#include <functional>
#include <map>
#include <string>
#include <list>

/* -------------------------------------------------------------------------- */
/*                              Argument Helpers                              */
/* -------------------------------------------------------------------------- */

#define INT_ARG *static_cast<int *>(arg)
#define FLOAT_ARG *static_cast<float *>(arg)
#define DOUBLE_ARG *static_cast<double *>(arg)

/* -------------------------------------------------------------------------- */
/*                               Component Base                               */
/* -------------------------------------------------------------------------- */

typedef void(RunFunction_t)(void *, void *);

struct ComponentInfo
{
    RunFunction_t *run;
};

class ComponentRegistry
{
private:
    std::map<std::string, ComponentInfo> components;

public:
    static ComponentRegistry *getInstance(void)
    {
        static ComponentRegistry instance;
        return &instance;
    }

    ComponentInfo *createAndRegister(std::string name, RunFunction_t *run)
    {
        return &(this->components[name] = {.run = run});
    }

    void runComponent(std::string name, void *arg, void *out)
    {
        auto component = this->components.find(name);

        if (component != this->components.end())
        {
            component->second.run(arg, out);
        }
    }
};

/* -------------------------------------------------------------------------- */
/*                              Component Macros                              */
/* -------------------------------------------------------------------------- */

/* ---------------------------------- State --------------------------------- */

#define STATE_NAME __state
#define STATE_KEY "__state"
#define STATE_GET() STATE_NAME::getState()
#define STATE(T)                                                                             \
    class STATE_NAME                                                                         \
    {                                                                                        \
    private:                                                                                 \
        static ComponentInfo *info;                                                          \
                                                                                             \
        static T readState(void);                                                            \
                                                                                             \
    public:                                                                                  \
        static void run(void *arg, void *out)                                                \
        {                                                                                    \
            *(T *)out = readState();                                                         \
        }                                                                                    \
                                                                                             \
        static T getState(void)                                                              \
        {                                                                                    \
            T state;                                                                         \
            ComponentRegistry::getInstance()->runComponent(STATE_KEY, NULL, (void *)&state); \
            return state;                                                                    \
        }                                                                                    \
    };                                                                                       \
                                                                                             \
    ComponentInfo *STATE_NAME::info = ComponentRegistry::getInstance()->createAndRegister(   \
        STATE_KEY,                                                                           \
        STATE_NAME::run);                                                                    \
                                                                                             \
    T STATE_NAME::readState(void)

/* ---------------------------------- Hooks --------------------------------- */

#define HOOK_NAME(x) __hook_##x
#define HOOK_KEY(x) "__hook_" #x
#define HOOK_RUN(x) ComponentRegistry::getInstance()->runComponent(HOOK_KEY(x), NULL, NULL)
#define HOOK(x)                                                                              \
    class HOOK_NAME(x)                                                                       \
    {                                                                                        \
    private:                                                                                 \
        static ComponentInfo *info;                                                          \
                                                                                             \
    public:                                                                                  \
        static void run(void *arg, void *out);                                               \
    };                                                                                       \
                                                                                             \
    ComponentInfo *HOOK_NAME(x)::info = ComponentRegistry::getInstance()->createAndRegister( \
        HOOK_KEY(x),                                                                         \
        HOOK_NAME(x)::run);                                                                  \
                                                                                             \
    void HOOK_NAME(x)::run(void *arg, void *out)

/* ------------------------------- Transitions ------------------------------ */

#define TRANSITION_NAME(a, b) __transition_##a##_##b
#define TRANSITION_KEY(a, b) "__transition_" #a "_" #b
#define TRANSITION_RUN(a, b, arg) ComponentRegistry::getInstance()->runComponent(TRANSITION_KEY(a, b), arg, NULL)
#define TRANSITION(a, b)                                                                              \
    class TRANSITION_NAME(a, b)                                                                       \
    {                                                                                                 \
    private:                                                                                          \
        static ComponentInfo *info;                                                                   \
                                                                                                      \
    public:                                                                                           \
        static void run(void *arg, void *out);                                                        \
    };                                                                                                \
                                                                                                      \
    ComponentInfo *TRANSITION_NAME(a, b)::info = ComponentRegistry::getInstance()->createAndRegister( \
        TRANSITION_KEY(a, b),                                                                         \
        TRANSITION_NAME(a, b)::run);                                                                  \
                                                                                                      \
    void TRANSITION_NAME(a, b)::run(void *arg, void *out)

#endif // __MOMOS_MACROS__
