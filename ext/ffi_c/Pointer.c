#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <ruby.h>
#include "rbffi.h"
#include "AbstractMemory.h"
#include "Pointer.h"

typedef struct Pointer {
    AbstractMemory memory;
    VALUE parent;
} Pointer;

VALUE rb_FFI_Pointer_class;
static VALUE classPointer = Qnil;
static void ptr_mark(Pointer* ptr);
static void ptr_free(Pointer* ptr);

VALUE
rb_FFI_Pointer_new(void* addr)
{
    Pointer* p;
    VALUE retval;
    if (addr == NULL) {
        return rb_FFI_NullPointer_singleton;
    }
    retval = Data_Make_Struct(classPointer, Pointer, NULL, ptr_free, p);
    p->memory.address = addr;
    p->memory.size = LONG_MAX;
    p->parent = Qnil;
    return retval;
}

static VALUE
ptr_plus(VALUE self, VALUE offset)
{
    AbstractMemory* ptr = (AbstractMemory *) DATA_PTR(self);
    Pointer* p;
    VALUE retval;
    long off = NUM2LONG(offset);

    checkBounds(ptr, off, 1);
    retval = Data_Make_Struct(classPointer, Pointer, ptr_mark, ptr_free, p);
    p->memory.address = ptr->address + off;
    p->memory.size = ptr->size == LONG_MAX ? LONG_MAX : ptr->size - off;
    p->parent = self;
    return retval;
}

static VALUE
ptr_inspect(VALUE self)
{
    AbstractMemory* ptr = (AbstractMemory *) DATA_PTR(self);
    char tmp[100];
    snprintf(tmp, sizeof(tmp), "#<Native Pointer address=%p>", ptr->address);
    return rb_str_new2(tmp);
}

static VALUE
ptr_null_p(VALUE self)
{
    AbstractMemory* ptr = (AbstractMemory *) DATA_PTR(self);
    return ptr->address == NULL ? Qtrue : Qfalse;
}

static VALUE
ptr_equals(VALUE self, VALUE other)
{
    AbstractMemory* p1 = (AbstractMemory *) DATA_PTR(self);
    AbstractMemory* p2;

    if (!rb_obj_is_kind_of(other, classPointer)) {
        rb_raise(rb_eArgError, "Comparing Pointer with non Pointer");
    }
    p2 = (AbstractMemory *) DATA_PTR(other);
    return p1->address == p2->address ? Qtrue : Qfalse;
}

static VALUE
ptr_address(VALUE self)
{
    AbstractMemory* ptr = (AbstractMemory *) DATA_PTR(self);
    return ULL2NUM((uintptr_t) ptr->address);
}

static void
ptr_mark(Pointer* ptr)
{
    if (ptr->parent != Qnil) {
        rb_gc_mark(ptr->parent);
    }
}

static void
ptr_free(Pointer* ptr)
{
    xfree(ptr);
}

void
rb_FFI_Pointer_Init()
{
    VALUE moduleFFI = rb_define_module("FFI");
    rb_FFI_Pointer_class = classPointer = rb_define_class_under(moduleFFI, "Pointer", rb_FFI_AbstractMemory_class);
    rb_define_method(classPointer, "inspect", ptr_inspect, 0);
    rb_define_method(classPointer, "+", ptr_plus, 1);
    rb_define_method(classPointer, "null?", ptr_null_p, 0);
    rb_define_method(classPointer, "address", ptr_address, 0);
    rb_define_method(classPointer, "==", ptr_equals, 1);
}