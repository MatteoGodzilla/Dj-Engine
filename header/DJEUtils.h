// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <fstream>
#include <iostream>

#define ENDL "\n"

class Log {
public:
	Log(bool toCERR = false) {
		m_toCERR = toCERR;
		normal = std::ofstream("log.log");
		error = std::ofstream("error.log");
	}

	template <class T>
	Log& operator<<(T t) {
		if (m_toCERR) {
			std::cerr << t;
			error << t;
		} else {
			std::cout << t;
			normal << t;
		}
		return *this;
	}

private:
	std::ofstream normal;
	std::ofstream error;
	bool m_toCERR = false;
};

static std::string VERSION = "alpha v1.6.1";

static Log NormalLog = Log();
static Log ErrorLog = Log(true);
