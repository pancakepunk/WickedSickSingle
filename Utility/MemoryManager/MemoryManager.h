#pragma once

#include <utility>
#include <type_traits>

#ifdef _DEBUG

#define PADSIZE 0
#else
#define PADSIZE 0
#endif
namespace WickedSick
{

  template <typename Type, int ElementsPerPage = 20>
  class MemoryManager
  {
  public:


    // page class
    class Page
    {
    public:

      struct Object
      {
        Object()
        {

        }
        enum MemState
        {
          Padding = 0xDD,
          Uninitialized = 0xCD,
          Allocated = 0xFF,
          Unallocated = 0xAA,
          Deleted = 0xFE
        };

        MemState state;
        //const char prePad[5]  = {[0 ... (5 - 1)] = Padding};
        char object[sizeof(Type)];
        //const char postPad[5] = {[0 ... (5 - 1)] = Padding};
      };


      Page() : allocated_objects_(0)
      {
        free_list_.resize(ElementsPerPage, nullptr);
        for (int i = 0; i < ElementsPerPage; ++i)
        {
          object_pool_[i].state = Object::Unallocated;
          #ifdef _DEBUG
          memset(object_pool_[i].object, Object::Unallocated, sizeof(object_pool_[i].object));
          #endif
          free_list_[i] = &object_pool_[i];
        }
      }

      Page(const Page& rhs);

      bool HasSpace()
      {
        return allocated_objects_ < ElementsPerPage;
      }

      Type* Allocate()
      {
        Type* toReturn = nullptr;
        if (HasSpace())
        {
          Object* toAllocate = nullptr;//object_pool_[0];
          //while (toAllocate->state == Object::Allocated)
          //{
          //  ++toAllocate;
          //}

          toAllocate = free_list_.back();
          free_list_.pop_back();

          toReturn = (Type*)&(toAllocate->object);//first get our pointer

          toAllocate->state = Object::Allocated;//set the state to allocated

          ++allocated_objects_;
        }
        return toReturn;
      }

      void Deallocate(Type* toDelete)
      {
        Object* object = (Object*)(((char*)toDelete) - (PADSIZE + sizeof(Object::MemState)));
        
        switch (object->state)
        {
        case Object::Deleted:
          __debugbreak();//double delete!
          break;
        case Object::Unallocated:
          __debugbreak();//how did you even get this pointer what
          break;
        case Object::Allocated:
          free_list_.push_back(object);
          object->state = Object::Deleted;
          memset(object->object, Object::Deleted, sizeof(Type));
          --allocated_objects_;
          break;
        default:
          __debugbreak();
          //dude you're doing some weird ass shit here....
        }
        
      }

      Object* GetObjectPool()
      {
        return &object_pool_[0];
      }

    private:
      Object object_pool_[ElementsPerPage];
      std::vector<Object*> free_list_;
      int allocated_objects_;

    };
    //end page class




    MemoryManager() : currently_allocated_(0),
                      total_allocations_(0),
                      total_deletions_(0),
                      pages_allocated_(0),
                      pages_active_(0)
    {
      page_pool_.push_back(new Page());
    }

    MemoryManager(const MemoryManager& rhs);//unimplemented for good reason
    ~MemoryManager()
    {
      //delete stuff!
      for (auto& it : page_pool_)
      {
        delete it;
      }
    }

    size_t BytesBetween()
    {
      return sizeof(Object);
    }

    template<typename ...Types>
    Type* New(const Types&... args)
    {
      
      Type* newObject = nullptr;
      for (auto it : page_pool_)
      {
        newObject = it->Allocate();
        if (newObject)
        {
          new (newObject) Type(args...);
          return newObject;
        }
      }

      page_pool_.push_back(new Page());
      ++pages_allocated_;
      ++pages_active_;
      newObject = page_pool_.back()->Allocate();
      new (newObject) Type(args...);
      return newObject;
    }

    template<typename ...Types>
    Type* New(Types&&... args)
    {

      Type* newObject = nullptr;
      for (auto it : page_pool_)
      {
        newObject = it->Allocate();
        if (newObject)
        {
          new (newObject) Type(std::forward<Types>(args)...);
          ++currently_allocated_;
          ++total_allocations_;
          return newObject;
        }
      }

      page_pool_.push_back(new Page());
      ++pages_allocated_;
      ++pages_active_;
      newObject = page_pool_.back()->Allocate();
      new (newObject) Type(std::forward<Types>(args)...);
      ++currently_allocated_;
      ++total_allocations_;
      return newObject;
    }


    void Preallocate(size_t total)
    {
      size_t pagesToMake = total / ElementsPerPage;

      while (page_pool_.size() < pagesToMake)
      {
        page_pool_.push_back(new Page());
        ++pages_allocated_;
      }


    }

    //Type* New()
    //{
    //  Type* newObject = nullptr;
    //  for (auto it : page_pool_)
    //  {
    //    newObject = it->Allocate();
    //    if (newObject)
    //    {
    //      return newObject;
    //    }
    //  }
    //  page_pool_.push_back(new Page());
    //  ++pages_allocated_;
    //  ++pages_active_;
    //  return page_pool_.back()->Allocate();
    //}

    void Delete(Type* type)
    {
      if (type)
      {
        type->~Type();

        char* basicPtr = (char*)type;
        char* pagePtr;
        for (auto& it : page_pool_)
        {
          pagePtr = (char*)it->GetObjectPool();
          if ((basicPtr > pagePtr) &&
              (basicPtr < (pagePtr + sizeof(Page))))
          {
            it->Deallocate(type);
            ++total_deletions_;
            --currently_allocated_;
            return;
          }
        }
      }
      __debugbreak();
      //NOT A VALID POINTER
    }


  private:

    int currently_allocated_;
    int total_allocations_;
    int total_deletions_;
    int pages_allocated_;
    int pages_active_;
    

    std::vector<Page*> page_pool_;

  };
}