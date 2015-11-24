#include "Precompiled.h"
#include "UtilityPrecompiled.h"
#include "FilePath.h"


namespace WickedSick
{
  FilePath::FilePath()
  {
  }

  FilePath::FilePath(const std::string& filepath) : path_(filepath)
  {
    last_modified_ = std::experimental::filesystem::last_write_time(path_);
  }

  FilePath::FilePath(const FilePath & p) :  path_(p.path_),
                                            last_modified_(p.last_modified_)
  {

  }

  FilePath::FilePath(FilePath && p)
  {
    std::swap(p, *this);
  }

  const FilePath & FilePath::operator=(FilePath p)
  {
    std::swap(p, *this);
    return *this;
  }

  const FilePath & FilePath::operator=(const std::string & p)
  {
    new (this) FilePath(p);
    return *this;
  }

  bool FilePath::Writable()
  {
    auto fstream = std::fstream(path_.string());
    if (fstream.is_open())
    {
      fstream.close();
      return true;
    }
    return false;
  }

  std::string FilePath::Path()
  {
    
    return path_.string();
  }

  FilePath::~FilePath()
  {
  }

  bool FilePath::WasModified()
  {
    auto currentState = std::experimental::filesystem::last_write_time(path_);
    bool modified = currentState != last_modified_;
    if (modified)
    {
      last_modified_ = currentState;
      return true;
    }
    return false;
  }
}
