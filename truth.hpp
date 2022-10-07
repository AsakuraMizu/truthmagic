#pragma once

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>
#include "texttable.hpp"

namespace Truth
{
  enum BinaryOp
  {
    _and,
    _or,
    _xor,
    _impl,
    _eq,
  };

  enum UnaryOp
  {
    _not
  };

  std::string tostring(BinaryOp op)
  {
    switch (op)
    {
    case _and:
      return "&";
    case _or:
      return "|";
    case _xor:
      return "^";
    case _impl:
      return ">";
    case _eq:
      return "==";
    default:
      return "";
    }
  }

  std::string tostring(UnaryOp op)
  {
    switch (op)
    {
    case _not:
      return "!";
    default:
      return "";
    }
  }

  using Map = std::map<std::string, bool>;

  struct Node
  {
    virtual std::string tostring() = 0;
    virtual bool calc(Map values) = 0;
  };

  using Wrapper = std::shared_ptr<Node>;

  struct Const : Node
  {
    bool value;
    Const(bool value) : value(value) {}

    std::string tostring()
    {
      return value ? "T" : "F";
    }
    bool calc(Map values)
    {
      return value;
    }
  };

  struct Var : Node
  {
    std::string name;
    Var(std::string name) : name(name) {}

    std::string tostring()
    {
      return name;
    }

    bool calc(Map values)
    {
      return values[name];
    }
  };

  struct Binary : Node
  {
    BinaryOp op;
    Wrapper left, right;
    Binary(BinaryOp op, Wrapper left, Wrapper right) : op(op), left(left), right(right) {}

    std::string tostring()
    {
      return '(' + left->tostring() + Truth::tostring(op) + right->tostring() + ')';
    }

    bool calc(Map values)
    {
      bool l = left->calc(values), r = right->calc(values);
      switch (op)
      {
      case _and:
        return l && r;
      case _or:
        return l || r;
      case _xor:
        return l != r;
      case _impl:
        return !l || r;
      case _eq:
        return l == r;
      default:
        return false;
      }
    }
  };

  struct Unary : Node
  {
    UnaryOp op;
    Wrapper arg;
    Unary(UnaryOp op, Wrapper arg) : op(op), arg(arg) {}

    std::string tostring()
    {
      return Truth::tostring(op) + arg->tostring();
    }

    bool calc(Map values)
    {
      bool a = arg->calc(values);
      switch (op)
      {
      case _not:
        return !a;
      default:
        return false;
      }
    }
  };

  Wrapper operator&(Wrapper left, Wrapper right)
  {
    return std::make_shared<Binary>(_and, left, right);
  }

  Wrapper operator|(Wrapper left, Wrapper right)
  {
    return std::make_shared<Binary>(_or, left, right);
  }

  Wrapper operator^(Wrapper left, Wrapper right)
  {
    return std::make_shared<Binary>(_xor, left, right);
  }

  Wrapper operator>(Wrapper left, Wrapper right)
  {
    return std::make_shared<Binary>(_impl, left, right);
  }

  Wrapper operator==(Wrapper left, Wrapper right)
  {
    return std::make_shared<Binary>(_eq, left, right);
  }

  Wrapper operator!(Wrapper arg)
  {
    return std::make_shared<Unary>(_not, arg);
  }

  Wrapper operator~(Wrapper arg)
  {
    return std::make_shared<Unary>(_not, arg);
  }

  std::vector<std::string> variables;
}

#define VAR(p)                          \
  Truth::Wrapper p(new Truth::Var(#p)); \
  Truth::variables.push_back(#p)

void calc(std::initializer_list<Truth::Wrapper> exprs)
{
  using Truth::Map;
  using Truth::variables;
  unsigned n = variables.size();
  TextTable t;
  for (auto x : variables)
    t.add(x);
  for (auto expr : exprs)
    t.add(expr->tostring());
  t.endOfRow();
  for (unsigned long long i = 0; i < (1ull << n); i++)
  {
    Map values;
    for (unsigned j = 0; j < n; j++)
    {
      values[variables[j]] = i & (1 << (n - j - 1));
      t.add(values[variables[j]] ? "1" : "0");
    }
    for (auto expr : exprs)
      t.add(expr->calc(values) ? "1" : "0");
    t.endOfRow();
  }
  std::cout << t;
}
void calc(Truth::Wrapper expr)
{
  calc({expr});
}

auto F = std::make_shared<Truth::Const>(false);
auto T = std::make_shared<Truth::Const>(true);
