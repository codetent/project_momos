#ifndef __MOMOS_MACROS__
#define __MOMOS_MACROS__

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <iostream>

#include <stdio.h>
#include <stdbool.h>
#include <functional>
#include <map>
#include <string>
#include <list>

/* -------------------------------------------------------------------------- */
/*                              Argument Helpers                              */
/* -------------------------------------------------------------------------- */

#define INT_ARG (*(int *)arg)
#define FLOAT_ARG (*(float *)arg)
#define STRING_ARG (*(std::string *)arg)

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

    bool hasComponent(std::string name)
    {
        return this->components.find(name) != this->components.end();
    }

    bool runComponent(std::string name, void *arg, void *out)
    {
        auto component = this->components.find(name);

        if (component != this->components.end())
        {
            component->second.run(arg, out);
            return true;
        }

        return false;
    }
};

/* -------------------------------------------------------------------------- */
/*                              Component Macros                              */
/* -------------------------------------------------------------------------- */

/* ---------------------------------- State --------------------------------- */

#define STATE(name, value) const auto name = value
#define STATE_GET(name) name

#define STATE_VAR(value) \
    auto __CURRENT__(void) { return value; }
#define STATE_VAR_GET() __CURRENT__()

/* ---------------------------------- Hooks --------------------------------- */

#define HOOK_NAME(x) __hook_##x
#define HOOK_KEY(x) "__hook_" #x
#define HOOK_RUN(x) ComponentRegistry::getInstance()->runComponent(HOOK_KEY(x), NULL, NULL)
#define HOOK(x)                                                                              \
    class HOOK_NAME(x)                                                                       \
    {                                                                                        \
    private:                                                                                 \
        static ComponentInfo *info;                                                          \
        static void _run(void *arg, void *out);                                              \
                                                                                             \
    public:                                                                                  \
        static bool skipped;                                                                 \
        static void run(void *arg, void *out)                                                \
        {                                                                                    \
            if (skipped)                                                                     \
            {                                                                                \
                skipped = false;                                                             \
                return;                                                                      \
            }                                                                                \
                                                                                             \
            _run(arg, out);                                                                  \
        }                                                                                    \
    };                                                                                       \
                                                                                             \
    ComponentInfo *HOOK_NAME(x)::info = ComponentRegistry::getInstance()->createAndRegister( \
        HOOK_KEY(x),                                                                         \
        HOOK_NAME(x)::run);                                                                  \
                                                                                             \
    bool HOOK_NAME(x)::skipped = false;                                                      \
    void HOOK_NAME(x)::_run(void *arg, void *out)

#define HOOK_SKIP(x) HOOK_NAME(x)::skipped = true

/* ------------------------------- Transitions ------------------------------ */

#define OVERLOAD3(_1, _2, _3, NAME, ...) NAME

#define PREPARE_NAME(a, ...)                                            \
    OVERLOAD3(__VA_ARGS__, PREPARE_NAME2, PREPARE_NAME1, PREPARE_NAME0) \
    (a, __VA_ARGS__)
#define PREPARE_NAME2(a, b, type, variant) __prepare_##a##_##b##_##type##_##variant
#define PREPARE_NAME1(a, b, type) __prepare_##a##_##b##_##type
#define PREPARE_NAME0(a, b) __prepare_##a##_##b

#define PREPARE_KEY(a, ...)                                          \
    OVERLOAD3(__VA_ARGS__, PREPARE_KEY2, PREPARE_KEY1, PREPARE_KEY0) \
    (a, __VA_ARGS__)
#define PREPARE_KEY2(a, b, type, variant) "__prepare_" #a "_" #b "$" #type "#" #variant
#define PREPARE_KEY1(a, b, type) "__prepare_" #a "_" #b "$" #type
#define PREPARE_KEY0(a, b) "__prepare_" #a "_" #b

#define PREPARE_RUN(a, b, type, variant, arg) __runPreparation(PREPARE_KEY(a, b, type, variant), arg)

#define PREPARE(...)                                                                                      \
    class PREPARE_NAME(__VA_ARGS__)                                                                       \
    {                                                                                                     \
    private:                                                                                              \
        static ComponentInfo *info;                                                                       \
                                                                                                          \
    public:                                                                                               \
        static void run(void *arg, void *out);                                                            \
    };                                                                                                    \
                                                                                                          \
    ComponentInfo *PREPARE_NAME(__VA_ARGS__)::info = ComponentRegistry::getInstance()->createAndRegister( \
        PREPARE_KEY(__VA_ARGS__),                                                                         \
        PREPARE_NAME(__VA_ARGS__)::run);                                                                  \
                                                                                                          \
    void PREPARE_NAME(__VA_ARGS__)::run(void *arg, void *out)

bool __runPreparation(std::string key, void *arg)
{
    bool result = ComponentRegistry::getInstance()->runComponent(key, arg, NULL);
    if (result)
    {
        return result;
    }

    result = ComponentRegistry::getInstance()->runComponent(key.substr(0, key.find("#")), arg, NULL);
    if (result)
    {
        return result;
    }

    return ComponentRegistry::getInstance()->runComponent(key.substr(0, key.find("$")), arg, NULL);
}

#endif // __MOMOS_MACROS__
