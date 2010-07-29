#include "StdAfx.h"
#include "cyClient.h"

using namespace System;
using namespace cySession;
using namespace cyPaym;
using namespace cyCore;

cyClient::cyClient(String ^name, cySessionHandler ^session, String ^hostName) {
	this->_name = name;
	if (!String::IsNullOrEmpty(hostName)) {
		// Connect here..
	}
	this->_session = session;
}
cyClient::cyClient(String ^name, cySessionHandler ^session) {
	this->_name = name;
	this->_session = session;
}
cyClient::~cyClient(void) {

}
