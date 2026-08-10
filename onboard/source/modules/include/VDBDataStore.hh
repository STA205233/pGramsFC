#ifndef GB_VDBDataStore_hh
#define GB_VDBDataStore_hh 1
#include "DBFieldSink.hh"
namespace gramsballoon::pgrams {
/**
 * @brief Pure virtual class for Database handling
 * @author Shota Arai
 * @date 2026-07-09 | Shota Arai | First version
 */
class VDBDataStore {
public:
  VDBDataStore() = default;
  virtual ~VDBDataStore() = default;

protected:
  VDBDataStore(const VDBDataStore &r) = default;

public:
  virtual void pushToDBSink(DBFieldSink *sink) const = 0;
  virtual void initializeDBTableInSink(DBFieldSink *sink, const std::string &table_name) const = 0;
  virtual bool hasData() const = 0;
};
} // namespace gramsballoon::pgrams
#endif // GB_VDBDataStore_hh