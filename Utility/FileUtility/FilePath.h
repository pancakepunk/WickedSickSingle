#pragma once


#include <chrono>
#include <filesystem>




namespace WickedSick
{
  class FilePath
  {
  public:
    FilePath();
    FilePath(const std::string& filepath);
    FilePath(const FilePath& p);
    FilePath(FilePath&& p);
    const FilePath& operator=(FilePath p);
    const FilePath& operator=(const std::string& p);

    bool Writable();

    std::string Path();

    ~FilePath();


    bool WasModified();

  private:

    std::experimental::filesystem::path path_;
    std::chrono::time_point<std::chrono::system_clock> last_modified_;
  };
}
