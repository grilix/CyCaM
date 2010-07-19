using System;
using System.Net.Sockets;
using System.Diagnostics;
using System.Net;

namespace cyNet
{
	public class cyConnection
	{
		public delegate void connectionEvent();
		public delegate void dataEvent(string data);
		public event dataEvent dataArrived;
		public event connectionEvent connectionEstablished;
		public event connectionEvent connetcionTerminated;

		protected readonly string DATAEND = "^ED";
		protected int _connectionPort;
		protected string _remoteHost;
		protected string _strdatabuffer = "";
		
		protected bool _abortThreads = false;
		protected bool _isConnected;
		protected System.Threading.Thread _recThread;
		protected Socket _sock;
		protected Socket _listenSocket;

		public cyConnection() {
			this._isConnected = false;
			this._remoteHost = "";
		}
		public void beginConnection(string remoteHost, int connectionPort) {
			this._remoteHost = remoteHost;
			this._connectionPort = connectionPort;
			this.doListen();
			this.doConnect();
		}
		public void beginConnection(int connectionPort) {
			this.beginConnection("", connectionPort);
		}
		public void beginConnection(string remoteHost) {
			this.beginConnection(remoteHost, 1235);
		}
		public void beginConnection() {
			this.beginConnection("");
		}
		protected void doConnect() {
			if (string.IsNullOrEmpty(this._remoteHost)) {
				return;
			}
			System.Threading.Thread t = new System.Threading.Thread(tryConnect);
			this._sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			t.Priority = System.Threading.ThreadPriority.BelowNormal;
			t.Name = "::connect";
			t.IsBackground = true;
			t.Start();
		}
		protected void tryConnect() {
			Debug.Print(System.Threading.Thread.CurrentThread.Name + ": Main loop running.");
			bool loop = true;
			while (loop && !this._abortThreads) {
				loop = false;
				if (!string.IsNullOrEmpty(this._remoteHost)) {
					try {
						this._sock.Connect(this._remoteHost, this._connectionPort);
						this.onConnect();
					} catch {
						System.Threading.Thread.Sleep(4000);
						loop = true;
					}
				}
			}
			Debug.Print(System.Threading.Thread.CurrentThread.Name + ": EndOfLife.");
			System.Threading.Thread.CurrentThread.Abort();
		}
		protected void doListen() {
			if (!string.IsNullOrEmpty(this._remoteHost)) {
				return;
			}
			Debug.Print("Listenning.");
			if (_listenSocket != null) {
				_listenSocket.Close();
				_listenSocket = null;
			}
			_listenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			_listenSocket.Bind(new IPEndPoint(IPAddress.Any, this._connectionPort));
			_listenSocket.Listen(1);
			_listenSocket.BeginAccept(new AsyncCallback(endAccept), null);
		}
		protected void endAccept(IAsyncResult ar) {
			try {
				this._sock = this._listenSocket.EndAccept(ar);
				Debug.Print("Connection accepted.");
				this._listenSocket.Close();
				this._listenSocket = null;
				this.onConnect();
			} catch {
				this.closeConnection(false);
			}
		}
		public void sendData(string data) {
			try {
				if (!string.IsNullOrEmpty(data) && this._isConnected) {
					this._sock.Send(System.Text.Encoding.ASCII.GetBytes(data + DATAEND));
					System.Threading.Thread.Sleep(0);
				}
			} catch (SocketException) {
				Debug.WriteLine("Can't send data, disconnectig..");
				this.closeConnection(true);
			}
		}
		protected void onDataArrival(string data) {
			if (this.dataArrived != null) {
				this.dataArrived(data);
			}
		}
		protected void receive() {
			Debug.Print(System.Threading.Thread.CurrentThread.Name + ": Main loop running.");
			int sticks = System.Environment.TickCount / 1000;
			int lastPingSent = sticks;
			int lastRec = sticks;
			while (this._sock.Connected && this._isConnected && !this._abortThreads) {
				sticks = System.Environment.TickCount / 1000;
				if (this._sock.Available > 0) {
					try {
						lastRec = sticks;
						byte[] data = new byte[this._sock.Available];
						if (this._sock.Receive(data) > 0) {
							string strdata = System.Text.Encoding.ASCII.GetString(data);
							this.onDataArrival(strdata);
						} else {
							Debug.Print("Can't get data.");
							break;
						}
					} catch (SocketException) {
						break;
					}
				}
				if (!(this._isConnected && this._sock.Connected)) {
					Debug.Print("Conection terminated.");
					break;
				}
				
				if (sticks - lastRec > 15) {
					Debug.Print("Conection timed out.");
					break;
				}
				if (sticks - lastPingSent >= 5) {
					lastPingSent = sticks;
					this.sendData(".");
					Debug.Print("Ping sent..");
				}
				System.Threading.Thread.Sleep(400);

			}
			if (this._isConnected) {
				this.closeConnection(true);
			}
			Debug.Print(System.Threading.Thread.CurrentThread.Name + ": EndOfLife.");
			System.Threading.Thread.CurrentThread.Abort();
		}
		protected void onConnect() {
			if (this._isConnected) {
				return;
			}
			this._isConnected = true;
			if (this.connectionEstablished != null) {
				this.connectionEstablished();
			}
			_recThread = new System.Threading.Thread(receive);
			_recThread.Name = "::receive";
			_recThread.IsBackground = true;
			_recThread.Start();
		}
		protected void onDisconnect() {
			this._isConnected = false;
			if (this.connetcionTerminated != null) {
				this.connetcionTerminated();
			}
		}
		public void closeConnection(bool reconnect) {
			if (!this._isConnected) {
				return;
			}
			Debug.Print("Disconnecting.");
			this.onDisconnect();
			this._sock.Close();
			if (reconnect) {
				this.doListen();
				this.doConnect();
			}
		}
		public string remoteHost {
			get {
				return this._remoteHost;
			}
		}
		public bool isConnected {
			get {
				return this._isConnected;
			}
		}
	}
}
