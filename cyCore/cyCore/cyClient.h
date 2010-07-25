#pragma once

using namespace System;
using namespace cySession;
using namespace cyPaym;
using namespace cyNet;

namespace cyCore {

	public ref class cyClient {
	protected:
		String ^_name;
		cySessionHandler ^_session;
		cyConnection ^_connection;

	public:
		cyClient(String ^name, cySessionHandler ^session, String ^hostName);
		cyClient(String ^name, cySessionHandler ^session);
		~cyClient(void);

		property bool isConnected {
			bool get() {
				return this->_connection->isConnected;
			}
		}
		property cySessionHandler ^session {
			cySessionHandler ^get() {
				return this->_session;
			}
			void set(cySessionHandler ^session) {
				this->_session = session;
			}
		}
	};
}