/*
 * useraction.cpp
 * Copyright 2017 - ~, Apin <apin.klas@gmail.com>
 *
 * This file is part of Turbin.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "useraction.h"
#include "global_constant.h"
#include "db.h"
#include <QDebug>

using namespace LibServer;
using namespace LibG;
using namespace LibDB;

UserAction::UserAction():
    ServerAction("users")
{
    mFunctionMap.insert(MSG_COMMAND::LOGIN, std::bind(&UserAction::login, this, std::placeholders::_1));
}

Message UserAction::login(Message *msg)
{
    LibG::Message message(msg);
    const QString &username = msg->data("username").toString();
    const QString &password = msg->data("password").toString();
    mDb->table(mTableName)->where("username = ", username);
    DbResult res = mDb->exec();
    if(res.isEmpty()) {
        message.setError("Username and password not match");
    } else {
        const QVariantMap &data = res.first();
        if(password.compare(data["password"].toString())) {
            message.setError("Username and password not match");
        } else {
            message.addData("username", data["username"].toString());
            message.addData("name", data["name"].toString());
            message.addData("permission", data["permission"].toString());
        }
    }
    return message;
}