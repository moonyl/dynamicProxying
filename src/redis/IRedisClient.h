//
// Created by moony on 3/11/21.
//

#pragma once

class IRedisClient
{
protected:
    virtual ~IRedisClient() = default;

public:
    virtual std::string hget(const std::string_view &key, const std::string_view &field) = 0;

};