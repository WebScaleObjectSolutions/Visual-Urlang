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

#include "Node.h"
#include "TypeLoc.h"

class Decl : public Node
{
  public:
    explicit Decl(Position pos) : Node(pos) {}
};

/* any decl with a name */
class NamedDecl : public Decl
{
  protected:
    std::string m_name;

  public:
    NamedDecl(Position pos, std::string name) : Decl(pos), m_name(name) {}
};

/* a variable declaration */
class DimDecl : public NamedDecl
{
  protected:
    TypeLoc m_typeLoc;

  public:
    DimDecl(Position pos, std::string name, TypeLoc typeLoc)
        : NamedDecl(pos, name), m_typeLoc(typeLoc)
    {
    }

    virtual void print(size_t indent);
};