﻿#ifndef GODOTJS_WEB_TEMPLATE_H
#define GODOTJS_WEB_TEMPLATE_H

#include "jsb_web_primitive.h"
#include "jsb_web_callback.h"

namespace v8
{
    class Template : public Data
    {
    public:
        void Set(Local<Name> name, Local<Data> value);

        void SetAccessorProperty(
            Local<Name> name,
            Local<FunctionTemplate> getter = Local<FunctionTemplate>(),
            Local<FunctionTemplate> setter = Local<FunctionTemplate>());
    };

    class FunctionTemplate : public Template
    {
    public:
        static Local<FunctionTemplate> New(Isolate* isolate, FunctionCallback callback = nullptr, Local<Value> data = Local<Value>());
    };

}
#endif
