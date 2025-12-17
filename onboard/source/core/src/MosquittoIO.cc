#include "MosquittoIO.hh"

namespace gramsballoon::pgrams {
template <>
int MosquittoIO<std::string>::Publish(const std::string &message, const std::string &topic, int qos) {
  if (verbose_ > 2)
    std::cout << "Publishing message: " << message << std::endl;
  if (verbose_ > 1)
    std::cout << "length: " << message.length() << std::endl;
  return HandleError(publish(NULL, topic.c_str(), message.size(), message.data(), qos));
}
template <>
int MosquittoIO<std::vector<uint8_t>>::Publish(const std::vector<uint8_t> &message, const std::string &topic, int qos) {
#ifndef GB_MOSQUITTOIO_ALL_BINARY
  std::string m_sptrstr;
  for (const auto &m: message) {
    m_sptrstr += std::to_string(static_cast<int>(m)) + ",";
  }
  if (verbose_ > 1) {
    std::cout << "Publishing message: " << m_sptrstr << std::endl;
  }
  if (verbose_ > 2) {
    std::cout << "Publishing message in int: ";
    for (size_t i = 0; i < message.size(); i++) {
      std::cout << static_cast<int>(message[i]) << " ";
    };
    std::cout << std::endl;
    std::cout << "Size: " << message.size() << std::endl;
  }
  return HandleError(publish(NULL, topic.c_str(), strlen(m_sptrstr.c_str()), m_sptrstr.c_str(), qos));
#else
  if (verbose_ > 2) {
    std::cout << "Publishing message in int: ";
    for (size_t i = 0; i < message.size(); i++) {
      std::cout << static_cast<int>(message[i]) << " ";
    };
    std::cout << std::endl;
    std::cout << "Size: " << message.size() << std::endl;
  }
  return HandleError(publish(NULL, topic.c_str(), message.size(), message.data(), qos));
#endif
}
template <>
void MosquittoIO<std::string>::on_message(const struct mosquitto_message *message) {
  std::shared_ptr<mqtt::mosquitto_message<std::string>> m_sptr = std::allocate_shared<mqtt::mosquitto_message<std::string>>(*allocatorMosq_);
  m_sptr->mid = message->mid;
  m_sptr->qos = message->qos;
  m_sptr->retain = message->retain;
  m_sptr->topic = std::string(message->topic);
  m_sptr->payloadlen = message->payloadlen;
  m_sptr->payload = "";
  for (int i = 0; i < message->payloadlen; i++) {
    m_sptr->payload += (static_cast<char *>(message->payload)[i]);
  }
  payLoad_.push_back(m_sptr);
  if (verbose_ < 3) {
    return;
  }
  std::cout << "Received topic: " << m_sptr->topic << std::endl;
  std::cout << "Received message: " << m_sptr->payload << std::endl;
  std::cout << "Received length: " << m_sptr->payloadlen << std::endl;
}
template <>
void MosquittoIO<std::vector<uint8_t>>::on_message(const struct mosquitto_message *message) {
  bool found = false;
  for (const auto &topic: topicSub_) {
    if (message->topic && std::string(message->topic) == topic) {
      if (verbose_ > 1) {
        std::cout << "Received message on subscribed topic: " << topic << std::endl;
      }
      found = true;
      break;
    }
  }
  if (!found) {
    if (verbose_ > 1) {
      std::cout << "Received message on non-subscribed topic: " << (message->topic ? message->topic : "null") << std::endl;
    }
    return;
  }
  std::shared_ptr<mqtt::mosquitto_message<std::vector<uint8_t>>> m_sptr = std::allocate_shared<mqtt::mosquitto_message<std::vector<uint8_t>>>(*allocatorMosq_);
  m_sptr->mid = message->mid;
  m_sptr->qos = message->qos;
  m_sptr->retain = message->retain;
  m_sptr->topic = std::string(message->topic);
#ifndef GB_MOSQUITTOIO_ALL_BINARY
  std::string value(static_cast<char *>(message->payload));
  std::string value_str;
  m_sptr->payloadlen = 0;
  for (size_t i = 0; i < value.size(); i++) {
    if (value[i] == ',') {
      m_sptr->payload.push_back(std::stoi(value_str));
      value_str.clear();
      m_sptr->payloadlen++;
      continue;
    }
    value_str += value[i];
  }
  payLoad_.push_back(m_sptr);
  if (verbose_ < 3) {
    return;
  }
#else
  for (int i = 0; i < message->payloadlen; i++) {
    m_sptr->payload.push_back(static_cast<uint8_t *>(message->payload)[i]);
  }
  m_sptr->payloadlen = message->payloadlen;
  payLoad_.push_back(m_sptr);
#endif
  std::cout << "Received topic: " << m_sptr->topic << std::endl;
  std::cout << "Received first element of message: " << static_cast<int>((m_sptr->payload)[0]) << std::endl;
  std::cout << "Received length: " << m_sptr->payloadlen << std::endl;
}
} // namespace gramsballoon::pgrams