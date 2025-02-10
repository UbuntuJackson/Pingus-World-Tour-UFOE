#pragma once

#include <memory>

template<typename T>
using UniquePtr = std::unique_ptr<T>;

//template<typename T, typename ...tArgs>
//constexpr UniquePtr<T> (*MakeUnique)(tArgs&&..._args) = &(std::make_unique<T>);

template<typename T, typename ...tArgs>
const auto MakeUnique = std::make_unique<T>(tArgs..._args);