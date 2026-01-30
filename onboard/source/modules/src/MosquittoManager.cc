#include "MosquittoManager.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
template <typename T>
ANLStatus MosquittoManager<T>::mod_define() {
  define_parameter("host", &mod_class::host_);
  define_parameter("port", &mod_class::port_);
  define_parameter("keep_alive", &mod_class::keepAlive_);
  define_parameter("threaded_set", &mod_class::threadedSet_);
  define_parameter("time_out", &mod_class::timeout_);
  set_parameter_description("Timeout for the connection. If negative, default value (1000 ms) is used according to the reference of mosquittopp.");
  set_parameter_unit(1.0, "msec");
  define_parameter("user", &mod_class::user_);
  define_parameter("password", &mod_class::passwd_);
  define_parameter("device_id", &mod_class::deviceId_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
template <typename T>
ANLStatus MosquittoManager<T>::mod_pre_initialize() {
  if (host_.empty()) {
    std::cerr << "Error in MosquittoManager::mod_pre_initialize: host is empty." << std::endl;
    return AS_ERROR;
  }
  if (port_ < 0) {
    std::cerr << "Error in MosquittoManager::mod_pre_initialize: port is negative." << std::endl;
    return AS_ERROR;
  }
  if (keepAlive_ < 0) {
    std::cerr << "Error in MosquittoManager::mod_pre_initialize: keep_alive is negative." << std::endl;
    return AS_ERROR;
  }
  if (user_.empty() && !passwd_.empty()) {
    std::cerr << "Error in MosquittoManager::mod_pre_initialize: passwd is set but user is empty." << std::endl;
    return AS_ERROR;
  }
  // FIXME: Must call mosqpp::lib_init and lib_cleanup
  mosquittoIO_ = std::make_shared<MosquittoIO<T>>(deviceId_, host_, port_, keepAlive_, threadedSet_);
  mosquittoIO_->setVerbose(chatter_);
  return AS_OK;
}
template <typename T>
ANLStatus MosquittoManager<T>::mod_initialize() {
  if (!mosquittoIO_) {
    return AS_ERROR;
  }
  if (!user_.empty()) {
    mosquittoIO_->username_pw_set(user_.c_str(), passwd_.c_str());
  }
  HandleError(mosquittoIO_->Connect());
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  return AS_OK;
}
template <typename T>
ANLStatus MosquittoManager<T>::mod_begin_run() {
  if (!mosquittoIO_) {
    return AS_ERROR;
  }
  for (int i = 0; i < 5; i++) {
    HandleError(mosquittoIO_->loop(timeout_, 10));
  }
  return AS_OK;
}
template <typename T>
ANLStatus MosquittoManager<T>::mod_analyze() {
  if (!mosquittoIO_) {
    return AS_OK;
  }
  for (int i = 0; i < 10; i++){
    const auto result = mosquittoIO_->loop(timeout_, 10);
    if (result != 0) {
      return HandleError(mosquittoIO_->Reconnect());
    }
  }
  return AS_OK;
}
template <typename T>
ANLStatus MosquittoManager<T>::mod_end_run() {
  if (!mosquittoIO_) {
    return AS_ERROR;
  }
  return HandleError(mosquittoIO_->loop(-1, 10));
}
template <typename T>
ANLStatus MosquittoManager<T>::mod_finalize() {
  if (!mosquittoIO_) {
    return AS_ERROR;
  }
  return HandleError(mosquittoIO_->Disconnect());
}
template <typename T>
ANLStatus MosquittoManager<T>::HandleError(int error_code) {
  if (error_code != 0) {
    std::cerr << "Error in ReceiveCommand::mod_initialize: Connecting MQTT failed. Error Message: " << mosqpp::strerror(error_code) << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::MQTT_COM_ERROR);
    }
    return AS_ERROR;
  }
  return AS_OK;
}

template ANLStatus MosquittoManager<std::string>::mod_define();
template ANLStatus MosquittoManager<std::vector<uint8_t>>::mod_define();
template ANLStatus MosquittoManager<std::string>::mod_pre_initialize();
template ANLStatus MosquittoManager<std::vector<uint8_t>>::mod_pre_initialize();
template ANLStatus MosquittoManager<std::string>::mod_initialize();
template ANLStatus MosquittoManager<std::vector<uint8_t>>::mod_initialize();
template ANLStatus MosquittoManager<std::string>::mod_begin_run();
template ANLStatus MosquittoManager<std::vector<uint8_t>>::mod_begin_run();
template ANLStatus MosquittoManager<std::string>::mod_analyze();
template ANLStatus MosquittoManager<std::vector<uint8_t>>::mod_analyze();
template ANLStatus MosquittoManager<std::string>::mod_end_run();
template ANLStatus MosquittoManager<std::vector<uint8_t>>::mod_end_run();
template ANLStatus MosquittoManager<std::string>::mod_finalize();
template ANLStatus MosquittoManager<std::vector<uint8_t>>::mod_finalize();

} // namespace gramsballoon::pgrams