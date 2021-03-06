/*******************************************************************

        PROPRIETARY NOTICE

These coded instructions, statements, and computer programs contain
proprietary information of the Visual Urlang project, and are protected
under copyright law. They may not be distributed, copied, or used except
under the provisions of the terms of the End-User License Agreement, in
the file "EULA.md", which should have been included with this file.

        Copyright Notice

    (c) 2020 The Visual Urlang Project.
              All rights reserved.
********************************************************************/

#pragma once

#include <string>
#include <vector>

#include "AST.h"

enum BuiltinKind
{
    evVoid,
    evByte,
    evBoolean,
    evInteger,
    evLong,
    evLongLong,
    evWord,
    evSingle,
    evDouble,
    evCharacter,
    evDate,
    evObject,
    evVariant,
};

class Decl;
class TypeParamDecl;
struct TypeParamBinding;
class Class;

class Type
{
  public:
    virtual Type *getTypeOfMember(std::string mem, Class *parentCls = nullptr)
    {
        UNIMPL;
        return nullptr;
    }

    virtual Type *copyAsClass()
    {
        this->makeClassType();
        return this;
    }
    virtual Type *copyWithSubs(std::vector<TypeParamBinding> subs)
    {
        UNIMPL;
        return this;
    }

    virtual void makeClassType() { UNIMPL; }

    virtual void print(size_t in) { std::cout << blanks(in) << "unknown type"; }
};

class TypeError : Type
{
};

/* A basic type such as Integer, Short, etc.
 * Strings etc are actually resolved to the real type of e.g. String class. */
class BuiltinType : public Type
{
    enum
    {
        evVoid,
    };

    virtual Type *copyWithSubs(std::vector<TypeParamBinding> subs);

    virtual void print(size_t in)
    {
        std::cout << blanks(in) + "(builtin-type)";
    }
};

/* To support HKTs, this should have an entry for its type parameters too.
 * Substitution then involves finding the Class defining the type name, and
 * then invoking it with any type args we have.
 */
class UnboundTypeArg : public Type
{
  public:
    std::string m_name;
    Decl *m_decl;
    bool m_isCls;

    explicit UnboundTypeArg(std::string name, Decl *decl)
        : m_name(name), m_decl(decl), m_isCls(false)
    {
    }

    virtual UnboundTypeArg *copyAsClass()
    {
        UnboundTypeArg *newTy = new UnboundTypeArg(*this);
        newTy->makeClassType();
        return this;
    }
    virtual Type *copyWithSubs(std::vector<TypeParamBinding> subs);

    virtual void makeClassType() { m_isCls = true; }

    virtual void print(size_t in);
};

class UnboundTypeArgDot : public Type
{
    UnboundTypeArg *m_base;

  public:
};

struct TypeParamBinding
{
    std::string name;
    Type *type;
    TypeParamDecl *m_decl;

    TypeParamBinding(std::string name, Type *type, TypeParamDecl *decl)
        : name(name), type(type), m_decl(decl)
    {
    }
};

class FunType : public Type
{
    Class *m_class;
    Type *m_rType;
    std::vector<Type *> m_argTypes;

  public:
    FunType(Class *class_, Type *type, std::vector<Type *> argTypes)
        : m_class(class_), m_rType(type), m_argTypes(argTypes)
    {
    }

    Type *rType() { return m_rType; }
};

/* Instantiated type*/
class ClassInstType : public Type
{
    std::vector<Type *> m_inherits;
    Class *m_class;
    /* Table of names to their concrete type replacements. Used to substitute
     * type parameter uses by Dims and methods of the class.*/
    std::vector<TypeParamBinding> m_params;
    /* Is it a class-of type? */
    bool m_isCls;
    bool m_isInvoked = false;

  public:
    ClassInstType(Class *class_, std::vector<TypeParamBinding> params)
        : m_class(class_), m_params(params)
    {
    }

    Class *cls() { return m_class; }

    void addArg(TypeParamBinding anArg) { m_params.push_back(anArg); }
    void addInherited(Type *inh) { m_inherits.push_back(inh); }

    Type *getTypeOfMember(std::string mem, Class *parentCls = nullptr);

    /* Invoke this type with these arguments. */
    virtual ClassInstType *invoke(std::vector<Type *> subs);
    bool isInvoked() { return m_isInvoked; }

    /* Copy type, substituting all instances of UnboundTypeArg that can be
     * subbed.*/
    virtual ClassInstType *
    copyWithSubs(std::vector<TypeParamBinding> subs) override;
    ClassInstType *copyAsClass() override;

    /* Make it a class-of type. */
    virtual void makeClassType();

    virtual void print(size_t in);
};