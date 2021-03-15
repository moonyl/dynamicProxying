//
// Created by moony on 3/11/21.
//

#pragma once

#include <sw/redis++/redis++.h>
#include "IRedisClient.h"

class RedisClient : public IRedisClient{
public:
    static RedisClient& instance();
    [[nodiscard]] sw::redis::Redis& redis() const;
    std::string hget(const std::string_view &key, const std::string_view &field) override;

private:
    std::unique_ptr<sw::redis::Redis> _redis;
    RedisClient();
};


