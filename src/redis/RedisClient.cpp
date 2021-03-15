//
// Created by moony on 3/11/21.
//

#include "RedisClient.h"

RedisClient &RedisClient::instance() {
    static RedisClient* instance = new RedisClient();
    return *instance;
}

sw::redis::Redis &RedisClient::redis() const { return *_redis;}

RedisClient::RedisClient() {
    sw::redis::ConnectionOptions connection_options;
    connection_options.host = "vmx-redis";
    //connection_options.port = 6379;
    connection_options.socket_timeout = std::chrono::milliseconds(1000);
    connection_options.connect_timeout = std::chrono::milliseconds(3000);
    _redis = std::make_unique<sw::redis::Redis>(connection_options);
}

std::string RedisClient::hget(const std::string_view &key, const std::string_view &field) {
    return _redis->hget(key, field).value_or("");
}
