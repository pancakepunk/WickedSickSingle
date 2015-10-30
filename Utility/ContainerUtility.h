#pragma once

#include <vector>

template<typename T>
void vector_remove(std::vector<T>& container, int toRemove)
{
  std::swap(container[toRemove], container[container.size() - 1]);
  container.pop_back();
}
