using System;
using cySession;
using cyPaym;
using cyCore;
using System.Collections.Generic;

namespace cyTest {
	class cyTest {
		protected cyClient _client;
		protected cyTariff _t;
		public delegate void cmdDef();

		public cyTest() {
			this._t = new cyTariff("test", 100);
			cyBill b = new cyBill(20);
			cySessionHandler s = new cySessionHandler(this._t, b);
			this._client = new cyClient("client 1", s, null);
		}
		protected void printClientStatus(cyClient client) {
			Console.WriteLine(TimeSpan.FromSeconds(client.session.getTotalTime()).ToString() + ": $" + client.session.getTotalMoney().ToString());
			if (client.session.isPrepaid) {
				Console.WriteLine(TimeSpan.FromSeconds(client.session.getAvailableTime()).ToString());
			}
		}
		public void mainLoop() {
			Dictionary<string, cmdDef> cmdList = this.loadCmds();

			string cmd = "";
			do {
				this.printClientStatus(this._client);
				Console.Write("> ");
				cmd = Console.ReadLine().Trim();
				switch (cmd) {
					case "help":
						Console.Write("Available commands: ");
						foreach (string k in cmdList.Keys) {
							Console.Write(k + " ");
						}
						Console.WriteLine();
					break;
					default:
						if (cmdList.ContainsKey(cmd)) {
							cmdList[cmd].Invoke();
						}
					break;
				}
			} while (cmd.Trim() != "exit");
		}
		protected Dictionary<string, cmdDef> loadCmds() {
			Dictionary<string, cmdDef> cmdList = new Dictionary<string, cmdDef>();
			cmdList.Add("tname", new cmdDef(this.cmd_tname));
			cmdList.Add("tprice", new cmdDef(this.cmd_tprice));
			cmdList.Add("start", new cmdDef(this.cmd_start));
			cmdList.Add("stop", new cmdDef(this.cmd_stop));
			return cmdList;
		}
		public void cmd_tname() {
			Console.Write("\tNew name: ");
			this._t.name = Console.ReadLine();
		}
		public void cmd_tprice() {
			Console.Write("\tNew price: ");
			float c = Convert.ToSingle(Console.ReadLine());
			this._t.cost = c;
		}
		public void cmd_start() {
			this._client.session.startNewTime();
		}
		public void cmd_stop() {
			this._client.session.closeActive();
		}
	}
}
