#pragma once

namespace WickedSick
{
  template<typename T>
  class Archetype
  {
  public:
    Archetype(const std::string& name)
              : name_(name)
    {

    }

    Archetype(Archetype&& rhs)
              : name_(std::forward<std::string>(rhs.name_)),
                source_(std::forward<std::string>(rhs.source_)),
                base_object_(std::forward<T>(rhs.base_object_))
    {

    }

    Archetype(const Archetype& rhs)
              : name_(rhs.name_),
                source_(rhs.source_),
                base_object_(rhs.base_object_)
    {
    }

    ~Archetype()
    {

    }

    T& GetBase()
    {
      return base_object_;
    }

    void Load(const std::string& source)
    {
      source_ = source;
    }

    void Reload()
    {
      if (!source_.empty())
      {
        //load logic
      }
    }

    void Fill(T& myClone)
    {
      new (&myClone) T(base_object_);
    }


  private:
    std::string name_;
    std::string source_;
    T base_object_;
  };
}

