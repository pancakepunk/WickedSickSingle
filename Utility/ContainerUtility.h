#pragma once



template<typename T>
void vector_remove(std::vector<T>& container, int toRemove)
{
  std::swap(container[toRemove], container[container.size() - 1]);
  container.pop_back();
}

template<typename T>
T& as_2d(std::vector<T>& container, unsigned width, unsigned height, unsigned wIndex)
{
  return container[(height * width) + wIndex];
}
