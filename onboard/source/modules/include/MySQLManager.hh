#ifndef GRAMSBalloon_MySQLManager_hh
#define GRAMSBalloon_MySQLManager_hh 1
#include "MySQLIO.hh"
#include "anlnext/BasicModule.hh"
#include "mysqlx/xdevapi.h"

namespace gramsballoon::pgrams {
namespace mysql {
class MySQLIO;
}
/**
 * Module for managing MySQL database connection.
 *
 * @author Shota Arai
 * @date 2026-02-04 | First design
 */
class MySQLManager: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(MySQLManager, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  MySQLManager() = default;
  virtual ~MySQLManager() = default;

protected:
  MySQLManager(const MySQLManager &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_finalize() override;

  mysql::MySQLIO *getMySQLIO() { return &mysqlIO_; }

private:
  mysql::MySQLIO mysqlIO_;
  std::string password_ = "test";
  std::string database_ = "pgrams";
  std::string user_ = "test";
  int port_ = 33060;
  std::string host_ = "localhost";
  bool checkExist_ = true;
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_MySQLManager_hh