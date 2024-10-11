#ifndef GODOTJS_QUICKJS_FUNCTION_INTEROP_H
#define GODOTJS_QUICKJS_FUNCTION_INTEROP_H
#include "jsb_quickjs_object.h"
#include "jsb_quickjs_isolate.h"

namespace v8
{
    template <typename T>
    class ReturnValue
    {
    public:
        ReturnValue(const Data& data) : data_(data) {}

        explicit operator JSValue() const
        {
            return data_.isolate_->operator[](data_.stack_pos_);
        }

        template <typename S>
        void Set(const Local<S>& value) const
        {
            if (value.IsEmpty())
            {
                data_.isolate_->stack_copy(data_.stack_pos_, jsb::impl::StackPos::Undefined);
                return;
            }
            data_.isolate_->stack_copy(data_.stack_pos_, value->stack_pos_);
        }

        template <typename S>
        void Set(const Global<S>& value) const
        {
            if (value.IsEmpty())
            {
                data_.isolate_->stack_copy(data_.stack_pos_, jsb::impl::StackPos::Undefined);
                return;
            }
            Set(value.Get(data_.isolate_));
        }

        void Set(int32_t value) const
        {
            data_.isolate_->push_steal(JS_NewInt32(data_.isolate_->ctx(), value));
        }

    private:
        Data data_;
    };

    template <typename T>
    class FunctionCallbackInfo
    {
    public:
        FunctionCallbackInfo(Isolate* isolate, int len, bool is_constructor)
        : isolate_(isolate), len_(len), stack_pos_(isolate->stack_pos_), is_constructor_(is_constructor)
        {
        }

        ReturnValue<T> GetReturnValue() const
        {
            return ReturnValue<T>(v8::Data(isolate_, stack_pos_ + jsb::impl::FunctionStackBase::ReturnValue));
        }

        Local<Object> This() const
        {
            return Local<Object>(v8::Data(isolate_, stack_pos_ + jsb::impl::FunctionStackBase::ReturnValue));
        }

        Local<Value> Data() const
        {
            return Local<Object>(v8::Data(isolate_, stack_pos_ + jsb::impl::FunctionStackBase::ReturnValue));
        }

        Local<Value> NewTarget() const
        {
            //TODO how?
        }

        Isolate* GetIsolate() const { return isolate_; }

        Local<Value> operator[](int index) const
        {
            jsb_check(index >= 0);
            if (index < len_)
            {
                return Local<Value>(v8::Data(isolate_, stack_pos_ + jsb::impl::FunctionStackBase::Num + index));
            }
            return Local<Value>(v8::Data(isolate_, jsb::impl::StackPos::Undefined));
        }

        int Length() const { return len_; }

        bool IsConstructCall() const { return is_constructor_; }

    private:
        Isolate* isolate_;
        int len_;
        uint16_t stack_pos_;
        bool is_constructor_;
    };

}
#endif