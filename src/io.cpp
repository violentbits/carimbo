#include "io.hpp"

using namespace storage;

std::vector<uint8_t> io::read(const std::string &filename) noexcept(false) {
  std::unique_ptr<PHYSFS_File, decltype(&PHYSFS_close)> ptr(PHYSFS_openRead(filename.c_str()), PHYSFS_close);
  if (!ptr) [[unlikely]] {
    std::ostringstream oss;
    oss << "[PHYSFS_openRead] error while opening file: " << filename
        << ", error: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
    throw std::runtime_error(oss.str());
  }

  PHYSFS_sint64 length = PHYSFS_fileLength(ptr.get());
  if (length <= 0) [[unlikely]] {
    std::ostringstream oss;
    oss << "[PHYSFS_fileLength] invalid file length, file: " << filename
        << ", error: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
    throw std::runtime_error(oss.str());
  }

  std::vector<uint8_t> buffer(length);
  auto bytesRead = PHYSFS_readBytes(ptr.get(), buffer.data(), length);
  if (bytesRead != length) [[unlikely]] {
    std::ostringstream oss;
    oss << "[PHYSFS_readBytes] error reading file: " << filename
        << ", expected " << length << " bytes but read " << bytesRead
        << ", error: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
    throw std::runtime_error(oss.str());
  }

  return buffer;
}
